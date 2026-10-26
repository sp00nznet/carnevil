/*
 * Midway DCS2 sound board, Seattle flavour (ADSP-2115 @ 16 MHz, SDRC ASIC,
 * 2 MB sound DRAM, 32 KB boot ROM, stereo out).
 *
 * Follows MAME's src/mame/shared/dcs.cpp and dcs.h (copyright-holders:
 * Aaron Giles) and the sound side of src/mame/williams/midwayic.cpp
 * (Aaron Giles), used under the BSD-3-Clause license: the SDRC memory map
 * and security register, latch semantics, ADSP control registers, timer and
 * the SPORT1 autobuffer audio path. MAME's HLE transfer path and polling
 * speedups are not used; the DSP runs every instruction.
 *
 * Debug: DCS_LOG=1 traces host<->DSP latch traffic (written at exit; =2
 * writes it as it happens) plus 5-second activity counters and the DSP's
 * share of host CPU; DCS_WAV=path writes everything the board plays to a WAV;
 * DCS_DUMP=path (with DCS_LOG) saves DSP program/data memory at exit.
 */
#include "dcs.h"
#include "adsp2100.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DCS_CLOCK 16000000.0
#define SLICE_MAX 512              /* longest uninterrupted DSP run, cycles */
#define DRAM_WORDS (1u << 20)      /* 2 MB */
#define AUDIO_FRAMES 8192           /* ~0.26 s at 31250 Hz; oldest frames drop if nobody pulls */

enum { LCTRL_OUTPUT_EMPTY = 0x400, LCTRL_INPUT_EMPTY = 0x800 };
enum { PROG_FLAG_DATA_REG = 5, PROG_FLAG_CONTROL_REG = 6,
       S1_AUTOBUF_REG = 15, S1_RFSDIV_REG, S1_SCLKDIV_REG, S1_CONTROL_REG,
       S0_AUTOBUF_REG, S0_RFSDIV_REG, S0_SCLKDIV_REG, S0_CONTROL_REG,
       TIMER_SCALE_REG = 27, TIMER_COUNT_REG, TIMER_PERIOD_REG, WAITSTATES_REG, SYSCONTROL_REG };

static adsp2100 g_cpu;
static dcs_callbacks g_cb;
static int g_log;

/* DCS_LOG latch trace: kept in memory and written at exit so that logging
 * doesn't slow the handshakes it is watching */
static char* g_trace;
static size_t g_trace_len;
#define TRACE_MAX (64u << 20)
static void trace_flush(void) { if (g_trace) fwrite(g_trace, 1, g_trace_len, stderr); }
static void trace(double t, const char* what, unsigned v) {
    if (!g_log) return;
    if (!g_trace) { g_trace = malloc(TRACE_MAX); if (!g_trace) { g_log = 0; return; } atexit(trace_flush); }
    if (g_log == 2) { fprintf(stderr, "[dcs] %10.6f %s %04X\n", t, what, v); return; }   /* DCS_LOG=2: in order with the console */
    if (g_trace_len + 64 < TRACE_MAX)
        g_trace_len += (size_t)snprintf(g_trace + g_trace_len, 64, "[dcs] %10.6f %s %04X\n", t, what, v);
}

/* memory */
static uint16_t g_rom[0x8000];     /* boot ROM, one byte per word */
static uint16_t g_sram[0x4000];    /* SDRC data SRAM */
static uint16_t* g_dram;
static uint16_t g_iram[0x400];     /* 3800-3bff (512 words internal + the reserved half) */
static uint16_t* g_rmap[16];
static uint16_t* g_wmap[16];
static uint16_t g_sdrc[4];
static uint8_t g_seed;
static uint16_t g_ctrl[32];
static uint16_t g_progflags;

/* latches */
static uint16_t g_latch_control, g_input_data, g_output_data, g_output_control;
static int g_last_output_full, g_last_input_empty;
static uint64_t g_output_control_cycles;
static uint32_t g_fifo_force_empty_pc;

/* time */
static uint64_t g_cycles;          /* cycles at the start of the current slice */
static int g_slice_len;
static double g_target;
static int g_in_reset;

/* timer */
static int g_timer_enable, g_timer_ignore, g_timer_armed;
static uint64_t g_timer_start_cycles, g_timer_next, g_timers_fired;
static uint32_t g_timer_start_count, g_timer_scale = 1, g_timer_period;

