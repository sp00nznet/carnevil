/* SPDX-License-Identifier: BSD-3-Clause
 *
 * Ported from MAME (https://github.com/mamedev/mame):
 * src/mame/williams/seattle.cpp, src/devices/machine/gt64xxx.cpp and
 * src/mame/williams/midwayic.cpp, which carry this licence:
 *
 *   license:BSD-3-Clause
 *   copyright-holders:Aaron Giles, Ted Green
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions are
 *   met: (1) Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer. (2)
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *   (3) Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *   IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 *   TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 *   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The port and the changes made to it are distributed under the same terms.
 */
/*
 * Midway Seattle board: memory map, GT64010 system controller (PCI config,
 * DMA, timers, interrupt cause/mask), PCI IDE disk, I/O ASIC with its PIC,
 * CMOS, light guns, board interrupt logic and video timing.
 *
 * Register behaviour follows MAME's seattle.cpp, gt64xxx.cpp and midwayic.cpp
 * (all BSD-3-Clause, Aaron Giles / Ted Green; see NOTICE) and the GT-64010 and
 * ATA documentation.
 *
 * Physical memory map (CS windows as the boot ROM programs them):
 *   00000000-007FFFFF  RAM (8 MB)
 *   08000000-09FFFFFF  PCI memory  (Voodoo BAR)
 *   0A000000-0BFFFFFF  PCI I/O     (IDE)
 *   0C000000-0C000FFF  GT64010 registers
 *   16000000-1600003F  I/O ASIC
 *   16100000-1611FFFF  CMOS
 *   16800000-1680001F  CarnEvil light guns
 *   17000000-17F00000  board control (interrupts, watchdog, LEDs, ...)
 *   1FC00000-1FC7FFFF  boot ROM
 */
#include "hw.h"
#include "voodoo.h"
#include "dcs.h"
#include "netplay.h"
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

uint8_t* g_rdram;
int32_t  g_hw_budget = HW_TICK_LOADS;
recomp_context* g_cur_ctx;

/* Diagnostics, switched on by environment variables at startup. */
static int g_trace_irq, g_trace_pci, g_trace_perf;

cabinet_inputs g_inputs = { 0xFFFF, 0xBDFE, { { 32768, 32768, 0 }, { 32768, 32768, 0 } } };

static voodoo_state_t* g_voodoo;

void hw_fatal(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "\n[fatal] ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    hw_shutdown();
    exit(1);
}

static void log_unmapped(const char* what, uint32_t paddr, uint32_t value) {
    static uint32_t seen[512];
    static int nseen;
    for (int i = 0; i < nseen; i++) if (seen[i] == paddr) return;
    if (nseen < 512) seen[nseen++] = paddr;
    fprintf(stderr, "[hw] unmapped %s 0x%08X = 0x%08X\n", what, paddr, value);
}

/* ======================================================================
 * Time
 * ====================================================================== */
/* Emulated time is counted in cycles of the CPU's Count register (half the
 * 150 MHz pipeline clock) and advances with the work the recompiled code
 * does: every HW_TICK_LOADS loads is one tick of TICK_CYCLES, and a read of
 * Count itself costs COUNT_READ_CYCLES (timed waits spin on Count without
 * touching memory). So time is a function of the program and its inputs
 * alone -- two machines fed the same inputs stay identical, which is what
 * netplay relies on, and a busy host can no longer make the game's timeouts
 * expire. The wall clock is only used to hold the result to the board's real
 * speed (see vblank_begin), and turbo skips that wait.
 *
 * TICK_CYCLES puts 4096 loads at about 55 us: a little faster than the
 * R5000 runs the same code, so the game keeps up wherever the board did. */
#define COUNT_HZ          75000000.0
#define TICK_CYCLES       4096
#define COUNT_READ_CYCLES 2
static uint64_t g_cycles;

double hw_time(void) { return (double)g_cycles / COUNT_HZ; }
#define now_s hw_time

uint32_t hw_cp0_count(void) {
    g_cycles += COUNT_READ_CYCLES;
    if (--g_hw_budget < 0) hw_tick();
    return (uint32_t)g_cycles;
}

/* Host time, for pacing and the perf log only. */
static LARGE_INTEGER g_qpf, g_qpc0;
static double wall_s(void) {
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return (double)(t.QuadPart - g_qpc0.QuadPart) / (double)g_qpf.QuadPart;
}

/* ======================================================================
 * CPU interrupts
 *
 * MIPS IRQ n is Cause.IP(n+2). Galileo is IRQ0, the I/O ASIC IRQ1, IDE IRQ2,
 * vblank IRQ3-5 as the board's interrupt config register selects. Delivery
 * runs the RTOS's own interrupt entry, rtos_80005244(cause, frame), with the
 * stack and frame its exception vector would have set up.
 * ====================================================================== */
static uint32_t g_status = 0x00400004;  /* BEV|ERL as at reset */
static uint32_t g_irq_lines;
static int g_in_irq;

static void set_irq(int n, int on) {
    if (on) g_irq_lines |= 1u << n; else g_irq_lines &= ~(1u << n);
}
uint32_t hw_get_cause(void) { return (g_irq_lines & 0x3F) << 10; }
uint32_t hw_get_status(void) { return g_status; }

extern void rtos_80005244(uint8_t* rdram, recomp_context* ctx);
#define RTOS_IRQ_FRAME 0x80014328u
#define RTOS_IRQ_STACK 0x800C1400u

static void deliver_irqs(void) {
    if (g_in_irq) return;
    for (int guard = 0; guard < 16; guard++) {
        uint32_t st = g_status, cause = hw_get_cause();
        if (!(cause & st & 0xFF00) || !(st & 1) || (st & 6)) return;
        if (g_trace_irq) fprintf(stderr, "[irq] cause=%08X status=%08X\n", cause, st);
        g_in_irq = 1;
        g_status = st | 2;                              /* EXL */
        *(uint64_t*)(g_rdram + PHYS(RTOS_IRQ_FRAME + 0x178)) = st;
        *(uint64_t*)(g_rdram + PHYS(RTOS_IRQ_FRAME + 0x180)) = cause;
        recomp_context c;
        recomp_ctx_init(&c);
        c.r4 = cause;
        c.r5 = (gpr)(int32_t)RTOS_IRQ_FRAME;
        c.r29 = (gpr)(int32_t)RTOS_IRQ_STACK;
        c.r28 = g_cur_ctx ? g_cur_ctx->r28 : 0;         /* interrupted code's $gp */
        recomp_context* saved = g_cur_ctx;
        g_cur_ctx = &c;
        rtos_80005244(g_rdram, &c);
        g_cur_ctx = saved;
        g_status = st;                                  /* eret */
        g_in_irq = 0;
    }
}

void hw_set_status(uint32_t value) {
    g_status = value;
    deliver_irqs();
}

/* ======================================================================
 * GT64010
 * ====================================================================== */
#define GREG(off) g_gt[(off) / 4]
static uint32_t g_gt[0x1000 / 4];
static uint32_t g_gt_pending;
static struct { int active; uint32_t count; double start; } g_gt_timer[4];

