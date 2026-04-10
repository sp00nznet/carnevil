/**
 * CarnEvil Seattle Runtime
 *
 * Hardware shim layer for the Midway Seattle arcade board.
 */

#include "recomp.h"
#include "voodoo.h"
#include "input.h"
#include "dcs_sound.h"
#include "rtos_scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ======================================================================
 * Memory -- 8MB RAM
 * ====================================================================== */

#define RAM_SIZE (8 * 1024 * 1024)
static uint8_t g_rdram[RAM_SIZE];
uint8_t* seattle_rdram = g_rdram;

/* ======================================================================
 * Hardware I/O
 *
 * Physical address map (from MAME seattle.cpp):
 *   0x00000000-0x007FFFFF  RAM (8MB)
 *   0x08000000-0x08100000  Galileo GT64010 registers
 *   0x0A000000-0x0A0003FF  IDE controller (PC87415)
 *   0x0C000000-0x0C0000FF  Galileo expansion (UART, etc.)
 *   0x13000000-0x13000003  Status LEDs
 *   0x14000000-0x14000003  CMOS unlock
 *   0x14100000-0x141FFFFF  CMOS/NVRAM (32KB)
 *   0x14300000-0x14300003  Ethernet
 *   0x16000000-0x160FFFFF  IOASIC
 *   0x16100000-0x161FFFFF  CMOS protect
 *   0x16800000-0x168FFFFF  Analog inputs (lightguns)
 *   0x16C00000-0x16CFFFFF  Status register
 *   0x17000000-0x170FFFFF  PCI bridge config
 *   0x17500000-0x175FFFFF  Ethernet data
 *   Voodoo mapped via PCI BAR (typically 0x08100000+)
 * ====================================================================== */

/* Voodoo GPU (non-static: accessed by seattle_overrides.c DMA consumer) */
voodoo_state_t g_voodoo;

/* Input system */
static input_state_t g_input;

/* DCS2 sound */
static dcs_state_t g_dcs;

/* I/O register state */
static struct {
    /* Galileo GT64010 */
    uint32_t galileo_regs[256];

    /* IOASIC */
    uint32_t ioasic_regs[64];
    uint32_t input_state;       /* DIP switches, buttons, coins */

    /* Timers */
    uint32_t timer_count;

    /* CMOS */
    uint8_t cmos[32768];

    /* Status */
    uint32_t status_leds;

    /* PCI configuration space */
    uint32_t pci_config_addr;

    /* Misc */
    int io_log_enabled;
} io;

/* I/O region names for debug logging */
static const char* io_region_name(uint32_t paddr) {
    if (paddr >= 0x08000000 && paddr < 0x08100000) return "GALILEO";
    if (paddr >= 0x08100000 && paddr < 0x09000000) return "VOODOO";
    if (paddr >= 0x0A000000 && paddr < 0x0A001000) return "IDE";
    if (paddr >= 0x13000000 && paddr < 0x14000000) return "LEDS";
    if (paddr >= 0x14000000 && paddr < 0x14100000) return "CMOS_UNLOCK";
    if (paddr >= 0x14100000 && paddr < 0x14200000) return "CMOS";
    if (paddr >= 0x16000000 && paddr < 0x16100000) return "IOASIC";
    if (paddr >= 0x16800000 && paddr < 0x16900000) return "LIGHTGUN";
    if (paddr >= 0x16C00000 && paddr < 0x16D00000) return "STATUS";
    if (paddr >= 0x17000000 && paddr < 0x17100000) return "PCI_BRIDGE";
    return "UNKNOWN";
}

uint32_t seattle_io_read32(uint32_t paddr) {
    /* Polling detector: if the same address is read > 200 times in a row,
     * return a "busy done" value to break infinite polling loops. */
    static uint32_t last_poll_addr = 0;
    static int poll_count = 0;
    if (paddr == last_poll_addr) {
        poll_count++;
        if (poll_count > 200) {
            static int poll_break_log = 0;
            if (poll_break_log < 10) {
                poll_break_log++;
                fprintf(stderr, "[IO] Polling break: addr=0x%08X after %d reads\n", paddr, poll_count);
            }
            /* Return non-zero to break common polling patterns */
            poll_count = 0;
            return 0xFFFFFFFF;
        }
    } else {
        last_poll_addr = paddr;
        poll_count = 0;
    }

    /* Galileo GT64010 internal registers */
    if (paddr >= 0x08000000 && paddr < 0x08100000) {
        uint32_t reg_off = paddr - 0x08000000;
        uint32_t reg = reg_off >> 2;
        if (reg < 256) {
            switch (reg_off) {
                case 0x000: return 0x00000400; /* CPU Interface Config: 32-bit, LE */
                case 0x850: /* Timer/Counter 0 */
                    io.timer_count += 1000;
                    return io.timer_count;
                case 0x854: /* Timer/Counter 1 */
                    return io.timer_count / 2;
                case 0x858: /* Timer/Counter 2 */
                    return io.timer_count / 4;
                case 0x85C: /* Timer/Counter 3 */
                    return io.timer_count / 8;
                case 0xC18: /* PCI Internal Command */
                    return 0x07; /* master enable, memory enable, IO enable */
                case 0xC3C: /* PCI Interrupt Acknowledge */
                    return 0;
                default:
                    return io.galileo_regs[reg];
            }
        }
    }

    /* Voodoo LFB reads: 0x00800000-0x017FFFFF
     * Read back pixel data from the back buffer. */
    if (paddr >= 0x00800000 && paddr < 0x01800000) {
        uint32_t lfb_offset = paddr - 0x00800000;
        uint32_t pixel_offset = lfb_offset / 2;
        uint32_t val = 0;
        if (pixel_offset < 640 * 480) {
            val = g_voodoo.backbuffer[pixel_offset];
            if (pixel_offset + 1 < 640 * 480) {
                val |= ((uint32_t)g_voodoo.backbuffer[pixel_offset + 1]) << 16;
            }
        }
        return val;
    }

    /* Voodoo register reads: 0x08100000-0x08FFFFFF */
    if (paddr >= 0x08100000 && paddr < 0x09000000) {
        uint32_t voodoo_offset = paddr - 0x08100000;
        uint32_t rv = voodoo_read(&g_voodoo, voodoo_offset);
        static int vrd_log = 0;
        vrd_log++;
        if (vrd_log <= 20 && voodoo_offset > 0) {
            fprintf(stderr, "[voodoo_rd] paddr=0x%08X off=0x%06X val=0x%08X\n",
                    paddr, voodoo_offset, rv);
        }
        return rv;
    }

    /* IOASIC */
    if (paddr >= 0x16000000 && paddr < 0x16100000) {
        uint32_t reg = (paddr >> 2) & 0x3F;
        switch (reg) {
            case 0: /* Input register 0 -- switches, service, etc. */
                return g_input.ioasic_buttons;
            case 1: /* Input register 1 */
                return 0xFFFFFFFF;
            case 2: /* Input register 2 */
                return 0xFFFFFFFF;
            case 3: /* Status */
                return 0x0000D000; /* PIC present, sound ready */
            default:
                return io.ioasic_regs[reg];
        }
    }

    /* CMOS/NVRAM */
    if (paddr >= 0x14100000 && paddr < 0x14200000) {
        uint32_t offset = (paddr - 0x14100000) & 0x7FFF;
        return io.cmos[offset];
    }

    /* Widget board / analog input registers (0x16800000-0x168FFFFF).
     * Registers 0-7 provide video DAC configuration + lightgun analog data.
     * func_800D4C24 reads bytes from these to determine display resolution.
     * For CarnEvil 512x384:
     *   Reg 0: H-res low byte (512 & 0xFF = 0x00)
     *   Reg 1: H-res high nibble + mode flags (0x42 = high=2, bit6=active)
     *   Reg 2: H-res2 low (same as reg 0)
     *   Reg 3: H-res2 high + flags (same as reg 1)
     *   Reg 4: V-res low byte (384 & 0xFF = 0x80)
     *   Reg 5: V-res high nibble + flags (0x41 = high=1, bit6=active)
     *   Reg 6: V-res2 low (same as reg 4)
     *   Reg 7: V-res2 high + flags (same as reg 5) */
    if (paddr >= 0x16800000 && paddr < 0x16900000) {
        uint32_t reg = (paddr - 0x16800000) >> 2;
        switch (reg) {
            case 0: return 0x00;   /* H-res low: 512 & 0xFF */
            case 1: return 0x72;   /* H-res high nibble(2) + bit4(render) + bit5(display ready) + bit6(active) */
            case 2: return 0x00;   /* H-res2 low */
            case 3: return 0x72;   /* H-res2 high + bit4(render) + bit5(display ready) + bit6(active) */
            case 4: return 0x80;   /* V-res low: 384 & 0xFF */
            case 5: return 0x71;   /* V-res high nibble(1) + bit4(render) + bit5(display ready) + bit6(active) */
            case 6: return 0x80;   /* V-res2 low */
            case 7: return 0x71;   /* V-res2 high + bit4(render) + bit5(display ready) + bit6(active) */
            default: return 0x80;  /* center (lightgun default) */
        }
    }

    /* Status register - advance frame counters to simulate vsync */
    if (paddr >= 0x16C00000 && paddr < 0x16D00000) {
        static int status_polls = 0;
        status_polls++;
        /* Advance sync counters when polled (game checks VBLANK status in tight loops) */
        if (status_polls % 100 == 0) {
            uint32_t* vbl = (uint32_t*)(g_rdram + 0x001A35CC);
            uint32_t* sync1 = (uint32_t*)(g_rdram + 0x001A1AB0);
            uint32_t* sync2 = (uint32_t*)(g_rdram + 0x001A1AB8);
            (*vbl)++;
            (*sync1)++;
            (*sync2) = (*sync1) - 1;
        }
        /* Toggle VBLANK active bit */
        return (status_polls & 1) ? 0x00000020 : 0x00000000;
    }

    /* CS3 extended reads (0x17000000-0x17FFFFFF) */
    if (paddr >= 0x17000000 && paddr < 0x18000000) {
        uint32_t cs3_off = paddr - 0x16000000;
        switch (cs3_off & 0x01F00000) {
            case 0x01300000: return io.galileo_regs[0xF0]; /* Interrupt enable */
            case 0x01400000: return io.galileo_regs[0xF1]; /* Interrupt config */
            case 0x01500000: return 0; /* Interrupt state - no pending interrupts */
            case 0x01600000: return 0; /* Interrupt state 2 */
            default: return 0;
        }
    }

    /* IDE controller (0x0A000000-0x0BFFFFFF) */
    if (paddr >= 0x0A000000 && paddr < 0x0C000000) {
        uint32_t ide_off = paddr & 0xFFFF;
        /* IDE status register: return "ready, seek complete" */
        if (ide_off == 0x01F7) return 0x50;
        if (ide_off == 0x03F6) return 0x50;
        return 0;
    }

    /* PCI configuration space (Galileo GT64010 PCI bridge)
     * 0x0C000CF8 = PCI Config Address register
     * 0x0C000CFC = PCI Config Data register
     * 0x0C000C18 = PCI Command/Status register */
    if (paddr >= 0x0C000000 && paddr < 0x0D000000) {
        uint32_t pci_off = paddr & 0xFFF;
        switch (pci_off) {
            case 0xCF8: return io.pci_config_addr;
            case 0xCFC: {
                /* PCI Config Data: decode the address to determine device and register */
                uint32_t addr = io.pci_config_addr;
                if (!(addr & 0x80000000)) return 0;
                uint32_t dev = (addr >> 11) & 0x1F;
                uint32_t reg = addr & 0xFC;

                if (dev == 0) {
                    /* Device 0 = Galileo GT64010 */
                    switch (reg) {
                        case 0x00: return 0x464011AB; /* Vendor=0x11AB(Marvell), Device=0x4640(GT64010) */
                        case 0x08: return 0x06800001; /* Class=Host Bridge, Rev=1 */
                        case 0x04: return 0x00000007; /* Command: master + memory + IO enable */
                        case 0x10: return 0x08000000; /* BAR0: internal registers at 0x08000000 */
                        default: return 0;
                    }
                }
                if (dev == 8) {
                    /* Device 8 = 3dfx Voodoo 1 */
                    switch (reg) {
                        case 0x00: return 0x0001121A; /* Vendor=0x121A(3dfx), Device=0x0001(Voodoo) */
                        case 0x08: return 0x03000002; /* Class=VGA, Rev=2 */
                        case 0x04: return 0x00000002; /* Command: memory enable */
                        case 0x10: return 0x08100000; /* BAR0: registers at 0x08100000 */
                        case 0x14: return 0x00800000; /* BAR1: LFB at 0x00800000 */
                        default: return voodoo_read(&g_voodoo, reg);
                    }
                }
                if (dev == 9) {
                    /* Device 9 = IDE controller (NS PC87415) */
                    switch (reg) {
                        case 0x00: return 0x0002100B; /* Vendor=0x100B(NS), Device=0x0002(PC87415) */
                        case 0x08: return 0x01018001; /* Class=IDE, Rev=1 */
                        case 0x10: return 0x0A0001F0; /* BAR0: IDE primary at 0x0A0001F0 */
                        default: return 0;
                    }
                }
                return 0xFFFFFFFF; /* No device */
            }
            case 0xC18: return io.galileo_regs[0xC18 / 4]; /* PCI Internal Command */
            default: return io.galileo_regs[(pci_off & 0xFFF) / 4];
        }
    }

    if (io.io_log_enabled) {
        fprintf(stderr, "[IO] READ32  %s 0x%08X\n", io_region_name(paddr), paddr);
    }
    return 0;
}