/* SPORT1 autobuffer (audio) and SPORT0 (1 kHz tick) */
static int g_s1_armed, g_ireg, g_size, g_incs, g_ireg_base;
static double g_s1_next, g_s1_period, g_rate = 31250.0;
static int g_s0_armed;
static double g_s0_next;

/* audio ring */
static int16_t g_audio[AUDIO_FRAMES * 2];
static unsigned g_audio_w, g_audio_r;
static FILE* g_wav;
static uint32_t g_wav_frames;

#define SDRC_ROM_ST ((g_sdrc[0] >> 0) & 3)
#define SDRC_ROM_SZ ((g_sdrc[0] >> 4) & 1)
#define SDRC_ROM_MS ((g_sdrc[0] >> 5) & 1)
#define SDRC_ROM_PG ((g_sdrc[0] >> 7) & 7)
#define SDRC_SM_EN  ((g_sdrc[0] >> 11) & 1)
#define SDRC_SM_BK  ((g_sdrc[0] >> 12) & 1)
#define SDRC_SMODE  ((g_sdrc[0] >> 13) & 7)
#define SDRC_DM_ST  ((g_sdrc[1] >> 0) & 3)
#define SDRC_DM_PG  ((g_sdrc[2] >> 0) & 0x7ff)

static uint64_t cur_cycles(void) { return g_cycles + (uint64_t)(g_slice_len - (g_cpu.icount > 0 ? g_cpu.icount : 0)); }

/* ---------------------------------------------------------------- SDRC */
static void map_blocks(uint16_t** map, int start, int len, uint16_t* base) {
    for (int a = start; a < start + len && a < 0x3800; a += 0x400) map[a >> 10] = base + (a - start);
}

static void sdrc_remap(void) {
    memset(g_rmap, 0, sizeof(g_rmap));
    memset(g_wmap, 0, sizeof(g_wmap));
    g_cpu.prog_ext = SDRC_SM_EN;
    if (SDRC_SM_EN) {
        if (SDRC_SM_BK == 0) {
            map_blocks(g_rmap, 0x0800, 0x1000, g_sram + 0x0000);
            map_blocks(g_rmap, 0x1800, 0x1000, g_sram + 0x1000);
        } else {
            map_blocks(g_rmap, 0x1800, 0x1000, g_sram + 0x3000);
        }
        map_blocks(g_rmap, 0x2800, 0x1000, g_sram + 0x2000);
        memcpy(g_wmap, g_rmap, sizeof(g_rmap));
    }
    if (SDRC_ROM_MS == 1 && SDRC_ROM_ST != 3) {   /* ROM page: read only, writes fall through */
        int base = SDRC_ROM_ST == 0 ? 0x0000 : SDRC_ROM_ST == 1 ? 0x3000 : 0x3400;
        int size = (SDRC_ROM_SZ == 0 && SDRC_ROM_ST != 0) ? 4096 : 1024;
        map_blocks(g_rmap, base, size, g_rom + (SDRC_ROM_PG * 4096) % 0x8000);
    }
    if (SDRC_DM_ST != 0) {
        int base = SDRC_DM_ST == 1 ? 0x0000 : SDRC_DM_ST == 2 ? 0x3000 : 0x3400;
        uint16_t* p = g_dram + (SDRC_DM_PG * 1024u) % DRAM_WORDS;
        map_blocks(g_rmap, base, 0x400, p);
        map_blocks(g_wmap, base, 0x400, p);
    }
    /* ponytail: 3a00-3bff (reserved on the 2115) reads as RAM, not 0xffff */
    g_rmap[14] = g_wmap[14] = g_iram;
}

static uint16_t sdrc_r(int off) {
    static const uint16_t k_sec[8] = { 0x5a81, 0x5aa4, 0, 0x5ab9, 0x5a03, 0x5a69, 0x5a20, 0x5aff };
    if (off != 3) return g_sdrc[off];
    if (SDRC_SMODE == 2) return (uint16_t)(0x5a00 | ((g_seed & 0x3f) << 1));
    return k_sec[SDRC_SMODE];
}