static uint32_t bus_read32(uint32_t p);
static void bus_write32(uint32_t p, uint32_t v);

static void gt_update_irq(void) {
    if (g_gt_pending) { GREG(0xC18) |= g_gt_pending; g_gt_pending = 0; }
    if (GREG(0xC18) & 0xFFFFFFFEu) GREG(0xC18) |= 1; else GREG(0xC18) &= ~1u;
    if (GREG(0xC18) & GREG(0xC1C) & 0x3C1FFFFEu) GREG(0xC18) |= 1u << 30; else GREG(0xC18) &= ~(1u << 30);
    set_irq(0, (GREG(0xC18) & GREG(0xC1C)) != 0);
}

static int gt_fetch_next(int ch) {
    uint32_t a = (GREG(0x840 + ch * 4) & 0x200) ? 0 : GREG(0x830 + ch * 4);
    if (!a) { GREG(0x840 + ch * 4) &= ~0x5000u; return 0; }
    GREG(0x800 + ch * 4) = bus_read32(a);
    GREG(0x810 + ch * 4) = bus_read32(a + 4);
    GREG(0x820 + ch * 4) = bus_read32(a + 8);
    GREG(0x830 + ch * 4) = bus_read32(a + 12);
    return 1;
}

/* DMA runs to completion when it is started; the completion interrupt is then
 * pending and is taken at the next tick, a few thousand instructions later. */
static void gt_run_dma(int ch) {
    for (int guard = 0; guard < 65536; guard++) {
        uint32_t ctl = GREG(0x840 + ch * 4);
        int sinc = ((ctl >> 2) & 3) == 0 ? 4 : ((ctl >> 2) & 3) == 1 ? -4 : 0;
        int dinc = ((ctl >> 4) & 3) == 0 ? 4 : ((ctl >> 4) & 3) == 1 ? -4 : 0;
        uint32_t src = GREG(0x810 + ch * 4), dst = GREG(0x820 + ch * 4);
        uint32_t n = GREG(0x800 + ch * 4) & 0xFFFF;
        for (; n >= 4; n -= 4, src += sinc, dst += dinc) bus_write32(dst, bus_read32(src));
        for (; n > 0; n--, src += sinc / 4, dst += dinc / 4) {
            uint32_t s = PHYS(src), d = PHYS(dst);
            if (s < RAM_SIZE && d < RAM_SIZE) g_rdram[d] = g_rdram[s];
        }
        GREG(0x810 + ch * 4) = src;
        GREG(0x820 + ch * 4) = dst;
        GREG(0x800 + ch * 4) &= ~0xFFFFu;
        if (!(ctl & (1 << 10))) g_gt_pending |= 1u << (4 + ch);
        if ((ctl & (1 << 9)) || !gt_fetch_next(ch)) {
            if ((ctl & (1 << 10)) && !(ctl & (1 << 9))) g_gt_pending |= 1u << (4 + ch);
            GREG(0x840 + ch * 4) &= ~0x5000u;
            break;
        }
    }
    gt_update_irq();
}

static uint32_t gt_timer_value(int i, double t) {
    uint32_t v = g_gt_timer[i].count;
    if (g_gt_timer[i].active) {
        double el = (t - g_gt_timer[i].start) * 50000000.0;
        v = el >= v ? 0 : v - (uint32_t)el;
    }
    return v;
}

static void gt_timers_update(double t) {
    for (int i = 0; i < 4; i++) {
        if (!g_gt_timer[i].active || gt_timer_value(i, t) != 0) continue;
        g_gt_pending |= 1u << (8 + i);
        uint32_t reload = GREG(0x850 + i * 4) & (i ? 0xFFFFFF : 0xFFFFFFFF);
        if (GREG(0x864) & (2u << (2 * i))) {           /* timer mode: reload */
            g_gt_timer[i].count = reload ? reload : 0xFFFFFF;
            g_gt_timer[i].start = t;
        } else {
            g_gt_timer[i].active = 0;
            g_gt_timer[i].count = 0;
        }
        gt_update_irq();
    }
}

/* ---- PCI configuration space ---- */
typedef struct { uint32_t cfg[64]; uint32_t bar_size[6]; } pci_dev;
static pci_dev g_pci_galileo, g_pci_voodoo, g_pci_ide;

static pci_dev* pci_find(uint32_t addr) {
    if (!(addr & 0x80000000u) || ((addr >> 16) & 0xFF) != 0 || ((addr >> 8) & 7) != 0) return NULL;
    switch ((addr >> 11) & 0x1F) {
        case 0: return &g_pci_galileo;
        case 8: return &g_pci_voodoo;
        case 9: return &g_pci_ide;
        default: return NULL;
    }
}

static uint32_t pci_cfg_read(void) {
    pci_dev* d = pci_find(GREG(0xCF8));
    return d ? d->cfg[(GREG(0xCF8) >> 2) & 63] : 0xFFFFFFFFu;
}

static void pci_cfg_write(uint32_t v) {
    pci_dev* d = pci_find(GREG(0xCF8));
    if (g_trace_pci) fprintf(stderr, "[pci] cfg %08X <- %08X\n", GREG(0xCF8), v);
    if (!d) return;
    int reg = (GREG(0xCF8) >> 2) & 63;
    if (reg >= 4 && reg < 10 && d->bar_size[reg - 4]) {
        uint32_t size = d->bar_size[reg - 4];
        d->cfg[reg] = (v & ~(size - 1)) | (d->cfg[reg] & (size < 0x100 ? 1u : 0u));
        return;
    }
    if (reg == 0 || reg == 2) return;                   /* read-only ids */
    d->cfg[reg] = v;
    if (d == &g_pci_voodoo) voodoo_pci_config_write(g_voodoo, (uint32_t)reg * 4, v);
}

static uint32_t voodoo_base(void) { return g_pci_voodoo.cfg[4] & 0xFF000000u; }

static uint32_t gt_read(uint32_t off) {
    off &= 0xFFC;
    switch (off) {
        case 0x850: case 0x854: case 0x858: case 0x85C:
            return gt_timer_value((off - 0x850) / 4, now_s());
        case 0xC00: return GREG(0xC00) | 1;            /* little-endian */
        case 0xCFC: return pci_cfg_read();
        default:    return GREG(off);
    }
}