void seattle_io_write32(uint32_t paddr, uint32_t value) {
    /* Galileo GT64010 */
    if (paddr >= 0x08000000 && paddr < 0x08100000) {
        uint32_t reg_off = paddr - 0x08000000;
        uint32_t reg = reg_off >> 2;
        if (reg < 256) io.galileo_regs[reg] = value;

        /* DMA Control register writes trigger DMA transfers.
         * GT64010 DMA channels (offsets from Galileo base 0x08000000):
         *   0x800+ch*4: Byte Count (ch 0-3)
         *   0x810+ch*4: Source Address (physical RAM)
         *   0x820+ch*4: Destination Address (PCI bus)
         *   0x830+ch*4: Next Record Pointer
         *   0x840+ch*4: Control (bit 12 = channel enable) */
        if (reg_off >= 0x840 && reg_off < 0x850 && (value & (1 << 12))) {
            int ch = (reg_off - 0x840) / 4;
            uint32_t byte_count = io.galileo_regs[(0x800 + ch*4) / 4];
            uint32_t src_phys   = io.galileo_regs[(0x810 + ch*4) / 4];
            uint32_t dst_pci    = io.galileo_regs[(0x820 + ch*4) / 4];
            uint32_t next_ptr   = io.galileo_regs[(0x830 + ch*4) / 4];

            static int dma_total = 0;
            dma_total++;

            int chain = 0;
            while (chain < 256) {
                chain++;
                if (byte_count == 0 && chain > 1) break;

                if (src_phys < 0x00800000 && byte_count > 0 && byte_count < 0x100000) {
                    uint32_t voff = (dst_pci >= 0x08100000) ? dst_pci - 0x08100000 : dst_pci;

                    if (dma_total <= 10) {
                        fprintf(stderr, "[dma] Ch%d: src=0x%06X dst=0x%08X count=%u voodoo=0x%06X\n",
                                ch, src_phys, dst_pci, byte_count, voff);
                    }

                    /* Transfer data from RAM to Voodoo as 32-bit register writes */
                    for (uint32_t i = 0; i < byte_count && src_phys + i + 4 <= 0x00800000; i += 4) {
                        uint32_t val = *(uint32_t*)(g_rdram + src_phys + i);
                        voodoo_write(&g_voodoo, voff + i, val);
                    }
                }

                if (next_ptr == 0 || next_ptr >= 0x00800000) break;
                uint32_t np = next_ptr & 0x1FFFFFFF;
                if (np + 16 > 0x00800000) break;
                byte_count = *(uint32_t*)(g_rdram + np + 0);
                src_phys   = *(uint32_t*)(g_rdram + np + 4);
                dst_pci    = *(uint32_t*)(g_rdram + np + 8);
                next_ptr   = *(uint32_t*)(g_rdram + np + 12);
            }

            io.galileo_regs[(0x840 + ch*4) / 4] &= ~(1 << 12);
        }
        return;
    }

    /* Voodoo LFB (Linear Frame Buffer) writes: 0x00800000-0x017FFFFF
     * These are DIRECT PIXEL writes to the framebuffer, NOT register commands.
     * On Voodoo 1, LFB writes go to the framebuffer memory. The format
     * depends on lfbMode register (typically 16-bit RGB565 or 32-bit ARGB). */
    if (paddr >= 0x00800000 && paddr < 0x01800000) {
        uint32_t lfb_offset = paddr - 0x00800000;
        /* Voodoo 1 LFB is organized as rows of pixels.
         * Each pixel is 16-bit (2 bytes) in RGB565 format.
         * A 32-bit write sets 2 adjacent pixels. */
        uint32_t pixel_offset = lfb_offset / 2; /* 16bpp = 2 bytes per pixel */
        if (pixel_offset < g_voodoo.width * g_voodoo.height * 2) {
            /* Write two 16-bit pixels to BACK buffer */
            uint16_t pixel_lo = (uint16_t)(value & 0xFFFF);
            uint16_t pixel_hi = (uint16_t)(value >> 16);
            if (pixel_offset < 640 * 480) {
                g_voodoo.backbuffer[pixel_offset] = pixel_lo;
            }
            if (pixel_offset + 1 < 640 * 480) {
                g_voodoo.backbuffer[pixel_offset + 1] = pixel_hi;
            }
        }
        static int lfb_log = 0;
        static uint32_t lfb_writes = 0;
        lfb_writes++;
        if (lfb_log < 5 && value != 0) {
            lfb_log++;
            fprintf(stderr, "[voodoo_lfb] offset=0x%06X pixel=%u val=0x%08X\n",
                    lfb_offset, pixel_offset, value);
        }
        if (lfb_writes % 100000 == 0) {
            fprintf(stderr, "[voodoo_lfb] %u LFB writes total\n", lfb_writes);
        }
        return;
    }

    /* Voodoo register writes: 0x08100000-0x08FFFFFF */
    if (paddr >= 0x08100000 && paddr < 0x09000000) {
        uint32_t voodoo_offset = paddr - 0x08100000;
        static int vlog = 0;
        if (value != 0 && vlog < 10) {
            vlog++;
            fprintf(stderr, "[IO] Voodoo write: paddr=0x%08X offset=0x%08X val=0x%08X\n",
                    paddr, voodoo_offset, value);
        }
        voodoo_write(&g_voodoo, voodoo_offset, value);
        return;
    }

    /* IOASIC */
    if (paddr >= 0x16000000 && paddr < 0x16100000) {
        uint32_t reg = (paddr >> 2) & 0x3F;
        io.ioasic_regs[reg] = value;

        /* Trace IOASIC writes to find rendering DMA commands */
        static int ioasic_log_count = 0;
        ioasic_log_count++;
        if (ioasic_log_count <= 200) {
            fprintf(stderr, "[ioasic] WRITE reg %2d (0x%08X) = 0x%08X\n",
                    reg, paddr, value);
        } else if (ioasic_log_count == 201) {
            fprintf(stderr, "[ioasic] ... suppressing (%d writes so far)\n", ioasic_log_count);
        }
        if (ioasic_log_count == 10000) {
            fprintf(stderr, "[ioasic] Total IOASIC writes: %d\n", ioasic_log_count);
        }
        return;
    }

    /* CMOS */
    if (paddr >= 0x14100000 && paddr < 0x14200000) {
        uint32_t offset = (paddr - 0x14100000) & 0x7FFF;
        io.cmos[offset] = (uint8_t)value;
        return;
    }

    /* LEDs */
    if (paddr >= 0x13000000 && paddr < 0x14000000) {
        io.status_leds = value;
        return;
    }

    /* CS3 extended I/O registers (0x16000000-0x1FFFFFFF, offset by CS3 base 0x16000000)
     * These are Seattle board control registers, NOT Galileo internal regs.
     * From MAME seattle.cpp seattle_cs3_map:
     *   +0x1100000 (0x17100000) = Watchdog kick
     *   +0x1300000 (0x17300000) = Interrupt enable
     *   +0x1400000 (0x17400000) = Interrupt config
     *   +0x1700000 (0x17700000) = VBLANK clear
     *   +0x1900000 (0x17900000) = Status LEDs
     *   +0x1F00000 (0x17F00000) = ASIC reset */
    if (paddr >= 0x17000000 && paddr < 0x18000000) {
        uint32_t cs3_off = paddr - 0x16000000;
        switch (cs3_off & 0x01F00000) {
            case 0x01100000: /* Watchdog - just acknowledge */
                break;
            case 0x01300000: /* Interrupt enable */
                io.galileo_regs[0xF0] = value; /* stash in spare slot */
                break;
            case 0x01400000: /* Interrupt config */
                io.galileo_regs[0xF1] = value;
                break;
            case 0x01700000: /* VBLANK clear */
                break;
            case 0x01900000: /* Status LEDs */
                io.status_leds = value;
                break;
            case 0x01F00000: /* ASIC reset */
                break;
            default: {
                static int cs3_log = 0;
                cs3_log++;
                if (cs3_log <= 20) {
                    fprintf(stderr, "[cs3] WRITE 0x%08X = 0x%08X\n", paddr, value);
                }
                break;
            }
        }
        return;
    }

    /* PCI configuration space writes */
    if (paddr >= 0x0C000000 && paddr < 0x0D000000) {
        uint32_t pci_off = paddr & 0xFFF;
        static int pci_write_log = 0;
        pci_write_log++;
        if (pci_write_log <= 30) {
            fprintf(stderr, "[pci_io] WRITE pci_off=0x%03X val=0x%08X (paddr=0x%08X)\n",
                    pci_off, value, paddr);
        }
        switch (pci_off) {
            case 0xCF8:
                io.pci_config_addr = value;
                break;
            case 0xCFC: {
                /* PCI Config Data write: dispatch to device */
                uint32_t addr = io.pci_config_addr;
                if (!(addr & 0x80000000)) break;
                uint32_t dev = (addr >> 11) & 0x1F;
                uint32_t reg = addr & 0xFC;
                if (dev == 8) {
                    /* Device 8 = Voodoo */
                    static int pci_voodoo_writes = 0;
                    pci_voodoo_writes++;
                    if (pci_voodoo_writes <= 30) {
                        fprintf(stderr, "[pci] Voodoo write: reg 0x%02X = 0x%08X\n", reg, value);
                    }
                    voodoo_write(&g_voodoo, reg, value);
                } else if (dev == 9) {
                    /* Device 9 = IDE controller */
                } else {
                    static int pci_other = 0;
                    pci_other++;
                    if (pci_other <= 10) {
                        fprintf(stderr, "[pci] Dev %d reg 0x%02X = 0x%08X\n", dev, reg, value);
                    }
                }
                break;
            }
            default:
                io.galileo_regs[(pci_off & 0xFFF) / 4] = value;
                break;
        }
        return;
    }

    /* IDE controller (0x0A000000) */
    if (paddr >= 0x0A000000 && paddr < 0x0C000000) {
        static int ide_log = 0;
        ide_log++;
        if (ide_log <= 20) {
            fprintf(stderr, "[ide] WRITE 0x%08X = 0x%08X\n", paddr, value);
        }
        return;
    }

    /* Log ALL unhandled writes */
    static int unhandled_log = 0;
    unhandled_log++;
    if (unhandled_log <= 50) {
        fprintf(stderr, "[IO] UNHANDLED WRITE 0x%08X = 0x%08X\n", paddr, value);
    }
}