static void sdrc_w(int off, uint16_t data) {
    if (off < 3) { g_sdrc[off] = data; sdrc_remap(); return; }
    switch (SDRC_SMODE) {
        case 1: g_seed = (uint8_t)data; break;
        case 3: g_seed = (uint8_t)((g_seed << 1) | 1); break;
        case 4: g_seed = (uint8_t)(g_seed + (g_seed >> 1)); break;
        case 5: g_seed ^= (uint8_t)((g_seed << 1) | 1); break;
        case 6: g_seed = (uint8_t)((((g_seed << 7) ^ (g_seed << 5) ^ (g_seed << 4) ^ (g_seed << 3)) & 0x80) | (g_seed >> 1)); break;
        case 7: g_seed = (uint8_t)~g_seed; break;
    }
}

/* ---------------------------------------------------------------- latches */
static void input_empty(int s) { if (g_cb.input_empty) g_cb.input_empty(s); }
static void output_full(int s) { if (g_cb.output_full) g_cb.output_full(s); }

static uint16_t latch_status_r(void) {
    uint16_t r = 0;
    if (!(g_latch_control & LCTRL_INPUT_EMPTY)) r |= 0x80;
    if (g_latch_control & LCTRL_OUTPUT_EMPTY) r |= 0x40;
    if (g_cb.fifo_status) r |= g_cb.fifo_status() & 0x38;
    /* As MAME's I/O ASIC: right after the DSP drained the FIFO, its next status
     * read says empty even if the host has refilled it in between. */
    if (g_fifo_force_empty_pc && g_cpu.pc >= g_fifo_force_empty_pc && g_cpu.pc < g_fifo_force_empty_pc + 0x10) {
        g_fifo_force_empty_pc = 0;
        r |= 0x08;
    }
    return r;
}

static unsigned g_n_poll, g_n_fifo_w, g_n_fifo_r, g_n_s1, g_n_cmd;   /* DCS_LOG counters */
static uint16_t fifo_input_r(void) {
    g_n_fifo_r++;
    if (!g_cb.fifo_read) return 0xffff;
    uint16_t v = g_cb.fifo_read();
    if (g_cb.fifo_status && (g_cb.fifo_status() & 0x08)) g_fifo_force_empty_pc = g_cpu.pc;
    return v;
}

/* ---------------------------------------------------------------- timer */
static void update_timer_count(void) {
    if (!g_timer_enable) return;
    uint64_t clocks = (cur_cycles() - g_timer_start_cycles) / g_timer_scale;
    if (clocks < (uint64_t)g_timer_start_count + 1) g_timer_start_count -= (uint32_t)clocks;
    else {
        clocks -= g_timer_start_count + 1;
        clocks -= (clocks / (g_timer_period + 1)) * (g_timer_period + 1);
        g_timer_start_count = g_timer_period - (uint32_t)clocks;
    }
    g_ctrl[TIMER_COUNT_REG] = (uint16_t)g_timer_start_count;
}

static void reset_timer(void) {
    if (!g_timer_enable) return;
    g_timer_start_cycles = cur_cycles();
    g_timers_fired = 0;
    /* a timer IRQ that only refreshes DRAM (ENA SEC_REG; SI=DM(0482);
     * DM(0482)=SI; DIS SEC_REG; RTI) changes nothing we emulate: skip it */
    if (!g_timer_ignore && g_cpu.prog[0x18] == 0x0c0030 && g_cpu.prog[0x19] == 0x804828 &&
        g_cpu.prog[0x1a] == 0x904828 && g_cpu.prog[0x1b] == 0x0c0020 && g_cpu.prog[0x1c] == 0x0a001f)
        g_timer_ignore = 1;
    g_timer_armed = !g_timer_ignore;
    g_timer_next = g_timer_start_cycles + (uint64_t)g_timer_scale * (g_timer_start_count + 1);
}

static void timer_fire(void) {
    g_timers_fired++;
    g_timer_next = g_timer_start_cycles + (uint64_t)g_timer_scale * (g_timer_start_count + 1 + g_timers_fired * (uint64_t)(g_timer_period + 1));
    g_timer_armed = !g_timer_ignore && (g_timer_period > 10 || g_timer_scale > 1);
    adsp_set_irq(&g_cpu, ADSP_TIMER, 1);
    adsp_set_irq(&g_cpu, ADSP_TIMER, 0);
}

void adsp_timer_enable(int state) {
    if (state) { g_timer_enable = 1; reset_timer(); }
    else { update_timer_count(); g_timer_enable = 0; g_timer_armed = 0; }
}