static void gt_write(uint32_t off, uint32_t v) {
    off &= 0xFFC;
    uint32_t old = GREG(off);
    GREG(off) = v;
    switch (off) {
        case 0x840: case 0x844: case 0x848: case 0x84C: {
            int ch = (off - 0x840) / 4;
            GREG(off) = (v & ~0x6000u) | (old & 0x4000u);
            if (v & 0x2000) gt_fetch_next(ch);
            if (!(old & 0x1000) && (v & 0x1000)) {
                GREG(off) |= 0x4000;
                gt_run_dma(ch);
            }
            break;
        }
        case 0x850: case 0x854: case 0x858: case 0x85C: {
            int i = (off - 0x850) / 4;
            if (!g_gt_timer[i].active) g_gt_timer[i].count = i ? (v & 0xFFFFFF) : v;
            break;
        }
        case 0x864: {
            double t = now_s();
            for (int i = 0; i < 4; i++) {
                int en = (v >> (2 * i)) & 1;
                if (en && !g_gt_timer[i].active) {
                    g_gt_timer[i].active = 1;
                    if (!g_gt_timer[i].count)
                        g_gt_timer[i].count = GREG(0x850 + i * 4) & (i ? 0xFFFFFF : 0xFFFFFFFF);
                    g_gt_timer[i].start = t;
                } else if (!en && g_gt_timer[i].active) {
                    g_gt_timer[i].count = gt_timer_value(i, t);
                    g_gt_timer[i].active = 0;
                }
            }
            break;
        }
        case 0xC18: GREG(off) = old & v; gt_update_irq(); break;
        case 0xC1C: GREG(off) = v & 0x3C1FFFFEu; gt_update_irq(); break;
        case 0xCFC: GREG(off) = old; pci_cfg_write(v); break;
    }
}

/* ======================================================================
 * IDE: National 87415 PCI IDE, one ATA drive (the game's raw disk image).
 * Commands are executed synchronously and raise IRQ2 when data is ready.
 * Writes go to an in-memory overlay so the image file is never modified.
 * ====================================================================== */
#define IDE_CYLS  6256
#define IDE_HEADS 16
#define IDE_SECS  63

static struct {
    FILE* img;
    uint8_t reg[8];             /* 1..7: error/feature .. status/command */
    uint8_t devctl;
    uint8_t status;
    uint8_t buf[512 * 16];
    int pos, len;               /* bytes in buf */
    int sectors_left;
    int block;                  /* sectors per DRQ block */
    int multiple;
    int writing;
    int irq;
    uint8_t bm_cmd, bm_status;
    uint32_t bm_prd;
    int dma_pending;            /* 0, 'r' or 'w' */
    struct { uint32_t lba; uint8_t data[512]; }* overlay;
    int noverlay;
} g_ide;

static void ide_set_irq(int on) {
    g_ide.irq = on;
    set_irq(2, on && !(g_ide.devctl & 2));
}

static uint32_t ide_lba(void) {
    uint8_t dh = g_ide.reg[6];
    if (dh & 0x40)
        return ((uint32_t)(dh & 0xF) << 24) | ((uint32_t)g_ide.reg[5] << 16) | ((uint32_t)g_ide.reg[4] << 8) | g_ide.reg[3];
    uint32_t cyl = g_ide.reg[4] | ((uint32_t)g_ide.reg[5] << 8);
    return (cyl * IDE_HEADS + (dh & 0xF)) * IDE_SECS + (g_ide.reg[3] ? g_ide.reg[3] - 1 : 0);
}

static void ide_set_lba(uint32_t lba) {
    if (g_ide.reg[6] & 0x40) {
        g_ide.reg[3] = lba & 0xFF; g_ide.reg[4] = (lba >> 8) & 0xFF; g_ide.reg[5] = (lba >> 16) & 0xFF;
        g_ide.reg[6] = (g_ide.reg[6] & 0xF0) | ((lba >> 24) & 0xF);
    } else {
        uint32_t sec = lba % IDE_SECS + 1, t = lba / IDE_SECS;
        g_ide.reg[3] = (uint8_t)sec; g_ide.reg[6] = (g_ide.reg[6] & 0xF0) | (t % IDE_HEADS);
        g_ide.reg[4] = (t / IDE_HEADS) & 0xFF; g_ide.reg[5] = ((t / IDE_HEADS) >> 8) & 0xFF;
    }
}

static uint64_t g_sectors_read;

static void ide_read_sector(uint32_t lba, uint8_t* out) {
    g_sectors_read++;
    for (int i = 0; i < g_ide.noverlay; i++)
        if (g_ide.overlay[i].lba == lba) { memcpy(out, g_ide.overlay[i].data, 512); return; }
    memset(out, 0, 512);
    if (g_ide.img && _fseeki64(g_ide.img, (int64_t)lba * 512, SEEK_SET) == 0) fread(out, 1, 512, g_ide.img);
}

static void ide_write_sector(uint32_t lba, const uint8_t* in) {
    int i;
    for (i = 0; i < g_ide.noverlay && g_ide.overlay[i].lba != lba; i++) {}
    if (i == g_ide.noverlay) {
        g_ide.overlay = realloc(g_ide.overlay, (size_t)(g_ide.noverlay + 1) * sizeof(*g_ide.overlay));
        g_ide.overlay[g_ide.noverlay++].lba = lba;
    }
    memcpy(g_ide.overlay[i].data, in, 512);
}

static void ide_put_string(uint16_t* w, int words, const char* s) {
    for (int i = 0; i < words; i++) {
        char a = *s ? *s++ : ' ', b = *s ? *s++ : ' ';
        w[i] = (uint16_t)((a << 8) | (uint8_t)b);
    }
}

static void ide_load_block(void) {
    int n = g_ide.sectors_left < g_ide.block ? g_ide.sectors_left : g_ide.block;
    uint32_t lba = ide_lba();
    for (int i = 0; i < n; i++) ide_read_sector(lba + i, g_ide.buf + i * 512);
    ide_set_lba(lba + n);
    g_ide.sectors_left -= n;
    g_ide.pos = 0;
    g_ide.len = n * 512;
    g_ide.status = 0x58;                                /* DRDY DSC DRQ */
    ide_set_irq(1);
}

static void ide_bm_start(void);

static void ide_command(uint8_t cmd) {
    int count = g_ide.reg[2] ? g_ide.reg[2] : 256;
    g_ide.reg[1] = 0;
    g_ide.writing = 0;
    switch (cmd) {
        case 0xEC: {                                    /* IDENTIFY DEVICE */
            uint16_t* w = (uint16_t*)g_ide.buf;
            uint32_t total = IDE_CYLS * IDE_HEADS * IDE_SECS;
            memset(w, 0, 512);
            w[0] = 0x0040; w[1] = IDE_CYLS; w[3] = IDE_HEADS; w[4] = 512 * IDE_SECS; w[5] = 512; w[6] = IDE_SECS;
            ide_put_string(w + 10, 10, "CARNEVIL-RECOMP");
            ide_put_string(w + 23, 4, "1.0");
            ide_put_string(w + 27, 20, "RECOMP ATA DISK");
            w[47] = 0x8010; w[49] = 0x0300; w[51] = 0x0200; w[53] = 1;
            w[54] = IDE_CYLS; w[55] = IDE_HEADS; w[56] = IDE_SECS;
            w[57] = total & 0xFFFF; w[58] = total >> 16;
            w[59] = g_ide.multiple ? (0x100 | g_ide.multiple) : 0;
            w[60] = total & 0xFFFF; w[61] = total >> 16;
            g_ide.pos = 0; g_ide.len = 512; g_ide.sectors_left = 0;
            g_ide.status = 0x58;
            ide_set_irq(1);
            return;
        }
        case 0x20: case 0x21:                           /* READ SECTORS */
            g_ide.block = 1; g_ide.sectors_left = count; ide_load_block(); return;
        case 0xC4:                                      /* READ MULTIPLE */
            g_ide.block = g_ide.multiple ? g_ide.multiple : 1; g_ide.sectors_left = count; ide_load_block(); return;
        case 0x30: case 0x31: case 0xC5:                /* WRITE SECTORS / MULTIPLE */
            g_ide.block = (cmd == 0xC5 && g_ide.multiple) ? g_ide.multiple : 1;
            g_ide.sectors_left = count;
            g_ide.writing = 1;
            g_ide.pos = 0;
            g_ide.len = (count < g_ide.block ? count : g_ide.block) * 512;
            g_ide.status = 0x58;
            return;
        case 0xC8: case 0xCA:                           /* READ / WRITE DMA */
            g_ide.dma_pending = cmd == 0xC8 ? 'r' : 'w';
            g_ide.sectors_left = count;
            g_ide.status = 0x58;
            if (g_ide.bm_cmd & 1) ide_bm_start();       /* engine already running: transfer now */
            return;
        case 0xC6:                                      /* SET MULTIPLE MODE */
            g_ide.multiple = g_ide.reg[2] <= 16 ? g_ide.reg[2] : 16;
            break;
        case 0x10: case 0x91: case 0xEF: case 0x40: case 0x41: case 0x70:
        case 0xE0: case 0xE1: case 0xE2: case 0xE3: case 0xE5: case 0xE7:
            break;                                      /* accepted, nothing to do */
        default:
            fprintf(stderr, "[ide] unsupported command 0x%02X\n", cmd);
            g_ide.status = 0x51;                        /* DRDY DSC ERR */
            g_ide.reg[1] = 0x04;                        /* ABRT */
            ide_set_irq(1);
            return;
    }
    g_ide.status = 0x50;
    ide_set_irq(1);
}