uint16_t seattle_io_read16(uint32_t paddr) { return (uint16_t)seattle_io_read32(paddr & ~3); }
void     seattle_io_write16(uint32_t paddr, uint16_t value) { seattle_io_write32(paddr & ~3, value); }
uint8_t  seattle_io_read8(uint32_t paddr) { return (uint8_t)seattle_io_read32(paddr & ~3); }
void     seattle_io_write8(uint32_t paddr, uint8_t value) { seattle_io_write32(paddr & ~3, value); }

/* ======================================================================
 * Function lookup
 * ====================================================================== */

#include "funcs.h"

typedef struct { int32_t vram; recomp_func_t* func; } func_entry_t;
static func_entry_t g_func_table[8192];
static int g_func_count = 0;

void seattle_register_func(int32_t vram, recomp_func_t* func) {
    if (g_func_count < 4096) {
        g_func_table[g_func_count].vram = vram;
        g_func_table[g_func_count].func = func;
        g_func_count++;
    }
}

static int reboot_count = 0;
static void reboot_handler(uint8_t* rdram, recomp_context* ctx) {
    reboot_count++;
    uint32_t str_phys = (uint32_t)ctx->r4 & 0x1FFFFFFF;
    const char* reason = "unknown";
    if (str_phys < 0x00800000 - 64) {
        reason = (const char*)(rdram + str_phys);
    }
    if (reboot_count <= 3) {
        fprintf(stderr, "[seattle] REBOOT #%d -- reason: \"%s\" (r2=%d r29=0x%08X)\n",
                reboot_count, reason, (int32_t)ctx->r2, (uint32_t)ctx->r29);
    }
    /* Force success and prevent the reboot loop */
    ctx->r2 = 0;
}

static void null_stub(uint8_t* rdram, recomp_context* ctx) {
    /* Safe no-op for unresolved function pointers */
}

recomp_func_t* get_function(int32_t vram) {
    /* Log vector table lookups to trace RTOS API usage */
    uint32_t uv = (uint32_t)vram;
    if (uv >= 0x800C408C && uv <= 0x800C42CC) {
        static uint32_t vec_counts[72] = {0};
        int idx = (uv - 0x800C408C) / 8;
        if (idx < 72) {
            vec_counts[idx]++;
            if (vec_counts[idx] <= 3) {
                fprintf(stderr, "[vec] Lookup vec[%d] = 0x%08X (call #%d)\n",
                        idx, uv, vec_counts[idx]);
            }
        }
    }

    for (int i = 0; i < g_func_count; i++) {
        if (g_func_table[i].vram == vram) return g_func_table[i].func;
    }

    /* Special addresses */
    if ((uint32_t)vram == 0xBFC00000) {
        return reboot_handler;
    }

    static int unknown_count = 0;
    unknown_count++;
    if (unknown_count <= 10) {
        fprintf(stderr, "[seattle] LOOKUP_FUNC: unknown 0x%08X -- stubbing as no-op\n", (uint32_t)vram);
    }
    return null_stub;
}

/* ======================================================================
 * IOASIC Command Handler
 *
 * Replaces the NOP stub at 0x800C4154 to handle IOASIC protocol.
 * Called as: ioasic_command(r4=type, r5=command_id, r6=response_ptr)
 * ====================================================================== */

void ioasic_command_handler(uint8_t* rdram, recomp_context* ctx) {
    uint32_t cmd_type = (uint32_t)ctx->r4;
    uint32_t cmd_id = (uint32_t)ctx->r5;
    uint32_t resp_addr = (uint32_t)ctx->r6;
    uint32_t resp_phys = resp_addr & PHYS_MASK;
    int32_t* resp = NULL;

    if (resp_phys < RAM_SIZE_HW - 4) {
        resp = (int32_t*)(rdram + resp_phys);
    }

    switch (cmd_id) {
    /* System status commands */
    case 0x7000: /* Get system status */
        if (resp) *resp = 0x0001; /* Status OK */
        break;
    case 0x7001: /* Get IOASIC version */
        if (resp) *resp = 0x0001;
        break;
    case 0x7002: /* Get board type / game ID */
        if (resp) *resp = 0x0000;
        break;

    /* PIC security chip */
    case 0x6300: /* PIC reset/init */
        if (resp) *resp = 0x0000; /* Success */
        break;
    case 0x6301: /* PIC challenge */
        if (resp) *resp = 0x0000; /* No challenge */
        break;
    case 0x6302: /* PIC response verify */
        if (resp) *resp = 0x0000; /* Verified OK */
        break;

    /* DCS2 sound commands */
    case 0x6901: /* Sound board version */
        if (resp) *resp = 0x0003; /* DCS version 3 */
        break;
    case 0x6902: /* Sound board status */
        if (resp) *resp = 0x0001; /* Ready */
        break;
    case 0x6903: /* Sound board ready */
        if (resp) *resp = 0x0001; /* Ready */
        break;
    case 0x6905: /* Send sound command */
        ctx->r2 = 0; /* Success */
        break;
    case 0x6907: /* Sound volume */
        if (resp) *resp = 0x0000;
        break;
    case 0x690E: /* Sound test */
        if (resp) *resp = 0x0000;
        break;
    case 0x6909: /* DCS sound buffer status */
        if (resp) *resp = 0x0001; /* Buffer ready/done */
        break;
    case 0x690F: /* DCS sound sync */
        if (resp) *resp = 0x0001;
        break;
    case 0x6910: /* DCS sound init complete */
        if (resp) *resp = 0x0001;
        break;

    /* IOASIC status commands */
    case 0x7403: /* IOASIC input poll */
        if (resp) *resp = 0xFFFFFFFF; /* All buttons released */
        break;
    case 0x7405: /* IOASIC status/ready check */
        if (resp) *resp = 0x0001; /* Ready */
        break;
    case 0x7406: /* IOASIC sync */
        if (resp) *resp = 0x0001;
        break;
    case 0x740B: /* IOASIC extended poll */
        if (resp) *resp = 0xFFFFFFFF;
        break;

    default:
        if (io.io_log_enabled) {
            fprintf(stderr, "[ioasic] cmd type=%d id=0x%04X resp_addr=0x%08X\n",
                    cmd_type, cmd_id, resp_addr);
        }
        /* Return non-zero to break polling loops for unhandled commands */
        if (resp) *resp = 0x0001;
        break;
    }

    ctx->r2 = 0; /* Return success */
}

/* RTOS task table -- shared with seattle_overrides.c */
typedef struct { int id; uint32_t callback_vram; } seattle_task_t;
seattle_task_t task_table[16];
int task_count = 0;

void rtos_create_task(uint8_t* rdram, recomp_context* ctx) {
    int task_id = (int)ctx->r4;
    uint32_t callback = (uint32_t)ctx->r5;

    if (task_count < 16) {
        task_table[task_count].id = task_id;
        task_table[task_count].callback_vram = callback;
        task_count++;
    }

    if (task_count <= 3) {
        fprintf(stderr, "[rtos] create_task(%d, func_%08X)\n", task_id, callback);
    }

    ctx->r2 = 0; /* Success */
}

/* Tracing */
static int trace_enabled = 1;
static int frame_count = 0;

/* Hook called from main_loop to trace game state */
void seattle_trace_frame(uint8_t* rdram, recomp_context* ctx) {
    if (!trace_enabled) return;
    frame_count++;

    /* Read game state from known globals */
    uint32_t tick  = *(uint32_t*)(rdram + 0x001A35C8);  /* tick_counter */
    uint32_t frame = *(uint32_t*)(rdram + 0x001A35CC);  /* frame_counter */
    uint32_t input = *(uint32_t*)(rdram + 0x002122E0);  /* input_state */

    if (frame_count <= 20 || (frame_count % 100 == 0)) {
        fprintf(stderr, "[trace] frame=%d tick=%u vblank=%u input=0x%08X sp=0x%08X\n",
                frame_count, tick, frame, input, (uint32_t)ctx->r29);
    }

    /* Safety: bail after too many frames */
    if (frame_count > 500) {
        fprintf(stderr, "[trace] 500 frames reached, exiting\n");
        exit(0);
    }
}

/* ======================================================================
 * COP0 / error handlers
 * ====================================================================== */

void cop0_status_write(recomp_context* ctx, gpr value) { ctx->status_reg = (uint32_t)value; }
gpr  cop0_status_read(recomp_context* ctx) { return (gpr)ctx->status_reg; }

void switch_error(const char* func, uint32_t vram, uint32_t jtbl) {
    fprintf(stderr, "[seattle] Switch error in %s at 0x%08X (jtbl 0x%08X)\n", func, vram, jtbl);
}
static int break_count = 0;
void do_break(uint32_t vram) {
    break_count++;
    if (break_count <= 3) {
        fprintf(stderr, "[seattle] BREAK at 0x%08X (OS trap stub - returning as no-op)\n", vram);
    } else if (break_count == 4) {
        fprintf(stderr, "[seattle] BREAK ... (suppressing further BREAK messages)\n");
    }
}
void recomp_syscall_handler(uint8_t* rdram, recomp_context* ctx, int32_t vram) {
    fprintf(stderr, "[seattle] SYSCALL at 0x%08X\n", (uint32_t)vram);
}
static int pause_count = 0;
void pause_self(uint8_t* rdram) {
    pause_count++;
    if (pause_count <= 5) {
        fprintf(stderr, "[seattle] pause_self #%d (fiber=%d)\n",
                pause_count, g_scheduler.current_fiber);
    }
    /* If we're in a fiber, yield instead of returning (prevents infinite loop) */
    if (g_scheduler.current_fiber >= 0) {
        g_scheduler.fibers[g_scheduler.current_fiber].active = 0;
        rtos_sched_yield(&g_scheduler, -1);
    }
}

static int32_t g_section_addresses[16] = {0};
int32_t* section_addresses = g_section_addresses;

/* ======================================================================
 * Main
 * ====================================================================== */

#ifdef CARNEVIL_STANDALONE

