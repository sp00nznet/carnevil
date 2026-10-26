/*
 * CarnEvil, recompiled.
 *
 * Boots the way the board does after its boot ROM: the BIOS image is copied
 * from the boot ROM to 0x80000000 and the CPU starts at its entry 0x80004000.
 * From there the BIOS sets up the hardware, mounts the disk, loads GAME.EXE
 * and runs it.
 *
 * usage: carnevil [--frames N] [--shot out.ppm] [--headless] [--nvram file]
 *   --frames N  quit after N vblanks (with --shot, save the last frame)
 * Files (relative to the working directory): carnevil1_9.u32 (boot ROM),
 * sound102.u95 (DCS2 ROM), carnevil/carnevi1.img (the hard disk, v1.0.1);
 * nvram.bin is created.
 *
 * Keys: 5 coin, 1 start, 9 service credit, F2 test switch, Esc quit.
 * Mouse: player 1 gun (left = trigger, right = reload).
 */
#include "hw.h"
#include <stdio.h>
#include <windows.h>

extern void rtos_80004000(uint8_t* rdram, recomp_context* ctx);
void lookup_add_game(void);
void lookup_add_rtos(void);
void profile_start(void);
void audio_pump(int enabled);

static HWND g_wnd;
static int g_headless, g_frames_left = -1;
static const char* g_shot;
static const char* g_nvram = "nvram.bin";
static uint32_t* g_pixels;
static int g_w = 512, g_h = 384;

static void set_bit(uint16_t* port, uint16_t bit, int pressed) {
    if (pressed) *port &= (uint16_t)~bit; else *port |= bit;
}

static void key(WPARAM vk, int down) {
    switch (vk) {
        case '5': set_bit(&g_in_system, 0x0001, down); break;
        case '1': set_bit(&g_in_system, 0x0004, down); break;
        case '2': set_bit(&g_in_system, 0x0020, down); break;
        case '9': set_bit(&g_in_system, 0x0040, down); break;
        case VK_F2: if (down) g_in_dips ^= 0x8000; break;
        case VK_ESCAPE: if (down) PostQuitMessage(0); break;
    }
}

static LRESULT CALLBACK wndproc(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
        case WM_KEYDOWN: key(w, 1); return 0;
        case WM_KEYUP:   key(w, 0); return 0;
        case WM_MOUSEMOVE: {
            RECT r; GetClientRect(h, &r);
            int x = (short)LOWORD(l), y = (short)HIWORD(l);
            if (r.right > 0 && r.bottom > 0) {
                g_gun_x[0] = (uint16_t)(x * 255 / r.right);
                g_gun_y[0] = (uint16_t)(y * 255 / r.bottom);
            }
            g_gun_buttons = (uint16_t)((g_gun_buttons & ~3) | ((w & MK_LBUTTON) ? 1 : 0) | ((w & MK_RBUTTON) ? 2 : 0));
            return 0;
        }
        case WM_LBUTTONDOWN: case WM_LBUTTONUP: case WM_RBUTTONDOWN: case WM_RBUTTONUP:
            g_gun_buttons = (uint16_t)((g_gun_buttons & ~3) | ((w & MK_LBUTTON) ? 1 : 0) | ((w & MK_RBUTTON) ? 2 : 0));
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC dc = BeginPaint(h, &ps);
            RECT r; GetClientRect(h, &r);
            BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), g_w, -g_h, 1, 32, BI_RGB } };
            StretchDIBits(dc, 0, 0, r.right, r.bottom, 0, 0, g_w, g_h, g_pixels, &bi, DIB_RGB_COLORS, SRCCOPY);
            EndPaint(h, &ps);
            return 0;
        }
        case WM_CLOSE: PostQuitMessage(0); return 0;
    }
    return DefWindowProcA(h, m, w, l);
}

static void save_shot(void) {
    FILE* f = fopen(g_shot, "wb");
    if (!f) return;
    fprintf(f, "P6\n%d %d\n255\n", g_w, g_h);
    for (int i = 0; i < g_w * g_h; i++) {
        uint32_t p = g_pixels[i];
        fputc((p >> 16) & 0xFF, f); fputc((p >> 8) & 0xFF, f); fputc(p & 0xFF, f);
    }
    fclose(f);
    fprintf(stderr, "[video] saved %s\n", g_shot);
}

void video_present(const uint16_t* fb, int stride, int width, int height) {
    if (width <= 0 || width > 640) width = 512;
    if (height <= 0 || height > 480) height = 384;
    g_w = width; g_h = height;
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            uint16_t p = fb[y * stride + x];
            uint32_t r = (p >> 11) & 0x1F, g = (p >> 5) & 0x3F, b = p & 0x1F;
            g_pixels[y * width + x] = (r << 19 | (r >> 2) << 16) | (g << 10 | (g >> 4) << 8) | (b << 3 | b >> 2);
        }
    if (g_wnd) InvalidateRect(g_wnd, NULL, FALSE);
}

int video_pump(void) {
    audio_pump(!g_headless);
    if (g_frames_left > 0 && --g_frames_left == 0) {
        if (g_shot) save_shot();
        return 0;
    }
    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) return 0;
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return 1;
}

int main(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--frames") && i + 1 < argc) g_frames_left = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--shot") && i + 1 < argc) g_shot = argv[++i];
        else if (!strcmp(argv[i], "--headless")) g_headless = 1;
        else if (!strcmp(argv[i], "--nvram") && i + 1 < argc) g_nvram = argv[++i];
    }
    g_pixels = calloc(640 * 480, 4);
    profile_start();

    hw_config cfg = { "carnevil/carnevi1.img", "carnevil1_9.u32", g_nvram, "sound102.u95" };
    hw_init(&cfg);

    /* The boot ROM's warm-reset path (0xBFC005D0): copy the BIOS image at ROM
     * offset 0xF20..0x16B60 to 0x80000000, then jump to 0x80004000 with $sp at
     * 0x80800000. (Cold reset runs a memory test and copies the variant with
     * the power-on self test, which the "Power On Self Test" DIP skips anyway.) */
    memcpy(g_rdram, hw_bootrom() + 0xF20, 0x16B60 - 0xF20);

    lookup_add_game();
    lookup_add_rtos();
    rtos_register_slots();
    proc_register();
    lookup_finish();

    if (!g_headless) {
        WNDCLASSA wc = { 0 };
        wc.lpfnWndProc = wndproc;
        wc.hInstance = GetModuleHandleA(NULL);
        wc.hCursor = LoadCursor(NULL, IDC_CROSS);
        wc.lpszClassName = "carnevil";
        RegisterClassA(&wc);
        RECT r = { 0, 0, 1024, 768 };
        AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
        g_wnd = CreateWindowA("carnevil", "CarnEvil", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                              CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left, r.bottom - r.top,
                              NULL, NULL, wc.hInstance, NULL);
    }

    ConvertThreadToFiber(NULL);
    recomp_context ctx;
    recomp_ctx_init(&ctx);
    ctx.r29 = (gpr)(int32_t)0x80800000;
    g_cur_ctx = &ctx;
    rtos_80004000(g_rdram, &ctx);
    hw_fatal("RTOS entry returned");
    return 0;
}