/* ---------------------------------------------------------------- SPORT1 audio */
static void set_ireg(int n, uint32_t v) {
    g_cpu.i[n] = v & 0x3fff;
    g_cpu.base[n] = g_cpu.i[n] & g_cpu.lmask[n];
}

static void wav_header(void) {
    uint8_t h[44];
    uint32_t rate = (uint32_t)(g_rate + 0.5), bytes = g_wav_frames * 4;
    memcpy(h, "RIFF", 4); *(uint32_t*)(h + 4) = 36 + bytes; memcpy(h + 8, "WAVEfmt ", 8);
    *(uint32_t*)(h + 16) = 16; *(uint16_t*)(h + 20) = 1; *(uint16_t*)(h + 22) = 2;
    *(uint32_t*)(h + 24) = rate; *(uint32_t*)(h + 28) = rate * 4; *(uint16_t*)(h + 32) = 4; *(uint16_t*)(h + 34) = 16;
    memcpy(h + 36, "data", 4); *(uint32_t*)(h + 40) = bytes;
    fseek(g_wav, 0, SEEK_SET); fwrite(h, 1, 44, g_wav); fseek(g_wav, 0, SEEK_END);
}
static void wav_close(void) { if (g_wav) { wav_header(); fclose(g_wav); g_wav = NULL; } }

static void audio_push(int16_t l, int16_t r) {
    unsigned w = g_audio_w % AUDIO_FRAMES;
    g_audio[w * 2] = l; g_audio[w * 2 + 1] = r;
    g_audio_w++;
    if (g_audio_w - g_audio_r > AUDIO_FRAMES) g_audio_r = g_audio_w - AUDIO_FRAMES;   /* nobody pulling: drop oldest */
    if (g_wav) {
        int16_t f[2] = { l, r };
        fwrite(f, 2, 2, g_wav);
        if (++g_wav_frames % 31250 == 0) wav_header();
    }
}

static void sport1_irq(void) {   /* one half buffer has gone out */
    int reg = (int)g_cpu.i[g_ireg];
    int count = g_size / (2 * (g_incs ? g_incs : 1));
    int16_t buf[0x800];
    if (count < 0) count = 0;
    if (count > 0x800) count = 0x800;
    for (int i = 0; i < count; i++) { buf[i] = (int16_t)adsp_data_read((uint32_t)reg & 0x3fff); reg += g_incs; }
    /* channel 0 is the right speaker, channel 1 the left (MAME's routing) */
    for (int i = 0; i + 1 < count; i += 2) audio_push(buf[i + 1], buf[i]);
    g_ireg_base = (int)g_cpu.base[g_ireg];
    if (reg >= g_ireg_base + g_size) {
        reg = g_ireg_base;
        adsp_set_irq(&g_cpu, ADSP_IRQ1, 1);
        adsp_check_irqs(&g_cpu);
        adsp_set_irq(&g_cpu, ADSP_IRQ1, 0);
    }
    set_ireg(g_ireg, (uint32_t)reg);
}

static void recompute_sample_rate(void) {
    if (g_ctrl[S1_CONTROL_REG] & 0x4000) g_rate = DCS_CLOCK / (2.0 * (g_ctrl[S1_SCLKDIV_REG] + 1) * 16 * 2);
    else g_rate = 31250.0;
    g_s1_armed = g_incs != 0;
    if (g_s1_armed) {
        g_s1_period = DCS_CLOCK / g_rate * g_size / (2.0 * 2 * g_incs);
        if (g_s1_period < 1) g_s1_period = 1;
        g_s1_next = (double)cur_cycles() + g_s1_period;
    }
    if (g_log) fprintf(stderr, "[dcs] audio: %.0f Hz, buffer I%d size %d step %d\n", g_rate, g_ireg, g_size, g_incs);
}

void adsp_sport_tx(int port, uint16_t data) {
    (void)data;
    if (port != 1) return;
    if ((g_ctrl[SYSCONTROL_REG] & 0x0800) && (g_ctrl[S1_AUTOBUF_REG] & 0x0002)) {
        g_ireg = (g_ctrl[S1_AUTOBUF_REG] >> 9) & 7;
        int mreg = ((g_ctrl[S1_AUTOBUF_REG] >> 7) & 3) | (g_ireg & 4);
        uint32_t source = g_cpu.i[g_ireg] & ~0xfu;
        g_incs = g_cpu.m[mreg];
        g_size = (int)g_cpu.l[g_ireg];
        set_ireg(g_ireg, source);
        g_ireg_base = (int)source;
        recompute_sample_rate();
        return;
    }
    g_s1_armed = 0;
}