int main(int argc, char** argv) {
    printf("CarnEvil Recompiled Runtime v0.1\n");
    printf("  RAM: %d MB\n", RAM_SIZE / 1024 / 1024);

    memset(g_rdram, 0, RAM_SIZE);
    memset(&io, 0, sizeof(io));
    io.io_log_enabled = 0;

    /* Load CMOS/NVRAM from MAME dump so the game skips the date confirmation screen.
     * Without valid CMOS, the game shows "CMOS: BAD" and waits for operator input.
     * The MAME NVRAM contains validated settings from a successful boot. */
    {
        FILE* cmos_f = fopen("extracted/cmos_nvram.bin", "rb");
        if (cmos_f) {
            size_t n = fread(io.cmos, 1, sizeof(io.cmos), cmos_f);
            fclose(cmos_f);
            fprintf(stderr, "[init] Loaded CMOS/NVRAM: %zu bytes\n", n);
        } else {
            fprintf(stderr, "[init] No CMOS file (extracted/cmos_nvram.bin) - game may show setup screen\n");
        }
    }
    voodoo_init(&g_voodoo);
    /* Dark blue fill as baseline - game rendering should overwrite this */
    for (int i = 0; i < 512 * 384; i++)
        g_voodoo.framebuffer[i] = 0x0008;
    input_init();
    dcs_init(&g_dcs);

    /* Initialize input to all-released state */
    memset(&g_input, 0, sizeof(g_input));
    g_input.player[0].aim_x = 0.5f; /* Center screen */
    g_input.player[0].aim_y = 0.5f;
    g_input.player[1].aim_x = 0.5f;
    g_input.player[1].aim_y = 0.5f;
    input_poll(&g_input);

    /* Load RTOS code into low RAM (0x80000000 = physical 0x00000000) */
    {
        FILE* rtos_f = fopen("extracted/RTOS.bin", "rb");
        if (rtos_f) {
            fseek(rtos_f, 0, SEEK_END);
            long rtos_size = ftell(rtos_f);
            fseek(rtos_f, 0, SEEK_SET);
            if (rtos_size > 0 && rtos_size <= 256*1024) {
                fread(g_rdram, 1, rtos_size, rtos_f);
                printf("  Loaded RTOS: %ld bytes at physical 0x000000\n", rtos_size);
            }
            fclose(rtos_f);
        } else {
            printf("  Warning: RTOS.bin not found (extracted/RTOS.bin)\n");
        }
    }

    /* Load game binary into RAM at load address */
    const char* bin_path = (argc > 1) ? argv[1] : "extracted/GAME.bin";
    FILE* f = fopen(bin_path, "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        long size = ftell(f);
        fseek(f, 0, SEEK_SET);
        size_t offset = 0x000C4000; /* 0x800C4000 & 0x1FFFFFFF */
        if (offset + size <= RAM_SIZE) {
            fread(g_rdram + offset, 1, size, f);
            printf("  Loaded %ld bytes at physical 0x%06lX\n", size, (unsigned long)offset);
        } else {
            fprintf(stderr, "  Binary too large for RAM!\n");
            fclose(f);
            return 1;
        }
        fclose(f);
    } else {
        fprintf(stderr, "  Failed to open %s\n", bin_path);
        return 1;
    }

    /* Initialize recomp context */
    recomp_context ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.mips3_float_mode = 1;
    ctx.r29 = 0x80800000ULL; /* Stack at top of 8MB */
    ctx.r28 = 0x80170000ULL; /* GP (global pointer, typical for MIPS) */

    /* Register runtime overrides FIRST (searched first by linear scan) */
    /* Override critical RTOS functions that need hardware emulation */
    seattle_register_func(0x801A2A3C, rtos_create_task);       /* RTOS task creation */

    /* vec[7] override is registered after vector table (see below) */

    /* Override task callbacks and RTOS primitives */
    extern void func_801515C8(uint8_t*, recomp_context*);
    extern void func_80145CE4(uint8_t*, recomp_context*);
    extern void func_80145F98(uint8_t*, recomp_context*);
    seattle_register_func(0x801515C8, func_801515C8);          /* Watchdog (disabled) */
    seattle_register_func(0x80145CE4, func_80145CE4);          /* RTOS event wait */
    seattle_register_func(0x80145F98, func_80145F98);          /* RTOS event signal */

    /* VEC[64] slot 0 rendering callback - split function stub */
    extern void func_800C5FE4(uint8_t*, recomp_context*);
    seattle_register_func(0x800C5FE4, func_800C5FE4);

    /* State machine mode dispatcher - split function stub */
    extern void func_8014A488(uint8_t*, recomp_context*);
    seattle_register_func(0x8014A488, func_8014A488);

    /* Attract mode scene loop - manual implementation */
    extern void func_800CAE2C(uint8_t*, recomp_context*);
    seattle_register_func(0x800CAE2C, func_800CAE2C);

    /* Register all recompiled game functions */
    #include "func_registration.inc"

    /* Register recompiled RTOS functions */
    {
        extern void rtos_register_all(void);
        rtos_register_all();
    }
    printf("  Registered %d functions for lookup\n", g_func_count);

    /* Initialize heap if not already set up.
     * The game's entrypoint should do BSS clear + heap init, but our
     * recompiled entrypoint is a stub (just sets SP). We manually init
     * the heap: head pointer at 0x001A1E90, first free block at 0x00236850,
     * free space extends to ~0x007FBFF8 (top of heap, below stack). */
    {
        uint32_t heap_start = *(uint32_t*)(g_rdram + 0x001A1E90);
        uint32_t hp = heap_start & 0x1FFFFFFF;
        uint32_t free_sz = (hp > 0 && hp < RAM_SIZE - 4) ? *(uint32_t*)(g_rdram + hp) : 0;
        if (heap_start == 0 || free_sz == 0) {
            /* Heap not initialized - set it up manually.
             * Heap head at physical 0x00236850 (virtual 0x80236850).
             * Free block format: [size_with_flag, next_ptr]
             * Size includes the low bit as "free" flag (1=free, 0=allocated). */
            uint32_t heap_phys = 0x00236850;
            uint32_t heap_end = 0x007FBFF8; /* just below stack area */
            uint32_t free_size = heap_end - heap_phys;
            *(uint32_t*)(g_rdram + 0x001A1E90) = 0x80000000 | heap_phys; /* heap head (virtual) */
            *(uint32_t*)(g_rdram + heap_phys + 0) = free_size | 1; /* free block size + free flag */
            *(uint32_t*)(g_rdram + heap_phys + 4) = 0; /* next = NULL (single block) */
            fprintf(stderr, "[init] Heap initialized: head=0x%08X free=%u KB\n",
                    0x80000000 | heap_phys, free_size / 1024);
        } else {
            fprintf(stderr, "[debug] Before entry: heap_head=[0x801A1E90] = 0x%08X\n", heap_start);
        }
    }

    /* Initialize fiber scheduler before entry_point so task creation works */
    rtos_sched_init(&g_scheduler, g_rdram);

    /* Register RTOS vector table entries.
     * The game entry point copies a jump table from RTOS.bin[0x000] to
     * game addresses 0x800C408C-0x800C42CC. Since those addresses were
     * compiled as NOPs, we need to register the RTOS target functions
     * at the vector addresses so calls to them work. */
    {
        #include "rtos_vectors.inc"
        fprintf(stderr, "[debug] Registered RTOS vector table (72 entries)\n");

        /* Simple vector call counting - no tracing wrappers needed */
        fprintf(stderr, "[debug] Vector table entries registered\n");
    }

    /* Pre-init RTOS globals that the RTOS boot code would normally set up.
     * The game checks these and won't render without them. */
    {
        /* Allocate display list buffer in high RAM (above game heap).
         * Use 0x700000 (phys) = 0x80700000 (virt), 64KB buffer.
         * This area is above the game heap ceiling (~0x3F7400) and below stack (0x800000). */
        uint32_t disp_list_phys = 0x700000;
        uint32_t disp_list_virt = 0x80700000;
        uint32_t disp_list_size = 0x10000; /* 64KB */
        memset(g_rdram + disp_list_phys, 0, disp_list_size);

        /* Set the display list pointer at 0x801A1E88 */
        *(uint32_t*)(g_rdram + 0x001A1E88) = disp_list_virt;

        /* Set the callback struct pointer at 0x801A1DF8
         * Allocate a 256-byte callback struct at 0x700000+0x10000 */
        uint32_t cb_struct_phys = disp_list_phys + disp_list_size;
        uint32_t cb_struct_virt = disp_list_virt + disp_list_size;
        memset(g_rdram + cb_struct_phys, 0, 256);
        *(uint32_t*)(g_rdram + 0x001A1DF8) = cb_struct_virt;

        /* Set the global at 0x801A25F8 (used by task callback for channel 0xA2)
         * The callback reads [ptr+0x24] from this global. Set up a struct. */
        uint32_t global_phys = cb_struct_phys + 256;
        uint32_t global_virt = cb_struct_virt + 256;
        memset(g_rdram + global_phys, 0, 128);
        *(uint32_t*)(g_rdram + global_phys + 0x24) = 0x00000001; /* swap buffer flag */
        *(uint32_t*)(g_rdram + 0x001A25F8) = global_virt;

        fprintf(stderr, "[init] Display list at 0x%08X, callback struct at 0x%08X, global at 0x%08X\n",
                disp_list_virt, cb_struct_virt, global_virt);
    }

    /* Pre-populate RTOS PCI init flags and config.
     * The PCI config functions check global at 0x8001588C - must be non-zero.
     * Also set the Galileo PCI window base for Voodoo access. */
    {
        /* PCI bus init flags */
        *(uint32_t*)(g_rdram + 0x0001588C) = 0x00000001;
        *(uint32_t*)(g_rdram + 0x00015888) = 0x00000000;

        /* CRITICAL: Voodoo PCI base address at 0x801AA660 (phys 0x001AA660).
         * This is checked by func_80156B5C - if zero, ALL rendering is skipped! */
        *(uint32_t*)(g_rdram + 0x001AA660) = 0x08100000;

        /* Voodoo device count at 0x801A3354 (phys 0x001A3354). */
        *(uint32_t*)(g_rdram + 0x001A3354) = 1;

        /* Voodoo device descriptor table at 0x801E52B0 (phys 0x001E52B0).
         * From disassembly of func_80157A0C:
         *   - Entry stride = 152 bytes (index * 19 * 8)
         *   - entry[+0] = PCI base address (matched against search param)
         *   - entry[+8] = secondary value (stored at 0x80236810)
         *   - func_80157A0C stores entry pointer at 0x801E0350
         *
         * Also need device count at 0x801A3354 = 1 */
        {
            uint32_t desc_base = 0x001E52B0;
            memset(g_rdram + desc_base, 0, 152); /* Clear entry 0 */

            /* entry[+0] = Voodoo PCI register base (BAR0) */
            *(uint32_t*)(g_rdram + desc_base + 0) = 0x08100000;

            /* entry[+8] = Voodoo PCI LFB/memory base (BAR1)
             * This is stored at 0x80236810 and used for framebuffer access */
            *(uint32_t*)(g_rdram + desc_base + 8) = 0x00800000;

            /* Device count */
            *(uint32_t*)(g_rdram + 0x001A3354) = 1;

            /* Also pre-populate the result pointers that func_80157A0C stores:
             * 0x801E0350 = entry pointer, 0x80236810 = entry[+8] value */
            *(uint32_t*)(g_rdram + 0x001E0350) = 0x801E52B0; /* ptr to entry */
            *(uint32_t*)(g_rdram + 0x00236810) = 0x00800000; /* LFB base from entry[+8] */

            fprintf(stderr, "[init] Populated Voodoo descriptor: BAR0=0x08100000, BAR1=0x00800000\n");

        /* PIC serial number at 0x801E6504 (phys 0x001E6504).
         * The game checks this against 528 (25"), 469, or 486 (39") in an
         * infinite loop at 0x800C515C. If it doesn't match, the game hangs!
         * CarnEvil uses 486 for the 39" cabinet (matches our PIC file). */
        *(uint32_t*)(g_rdram + 0x001E6504) = 486;
        fprintf(stderr, "[init] Set PIC serial at 0x801E6504 = 486\n");
        }

        /* Voodoo display init is called AFTER entry point (env strings need to be parsed first) */

        fprintf(stderr, "[init] Set Voodoo PCI base at 0x801AA660 = 0x08100000\n");

        /* PCI bus base address at 0x80015888 (phys 0x00015888)
         * This is the Galileo PCI memory window base, used for BAR decode.
         * On real hardware it's configured by the Galileo to 0x08100000. */
        *(uint32_t*)(g_rdram + 0x00015888) = 0x08100000;

        fprintf(stderr, "[init] Set PCI init flag and bus base address\n");
    }

    /* RTOS tables:
     * 0x800146A0: Function pointer table - populated by VEC[64] during main_loop
     * 0x800146C0: Status/index table - maps device slots to func_table indices
     *             Populated by RTOS boot. We set initial values so rtos_80005100 can work.
     * 0x800146E0: Callback table - populated by VEC[60] (rtos_800060BC)
     *
     * The status table entries map each of 8 device slots to a function pointer index.
     * -1 means "no device". On real hardware, PCI scan fills this. */
    {
        /* Status table: each entry = the slot number that maps to this position.
         * VEC[64] registration (rtos_80006220) searches this table for entries
         * matching the target slot. It stores the func ptr only when it finds
         * a matching entry. So we need: status[i] = i for each slot we want. */
        for (int i = 0; i < 8; i++)
            *(uint32_t*)(g_rdram + 0x000146C0 + i * 4) = (uint32_t)i;
    }

    /* Don't call rtos_80005100 here - the function table it uses isn't populated yet.
     * VEC[64] (rtos_80006220) registers callbacks during the game's main_loop.
     * The device init happens naturally through the game's own init sequence. */

    /* Check device table state before entry point */
    {
        uint32_t dev_table_phys = 0x000B83E8;
        fprintf(stderr, "[debug] Device table BEFORE entry point:\n");
        int any_nonzero = 0;
        for (int i = 0; i < 16; i++) {
            uint32_t entry_phys = dev_table_phys + i * 36;
            if (entry_phys + 36 > RAM_SIZE) break;
            uint32_t flags = *(uint32_t*)(g_rdram + entry_phys + 16);
            uint32_t handler = *(uint32_t*)(g_rdram + entry_phys + 24);
            if (flags || handler) {
                fprintf(stderr, "  dev[%d]: flags=0x%08X handler=0x%08X\n", i, flags, handler);
                any_nonzero = 1;
            }
        }
        if (!any_nonzero) fprintf(stderr, "  (all zero)\n");
    }

    printf("  Phase 1: Init...\n");
    fflush(stdout);

    /* The entry_point calls: sys_init -> main_init -> func_800C4234 -> main_loop.
     * On real hardware, main_loop is called once per frame by the RTOS.
     * Since we don't have an RTOS, we let entry_point run (which does one frame),
     * then loop main_loop ourselves. */
    recomp_entrypoint(g_rdram, &ctx);

    fprintf(stderr, "[debug] Phase 1 done: r2=0x%08X, voodoo_writes=%u\n",
            (uint32_t)ctx.r2, voodoo_get_write_count());

    /* Call the FULL Voodoo PCI driver init (func_80161140) instead of display init.
     * This does PCI scan, finds Voodoo, and calls display init with correct params.
     * func_80155888(a0=pci_base, a1=config_ptr, a2=fbi_mem, a3=tmu0_mem, sp[16]=tmu1_mem)
     * Checks: if a2 == 0, bails immediately (line 0x801558C4) */
    {
        /* Call func_80155888 directly with correct arguments.
         * func_80155888(a0=pci_base, a1=display_mode, a2=fbi_mem, a3=tmu0_mem, sp[16]=tmu1_mem)
         * a1 = display mode index: s0 in the function, used for resolution switch.
         *   Check: s0 - 2 must be < 11 (unsigned), so s0 must be 2..12
         *   Mode 4 = 512x384 at 0x80155968 in the switch table */
        /* Call the REAL func_80155888 with correct args.
         * Now we know the jump table layout is correct. */
        {
            recomp_func_t* disp_init = get_function(0x80155888);
            if (disp_init) {
                /* Re-populate descriptor (warm-up frame clears BSS).
             * Entry struct is 152 bytes at 0x001E52B0.
             * Fields used by func_80155888 and the rendering code: */
            {
                uint32_t e = 0x001E52B0;
                memset(g_rdram + e, 0, 152);
                *(uint32_t*)(g_rdram + e + 0)  = 0x08100000; /* PCI register base (BAR0) */
                *(uint32_t*)(g_rdram + e + 4)  = 0x121A;     /* vendor ID (3dfx) */
                *(uint32_t*)(g_rdram + e + 8)  = 0x00800000; /* LFB/framebuffer base (BAR1) */
                *(uint32_t*)(g_rdram + e + 12) = 0x00000001; /* device type (Voodoo 1) */
                *(uint32_t*)(g_rdram + e + 16) = 0x00000008; /* PCI device number */
                *(uint32_t*)(g_rdram + e + 20) = 0x00200000; /* FBI memory size (2MB) */
                *(uint32_t*)(g_rdram + e + 24) = 0x00400000; /* TMU0 memory size (4MB) */
                *(uint32_t*)(g_rdram + e + 28) = 0x00000000; /* TMU1 memory size (0) */
                *(uint32_t*)(g_rdram + e + 32) = 512;        /* width */
                *(uint32_t*)(g_rdram + e + 36) = 384;        /* height */
                *(uint32_t*)(g_rdram + e + 40) = 60;         /* refresh rate */
                *(uint32_t*)(g_rdram + e + 44) = 0x00000001; /* initialized flag */
                *(uint32_t*)(g_rdram + 0x001A3354) = 1;      /* device count */
                *(uint32_t*)(g_rdram + 0x001E0350) = 0x801E52B0; /* entry pointer */
                *(uint32_t*)(g_rdram + 0x00236810) = 0x00800000; /* LFB cache */
            }
            fprintf(stderr, "[init] Populated Voodoo descriptor with full entry data\n");

            fprintf(stderr, "[init] Pre-check: dev_count=0x%08X desc[0]=0x%08X entry_ptr=0x%08X\n",
                    *(uint32_t*)(g_rdram + 0x001A3354),
                    *(uint32_t*)(g_rdram + 0x001E52B0),
                    *(uint32_t*)(g_rdram + 0x001E0350));
            /* Call the FULL PCI driver init (func_80161140) which calls display init
             * AND sets up the rendering context. */
            {
                recomp_func_t* pci_drv = get_function(0x80161140);
                if (pci_drv) {
                    fprintf(stderr, "[init] Calling Voodoo PCI driver (func_80161140)...\n");
                    recomp_context pc = ctx;
                    pci_drv(g_rdram, &pc);
                    fprintf(stderr, "[init] PCI driver: r2=0x%08X writes=%u base=0x%08X render=0x%08X\n",
                            (uint32_t)pc.r2, voodoo_get_write_count(),
                            *(uint32_t*)(g_rdram + 0x001AA660),
                            *(uint32_t*)(g_rdram + 0x0022A454));
                }
            }
            /* Call func_80120020 which sets the rendering pointer at 0x0022A454.
             * This function is normally called by the Voodoo PCI driver after detection.
             * It expects v0 != 0 (device found flag). */
            {
                recomp_func_t* render_setup = get_function(0x80120020);
                if (render_setup) {
                    recomp_context rs = ctx;
                    rs.r2 = 1;  /* v0 = device found flag */
                    rs.r4 = 0x08100000; /* a0 = PCI base (in case it needs it) */
                    rs.r5 = 0x801E52B0; /* a1 = descriptor entry ptr */
                    fprintf(stderr, "[init] Calling func_80120020 (render ptr setup)...\n");
                    render_setup(g_rdram, &rs);
                    fprintf(stderr, "[init] render_ptr = 0x%08X\n",
                            *(uint32_t*)(g_rdram + 0x0022A454));
                }
            }
            fprintf(stderr, "[init] Also calling func_80155888(pci=0x08100000, mode=7, fbi=2, tmu=4)...\n");
                recomp_context vc = ctx;
                vc.r4 = 0x08100000;  /* a0 = PCI base */
                vc.r5 = 7;           /* a1 = display mode (512x384) */
                vc.r6 = 2;           /* a2 = FBI_MEM (2MB) */
                vc.r7 = 4;           /* a3 = TMU0_MEM (4MB) */
                uint32_t sp = (uint32_t)vc.r29 & 0x1FFFFFFF;
                if (sp + 20 < RAM_SIZE) *(uint32_t*)(g_rdram + sp + 16) = 0; /* sp[16] = TMU1 = 0 */
                disp_init(g_rdram, &vc);
                fprintf(stderr, "[init] func_80155888 returned: r2=0x%08X, writes=%u\n",
                        (uint32_t)vc.r2, voodoo_get_write_count());
                fprintf(stderr, "[init] Voodoo base at 0x801AA660 = 0x%08X\n",
                        *(uint32_t*)(g_rdram + 0x001AA660));
            }

            /* Also do direct Voodoo PCI init as backup */
            fprintf(stderr, "[init] Direct Voodoo PCI init (512x384)...\n");
            extern void seattle_io_write32(uint32_t, uint32_t);

            /* Write Voodoo registers through PCI config space */
            #define PCI_VOODOO_WRITE(reg, val) do { \
                seattle_io_write32(0x0C000CF8, 0x80004000 | ((reg) & 0xFC)); \
                seattle_io_write32(0x0C000CFC, (val)); \
            } while(0)
            /* Note: device 8 = bit 14:11 = 0x4000 */

            /* fbiInit0: enable FBI, VGA passthrough off */
            PCI_VOODOO_WRITE(0x10 + 0x00, 0x00000411); /* BAR0 offset + fbiInit0 */

            /* Actually, PCI config regs 0x00-0xFF are PCI config (vendor/class/BAR).
             * Voodoo RENDERING registers are at the BAR0 memory-mapped address.
             * We should write through MMIO at 0x08100000 + reg_offset instead. */

            /* Direct MMIO writes to Voodoo register space */
            seattle_io_write32(0x08100210, 0x00000411); /* fbiInit0 */
            seattle_io_write32(0x08100214, 0x00201070); /* fbiInit1: 8 tiles, video out */
            seattle_io_write32(0x08100218, 0x00000200); /* fbiInit2: swap auto */
            seattle_io_write32(0x0810021C, (383 << 22)); /* fbiInit3: yorigin=383 */
            seattle_io_write32(0x0810020C, 512 | (384 << 16)); /* videoDimensions */
            seattle_io_write32(0x08100110, 0x00004391); /* fbzMode: clip+depth+dither+rgb */
            seattle_io_write32(0x08100104, 0x00000009); /* fbzColorPath */
            seattle_io_write32(0x08100118, 512);         /* clipLeftRight */
            seattle_io_write32(0x0810011C, 384);         /* clipLowYHighY */

            /* FastFill to clear the screen to black */
            seattle_io_write32(0x08100148, 0x00000000); /* color1 = black */
            seattle_io_write32(0x08100130, 0x00000000); /* zaColor = 0 */
            seattle_io_write32(0x08100124, 0x00000000); /* fastfillCMD */

            /* SwapBuffers */
            seattle_io_write32(0x08100128, 0x00000001); /* swapbufferCMD */

            fprintf(stderr, "[init] Direct Voodoo init complete: writes=%u\n",
                    voodoo_get_write_count());
            /* Force Voodoo base back to valid address even if driver cleared it */
            if (*(uint32_t*)(g_rdram + 0x001AA660) == 0) {
                *(uint32_t*)(g_rdram + 0x001AA660) = 0x08100000;
                fprintf(stderr, "[init] Re-set Voodoo base to 0x08100000\n");
            }
            /* CRITICAL: rendering state at 0x0022A454.
             * func_8015E2F4 (147 callers) loads this as an INDEX into a
             * 70,316-byte-stride rendering context array at 0x801E6A20.
             * Index 0 → entry at 0x001E6A20, Index 1 → entry at 0x001F7CCC.
             *
             * After computing the entry address, the function checks:
             *   entry[+0x11178] == 4  (Voodoo "ready" state flag)
             * If not 4, rendering is skipped. */
            /* Set BOTH possible render_ptr addresses */
            *(uint32_t*)(g_rdram + 0x0022A444) = 1; /* func_8015E2F4 loads from A444! */
            *(uint32_t*)(g_rdram + 0x0022A454) = 1; /* func_80120020 uses A454 */

            /* Entry 0 at physical 0x001E6A20, size 70316 bytes.
             * Set the "Voodoo ready" flag at entry[+0x11178] = 4.
             * Physical address: 0x001E6A20 + 0x11178 = 0x001F7B98 */
            uint32_t entry0 = 0x001E6A20;
            if (entry0 + 0x11178 + 4 < RAM_SIZE) {
                *(uint32_t*)(g_rdram + entry0 + 0x11178) = 4;
                *(uint32_t*)(g_rdram + entry0 + 0) = 0x08100000;
                fprintf(stderr, "[init] Set Voodoo ready flag at entry[0][0x11178]=4\n");
            }
            /* Also init entry[1] (stride 70316 = 0x1129C) */
            uint32_t entry1_init = entry0 + 70316;
            if (entry1_init + 0x11178 + 4 < RAM_SIZE) {
                *(uint32_t*)(g_rdram + entry1_init + 0x11178) = 4;
                *(uint32_t*)(g_rdram + entry1_init + 0) = 0x08100000;
                fprintf(stderr, "[init] Set Voodoo ready flag at entry[1][0x11178]=4\n");
            }
        }
    }

    /* Check device table AFTER entry point */
    {
        uint32_t dev_table_phys = 0x000B83E8;
        fprintf(stderr, "[debug] Device table AFTER entry point:\n");
        int any_nonzero = 0;
        for (int i = 0; i < 16; i++) {
            uint32_t entry_phys = dev_table_phys + i * 36;
            if (entry_phys + 36 > RAM_SIZE) break;
            uint32_t w0 = *(uint32_t*)(g_rdram + entry_phys);
            uint32_t flags = *(uint32_t*)(g_rdram + entry_phys + 16);
            uint32_t ptr20 = *(uint32_t*)(g_rdram + entry_phys + 20);
            uint32_t handler = *(uint32_t*)(g_rdram + entry_phys + 24);
            if (w0 || flags || handler) {
                fprintf(stderr, "  dev[%d] @0x%06X: w0=0x%08X flags=0x%08X +20=0x%08X handler=0x%08X\n",
                        i, entry_phys, w0, flags, ptr20, handler);
                any_nonzero = 1;
            }
        }
        if (!any_nonzero) fprintf(stderr, "  (all zero - device init FAILED)\n");
    }

    /* Phase 1b: Warm-up frames. The game shows a diagnostic/date confirmation
     * screen. With valid CMOS data, it auto-advances after ~15 seconds.
     * We simulate button presses to speed up dismissal. */
    printf("  Phase 1b: Warm-up (20 frames)...\n");
    fflush(stdout);
    for (int wf = 0; wf < 20; wf++) {
        /* Press service credit + start buttons on frames 5-8 to dismiss menus */
        if (wf >= 5 && wf <= 8) {
            /* Active LOW: clear bits to "press" buttons */
            g_input.ioasic_buttons = 0xFFFFFFFF & ~((1<<10) | (1<<2) | (1<<0)); /* service + P1 start + P1 trigger */
            *(uint32_t*)(g_rdram + 0x002122E0) = g_input.ioasic_buttons | 0x10000;
        } else {
            g_input.ioasic_buttons = 0xFFFFFFFF; /* all released */
            *(uint32_t*)(g_rdram + 0x002122E0) = g_input.ioasic_buttons | 0x10000;
        }
        uint32_t* vblank = (uint32_t*)(g_rdram + 0x001A35CC);
        uint32_t* tick   = (uint32_t*)(g_rdram + 0x001A35C8);
        (*vblank)++;
        (*tick) += 16667;
        if (wf == 0) {
            /* First warm-up frame: call full main_loop for init (allocs DMA buffers) */
            ctx.r4 = ctx.r2;
            func_800C4524(g_rdram, &ctx);

            /* The main_loop's render buffer allocation:
             * malloc(0x1C0020) returns at ~0x80236970, aligned to 0x80236980.
             * Physical addresses stored at:
             *   0x001DFED4 = buffer start (physical)
             *   0x001DFEDC = buffer end (start + 0xE0000)
             * BUT: init code or the PCI driver may corrupt these values.
             * Fix: find the malloc result and force correct values. */
            {
                /* The large allocation starts after the heap area. Look for it. */
                uint32_t hp = *(uint32_t*)(g_rdram + 0x001A1E90) & 0x1FFFFFFF;
                /* The render buffer is the last large alloc before the heap head.
                 * It was malloc'd at 0x80236970. Physical = 0x00236970.
                 * Align to 32 bytes: 0x00236980. */
                uint32_t rbuf = *(uint32_t*)(g_rdram + 0x001DFED4);
                uint32_t rend = *(uint32_t*)(g_rdram + 0x001DFEDC);
                fprintf(stderr, "[init] After main_loop: render_buf=0x%08X end=0x%08X GP=0x%08X heap=0x%08X\n",
                        rbuf, rend, (uint32_t)ctx.r28, hp);

                /* If the stored values look wrong (not within the heap area),
                 * search for the render buffer by looking at the heap layout. */
                extern uint32_t g_render_buffer_addr;
                if (g_render_buffer_addr != 0 &&
                    (rbuf == 0 || rbuf > 0x00800000 || rbuf < 0x00200000 ||
                     rbuf != g_render_buffer_addr)) {
                    uint32_t forced_buf = g_render_buffer_addr;
                    uint32_t forced_end = forced_buf + 0x000E0000;
                    *(uint32_t*)(g_rdram + 0x001DFED4) = forced_buf;
                    *(uint32_t*)(g_rdram + 0x001DFEDC) = forced_end;
                    fprintf(stderr, "[init] FORCED render_buf=0x%08X end=0x%08X\n",
                            forced_buf, forced_end);
                }
            }
        }
        /* All frames: run process dispatcher + callbacks */
        {
            extern void func_80151718(uint8_t*, recomp_context*);
            func_80151718(g_rdram, &ctx);
            extern void rtos_run_callbacks(uint8_t* rdram);
            rtos_run_callbacks(g_rdram);
        }
    }
    fprintf(stderr, "[debug] After warm-up: r2=0x%08X\n", (uint32_t)ctx.r2);

    /* RTOS tables are populated by VEC[64]/[60] during warm-up frames.
     * Device init (rtos_80005100) is NOT called - its func_table entries
     * are periodic callbacks, not device init functions.
     * The game's own code handles device discovery via PCI config reads.
     * With valid CMOS, the game auto-advances past the diagnostic screen.
     */
    {
        fprintf(stderr, "[debug] After warm-up: RTOS tables populated by VEC[64]/[60]\n");
    }

    /* Call attract mode ONCE to populate the scene graph.
     * Must happen BEFORE the frame loop (while heap has free space).
     * The IOASIC 0x7406 polling loop now terminates quickly. */
    {
        uint32_t head = *(uint32_t*)(g_rdram + 0x001A1E90);
        uint32_t phys = head & 0x1FFFFFFF;
        uint32_t free_sz = (phys < RAM_SIZE - 4) ? (*(uint32_t*)(g_rdram + phys) & ~1u) : 0;
        fprintf(stderr, "[init] Heap before attract mode: %u KB free\n", free_sz / 1024);

        /* Force all rendering flags before attract mode */
        *(uint32_t*)(g_rdram + 0x00179258) = 1;
        *(uint32_t*)(g_rdram + 0x002122D4) = 0x40;
        *(uint32_t*)(g_rdram + 0x0016925C) = 1;
        *(uint32_t*)(g_rdram + 0x001AA660) = 0x08100000;
        *(uint32_t*)(g_rdram + 0x001E6A20 + 0x11178) = 4;

        extern void func_800C50AC(uint8_t*, recomp_context*);
        fprintf(stderr, "[init] Calling attract mode to populate scene...\n");
        fflush(stderr);
        func_800C50AC(g_rdram, &ctx);
        fprintf(stderr, "[init] Attract mode returned!\n");
        fflush(stderr);

        head = *(uint32_t*)(g_rdram + 0x001A1E90);
        phys = head & 0x1FFFFFFF;
        free_sz = (phys < RAM_SIZE - 4) ? (*(uint32_t*)(g_rdram + phys) & ~1u) : 0;
        fprintf(stderr, "[init] Heap after attract mode: %u KB free\n", free_sz / 1024);
    }

    /* Phase 2: Start frame loop */
    fprintf(stderr, "[debug] Starting frame loop with %d fibers\n",
            g_scheduler.fiber_count);

    /* Now loop main_loop for subsequent frames */
    printf("  Entering main loop...\n");
    fflush(stdout);

    /* Check heap before entering frame loop */
    {
        uint32_t head = *(uint32_t*)(g_rdram + 0x001A1E90);
        uint32_t phys = head & 0x1FFFFFFF;
        uint32_t free_sz = (phys < RAM_SIZE - 4) ? *(uint32_t*)(g_rdram + phys) : 0;
        fprintf(stderr, "[debug] Before frame loop: heap_head=0x%08X free_block=%u KB\n",
                head, (free_sz & ~1u) / 1024);
    }

    int max_frames = 500; /* Reduced for testing - DCS2 polling fixed */

    /* Heap management: save heap state AFTER first frame's permanent allocs.
     * Frame 0 does the 1.75MB render buffer alloc.
     * Frame 1+ do per-frame scratch allocs that need to be freed. */
    uint32_t heap_snapshot_head = 0;
    uint32_t heap_snapshot_word0 = 0;
    uint32_t heap_snapshot_word1 = 0;

    for (int frame = 0; frame < max_frames; frame++) {
        /* Simulate VSync interrupt */
        uint32_t* vblank = (uint32_t*)(g_rdram + 0x001A35CC);
        uint32_t* tick   = (uint32_t*)(g_rdram + 0x001A35C8);
        uint32_t* sync1  = (uint32_t*)(g_rdram + 0x001A1AB0);
        uint32_t* sync2  = (uint32_t*)(g_rdram + 0x001A1AB8);
        (*vblank)++;
        (*tick) += 16667;
        (*sync1)++;
        /* Advance sync2 alongside sync1 to simulate frame completion.
         * On real hardware, sync2 increments when a frame finishes rendering.
         * Without it advancing, the game's state machine stalls (waits for
         * rendering to catch up before transitioning to attract mode). */
        (*sync2) = (*sync1) - 1;

        input_poll(&g_input);
        input_write_to_ram(g_rdram, &g_input);

        /* Force PIC serial number EVERY frame (game clears it during init) */
        *(uint32_t*)(g_rdram + 0x001E6504) = 486;

        /* Force rendering context index EVERY frame.
         * func_8015E2F4 (147 callers) loads from 0x8022A444. If 0, all rendering bails.
         * Value is an index into 70,316-byte stride array at 0x801E6A20.
         * Index 1 → entry at 0x001F7CCC. Must be non-zero for rendering to proceed. */
        *(uint32_t*)(g_rdram + 0x0022A444) = 1;

        /* Force Voodoo PCI base address - rendering code reads this to compute
         * destination addresses for Voodoo register writes. If 0, no rendering. */
        *(uint32_t*)(g_rdram + 0x001AA660) = 0x08100000;

        /* Ensure rendering context entries have Voodoo ready flag = 4.
         * Entry[0] at 0x001E6A20, Entry[1] at 0x001F7CCC (stride 70316 = 0x11298+delta).
         * Actually stride = 70316 = 0x1129C. Entry[1] = 0x001E6A20 + 70316 = 0x001F7CBC. */
        *(uint32_t*)(g_rdram + 0x001E6A20 + 0x11178) = 4;  /* entry[0] */
        {
            uint32_t entry1 = 0x001E6A20 + 70316;
            if (entry1 + 0x11178 + 4 < 0x00800000) {
                *(uint32_t*)(g_rdram + entry1 + 0x11178) = 4;           /* Voodoo ready flag */
                *(uint32_t*)(g_rdram + entry1) = 0x08100000;            /* Voodoo PCI base */
            }
        }

        /* Force display initialized flag (gate for scene graph rendering) */
        *(uint32_t*)(g_rdram + 0x00179258) = 1;

        /* Display mode: 0x40=setup, 2=render. func_800D5D04 needs mode==2.
         * First frame: setup. Subsequent frames: render. */
        *(uint32_t*)(g_rdram + 0x002122D4) = (frame == 0) ? 0x40 : 2;

        /* Don't force *(0x1DDD80) — the renderer manages this as a vblank sync flag.
         * Patched in funcs_4.c to write 2 (simulates vblank) instead of 0. */

        /* Force Widget display mode flags (third rendering gate).
         * func_800D4C24 checks *(0x001E6580) != 0 (bit 6 of Widget reg 1).
         * Also set the second display channel's mode flag. */
        if (*(uint32_t*)(g_rdram + 0x001E6580) == 0)
            *(uint32_t*)(g_rdram + 0x001E6580) = 0x40;
        if (*(uint32_t*)(g_rdram + 0x001E65A4) == 0)
            *(uint32_t*)(g_rdram + 0x001E65A4) = 0x40;

        /* Force render submission flag (gate in static_0_800D547C → func_800D5484).
         * func_800D5484 checks *0x0016925C != 0 before processing render list. */
        if (*(uint32_t*)(g_rdram + 0x0016925C) == 0) {
            *(uint32_t*)(g_rdram + 0x0016925C) = 1;
        }

        /* Log state machine flag on first frame */
        if (frame == 0) {
            uint32_t sm_flag = *(uint32_t*)(g_rdram + 0x001A25F4);
            uint32_t sm_current = *(uint32_t*)(g_rdram + 0x001A25F8);
            uint32_t sm_entry = *(uint32_t*)(g_rdram + 0x001A25F0);
            fprintf(stderr, "[state] SM flag @0x1A25F4 = 0x%08X, current @0x1A25F8 = 0x%08X, entry @0x1A25F0 = 0x%08X\n",
                    sm_flag, sm_current, sm_entry);
            /* Also check the entry at 0x1A25F0 */
            if (sm_entry) {
                uint32_t ep2 = sm_entry & 0x1FFFFFFF;
                if (ep2 + 0x28 < 0x00800000) {
                    fprintf(stderr, "[state] Entry@25F0: [0]=0x%08X [4]=0x%08X [10]=0x%08X [24]=0x%08X\n",
                            *(uint32_t*)(g_rdram + ep2),
                            *(uint32_t*)(g_rdram + ep2 + 4),
                            *(uint32_t*)(g_rdram + ep2 + 0x10),
                            *(uint32_t*)(g_rdram + ep2 + 0x24));
                }
            }
            if (sm_current) {
                uint32_t ep = sm_current & 0x1FFFFFFF;
                if (ep + 0x28 < 0x00800000) {
                    fprintf(stderr, "[state] Entry: [0]=0x%08X [4]=0x%08X [8]=0x%08X [10]=0x%08X [24]=0x%08X\n",
                            *(uint32_t*)(g_rdram + ep),
                            *(uint32_t*)(g_rdram + ep + 4),
                            *(uint32_t*)(g_rdram + ep + 8),
                            *(uint32_t*)(g_rdram + ep + 0x10),
                            *(uint32_t*)(g_rdram + ep + 0x24));
                    uint32_t buf = *(uint32_t*)(g_rdram + ep + 0x10) & 0x1FFFFFFF;
                    if (buf > 0 && buf + 0x20 < 0x00800000) {
                        /* Check for action function at expected offset */
                        fprintf(stderr, "[state] Buffer: [0]=0x%08X [4]=0x%08X [8]=0x%08X\n",
                                *(uint32_t*)(g_rdram + buf),
                                *(uint32_t*)(g_rdram + buf + 4),
                                *(uint32_t*)(g_rdram + buf + 8));
                    }
                }
            }
        }

        /* Force render buffer pointer at 0x001DFED4 every frame.
         * main_loop allocates this, but init code corrupts the stored value. */
        {
            extern uint32_t g_render_buffer_addr;
            if (g_render_buffer_addr != 0) {
                *(uint32_t*)(g_rdram + 0x001DFED4) = g_render_buffer_addr;
                *(uint32_t*)(g_rdram + 0x001DFEDC) = g_render_buffer_addr + 0x000E0000;
            }
            if (frame == 0) {
                fprintf(stderr, "[frame0] Render buffer = 0x%08X\n", g_render_buffer_addr);
            }
        }

        /* Debug HUD moved to after frame loop, before PPM dump */

        /* Restore heap to post-init snapshot (preserves permanent allocs) */
        if (frame > 0 && heap_snapshot_head != 0) {
            *(uint32_t*)(g_rdram + 0x001A1E90) = heap_snapshot_head;
            uint32_t hp = heap_snapshot_head & 0x1FFFFFFF;
            if (hp < RAM_SIZE - 8) {
                *(uint32_t*)(g_rdram + hp) = heap_snapshot_word0;
                *(uint32_t*)(g_rdram + hp + 4) = heap_snapshot_word1;
            }
        }

        /* Call the per-frame functions from main_loop individually.
         * main_loop always re-runs its init (malloc 1.75MB) which we can't allow.
         * The per-frame sequence from main_loop 0x800C46E8+:
         * 1. func_80151B74(args) - state machine / mode registration
         * 2. func_80151528() - sync
         * 3. func_800C4A08() - create tasks
         * 4. func_80151528() - sync
         * 5. func_80151718() - process dispatcher
         * 6. func_801438D0(0) - cleanup */
        {
            extern void func_80151B74(uint8_t*, recomp_context*);
            extern void func_80151528(uint8_t*, recomp_context*);
            extern void func_800C4A08(uint8_t*, recomp_context*);
            extern void func_80151718(uint8_t*, recomp_context*);
            extern void func_801438D0(uint8_t*, recomp_context*);

            /* func_80151B74: State machine mode registration.
             * Only register the attract mode on the first frame.
             * After that, the attract mode itself registers subsequent modes
             * (func_800C5830, etc.) via internal func_80151B74 calls.
             * Re-registering every frame would OVERWRITE mode transitions! */
            if (frame == 0) {
                ctx.r4 = 0x8016DAB8; /* game name string */
                ctx.r5 = 0;
                ctx.r6 = 0x800C50AC; /* mode function (attract mode) */
                ctx.r7 = 0;
                uint32_t sp_phys = (uint32_t)ctx.r29 & 0x1FFFFFFF;
                if (sp_phys + 24 < RAM_SIZE) {
                    *(uint32_t*)(g_rdram + sp_phys + 16) = 0;
                    *(uint32_t*)(g_rdram + sp_phys + 20) = 0;
                    *(uint32_t*)(g_rdram + sp_phys + 24) = 0;
                }
                func_80151B74(g_rdram, &ctx);
            }

            func_80151528(g_rdram, &ctx);

            /* Mark per-frame fibers as inactive (they'll be reactivated by
             * func_800C4A08 → func_801A2A3C on the next frame).
             * Don't reset fiber_count — reuse existing fiber objects. */
            for (int fi = 0; fi < g_scheduler.fiber_count; fi++) {
                g_scheduler.fibers[fi].active = 0;
                g_scheduler.fibers[fi].blocked = 0;
            }

            func_800C4A08(g_rdram, &ctx); /* create tasks */
            func_80151528(g_rdram, &ctx);
            func_80151718(g_rdram, &ctx); /* process dispatcher */

            extern void rtos_run_callbacks(uint8_t* rdram);
            rtos_run_callbacks(g_rdram);

            /* Call the state machine's current mode function.
             * After frame 0 (attract mode init), this dispatches func_800C5830
             * which handles actual scene rendering and attract sequence. */
            {
                extern void func_8014A488(uint8_t*, recomp_context*);
                func_8014A488(g_rdram, &ctx);
            }
        }

        /* func_800C50AC is the attract mode main loop - it never returns.
         * It enters naturally through the game's task/callback system.
         * The PIC serial (IOASIC cmd 0x7001 = 486) is now correct. */

        /* Check if rendering functions wrote to the GP-relative command buffer */
        if (frame == 10 || frame == 100 || frame == 500) {
            uint32_t gp = (uint32_t)ctx.r28 & 0x1FFFFFFF;
            fprintf(stderr, "[gp_check] frame=%d GP=0x%08X phys=0x%06X\n",
                    frame, (uint32_t)ctx.r28, gp);
            if (gp > 0 && gp + 256 < RAM_SIZE) {
                int nz = 0;
                for (int i = 0; i < 256; i += 4) {
                    uint32_t v = *(uint32_t*)(g_rdram + gp + i);
                    if (v != 0) nz++;
                }
                fprintf(stderr, "[render_buf] GP=0x%08X, first 64 words: %d non-zero\n",
                        (uint32_t)ctx.r28, nz);
                if (nz > 0) {
                    fprintf(stderr, "[render_buf] Command buffer contents:\n");
                    for (int i = 0; i < 128; i += 4) {
                        uint32_t v = *(uint32_t*)(g_rdram + gp + i);
                        if (v != 0)
                            fprintf(stderr, "  GP[+0x%02X] = 0x%08X\n", i, v);
                    }
                }
            }
        }

        /* No placeholder rendering - let game LFB writes fill the framebuffer.
         * If the game doesn't write anything, framebuffer stays from last frame. */

        /* After frame 0: snapshot the heap (permanent allocs done) */
        if (frame == 0) {
            heap_snapshot_head = *(uint32_t*)(g_rdram + 0x001A1E90);
            uint32_t hp = heap_snapshot_head & 0x1FFFFFFF;
            if (hp < RAM_SIZE - 8) {
                heap_snapshot_word0 = *(uint32_t*)(g_rdram + hp);
                heap_snapshot_word1 = *(uint32_t*)(g_rdram + hp + 4);
            }
            uint32_t free_kb = (heap_snapshot_word0 & ~1u) / 1024;
            fprintf(stderr, "[heap] Snapshot after frame 0: head=0x%08X free=%uKB\n",
                    heap_snapshot_head, free_kb);
        }

        if (frame < 5 || frame == 50 || frame == 99) {
            uint32_t hp = *(uint32_t*)(g_rdram + 0x001A1E90) & 0x1FFFFFFF;
            uint32_t fs = (hp < RAM_SIZE - 4) ? (*(uint32_t*)(g_rdram + hp) & ~1u) : 0;

            /* Dump key game state variables */
            uint32_t sync1 = *(uint32_t*)(g_rdram + 0x001A1AB0);
            uint32_t sync2 = *(uint32_t*)(g_rdram + 0x001A1AB8);
            uint32_t disp_list = *(uint32_t*)(g_rdram + 0x001A1E88);
            uint32_t game_state = *(uint32_t*)(g_rdram + 0x001E03C8);
            uint32_t vblank = *(uint32_t*)(g_rdram + 0x001A35CC);

            fprintf(stderr, "[debug] Frame %d: heap=%uKB sync=%u/%u disp=0x%08X state=0x%08X vbl=%u r2=0x%X\n",
                    frame, fs / 1024, sync1, sync2, disp_list, game_state, vblank, (uint32_t)ctx.r2);

            /* Check if any Voodoo commands were written to GP-relative buffer */
            uint32_t gp_phys = (uint32_t)ctx.r28 & 0x1FFFFFFF;
            if (gp_phys < RAM_SIZE - 64) {
                uint32_t cmd_count = *(uint32_t*)(g_rdram + gp_phys + 0);
                uint32_t buf_ptr = *(uint32_t*)(g_rdram + gp_phys + 4);
                uint32_t cmd_id = *(uint32_t*)(g_rdram + gp_phys + 8);
                fprintf(stderr, "[debug]   GP+0=%08X GP+4=%08X GP+8=%08X GP+C=%08X\n",
                        cmd_count, buf_ptr, cmd_id,
                        *(uint32_t*)(g_rdram + gp_phys + 12));
            }
        }

        /* Also run task callbacks directly (legacy path that produces Voodoo I/O) */
        for (int t = 0; t < task_count; t++) {
            recomp_func_t* task_func = get_function((int32_t)task_table[t].callback_vram);
            if (task_func) {
                ctx.r4 = (gpr)task_table[t].id;
                task_func(g_rdram, &ctx);
            }
        }

        /* TODO: The game's rendering pipeline requires cooperative multithreading
         * (RTOS coroutines). The task callbacks block on event_wait and produce
         * messages that other tasks consume. Without a proper fiber/coroutine
         * implementation, the rendering chain can't progress.
         *
         * Next steps:
         * 1. Implement setjmp/longjmp based coroutines for task switching
         * 2. Or use Windows fibers/pthreads for actual cooperative scheduling
         * 3. Each task runs until event_wait, then yields to the scheduler
         * 4. Scheduler runs the next ready task (one with pending messages)
         */

        if (frame < 10 || frame % 20 == 0) {
            fprintf(stderr, "[frame %d] r2=0x%08X vblank=%u tasks=%d\n",
                    frame, (uint32_t)ctx.r2, *vblank, task_count);
        }
    }

    extern uint32_t voodoo_get_write_count(void);
    extern uint32_t voodoo_get_nonzero_count(void);
    printf("  Ran %d frames. Voodoo swaps: %d, writes: %u total (%u non-zero)\n",
           max_frames, g_voodoo.swap_count, voodoo_get_write_count(), voodoo_get_nonzero_count());

    /* Dump display list and GP-relative area to check for rendering commands */
    {
        uint32_t gp_phys = (uint32_t)ctx.r28 & 0x1FFFFFFF;
        fprintf(stderr, "[debug] Post-run: GP phys=0x%06X\n", gp_phys);
        if (gp_phys < RAM_SIZE - 64) {
            fprintf(stderr, "[debug] GP area:\n");
            for (int i = 0; i < 32; i++) {
                uint32_t v = *(uint32_t*)(g_rdram + gp_phys + i * 4);
                if (v) fprintf(stderr, "  GP[%+d] = 0x%08X\n", i * 4, v);
            }
        }

        /* Check rendering buffer pointer at 0x801DFED4 and computed address */
        {
            uint32_t rbuf = *(uint32_t*)(g_rdram + 0x001DFED4);
            uint32_t render_ptr = *(uint32_t*)(g_rdram + 0x0022A454);
            uint32_t voodoo_base = *(uint32_t*)(g_rdram + 0x001AA660);
            uint32_t render_ptr2 = *(uint32_t*)(g_rdram + 0x0022A444);
            fprintf(stderr, "[debug] Render buf=0x%08X ptr@A454=0x%08X ptr@A444=0x%08X vbase=0x%08X\n",
                    rbuf, render_ptr, render_ptr2, voodoo_base);

            /* Check if the 70KB rendering context at 0x001E6A20 has any data */
            int ctx_nz = 0;
            for (uint32_t ca = 0x001E6A20; ca < 0x001E6A20 + 70316 && ca < RAM_SIZE; ca += 4) {
                uint32_t cv = *(uint32_t*)(g_rdram + ca);
                if (cv != 0) {
                    ctx_nz++;
                    if (ctx_nz <= 30) {
                        fprintf(stderr, "  ctx[+0x%05X] = 0x%08X\n", ca - 0x001E6A20, cv);
                    }
                }
            }
            fprintf(stderr, "[debug] Rendering context: %d non-zero words\n", ctx_nz);

            /* Also check the entry[0x11178] ready flag */
            uint32_t ready = *(uint32_t*)(g_rdram + 0x001E6A20 + 0x11178);
            fprintf(stderr, "[debug] Voodoo ready flag entry[0x11178] = %u\n", ready);
        }

        /* Check display list buffer */
        uint32_t dl_phys = 0x700000;
        int dl_nonzero = 0;
        for (uint32_t i = 0; i < 0x10000; i += 4) {
            if (*(uint32_t*)(g_rdram + dl_phys + i) != 0) dl_nonzero++;
        }
        fprintf(stderr, "[debug] Display list at 0x700000: %d non-zero words / %d\n",
                dl_nonzero, 0x10000 / 4);

        /* Check the big alloc area (around 0x236000-0x400000) */
        int alloc_nz = 0;
        for (uint32_t i = 0x236000; i < 0x400000; i += 4) {
            if (*(uint32_t*)(g_rdram + i) != 0) alloc_nz++;
        }
        fprintf(stderr, "[debug] Game alloc area (0x236000-0x400000): %d non-zero words\n", alloc_nz);

        /* Scan ALL RAM for Voodoo PCI address references */
        int voodoo_ptrs = 0;
        fprintf(stderr, "[debug] Scanning for Voodoo PCI addresses (0x08100000+)...\n");
        for (uint32_t i = 0; i < RAM_SIZE - 4; i += 4) {
            uint32_t v = *(uint32_t*)(g_rdram + i);
            if (v >= 0x08100000 && v < 0x09000000) {
                voodoo_ptrs++;
                if (voodoo_ptrs <= 80) {
                    fprintf(stderr, "  [0x%06X] = 0x%08X (Voodoo off 0x%06X)\n",
                            i, v, v - 0x08100000);
                }
            }
        }
        fprintf(stderr, "[debug] Total Voodoo PCI refs: %d\n", voodoo_ptrs);

        /* Also scan for command list patterns: look for clusters of sequential
         * Voodoo register offsets (0x000-0x3FF range) in the heap area */
        fprintf(stderr, "[debug] Scanning heap for Voodoo command list patterns...\n");
        int cmd_lists_found = 0;
        for (uint32_t i = 0x236000; i < RAM_SIZE - 16; i += 4) {
            uint32_t w0 = *(uint32_t*)(g_rdram + i);
            uint32_t w1 = *(uint32_t*)(g_rdram + i + 4);
            /* Look for (count, voodoo_register) patterns */
            if (w0 > 0 && w0 < 1000 && w1 >= 0x08100000 && w1 < 0x09000000) {
                cmd_lists_found++;
                if (cmd_lists_found <= 10) {
                    fprintf(stderr, "  Possible cmd list at 0x%06X: count=%d dest=0x%08X",
                            i, w0, w1);
                    uint32_t w2 = *(uint32_t*)(g_rdram + i + 8);
                    uint32_t w3 = *(uint32_t*)(g_rdram + i + 12);
                    fprintf(stderr, " next=[0x%08X, 0x%08X]\n", w2, w3);
                }
            }
        }
        fprintf(stderr, "[debug] Possible command lists found: %d\n", cmd_lists_found);
    }

    /* Dump framebuffer as raw PPM image */
    if (g_voodoo.swap_count > 0 || 1) {
        /* Debug HUD disabled — show actual game rendering */

        FILE* fb = fopen("framebuffer.ppm", "wb");
        if (fb) {
            int w = g_voodoo.width > 0 ? g_voodoo.width : 512;
            int h = g_voodoo.height > 0 ? g_voodoo.height : 256;
            if (w > 640) w = 640;
            if (h > 480) h = 480;
            fprintf(fb, "P6\n%d %d\n255\n", w, h);
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    /* Dump front buffer (swapped from back after rendering) */
                    uint16_t pixel = g_voodoo.framebuffer[y * 640 + x];
                    /* RGB565 to RGB888 */
                    uint8_t r = (pixel >> 11) << 3;
                    uint8_t g = ((pixel >> 5) & 0x3F) << 2;
                    uint8_t b = (pixel & 0x1F) << 3;
                    fputc(r, fb); fputc(g, fb); fputc(b, fb);
                }
            }
            fclose(fb);
            printf("  Framebuffer dumped to framebuffer.ppm (%dx%d)\n", w, h);
        }
    }

    /* Check heap state after entry point returns */
    uint32_t heap_head = *(uint32_t*)(g_rdram + 0x001A1E90);
    uint32_t heap_first_word = 0;
    if ((heap_head & 0x1FFFFFFF) < RAM_SIZE - 4) {
        heap_first_word = *(uint32_t*)(g_rdram + (heap_head & 0x1FFFFFFF));
    }
    fprintf(stderr, "[debug] Heap head [0x801A1E90] = 0x%08X, first word = 0x%08X\n",
            heap_head, heap_first_word);

    printf("  Entrypoint returned (r2=0x%08X).\n", (uint32_t)ctx.r2);
    return 0;
}

#endif