static uint32_t ide_data_read(int bytes) {
    uint32_t v = 0;
    for (int i = 0; i < bytes && g_ide.pos < g_ide.len; i++) v |= (uint32_t)g_ide.buf[g_ide.pos++] << (8 * i);
    if (g_ide.len && g_ide.pos >= g_ide.len && !g_ide.writing) {
        g_ide.len = 0;
        if (g_ide.sectors_left > 0) ide_load_block();
        else g_ide.status = 0x50;
    }
    return v;
}

static void ide_data_write(uint32_t v, int bytes) {
    if (!g_ide.writing) return;
    for (int i = 0; i < bytes && g_ide.pos < g_ide.len; i++) g_ide.buf[g_ide.pos++] = (uint8_t)(v >> (8 * i));
    if (g_ide.pos >= g_ide.len) {
        int n = g_ide.len / 512;
        uint32_t lba = ide_lba();
        for (int i = 0; i < n; i++) ide_write_sector(lba + i, g_ide.buf + i * 512);
        ide_set_lba(lba + n);
        g_ide.sectors_left -= n;
        g_ide.pos = 0;
        if (g_ide.sectors_left > 0) {
            g_ide.len = (g_ide.sectors_left < g_ide.block ? g_ide.sectors_left : g_ide.block) * 512;
            g_ide.status = 0x58;
        } else {
            g_ide.writing = 0; g_ide.len = 0;
            g_ide.status = 0x50;
        }
        ide_set_irq(1);
    }
}

/* Bus-master DMA: walk the PRD table, then interrupt. */
static void ide_bm_start(void) {
    if (!g_ide.dma_pending) return;
    uint32_t prd = g_ide.bm_prd, lba = ide_lba();
    uint8_t sec[512];
    int secpos = 512;
    for (int guard = 0; guard < 4096 && g_ide.sectors_left > 0; guard++, prd += 8) {
        uint32_t addr = bus_read32(prd), cw = bus_read32(prd + 4);
        uint32_t n = cw & 0xFFFF ? cw & 0xFFFF : 0x10000;
        for (uint32_t i = 0; i < n && g_ide.sectors_left > 0; i++) {
            uint32_t p = PHYS(addr + i);
            if (g_ide.dma_pending == 'r') {
                if (secpos == 512) { ide_read_sector(lba++, sec); secpos = 0; }
                if (p < RAM_SIZE) g_rdram[p] = sec[secpos];
                if (++secpos == 512) g_ide.sectors_left--;
            } else {
                sec[secpos == 512 ? (secpos = 0) : secpos] = p < RAM_SIZE ? g_rdram[p] : 0;
                if (++secpos == 512) { ide_write_sector(lba++, sec); g_ide.sectors_left--; }
            }
        }
        if (cw & 0x80000000u) break;
    }
    ide_set_lba(lba);
    g_ide.dma_pending = 0;
    g_ide.status = 0x50;
    g_ide.bm_status = (g_ide.bm_status & ~1) | 4;
    ide_set_irq(1);
}

static int g_ide_trace = -1;
#define IDE_TRACE(...) do { if (g_ide_trace < 0) g_ide_trace = getenv("CARNEVIL_IDE_TRACE") != NULL; \
                            if (g_ide_trace) fprintf(stderr, __VA_ARGS__); } while (0)

static uint32_t ide_io_read(uint32_t port, int size) {
    if (port != 0x1F0) IDE_TRACE("[ide] rd %03X = %02X\n", port, port == 0x3F6 || port == 0x1F7 ? g_ide.status : g_ide.reg[port & 7]);
    if (port == 0x1F0) return ide_data_read(size);
    if (port > 0x1F0 && port <= 0x1F7) {
        if (port == 0x1F7) { ide_set_irq(0); return g_ide.status; }
        return g_ide.reg[port - 0x1F0];
    }
    if (port == 0x3F6) return g_ide.status;
    log_unmapped("ide read", port, 0);
    return 0;
}

static void ide_io_write(uint32_t port, uint32_t v, int size) {
    if (port != 0x1F0) IDE_TRACE("[ide] wr %03X = %02X\n", port, v);
    if (port == 0x1F0) { ide_data_write(v, size); return; }
    if (port > 0x1F0 && port < 0x1F7) { g_ide.reg[port - 0x1F0] = (uint8_t)v; return; }
    if (port == 0x1F7) { ide_command((uint8_t)v); return; }
    if (port == 0x3F6) {
        if ((v & 4) && !(g_ide.devctl & 4)) { g_ide.status = 0x50; g_ide.reg[1] = 1; g_ide.len = 0; g_ide.writing = 0; }
        g_ide.devctl = (uint8_t)v;
        ide_set_irq(g_ide.irq);
        return;
    }
    log_unmapped("ide write", port, v);
}

/* The bus-master block sits at the IDE's BAR4. The RTOS programs PCI I/O
 * BARs with CPU physical addresses (0x0A000xxx), so they are compared as such. */
static int ide_bm_port(uint32_t io) {
    uint32_t bar = (g_pci_ide.cfg[8] & ~3u) & 0x01FFFFFFu;
    return (bar && io >= bar && io < bar + 16) ? (int)(io - bar) : -1;
}

static uint32_t pci_io_read(uint32_t io, int size) {
    int bm = ide_bm_port(io);
    if (bm >= 0) {
        switch (bm) {
            case 0: return g_ide.bm_cmd;
            case 2: return g_ide.bm_status;
            case 4: return g_ide.bm_prd;
            default: return 0;
        }
    }
    uint32_t port = io & 0xFFFF;
    if ((port >= 0x1F0 && port <= 0x1F7) || port == 0x3F6) return ide_io_read(port, size);
    log_unmapped("pci io read", io, 0);
    return 0xFFFFFFFFu;
}