/* ---------------------------------------------------------------- boot / reset */
static void dcs_boot(void) {
    uint8_t buf[0x1000];
    const uint16_t* base = g_rom + (SDRC_ROM_PG * 0x1000) % 0x8000;
    for (int i = 0; i < 0x1000; i++) buf[i] = (uint8_t)base[i];
    adsp_load_boot_data(&g_cpu, buf);
}

static void dcs_reset(void) {
    memset(g_sdrc, 0, sizeof(g_sdrc));
    sdrc_remap();
    memset(g_ctrl, 0, sizeof(g_ctrl));
    adsp_set_irq(&g_cpu, ADSP_IRQ0, 0);
    adsp_set_irq(&g_cpu, ADSP_IRQ1, 0);
    adsp_set_irq(&g_cpu, ADSP_IRQ2, 0);
    g_latch_control |= LCTRL_INPUT_EMPTY | LCTRL_OUTPUT_EMPTY;
    if (!g_last_input_empty) input_empty(g_last_input_empty = 1);
    if (g_last_output_full) output_full(g_last_output_full = 0);
    dcs_boot();
    adsp_reset(&g_cpu);
    g_timer_ignore = 0; g_timer_enable = 0; g_timer_scale = 1; g_timer_armed = 0;
    g_s1_armed = g_s0_armed = 0;
}

/* ---------------------------------------------------------------- ADSP control registers */
static uint16_t adsp_control_r(int off) {
    switch (off) {
        case PROG_FLAG_DATA_REG: {
            uint16_t r = (uint16_t)((g_ctrl[PROG_FLAG_CONTROL_REG] & g_ctrl[PROG_FLAG_DATA_REG]) | (g_progflags & ~g_ctrl[PROG_FLAG_CONTROL_REG]));
            g_progflags ^= 0x6;
            return r;
        }
        case 0: return 0xffff;
        case TIMER_COUNT_REG: update_timer_count(); return g_ctrl[off];
        default: return g_ctrl[off];
    }
}

static void adsp_control_w(int off, uint16_t data) {
    g_ctrl[off] = data;
    switch (off) {
        case SYSCONTROL_REG:
            if (data & 0x0200) {   /* software reboot from the current ROM page */
                if (g_log) fprintf(stderr, "[dcs] reboot from ROM page %d\n", SDRC_ROM_PG);
                adsp_reset(&g_cpu);
                dcs_boot();
                g_ctrl[SYSCONTROL_REG] = 0;
            }
            if (!(data & 0x0800)) g_s1_armed = 0;
            if (data & 0x1000) { g_s0_armed = 1; g_s0_next = (double)cur_cycles() + DCS_CLOCK * 10e-6; }
            else g_s0_armed = 0;
            break;
        case S1_AUTOBUF_REG:
            if (!(data & 0x0002)) g_s1_armed = 0;
            break;
        case TIMER_SCALE_REG:
            data = (uint16_t)((data & 0xff) + 1);
            if (data != g_timer_scale) { update_timer_count(); g_timer_scale = data; reset_timer(); }
            break;
        case TIMER_COUNT_REG:
            g_timer_start_count = data;
            reset_timer();
            break;
        case TIMER_PERIOD_REG:
            if (data != g_timer_period) { update_timer_count(); g_timer_period = data; reset_timer(); }
            break;
    }
}

/* ---------------------------------------------------------------- DSP data bus */
uint16_t adsp_data_read(uint32_t a) {
    uint16_t* p = g_rmap[a >> 10];
    if (p) return p[a & 0x3ff];
    switch (a) {
        case 0x0400: return g_input_data;
        case 0x0402: g_output_control_cycles = cur_cycles(); return g_output_control;
        case 0x0403: return latch_status_r();
        case 0x0404: case 0x0405: case 0x0406: case 0x0407: return fifo_input_r();
        case 0x0480: case 0x0481: case 0x0482: case 0x0483: return sdrc_r(a & 3);
    }
    if (a >= 0x3fe0) return adsp_control_r(a & 0x1f);
    return 0xffff;
}

