/*
 * Standalone check of the DCS2 board emulation (dcs.c + adsp2100.c): boots
 * sound102.u95 and runs the boot ROM's self tests the way the game's
 * snd_dbg_boot_info() does (SRAM 0x3A -> 0xCC01, DRAM 0x4A -> 0xCC02,
 * tone 0xAA -> 0xCC04), writing the tone to a WAV.
 *
 *   cl /O2 /Isrc/runtime src/runtime/dcs_test.c src/runtime/dcs.c src/runtime/adsp2100.c
 *   dcs_test [sound102.u95] [out.wav]
 * Exit code 0 when every reply matches and the tone is not silent.
 */
#include "dcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static int g_input_empty = 1, g_output_full;
static uint16_t fifo_read(void) { return 0; }
static uint16_t fifo_status(void) { return 0x08; }
static void input_empty(int s) { g_input_empty = s; }
static void output_full(int s) { g_output_full = s; }

static FILE* g_wav;
static uint32_t g_frames;
static double g_sumsq;
static int g_peak;

static void wav_header(int rate) {
    uint8_t h[44];
    uint32_t bytes = g_frames * 4;
    memcpy(h, "RIFF", 4); *(uint32_t*)(h + 4) = 36 + bytes; memcpy(h + 8, "WAVEfmt ", 8);
    *(uint32_t*)(h + 16) = 16; *(uint16_t*)(h + 20) = 1; *(uint16_t*)(h + 22) = 2;
    *(uint32_t*)(h + 24) = (uint32_t)rate; *(uint32_t*)(h + 28) = (uint32_t)rate * 4; *(uint16_t*)(h + 32) = 4; *(uint16_t*)(h + 34) = 16;
    memcpy(h + 36, "data", 4); *(uint32_t*)(h + 40) = bytes;
    fseek(g_wav, 0, SEEK_SET); fwrite(h, 1, 44, g_wav); fseek(g_wav, 0, SEEK_END);
}

/* run the board for `ms` milliseconds in 0.25 ms steps, collecting audio */
static void run(double ms) {
    int16_t buf[4096];
    int rate;
    for (double t = 0; t < ms; t += 0.25) {
        dcs_run(0.00025);
        int n = dcs_audio(buf, 2048, &rate);
        for (int i = 0; i < n * 2; i++) { g_sumsq += (double)buf[i] * buf[i]; if (abs(buf[i]) > g_peak) g_peak = abs(buf[i]); }
        if (g_wav && n) { fwrite(buf, 4, (size_t)n, g_wav); g_frames += (uint32_t)n; }
    }
}

/* wait for a word from the DSP, as the game's polling loop does */
static int read_word(double timeout_ms) {
    for (double t = 0; t < timeout_ms; t += 0.25) {
        if (!(dcs_control_r() & 0x400)) {   /* output not empty */
            int v = dcs_data_r();
            dcs_ack_w();
            return v;
        }
        run(0.25);
    }
    return -1;
}

static void write_word(uint16_t v) {
    for (int i = 0; i < 4000 && !(dcs_control_r() & 0x800); i++) run(0.25);
    dcs_data_w(v);
}

static int reset_board(void) {
    dcs_reset_w(0); run(2); dcs_reset_w(1);
    return read_word(100);
}

int main(int argc, char** argv) {
    static uint8_t rom[0x8000];
    const char* rom_path = argc > 1 ? argv[1] : "sound102.u95";
    const char* wav_path = argc > 2 ? argv[2] : "dcs_tone.wav";
    FILE* f = fopen(rom_path, "rb");
    if (!f) { fprintf(stderr, "cannot open %s\n", rom_path); return 2; }
    size_t n = fread(rom, 1, sizeof(rom), f);
    fclose(f);

    dcs_init(rom, n);
    dcs_callbacks cb = { fifo_read, fifo_status, input_empty, output_full };
    dcs_set_callbacks(&cb);

    static const struct { uint16_t cmd, want; const char* what; double ms; } tests[] = {
        { 0x3A, 0xCC01, "SRAM test", 3000 }, { 0x4A, 0xCC02, "DRAM test", 6000 }, { 0xAA, 0xCC04, "tone test", 6000 },
    };
    int fails = 0;
    for (int i = 0; i < 3; i++) {
        int ready = reset_board();
        if (i == 2 && (g_wav = fopen(wav_path, "wb")) != NULL) wav_header(31250);
        g_sumsq = 0; g_peak = 0; g_frames = 0;
        write_word(tests[i].cmd);
        int r = read_word(tests[i].ms);
        printf("%-10s boot=%04X cmd=%04X reply=%04X want=%04X %s\n", tests[i].what, ready & 0xffff, tests[i].cmd,
               r & 0xffff, tests[i].want, r == tests[i].want ? "ok" : "FAIL");
        if (r != tests[i].want) fails++;
    }
    run(200);
    double rms = g_frames ? sqrt(g_sumsq / (g_frames * 2.0)) : 0;
    printf("tone: %u frames, rms %.0f, peak %d -> %s\n", g_frames, rms, g_peak, wav_path);
    if (g_wav) { wav_header(31250); fclose(g_wav); }
    if (rms < 100) fails++;
    return fails ? 1 : 0;
}