static void pci_io_write(uint32_t io, uint32_t v, int size) {
    int bm = ide_bm_port(io);
    if (bm >= 0) {
        IDE_TRACE("[ide] bm wr %d = %08X (size %d, dma_pending %c)\n", bm, v, size, g_ide.dma_pending ? g_ide.dma_pending : '-');
        switch (bm) {
            case 0:
                g_ide.bm_cmd = (uint8_t)v;
                if (size == 4) g_ide.bm_status = (uint8_t)((g_ide.bm_status & 0x07 & ~((v >> 16) & 6)) | ((v >> 16) & 0x60));
                if (v & 1) ide_bm_start();
                break;
            case 2: g_ide.bm_status &= ~(v & 6); break;
            case 4: g_ide.bm_prd = v & ~3u; break;
        }
        return;
    }
    uint32_t port = io & 0xFFFF;
    if ((port >= 0x1F0 && port <= 0x1F7) || port == 0x3F6) { ide_io_write(port, v, size); return; }
    log_unmapped("pci io write", io, v);
}

/* ======================================================================
 * I/O ASIC (16 x 16-bit registers, shuffled once unlocked) and its PIC
 * ====================================================================== */
enum { IO_PORT0, IO_PORT1, IO_PORT2, IO_PORT3, IO_UARTCTL, IO_UARTOUT, IO_UARTIN, IO_COIN,
       IO_SOUNDCTL, IO_SOUNDOUT, IO_SOUNDSTAT, IO_SOUNDIN, IO_PICOUT, IO_PICIN, IO_INTSTAT, IO_INTCTL };
static const uint8_t k_carnevil_shuffle[16] = { 0x7,0x3,0x2,0x0,0x1,0xc,0xd,0xe,0xf,0x4,0x5,0x6,0x8,0x9,0xa,0xb };
static struct { uint16_t reg[16]; int shuffle; int irq; uint16_t sound_irq; } g_io;

/* Security PIC: serial number, clock and a small NVRAM, clocked one nibble
 * at a time through PICOUT. Serial data follows MAME's generator for
 * "upper" 486 (the 39" cabinet). */
static struct {
    uint8_t data[16], buffer[16], nvram[0x100], time_buf[7];
    int latch, latch_reads, state, index, total, nvram_addr, time_index;
} g_pic;

static void pic_generate_serial(int upper) {
    int year = 1998, month = 12, day = 11;
    uint32_t sn = 123450 + upper * 1000000, t;
    uint8_t d[9];
    for (int i = 8, v = (int)sn; i >= 0; i--, v /= 10) d[i] = (uint8_t)(v % 10);
    g_pic.data[12] = 0x5A; g_pic.data[13] = 0xA5; g_pic.data[14] = 0; g_pic.data[15] = 0;
    t = 0x174 * (year - 1980) + 0x1f * (month - 1) + day;
    g_pic.data[10] = (t >> 8) & 0xff; g_pic.data[11] = t & 0xff;
    t = d[4] + d[7] * 10 + d[1] * 100;
    t = (t + 5 * g_pic.data[13]) * 0x1bcd + 0x1f3f0;
    g_pic.data[7] = t & 0xff; g_pic.data[8] = (t >> 8) & 0xff; g_pic.data[9] = (t >> 16) & 0xff;
    t = d[6] + d[8] * 10 + d[0] * 100 + d[2] * 10000;
    t = (t + 2 * g_pic.data[13] + g_pic.data[12]) * 0x107f + 0x71e259;
    g_pic.data[3] = t & 0xff; g_pic.data[4] = (t >> 8) & 0xff; g_pic.data[5] = (t >> 16) & 0xff; g_pic.data[6] = (t >> 24) & 0xff;
    t = d[5] * 10 + d[3] * 100;
    t = (t + g_pic.data[12]) * 0x245 + 0x3d74;
    g_pic.data[0] = t & 0xff; g_pic.data[1] = (t >> 8) & 0xff; g_pic.data[2] = (t >> 16) & 0xff;
}

static int bcd(int v) { return ((v / 10) << 4) | (v % 10); }

/* The PIC's real-time clock: the time the game first asks, then running on
 * emulated time. In a netplay session both machines start from the same
 * fixed date instead, so they read the same clock. */
static void pic_clock(SYSTEMTIME* out) {
    static ULONGLONG base;   /* FILETIME, 100 ns units */
    if (!base) {
        SYSTEMTIME st;
        static const SYSTEMTIME fixed = { 1999, 1, 5, 1, 12, 0, 0, 0 };
        if (netplay_active()) st = fixed; else GetLocalTime(&st);
        FILETIME ft;
        SystemTimeToFileTime(&st, &ft);
        base = ((ULONGLONG)ft.dwHighDateTime << 32 | ft.dwLowDateTime) - (ULONGLONG)(hw_time() * 1e7);
    }
    ULONGLONG now = base + (ULONGLONG)(hw_time() * 1e7);
    FILETIME ft = { (DWORD)now, (DWORD)(now >> 32) };
    FileTimeToSystemTime(&ft, out);
}

static uint8_t pic_status(void) {
    if (g_pic.latch & 0xF00) {
        if (++g_pic.latch_reads >= 4) g_pic.latch &= 0xFF;
        return 1;
    }
    return 0;
}

static uint8_t pic_read(void) {
    if (g_pic.latch & 0xF00) return g_pic.latch & 0xFF;
    return g_pic.index < g_pic.total ? 0xFF : 0;
}