void adsp_data_write(uint32_t a, uint16_t v) {
    uint16_t* p = g_wmap[a >> 10];
    if (p) { p[a & 0x3ff] = v; return; }
    switch (a) {
        case 0x0400:   /* input latch acknowledge */
            if (!g_last_input_empty) input_empty(g_last_input_empty = 1);
            g_latch_control |= LCTRL_INPUT_EMPTY;
            adsp_set_irq(&g_cpu, ADSP_IRQ2, 0);
            return;
        case 0x0401:
            trace(cur_cycles() / DCS_CLOCK, "dsp->host", v);
            g_output_data = v;
            g_latch_control &= ~LCTRL_OUTPUT_EMPTY;
            if (!g_last_output_full) output_full(g_last_output_full = 1);
            return;
        case 0x0402: g_output_control = v; g_output_control_cycles = 0; return;
        case 0x0480: case 0x0481: case 0x0482: case 0x0483: sdrc_w(a & 3, v); return;
    }
    if (a >= 0x3fe0) adsp_control_w(a & 0x1f, v);
}

/* ---------------------------------------------------------------- host side (dcs.h) */
void dcs_init(const uint8_t* rom, size_t rom_size) {
    const char* wav = getenv("DCS_WAV");
    g_log = getenv("DCS_LOG") ? atoi(getenv("DCS_LOG")) : 0;
    adsp_init(&g_cpu);
    g_dram = calloc(DRAM_WORDS, sizeof(uint16_t));
    memset(g_rom, 0, sizeof(g_rom));
    for (size_t i = 0; i < rom_size && i < 0x8000; i++) g_rom[i] = rom[i];
    if (wav && (g_wav = fopen(wav, "wb")) != NULL) { wav_header(); atexit(wav_close); }
    dcs_reset();
}

void dcs_set_callbacks(const dcs_callbacks* cb) { g_cb = *cb; }

void dcs_reset_w(int state) {
    if (state == g_in_reset) trace(g_cycles / DCS_CLOCK, state ? "run" : "reset", 0);
    if (!state) { dcs_reset(); g_in_reset = 1; }
    else g_in_reset = 0;
}

void dcs_data_w(uint16_t data) {
    trace(g_cycles / DCS_CLOCK, "host->dsp", data);
    g_n_cmd++;
    adsp_set_irq(&g_cpu, ADSP_IRQ2, 1);
    if (g_last_input_empty) input_empty(g_last_input_empty = 0);
    g_latch_control &= ~LCTRL_INPUT_EMPTY;
    g_input_data = data;
}

uint16_t dcs_data_r(void) {
    trace(g_cycles / DCS_CLOCK, "host reads", g_output_data);
    if (g_last_output_full) output_full(g_last_output_full = 0);
    return g_output_data;
}

void dcs_ack_w(void) { trace(g_cycles / DCS_CLOCK, "host ack", g_latch_control); g_latch_control |= LCTRL_OUTPUT_EMPTY; }
int dcs_data2_r(void) { return g_output_control; }
/* MAME uses this only for its HLE transfers; the DSP reads the FIFO itself */
void dcs_fifo_notify(int count, int max) { (void)count; (void)max; g_n_fifo_w++; }

static void run_until(double until) {
    while ((double)g_cycles < until) {
        double next = until, now;
        if (g_timer_armed && (double)g_timer_next < next) next = (double)g_timer_next;
        if (g_s1_armed && g_s1_next < next) next = g_s1_next;
        if (g_s0_armed && g_s0_next < next) next = g_s0_next;
        int n = (int)(next - (double)g_cycles + 0.999);
        if (n < 1) n = 1;
        if (n > SLICE_MAX) n = SLICE_MAX;
        g_slice_len = n;
        adsp_execute(&g_cpu, n);
        g_cycles += (uint64_t)n;
        g_slice_len = 0;
        g_cpu.icount = 0;
        now = (double)g_cycles;
        if (g_timer_armed && (double)g_timer_next <= now) timer_fire();
        if (g_s1_armed && g_s1_next <= now) { g_s1_next += g_s1_period; sport1_irq(); g_n_s1++; }
        if (g_s0_armed && g_s0_next <= now) {
            g_s0_next += DCS_CLOCK / 1000.0;
            /* as MAME: skip the tick if the DSP's main loop just read output_control,
             * whose read-modify-write the SPORT0 handler would otherwise race */
            if (g_cycles - g_output_control_cycles > 5) {
                adsp_set_irq(&g_cpu, ADSP_SPORT0_RX, 1);
                adsp_set_irq(&g_cpu, ADSP_SPORT0_RX, 0);
            }
        }
    }
}

