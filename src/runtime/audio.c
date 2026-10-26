/*
 * Host audio: plays what the DCS2 board renders through WinMM waveOut.
 * audio_pump() is called once per vblank and keeps a few buffers queued.
 */
#include "dcs.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

#define NBUF 6
#define BUF_FRAMES 2048

static HWAVEOUT g_wo;
static WAVEHDR g_hdr[NBUF];
static int16_t g_buf[NBUF][BUF_FRAMES * 2];
static int g_cur, g_fill;         /* buffer being filled, frames in it */
static int g_rate;

static void open_device(int rate) {
    WAVEFORMATEX wf = { WAVE_FORMAT_PCM, 2, (DWORD)rate, (DWORD)rate * 4, 4, 16, 0 };
    if (waveOutOpen(&g_wo, WAVE_MAPPER, &wf, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) {
        fprintf(stderr, "[audio] no output device\n");
        g_wo = NULL;
        return;
    }
    for (int i = 0; i < NBUF; i++) {
        g_hdr[i].lpData = (LPSTR)g_buf[i];
        g_hdr[i].dwBufferLength = sizeof(g_buf[i]);
        g_hdr[i].dwFlags = WHDR_DONE;             /* free to fill */
    }
    g_rate = rate;
}

void audio_pump(int enabled) {
    int16_t tmp[BUF_FRAMES * 2];
    int rate = 0;
    int n = dcs_audio(tmp, BUF_FRAMES, &rate);
    if (!enabled || n <= 0 || rate <= 0) return;
    if (!g_wo && !g_rate) open_device(rate);
    if (!g_wo) return;
    int pos = 0;
    while (pos < n) {
        WAVEHDR* h = &g_hdr[g_cur];
        if (!(h->dwFlags & WHDR_DONE)) return;    /* device behind: drop the rest */
        if (h->dwFlags & WHDR_PREPARED) waveOutUnprepareHeader(g_wo, h, sizeof(WAVEHDR));
        int take = n - pos < BUF_FRAMES - g_fill ? n - pos : BUF_FRAMES - g_fill;
        memcpy(g_buf[g_cur] + g_fill * 2, tmp + pos * 2, (size_t)take * 4);
        g_fill += take;
        pos += take;
        if (g_fill == BUF_FRAMES) {
            h->dwFlags = 0;
            waveOutPrepareHeader(g_wo, h, sizeof(WAVEHDR));
            waveOutWrite(g_wo, h, sizeof(WAVEHDR));
            g_fill = 0;
            g_cur = (g_cur + 1) % NBUF;
        }
    }
}