static void pic_write(uint8_t data) {
    g_pic.latch = (data & 0x0F) | 0x480;
    g_pic.latch_reads = 0;
    if (!(data & 0x10)) return;
    int cmd = g_pic.state ? (g_pic.state & 0x0F) : (g_pic.latch & 0x0F);
    switch (cmd) {
        case 0:
            if (g_pic.index < g_pic.total) g_pic.latch = 0x400 | g_pic.buffer[g_pic.index++];
            break;
        case 1:
            if (g_pic.index < g_pic.total) g_pic.latch = 0x400 | g_pic.buffer[g_pic.index++];
            else { memcpy(g_pic.buffer, g_pic.data, 16); g_pic.total = 16; g_pic.index = 0; }
            break;
        case 3: {                                       /* read the clock */
            SYSTEMTIME st;
            pic_clock(&st);
            g_pic.index = 0; g_pic.total = 0;
            g_pic.buffer[g_pic.total++] = (uint8_t)bcd(st.wSecond);
            g_pic.buffer[g_pic.total++] = (uint8_t)bcd(st.wMinute);
            g_pic.buffer[g_pic.total++] = (uint8_t)bcd(st.wHour);
            g_pic.buffer[g_pic.total++] = (uint8_t)bcd(st.wDayOfWeek + 1);
            g_pic.buffer[g_pic.total++] = (uint8_t)bcd(st.wDay);
            g_pic.buffer[g_pic.total++] = (uint8_t)bcd(st.wMonth);
            g_pic.buffer[g_pic.total++] = (uint8_t)bcd((st.wYear - 1900 - 80) % 100);
            break;
        }
        case 4:                                         /* write the clock: accepted, not kept */
            if (g_pic.state == 0) { g_pic.state = 0x14; g_pic.time_index = 0; }
            else if (g_pic.state == 0x14) { g_pic.time_buf[g_pic.time_index] = g_pic.latch & 0x0F; g_pic.state = 0x24; }
            else if (g_pic.state == 0x24) {
                g_pic.time_buf[g_pic.time_index++] |= (uint8_t)(g_pic.latch << 4);
                g_pic.state = g_pic.time_index < 7 ? 0x14 : 0;
            }
            break;
        case 5:                                         /* write NVRAM */
            if (g_pic.state == 0) g_pic.state = 0x15;
            else if (g_pic.state == 0x15) { g_pic.nvram_addr = g_pic.latch & 0x0F; g_pic.state = 0x25; }
            else if (g_pic.state == 0x25) { g_pic.nvram_addr |= (g_pic.latch & 0x0F) << 4; g_pic.state = 0x35; }
            else if (g_pic.state == 0x35) { g_pic.nvram[g_pic.nvram_addr] = g_pic.latch & 0x0F; g_pic.state = 0x45; }
            else if (g_pic.state == 0x45) { g_pic.nvram[g_pic.nvram_addr] |= (uint8_t)(g_pic.latch << 4); g_pic.state = 0; }
            break;
        case 6:                                         /* read NVRAM */
            if (g_pic.state == 0) g_pic.state = 0x16;
            else if (g_pic.state == 0x16) { g_pic.nvram_addr = g_pic.latch & 0x0F; g_pic.state = 0x26; }
            else if (g_pic.state == 0x26) {
                g_pic.nvram_addr |= (g_pic.latch & 0x0F) << 4;
                g_pic.state = 0; g_pic.total = 0; g_pic.index = 0;
                g_pic.buffer[g_pic.total++] = g_pic.nvram[g_pic.nvram_addr];
            }
            break;
    }
}

/* Sound FIFO: CarnEvil streams sample data to the DCS through it (written
 * at 0x13000000, read by the DSP). */
#define SOUND_FIFO_SIZE 512
static struct { uint16_t data[SOUND_FIFO_SIZE]; int in, out, count, force_full; } g_fifo;

static uint16_t fifo_status(void) {
    uint16_t r = 0;
    if (g_fifo.count == 0 && !g_fifo.force_full) r |= 0x08;
    if (g_fifo.count >= SOUND_FIFO_SIZE / 2) r |= 0x10;
    if (g_fifo.count >= SOUND_FIFO_SIZE || g_fifo.force_full) r |= 0x20;
    return r;
}

static void ioasic_update_irq(void) {
    uint16_t bits = 0x2000 | (g_io.sound_irq & 0xFF) | (g_io.reg[IO_UARTIN] & 0x3F00);
    if (fifo_status() & 0x08) bits |= 0x0008;
    bits |= 1;
    g_io.reg[IO_INTSTAT] = bits;
    g_io.irq = (g_io.reg[IO_INTCTL] & 1) && (bits & g_io.reg[IO_INTCTL] & 0x3FFE);
    set_irq(1, g_io.irq);
}

static void fifo_reset(void) {
    g_fifo.in = g_fifo.out = g_fifo.count = g_fifo.force_full = 0;
    ioasic_update_irq();
}

static void fifo_write(uint16_t v) {
    if (g_fifo.count < SOUND_FIFO_SIZE) {
        g_fifo.data[g_fifo.in] = v;
        g_fifo.in = (g_fifo.in + 1) % SOUND_FIFO_SIZE;
        g_fifo.count++;
        ioasic_update_irq();
    }
    dcs_fifo_notify(g_fifo.count, SOUND_FIFO_SIZE);
}

static uint16_t fifo_read(void) {
    if (!g_fifo.count) return 0;
    uint16_t v = g_fifo.data[g_fifo.out];
    g_fifo.out = (g_fifo.out + 1) % SOUND_FIFO_SIZE;
    g_fifo.count--;
    ioasic_update_irq();
    return v;
}

static void dcs_input_empty(int state) {
    if (state) g_io.sound_irq |= 0x0080; else g_io.sound_irq &= ~0x0080;
    ioasic_update_irq();
}

static void dcs_output_full(int state) {
    if (state) g_io.sound_irq |= 0x0040; else g_io.sound_irq &= ~0x0040;
    ioasic_update_irq();
}

static void ioasic_reset(void) {
    g_io.shuffle = 0;
    g_io.sound_irq = 0x0080;
    g_io.reg[IO_INTCTL] = 0;
    g_pic.index = g_pic.state = g_pic.total = 0;
    fifo_reset();
}

static uint32_t ioasic_read(int r) {
    if (g_io.shuffle) r = k_carnevil_shuffle[r & 15];
    switch (r) {
        case IO_PORT0:  return g_io.shuffle ? g_inputs.dips : 0x2001;
        case IO_PORT1:  return g_inputs.system;
        case IO_PORT2:  return 0xFFFF;                  /* CarnEvil has no joysticks */
        case IO_PORT3:  return 0xFFFF;
        case IO_UARTIN: { uint16_t v = g_io.reg[r]; g_io.reg[r] &= ~0x1000; ioasic_update_irq(); return v; }
        case IO_SOUNDSTAT:
            return (((dcs_control_r() >> 4) ^ 0x40) & 0xC0) | (fifo_status() & 0x38) | (dcs_data2_r() & 0xFF00);
        case IO_SOUNDIN: return dcs_data_r();
        case IO_PICIN:  return pic_read() | (pic_status() << 8);
        default:        return g_io.reg[r];
    }
}

static void ioasic_write(int r, uint32_t v) {
    if (g_io.shuffle) r = k_carnevil_shuffle[r & 15];
    if (g_io.shuffle) g_io.reg[r] = (uint16_t)v;
    switch (r) {
        case IO_PORT0:
            if (v == 0xE2) { g_io.shuffle = 1; g_io.reg[IO_INTCTL] = 0; g_io.reg[IO_UARTCTL] = 0; }
            break;
        case IO_UARTOUT:
            if (g_io.reg[IO_UARTCTL] & 0x800) g_io.reg[IO_UARTIN] = (uint16_t)((v & 0xFF) | 0x3000);
            else { fputc((int)(v & 0xFF), stderr); g_io.reg[IO_UARTIN] |= 0x2000; }
            ioasic_update_irq();
            break;
        case IO_SOUNDCTL:
            dcs_reset_w(g_io.reg[r] & 1);
            if (!(g_io.reg[r] & 4)) fifo_reset();
            break;
        case IO_SOUNDOUT: dcs_data_w(g_io.reg[r]); break;
        case IO_SOUNDIN:  dcs_ack_w(); break;
        case IO_PICOUT: pic_write((uint8_t)v); break;
        case IO_INTCTL: ioasic_update_irq(); break;
    }
}