/* timing stats for DCS_LOG */
static double g_host_time, g_wall_time, g_t0;
static double host_seconds(void) { struct timespec ts; timespec_get(&ts, TIME_UTC); return ts.tv_sec + ts.tv_nsec * 1e-9; }
static void report(void) {
    fprintf(stderr, "[dcs] %.2f s of DSP time emulated in %.2f s of host time (%.1f%% of one core)\n",
            g_cycles / DCS_CLOCK, g_host_time, g_wall_time > 0 ? 100.0 * g_host_time / g_wall_time : 0.0);
    if (getenv("DCS_DUMP")) {   /* program + data memory, for offline study */
        FILE* f = fopen(getenv("DCS_DUMP"), "wb");
        if (f) {
            uint16_t dm[0x4000];
            for (uint32_t a = 0; a < 0x4000; a++) dm[a] = (a >= 0x400 && a < 0x500) || a >= 0x3fe0 ? 0 : adsp_data_read(a);
            fwrite(g_cpu.prog, 4, 0x4000, f); fwrite(dm, 2, 0x4000, f); fwrite(g_ctrl, 2, 32, f); fclose(f);
        }
    }
}
static void stats_begin(void) {
    static int registered;
    if (!g_log) return;
    if (!registered) { registered = 1; atexit(report); }
    g_t0 = host_seconds();
}
static void stats_end(void) { if (g_log) g_host_time += host_seconds() - g_t0; }

/* The host is polling the latch status. As MAME tightens the CPU interleave
 * here, let the DSP run ~10 us further on each poll so a handshake takes
 * microseconds, as on the real board, instead of one hw_tick per word.
 * dcs_run absorbs the lead afterwards. The cap is generous on purpose: the
 * game's latch timeouts are wall-clock, so if the host process stalls (OS
 * scheduling) mid-wait, the DSP must be able to catch up from inside the
 * host's poll loop; with a 2 ms cap, 40 ms stalls failed sound init. */
#define AHEAD_MAX (DCS_CLOCK * 0.05)
int dcs_control_r(void) {
    g_n_poll++;
    uint16_t before = g_latch_control;
    if (g_in_reset || (double)g_cycles >= g_target + AHEAD_MAX) return g_latch_control;
    stats_begin();
    for (int k = 0; k < 5 && g_latch_control == before; k++) run_until((double)g_cycles + 32);
    stats_end();
    { static uint16_t last; if (g_latch_control != last) { last = g_latch_control; trace(g_cycles / DCS_CLOCK, "status", last); } }
    return g_latch_control;
}

void dcs_run(double seconds) {
    if (seconds <= 0) return;
    if (seconds > 0.1) seconds = 0.1;   /* host stalled: don't try to catch up a long gap */
    g_target += seconds * DCS_CLOCK;
    if (g_log) g_wall_time += seconds;
    if (g_in_reset) { if ((double)g_cycles < g_target) g_cycles = (uint64_t)g_target; return; }
    stats_begin();
    run_until(g_target);
    stats_end();
    if (g_log) {
        static double next = 5 * DCS_CLOCK;
        if ((double)g_cycles >= next) {
            next += 5 * DCS_CLOCK;
            fprintf(stderr, "[dcs] t=%.0f s: %u status polls, %u host words, %u FIFO writes, %u DSP FIFO reads, %u audio blocks\n",
                    g_cycles / DCS_CLOCK, g_n_poll, g_n_cmd, g_n_fifo_w, g_n_fifo_r, g_n_s1);
            g_n_poll = g_n_cmd = g_n_fifo_w = g_n_fifo_r = g_n_s1 = 0;
        }
    }
}

int dcs_audio(int16_t* out, int max_frames, int* sample_rate) {
    int n = 0;
    *sample_rate = (int)(g_rate + 0.5);
    while (n < max_frames && g_audio_r != g_audio_w) {
        unsigned r = g_audio_r++ % AUDIO_FRAMES;
        out[n * 2] = g_audio[r * 2];
        out[n * 2 + 1] = g_audio[r * 2 + 1];
        n++;
    }
    return n;
}