/* ======================================================================
 * Board control, CMOS, guns, video timing
 * ====================================================================== */
static uint32_t g_cmos[0x20000 / 4];
static int g_cmos_unlocked, g_cmos_dirty;
static const char* g_nvram_path;
static uint32_t g_int_enable, g_int_config, g_asic_reset, g_leds;
static int g_vblank_latch, g_vblank_state;
static uint8_t g_bootrom[0x80000];

static void vblank_update_irq(void) {
    int cfg = (g_int_config >> 14) & 3;
    for (int n = 3; n <= 5; n++) set_irq(n, 0);
    if (cfg) set_irq(2 + cfg, g_vblank_latch && (g_int_enable & 0x80));
}

static void vblank_edge(int state) {
    g_vblank_state = state;
    if ((state && !(g_int_enable & 0x100)) || (!state && (g_int_enable & 0x100))) {
        g_vblank_latch = 1;
        vblank_update_irq();
    }
}

/* Light guns. The gun board reports where on the screen the gun saw the beam:
 * a 12-bit X count and a 10-bit Y count, bit 0x40 when it saw light at all,
 * and the trigger (0x10) and pump (0x20). The counts at the screen edges are
 * the game's own factory calibration (what it writes to a blank CMOS):
 *   pixel  = (x - 252.78) / 3.8611          over 400 pixels
 *   line   = 256 - (y - y0) / 0.85185       y0 = 18.96, player 2: 39.11
 * so a gun aimed at a point reports the counts the game turns back into it.
 * Byte layout per MAME's seattle.cpp (carnevil_gun_r). */
static uint32_t gun_read(int off) {
    int p = (off >> 2) & 1;
    const gun_state* g = &g_inputs.gun[p];
    int seen = !(g->buttons & GUN_OFFSCREEN);
    uint32_t x = 0, y = 0;
    if (seen) {
        x = (uint32_t)(252.78 + 3.8611 * (g->x * 400.0 / 65536.0));
        y = (uint32_t)((p ? 39.11 : 18.96) + 0.85185 * (256.0 - g->y * 256.0 / 65536.0));
    }
    switch (off & 3) {
        case 0: return x & 0xFF;
        case 1: return ((x >> 8) & 0x0F) | ((g->buttons & (GUN_TRIGGER | GUN_PUMP)) << 4) | (seen ? 0x40 : 0);
        case 2: return y & 0xFF;
        default: return (y >> 8) & 0x03;
    }
}

static uint32_t board_read(uint32_t off) {
    switch (off) {
        case 0x01000000: return g_cmos_unlocked;
        case 0x01300000: return g_int_enable;
        case 0x01400000: return g_int_config;
        case 0x01500000: return (uint32_t)g_vblank_latch << 7;
        case 0x01600000: return ((uint32_t)g_vblank_latch << 7) | ((uint32_t)g_vblank_state << 8);
        case 0x01900000: return g_leds | 0xFFFFFF00u;
        case 0x01F00000: return g_asic_reset;
    }
    log_unmapped("board read", 0x16000000 + off, 0);
    return 0;
}

static void board_write(uint32_t off, uint32_t v) {
    switch (off) {
        case 0x01000000: g_cmos_unlocked = 1; return;
        case 0x01100000: return;                        /* watchdog */
        case 0x01300000: g_int_enable = v; vblank_update_irq(); return;
        case 0x01400000: g_int_config = v; vblank_update_irq(); return;
        case 0x01700000: g_vblank_latch = 0; vblank_update_irq(); return;
        case 0x01800000: return;
        case 0x01900000: g_leds = v & 0xFF; return;
        case 0x01F00000: g_asic_reset = v; if (!(v & 2)) ioasic_reset(); return;
    }
    log_unmapped("board write", 0x16000000 + off, v);
}

static void save_nvram(void) {
    if (!g_cmos_dirty || !g_nvram_path) return;
    FILE* f = fopen(g_nvram_path, "wb");
    if (f) { fwrite(g_cmos, 1, sizeof(g_cmos), f); fclose(f); g_cmos_dirty = 0; }
}

/* ======================================================================
 * Bus dispatch
 * ====================================================================== */
uint32_t hw_read(uint32_t p, int size) {
    uint32_t vb = voodoo_base();
    if (vb && p >= vb && p < vb + 0x1000000) {
        uint32_t v = voodoo_read(g_voodoo, p - vb);
        return size == 4 ? v : (v >> ((p & 3) * 8)) & (size == 2 ? 0xFFFF : 0xFF);
    }
    if (p >= 0x0A000000 && p < 0x0C000000) return pci_io_read(p - 0x0A000000, size);
    if (p >= 0x0C000000 && p < 0x0C001000) return gt_read(p - 0x0C000000);
    if (p >= 0x16000000 && p < 0x16000040) return ioasic_read((p >> 2) & 15);
    if (p >= 0x16100000 && p < 0x16120000) return g_cmos[(p - 0x16100000) >> 2];
    if (p >= 0x16800000 && p < 0x16800020) return gun_read((p >> 2) & 7);
    if (p >= 0x17000000 && p < 0x18000000) return board_read((p - 0x16000000) & ~3u);
    if (p >= 0x1FD00000 && p < 0x1FE00000) return 0;   /* diagnostics board socket: empty */
    if (p >= 0x1FC00000 && p < 0x1FC80000) {
        uint32_t o = p - 0x1FC00000;
        return size == 1 ? g_bootrom[o] : size == 2 ? *(uint16_t*)(g_bootrom + o) : *(uint32_t*)(g_bootrom + o);
    }
    log_unmapped("read", p, 0);
    return 0;
}

void hw_write(uint32_t p, uint32_t v, int size) {
    uint32_t vb = voodoo_base();
    if (vb && p >= vb && p < vb + 0x1000000) {
        if (size == 4) voodoo_write(g_voodoo, p - vb, v);
        else {
            uint32_t sh = (p & 3) * 8;
            voodoo_write_masked(g_voodoo, (p - vb) & ~3u, v << sh, (size == 2 ? 0xFFFFu : 0xFFu) << sh);
        }
        return;
    }
    if (p >= 0x0A000000 && p < 0x0C000000) { pci_io_write(p - 0x0A000000, v, size); return; }
    if (p >= 0x0C000000 && p < 0x0C001000) { gt_write(p - 0x0C000000, v); return; }
    if (p >= 0x16000000 && p < 0x16000040) { ioasic_write((p >> 2) & 15, v); return; }
    if (p >= 0x16100000 && p < 0x16120000) {
        if (g_trace_pci) fprintf(stderr, "[cmos] wr %08X = %08X size %d unlocked %d\n", p, v, size, g_cmos_unlocked);
        if (g_cmos_unlocked) { g_cmos[(p - 0x16100000) >> 2] = v; g_cmos_dirty = 1; }
        g_cmos_unlocked = 0;
        return;
    }
    if (p >= 0x16800000 && p < 0x16800020) return;
    if (p == 0x13000000) { fifo_write((uint16_t)v); return; }   /* sound FIFO (CS1) */
    if (p >= 0x17000000 && p < 0x18000000) { board_write((p - 0x16000000) & ~3u, v); return; }
    log_unmapped("write", p, v);
}

static uint32_t bus_read32(uint32_t p) {
    p = PHYS(p);
    return p < RAM_SIZE ? *(uint32_t*)(g_rdram + p) : hw_read(p, 4);
}
static void bus_write32(uint32_t p, uint32_t v) {
    p = PHYS(p);
    if (p < RAM_SIZE) *(uint32_t*)(g_rdram + p) = v; else hw_write(p, v, 4);
}

/* ======================================================================
 * Tick: video timing, timers, interrupt delivery
 * ====================================================================== */
#define VBLANK_HZ 57.0
static double g_next_vblank;
static double g_last_nvram_save;
static double g_last_tick;

#define VBLANK_LENGTH 0.0012   /* about 22 of the 400x256 mode's lines */
static double g_vblank_end;

static uint64_t g_ticks;

static uint64_t g_frames;
int g_turbo;
uint64_t hw_frame(void) { return g_frames; }

/* FNV-1a over main RAM, a word at a time: cheap enough every couple of seconds. */
uint32_t hw_ram_hash(void) {
    const uint32_t* w = (const uint32_t*)g_rdram;
    uint32_t h = 2166136261u;
    for (size_t i = 0; i < RAM_SIZE / 4; i++) h = (h ^ w[i]) * 16777619u;
    return h;
}
/* Hold emulated time to the wall clock: wait until the wall has caught up
 * with the frame about to start. A host that falls more than a quarter of a
 * second behind (a stall, a debugger, turbo just released) is let off rather
 * than made to race to catch up. */
static double g_wall_base;      /* wall time at which emulated time 0 would have been */
static void pace(double t) {
    double now = wall_s();
    if (g_turbo || now - (g_wall_base + t) > 0.25) { g_wall_base = now - t; return; }
    for (;;) {
        double ahead = g_wall_base + t - wall_s();
        if (ahead <= 0) break;
        if (ahead > 0.0015) Sleep(1);
    }
}

static void vblank_begin(double t) {
    g_frames++;
    static int n;
    static double last;
    static uint64_t last_ticks;
    if (++n % 57 == 0 && g_trace_perf) {
        double w = wall_s();
        fprintf(stderr, "[perf] %d vblanks, %.2fs wall for the last 57, %llu ticks, %llu sectors read\n", n, w - last,
                (unsigned long long)(g_ticks - last_ticks), (unsigned long long)g_sectors_read);
        last = w; last_ticks = g_ticks;
    }
    pace(t);
    int w, h, stride;
    const uint16_t* fb = voodoo_display(g_voodoo, &w, &h, &stride);
    video_present(fb, stride, w, h);
    if (!video_pump()) { hw_shutdown(); exit(0); }
    voodoo_vblank(g_voodoo, 1);
    vblank_edge(1);
    g_vblank_end = t + VBLANK_LENGTH;
    if (t - g_last_nvram_save > 5.0) { save_nvram(); g_last_nvram_save = t; }
}

void hw_tick(void) {
    g_hw_budget = HW_TICK_LOADS;
    g_ticks++;
    g_cycles += TICK_CYCLES;
    double t = hw_time();
    dcs_run(t - g_last_tick);
    g_last_tick = t;
    if (g_vblank_end && t >= g_vblank_end) {
        g_vblank_end = 0;
        voodoo_vblank(g_voodoo, 0);
        vblank_edge(0);
    }
    if (t >= g_next_vblank) {
        g_next_vblank += 1.0 / VBLANK_HZ;
        vblank_begin(t);
    }
    gt_timers_update(t);
    deliver_irqs();
}

/* A host-side wait that runs no MIPS code (the process dispatcher with
 * nothing to run): let a tick's worth of emulated time pass. */
void hw_idle(void) {
    hw_tick();
}

/* ======================================================================
 * Init / shutdown
 * ====================================================================== */
static void load_file(const char* path, void* dst, size_t max, size_t* got) {
    FILE* f = fopen(path, "rb");
    if (!f) hw_fatal("cannot open %s", path);
    size_t n = fread(dst, 1, max, f);
    fclose(f);
    if (got) *got = n;
}

const uint8_t* hw_bootrom(void) { return g_bootrom; }

void hw_init(const hw_config* cfg) {
    g_trace_irq = getenv("CARNEVIL_IRQ_TRACE") != NULL;
    g_trace_pci = getenv("CARNEVIL_PCI_TRACE") != NULL;
    g_trace_perf = getenv("CARNEVIL_PERF") != NULL;
    if (getenv("CARNEVIL_TURBO")) g_turbo = 1;
    QueryPerformanceFrequency(&g_qpf);
    QueryPerformanceCounter(&g_qpc0);
    timeBeginPeriod(1);

    g_rdram = calloc(1, RAM_SIZE);
    load_file(cfg->bootrom, g_bootrom, sizeof(g_bootrom), NULL);

    g_ide.img = fopen(cfg->disk, "rb");
    if (!g_ide.img) hw_fatal("cannot open disk image %s", cfg->disk);
    g_ide.status = 0x50;

    g_nvram_path = cfg->nvram;
    memset(g_cmos, 0xFF, sizeof(g_cmos));               /* a never-initialised CMOS reads all ones */
    FILE* f = fopen(cfg->nvram, "rb");
    if (f) { fread(g_cmos, 1, sizeof(g_cmos), f); fclose(f); }
    memset(g_pic.nvram, 0xFF, sizeof(g_pic.nvram));
    pic_generate_serial(486);

    /* PCI devices */
    g_pci_galileo.cfg[0] = 0x014611AB; g_pci_galileo.cfg[2] = 0x06000001;
    g_pci_voodoo.cfg[0]  = 0x0001121A; g_pci_voodoo.cfg[2]  = 0x04000002;
    g_pci_voodoo.bar_size[0] = 0x1000000;
    g_pci_ide.cfg[0]     = 0x0002100B; g_pci_ide.cfg[2]     = 0x01018A01;
    g_pci_ide.bar_size[0] = 8; g_pci_ide.bar_size[1] = 4; g_pci_ide.bar_size[2] = 8;
    g_pci_ide.bar_size[3] = 4; g_pci_ide.bar_size[4] = 16;
    for (int i = 0; i < 5; i++) g_pci_ide.cfg[4 + i] = 1;  /* I/O BARs */

    g_voodoo = voodoo_create();
    {
        static uint8_t rom[0x8000];
        size_t n = 0;
        load_file(cfg->dcs_rom, rom, sizeof(rom), &n);
        dcs_init(rom, n);
        static const dcs_callbacks cb = { fifo_read, fifo_status, dcs_input_empty, dcs_output_full };
        dcs_set_callbacks(&cb);
    }
    ioasic_reset();
    g_io.reg[IO_SOUNDCTL] = 1;
    g_last_tick = now_s();
    g_next_vblank = g_last_tick + 1.0 / VBLANK_HZ;
}

void hw_shutdown(void) {
    save_nvram();
    if (g_ide.noverlay) fprintf(stderr, "[ide] %d sectors written this session (kept in memory only)\n", g_ide.noverlay);
}
