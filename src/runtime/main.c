/*
 * CarnEvil, recompiled.
 *
 * Boots the way the board does after its boot ROM: the BIOS image is copied
 * from the boot ROM to 0x80000000 and the CPU starts at its entry 0x80004000.
 * From there the BIOS sets up the hardware, mounts the disk, loads GAME.EXE
 * and runs it.
 *
 * usage: carnevil [--frames N] [--shot out.ppm] [--headless] [--nvram file]
 *                 [--script file] [--host PORT [--delay N]] [--join ADDR:PORT]
 *   --frames N  quit after N vblanks (with --shot, save the last frame)
 *   --headless  no window, no sound, no carnevil.ini; runs flat out
 *   --host / --join   a two-player netplay session (see netplay.h)
 * Files (relative to the working directory): carnevil1_9.u32 (boot ROM),
 * sound102.u95 (DCS2 ROM), carnevil/carnevi1.img (the hard disk, v1.0.1);
 * nvram.bin is created, carnevil.ini keeps the menu's settings.
 *
 * The window: a GDI picture with a menu bar (File, Video, Sound, Controls,
 * Cheats, Multiplayer), the same set as the LostWorld recompilation.
 *
 * Determinism: the run is a pure function of g_inputs per frame, so g_inputs
 * changes only at the end of video_pump. Window messages only record device
 * state; video_pump turns it into this machine's inputs, adds the test
 * script, trades them with the netplay peer and applies the cheats. In a
 * session this machine's player always uses the player 1 slots; netplay moves
 * the joiner's to player 2.
 */
#include "hw.h"
#include "netplay.h"
#include "cheats.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <xinput.h>

extern void rtos_80004000(uint8_t* rdram, recomp_context* ctx);
void lookup_add_game(void);
void lookup_add_rtos(void);
void profile_start(void);
void audio_pump(int enabled);
void audio_set_volume(int percent);

#define BASE_W 400          /* the game's display mode */
#define BASE_H 256
#define INI_PATH "carnevil.ini"
#define NET_NVRAM "nvram_net.bin"
#define FAST_BOOT_FRAMES 1300   /* power-on to attract mode */

static HWND g_wnd;
static HMENU g_menu;
static int g_headless, g_frames_left = -1, g_booted, g_quit;
static const char* g_shot;
static const char* g_nvram = "nvram.bin";
static uint32_t* g_pixels;
static uint32_t* g_scan;            /* the picture with every other line darkened */
static int g_w = BASE_W, g_h = BASE_H;
static int g_argc;
static char** g_argv;

/* ======================================================================
 * Settings (carnevil.ini)
 * ====================================================================== */
enum { CUR_HIDDEN, CUR_CROSSHAIR, CUR_POINTER };

static struct {
    int scale;          /* window size, multiples of 256 lines */
    int fullscreen;
    int smooth;         /* bilinear (GDI HALFTONE) rather than sharp pixels */
    int scanlines;
    int aspect43;       /* 4:3 letterboxed, as on the cabinet; else stretch to the window */
    int fastboot;       /* run flat out from power-on to attract mode */
    int volume;         /* percent */
    int mute;
    int mouse;          /* the mouse is player 1's gun */
    int pad;            /* gamepad: 0 off, 1 player 1, 2 player 2 */
    int cursor;         /* CUR_* */
    int sinden;         /* white border: 0 off, 1 thin, 2 medium, 3 thick */
    int net_port;
    int net_delay;
    char net_join[128];
} opt = { 3, 0, 0, 0, 1, 1, 100, 0, 1, 2, CUR_CROSSHAIR, 0, 7777, 3, "" };

static uint32_t g_cheats;                   /* toggled cheats, CHEAT_* bits */
static uint32_t g_cheat_pulse;              /* one-shot cheats for the next frame */
static uint8_t g_options[MAX_OPTIONS];      /* the menu's (and ini's) choices */
static uint8_t g_run_options[MAX_OPTIONS];  /* what this run uses (a joiner: the host's) */

static const struct { const char* key; int* v; } k_ini[] = {
    { "scale", &opt.scale }, { "fullscreen", &opt.fullscreen }, { "smooth", &opt.smooth },
    { "scanlines", &opt.scanlines }, { "aspect43", &opt.aspect43 }, { "fastboot", &opt.fastboot },
    { "volume", &opt.volume }, { "mute", &opt.mute }, { "mouse", &opt.mouse }, { "pad", &opt.pad },
    { "cursor", &opt.cursor }, { "sinden", &opt.sinden }, { "net_port", &opt.net_port },
    { "net_delay", &opt.net_delay },
};

static void settings_load(void) {
    char line[512];
    FILE* f = fopen(INI_PATH, "r");
    if (!f) return;
    while (fgets(line, sizeof line, f)) {
        char *eq = strchr(line, '='), *nl;
        if (!eq) continue;
        *eq++ = 0;
        if ((nl = strpbrk(eq, "\r\n")) != NULL) *nl = 0;
        for (size_t i = 0; i < sizeof k_ini / sizeof k_ini[0]; i++)
            if (!strcmp(line, k_ini[i].key)) *k_ini[i].v = atoi(eq);
        if (!strcmp(line, "net_join")) snprintf(opt.net_join, sizeof opt.net_join, "%s", eq);
        if (!strcmp(line, "cheats")) g_cheats = (uint32_t)strtoul(eq, NULL, 0);
        if (!strncmp(line, "option", 6)) {
            int i = atoi(line + 6), v = atoi(eq);
            if (i >= 0 && i < k_option_count && v >= 0 && v < k_options[i].count) g_options[i] = (uint8_t)v;
        }
    }
    fclose(f);
    if (opt.scale < 1 || opt.scale > 4) opt.scale = 3;
    if (opt.net_delay < 1 || opt.net_delay > 8) opt.net_delay = 3;
    if (opt.volume < 0 || opt.volume > 100) opt.volume = 100;
    if (opt.pad < 0 || opt.pad > 2) opt.pad = 2;
    if (opt.cursor < 0 || opt.cursor > 2) opt.cursor = CUR_CROSSHAIR;
    if (opt.sinden < 0 || opt.sinden > 3) opt.sinden = 0;
    /* only toggles persist; a stale action bit must not fire every frame */
    for (int i = 0; i < 32; i++)
        if (i >= k_cheat_count || k_cheats[i].is_action) g_cheats &= ~(1u << i);
}

static void settings_save(void) {
    if (g_headless) return;
    FILE* f = fopen(INI_PATH, "w");
    if (!f) return;
    for (size_t i = 0; i < sizeof k_ini / sizeof k_ini[0]; i++) fprintf(f, "%s=%d\n", k_ini[i].key, *k_ini[i].v);
    fprintf(f, "net_join=%s\n", opt.net_join);
    fprintf(f, "cheats=0x%08X\n", g_cheats);
    for (int i = 0; i < k_option_count; i++) fprintf(f, "option%d=%d\n", i, g_options[i]);
    fclose(f);
}

/* ======================================================================
 * Device state, recorded by the window procedure, read once a frame
 * ====================================================================== */
enum { C_COIN1 = 1, C_COIN2 = 2, C_START1 = 4, C_START2 = 8, C_SERVICE = 16,
       C_FIRE = 32, C_RELOAD = 64, C_PUMP = 128, C_TURBO = 256 };
static unsigned g_held;          /* controls held now */
static unsigned g_latched;       /* pressed since the last frame (a tap shorter than a frame still counts) */
static uint16_t g_dips;          /* this machine's DIPs; F2 toggles the test switch */
static int g_user_touched;       /* any press: ends a fast boot */
static float g_pad_x = 0.5f, g_pad_y = 0.5f;
static int g_pad_connected = -1;

static void control(unsigned c, int down) {
    if (down) { g_held |= c; g_latched |= c; g_user_touched = 1; }
    else g_held &= ~c;
}

/* ======================================================================
 * Picture
 * ====================================================================== */
static RECT view_rect(void) {
    RECT c; GetClientRect(g_wnd, &c);
    int cw = c.right > 0 ? c.right : 1, ch = c.bottom > 0 ? c.bottom : 1;
    RECT r = { 0, 0, cw, ch };
    if (opt.aspect43) {
        if (cw * 3 > ch * 4) { int w = ch * 4 / 3; r.left = (cw - w) / 2; r.right = r.left + w; }
        else { int h = cw * 3 / 4; r.top = (ch - h) / 2; r.bottom = r.top + h; }
    }
    return r;
}

static void crosshair(HDC dc, const RECT* r, const gun_state* g, COLORREF color) {
    if (g->buttons & GUN_OFFSCREEN) return;
    int x = r->left + (int)((int64_t)g->x * (r->right - r->left) / 65536);
    int y = r->top + (int)((int64_t)g->y * (r->bottom - r->top) / 65536);
    int s = (r->bottom - r->top) / 40 + 4;
    HPEN dark = CreatePen(PS_SOLID, 3, RGB(0, 0, 0)), pen = CreatePen(PS_SOLID, 1, color);
    HGDIOBJ old = SelectObject(dc, dark);
    for (int pass = 0; pass < 2; pass++) {
        MoveToEx(dc, x - s, y, NULL); LineTo(dc, x - 2, y);
        MoveToEx(dc, x + 2, y, NULL); LineTo(dc, x + s + 1, y);
        MoveToEx(dc, x, y - s, NULL); LineTo(dc, x, y - 2);
        MoveToEx(dc, x, y + 2, NULL); LineTo(dc, x, y + s + 1);
        SelectObject(dc, pen);
    }
    SelectObject(dc, old);
    DeleteObject(dark); DeleteObject(pen);
}

static void paint(HDC dc) {
    static HDC mem;
    static HBITMAP bmp;
    static int bw, bh;
    RECT c; GetClientRect(g_wnd, &c);
    if (c.right <= 0 || c.bottom <= 0) return;
    if (!mem) mem = CreateCompatibleDC(dc);
    if (bw != c.right || bh != c.bottom) {
        if (bmp) DeleteObject(bmp);
        bmp = CreateCompatibleBitmap(dc, c.right, c.bottom);
        SelectObject(mem, bmp);
        bw = c.right; bh = c.bottom;
    }
    FillRect(mem, &c, (HBRUSH)GetStockObject(BLACK_BRUSH));
    RECT r = view_rect();
    int rw = r.right - r.left, rh = r.bottom - r.top;

    /* Scanlines: a source twice as tall with the odd lines darkened, once
     * every line is at least two pixels tall on screen. */
    const uint32_t* src = g_pixels;
    int sh = g_h;
    if (opt.scanlines && rh >= g_h * 2) {
        for (int y = 0; y < g_h; y++) {
            const uint32_t* s = g_pixels + y * g_w;
            uint32_t* a = g_scan + (2 * y) * g_w;
            uint32_t* b = a + g_w;
            memcpy(a, s, (size_t)g_w * 4);
            for (int x = 0; x < g_w; x++) b[x] = (s[x] >> 1 & 0x7F7F7F) + (s[x] >> 2 & 0x3F3F3F);   /* 75% */
        }
        src = g_scan; sh = g_h * 2;
    }
    BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), g_w, -sh, 1, 32, BI_RGB } };
    SetStretchBltMode(mem, opt.smooth ? HALFTONE : COLORONCOLOR);
    SetBrushOrgEx(mem, 0, 0, NULL);
    StretchDIBits(mem, r.left, r.top, rw, rh, 0, 0, g_w, sh, src, &bi, DIB_RGB_COLORS, SRCCOPY);

    /* A Sinden gun finds the screen by its white border. */
    if (opt.sinden) {
        int t = rh * 15 * opt.sinden / 1000 + 1;
        HBRUSH w = (HBRUSH)GetStockObject(WHITE_BRUSH);
        RECT e[4] = { { r.left, r.top, r.right, r.top + t }, { r.left, r.bottom - t, r.right, r.bottom },
                      { r.left, r.top, r.left + t, r.bottom }, { r.right - t, r.top, r.right, r.bottom } };
        for (int i = 0; i < 4; i++) FillRect(mem, &e[i], w);
    }

    /* Crosshairs: the cabinet had none, so for the mouse they are an option;
     * a gamepad or a netplay partner has nothing else to aim by. */
    if (opt.cursor == CUR_CROSSHAIR || (g_pad_connected >= 0 && opt.pad == 1))
        crosshair(mem, &r, &g_inputs.gun[0], RGB(255, 64, 64));
    if ((g_pad_connected >= 0 && opt.pad == 2) || netplay_active())
        crosshair(mem, &r, &g_inputs.gun[1], RGB(64, 160, 255));

    BitBlt(dc, 0, 0, c.right, c.bottom, mem, 0, 0, SRCCOPY);
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
    if (g_wnd && !IsIconic(g_wnd)) {
        HDC dc = GetDC(g_wnd);
        paint(dc);
        ReleaseDC(g_wnd, dc);
    }
}

/* ======================================================================
 * Window: size, fullscreen, title
 * ====================================================================== */
static int g_is_fs;
static WINDOWPLACEMENT g_placement = { sizeof(WINDOWPLACEMENT) };

static void apply_video(int resize) {
    if (!g_wnd) return;
    if (opt.fullscreen && !g_is_fs) {
        MONITORINFO mi = { sizeof mi };
        GetWindowPlacement(g_wnd, &g_placement);
        GetMonitorInfoA(MonitorFromWindow(g_wnd, MONITOR_DEFAULTTONEAREST), &mi);
        SetMenu(g_wnd, NULL);
        SetWindowLongA(g_wnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
        SetWindowPos(g_wnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left,
                     mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_FRAMECHANGED);
        g_is_fs = 1;
    } else if (!opt.fullscreen && g_is_fs) {
        SetWindowLongA(g_wnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
        SetMenu(g_wnd, g_menu);
        SetWindowPlacement(g_wnd, &g_placement);
        SetWindowPos(g_wnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
        g_is_fs = 0;
    }
    if (resize && !g_is_fs) {
        ShowWindow(g_wnd, SW_RESTORE);
        int h = BASE_H * opt.scale, w = opt.aspect43 ? h * 4 / 3 : BASE_W * opt.scale;
        RECT wr, cr;
        /* size by the difference between window and client, which counts a menu bar that wraps */
        for (int pass = 0; pass < 2; pass++) {
            GetWindowRect(g_wnd, &wr); GetClientRect(g_wnd, &cr);
            SetWindowPos(g_wnd, NULL, 0, 0, w + (wr.right - wr.left) - cr.right, h + (wr.bottom - wr.top) - cr.bottom,
                         SWP_NOMOVE | SWP_NOZORDER);
        }
    }
    audio_set_volume(opt.mute ? 0 : opt.volume);
    InvalidateRect(g_wnd, NULL, FALSE);
}

static void update_title(double fps) {
    char t[256];
    const char* net = netplay_status();
    int n = snprintf(t, sizeof t, "CarnEvil");
    if (net && *net) n += snprintf(t + n, sizeof t - n, "  -  %s", net);
    if (g_turbo || fps < 54) n += snprintf(t + n, sizeof t - n, "  -  %.0f fps%s", fps, g_turbo ? " (fast)" : "");
    if (g_wnd) SetWindowTextA(g_wnd, t);
}

/* ======================================================================
 * Relaunch: the machine cannot be reset in place, an option that takes
 * effect from power-on needs a power cycle, and a netplay session has to
 * start at power-on on both sides.
 * ====================================================================== */
static void relaunch(const char* net_flag, const char* net_arg, int delay) {
    char exe[MAX_PATH], cmd[8192];
    int n;
    settings_save();
    netplay_shutdown();
    GetModuleFileNameA(NULL, exe, sizeof exe);
    n = snprintf(cmd, sizeof cmd, "\"%s\"", exe);
    for (int i = 1; i < g_argc; i++) {
        if ((!strcmp(g_argv[i], "--host") || !strcmp(g_argv[i], "--join") || !strcmp(g_argv[i], "--delay")) && i + 1 < g_argc) { i++; continue; }
        n += snprintf(cmd + n, sizeof cmd - n, strchr(g_argv[i], ' ') ? " \"%s\"" : " %s", g_argv[i]);
    }
    if (net_flag) n += snprintf(cmd + n, sizeof cmd - n, " %s %s", net_flag, net_arg);
    if (net_flag && !strcmp(net_flag, "--host")) snprintf(cmd + n, sizeof cmd - n, " --delay %d", delay);
    if (g_booted) hw_shutdown();   /* battery RAM to disk first: the new process boots from it */
    STARTUPINFOA si = { sizeof si };
    PROCESS_INFORMATION pi;
    if (CreateProcessA(exe, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    } else {
        MessageBoxA(g_wnd, "Could not restart the game.", "CarnEvil", MB_ICONERROR);
    }
    exit(0);
}

/* ======================================================================
 * A one-line text prompt, built in memory so there is no resource file
 * (from the LostWorld recompilation's platform layer).
 * ====================================================================== */
static WCHAR g_prompt_buf[256];
static const WCHAR* g_prompt_label;

static INT_PTR CALLBACK prompt_proc(HWND d, UINT m, WPARAM w, LPARAM l) {
    (void)l;
    switch (m) {
        case WM_INITDIALOG:
            SetDlgItemTextW(d, 10, g_prompt_label);
            SetDlgItemTextW(d, 11, g_prompt_buf);
            return TRUE;
        case WM_COMMAND:
            if (LOWORD(w) == IDOK) { GetDlgItemTextW(d, 11, g_prompt_buf, 256); EndDialog(d, 1); return TRUE; }
            if (LOWORD(w) == IDCANCEL) { EndDialog(d, 0); return TRUE; }
            break;
    }
    return FALSE;
}

static WORD* dlg_str(WORD* p, const WCHAR* s) {
    while ((*p++ = (WORD)*s++) != 0) {}
    return p;
}

static WORD* dlg_item(WORD* p, DWORD style, short x, short y, short cx, short cy, WORD id, WORD cls, const WCHAR* text) {
    p = (WORD*)(((ULONG_PTR)p + 3) & ~(ULONG_PTR)3);
    DLGITEMTEMPLATE* it = (DLGITEMTEMPLATE*)p;
    it->style = style | WS_CHILD | WS_VISIBLE;
    it->dwExtendedStyle = 0;
    it->x = x; it->y = y; it->cx = cx; it->cy = cy; it->id = id;
    p = (WORD*)(it + 1);
    *p++ = 0xFFFF; *p++ = cls;
    p = dlg_str(p, text);
    *p++ = 0;
    return p;
}

static int prompt(const WCHAR* title, const WCHAR* label, char* io, size_t n) {
    static WORD buf[1024];
    DLGTEMPLATE* t = (DLGTEMPLATE*)buf;
    memset(buf, 0, sizeof buf);
    t->style = DS_MODALFRAME | DS_CENTER | DS_SETFONT | WS_POPUP | WS_CAPTION | WS_SYSMENU;
    t->cdit = 4;
    t->cx = 240; t->cy = 64;
    WORD* p = (WORD*)(t + 1);
    *p++ = 0; *p++ = 0;                     /* no menu, default class */
    p = dlg_str(p, title);
    *p++ = 9;                               /* font size */
    p = dlg_str(p, L"Segoe UI");
    p = dlg_item(p, SS_LEFT, 8, 6, 224, 10, 10, 0x0082, L"");
    p = dlg_item(p, ES_AUTOHSCROLL | WS_BORDER | WS_TABSTOP, 8, 20, 224, 13, 11, 0x0081, L"");
    p = dlg_item(p, BS_DEFPUSHBUTTON | WS_TABSTOP, 128, 42, 50, 14, IDOK, 0x0080, L"OK");
    (void)dlg_item(p, BS_PUSHBUTTON | WS_TABSTOP, 182, 42, 50, 14, IDCANCEL, 0x0080, L"Cancel");
    g_prompt_label = label;
    MultiByteToWideChar(CP_UTF8, 0, io, -1, g_prompt_buf, 256);
    g_held = 0;   /* the dialog eats the key-ups */
    if (DialogBoxIndirectW(GetModuleHandleW(NULL), t, g_wnd, prompt_proc) != 1) return 0;
    WideCharToMultiByte(CP_UTF8, 0, g_prompt_buf, -1, io, (int)n, NULL, NULL);
    return 1;
}

/* ======================================================================
 * Menu bar
 * ====================================================================== */
enum {
    ID_RESET = 100, ID_QUIT,
    ID_SCALE = 110,                 /* +1..4 */
    ID_FULL = 120, ID_SHARP, ID_SMOOTH, ID_SCAN, ID_ASPECT43, ID_STRETCH, ID_FASTBOOT,
    ID_MUTE = 140, ID_VOL = 141,    /* +0..3 */
    ID_MOUSE = 160, ID_PAD = 170, ID_CURSOR = 180, ID_SINDEN = 190,
    ID_HOST = 400, ID_JOIN, ID_DISCONNECT, ID_DELAY = 410,   /* +1..8 */
    ID_CHEAT = 500,                 /* +bit */
    ID_OPTION = 1000,               /* +option*64+choice */
};
static const int k_volumes[4] = { 100, 75, 50, 25 };

static void item(HMENU m, UINT id, const char* label, int checked) {
    AppendMenuA(m, MF_STRING | (checked ? MF_CHECKED : 0), id, label);
}
static void note(HMENU m, const char* label) { AppendMenuA(m, MF_STRING | MF_GRAYED, 0, label); }
static void sep(HMENU m) { AppendMenuA(m, MF_SEPARATOR, 0, NULL); }
static void sub(HMENU m, HMENU s, const char* label) { AppendMenuA(m, MF_POPUP, (UINT_PTR)s, label); }

static void build_menu(void) {
    char b[128];
    int net = netplay_active();
    HMENU bar = CreateMenu(), m, s;

    m = CreatePopupMenu();
    item(m, ID_RESET, "&Reset", 0);
    item(m, ID_QUIT, "&Quit\tEsc", 0);
    sub(bar, m, "&File");

    m = CreatePopupMenu(); s = CreatePopupMenu();
    for (int i = 1; i <= 4; i++) {
        int h = BASE_H * i;
        snprintf(b, sizeof b, "%dx  (%d x %d)", i, opt.aspect43 ? h * 4 / 3 : BASE_W * i, h);
        item(s, ID_SCALE + i, b, opt.scale == i);
    }
    sub(m, s, "&Window size");
    item(m, ID_FULL, "&Fullscreen\tF11", opt.fullscreen);
    sep(m);
    item(m, ID_SHARP, "Sharp pixels", !opt.smooth);
    item(m, ID_SMOOTH, "Smooth (bilinear)", opt.smooth);
    item(m, ID_SCAN, "Scanlines", opt.scanlines);
    sep(m);
    item(m, ID_ASPECT43, "4:3, as on the cabinet", opt.aspect43);
    item(m, ID_STRETCH, "Stretch to the window", !opt.aspect43);
    sep(m);
    item(m, ID_FASTBOOT, "Fast &boot (skip the power-on wait)", opt.fastboot);
    sub(bar, m, "&Video");

    m = CreatePopupMenu();
    item(m, ID_MUTE, "&Mute\tF9", opt.mute);
    sep(m);
    for (int i = 0; i < 4; i++) {
        snprintf(b, sizeof b, "Volume %d%%", k_volumes[i]);
        item(m, ID_VOL + i, b, opt.volume == k_volumes[i]);
    }
    sub(bar, m, "&Sound");

    m = CreatePopupMenu();
    item(m, ID_MOUSE, "&Mouse is player 1's gun", opt.mouse);
    s = CreatePopupMenu();
    item(s, ID_PAD + 0, "Off", opt.pad == 0);
    item(s, ID_PAD + 1, "Player 1", opt.pad == 1);
    item(s, ID_PAD + 2, "Player 2", opt.pad == 2);
    sub(m, s, g_pad_connected >= 0 ? "&Gamepad" : "&Gamepad  (none connected)");
    s = CreatePopupMenu();
    item(s, ID_CURSOR + CUR_HIDDEN, "Hidden, as on the cabinet", opt.cursor == CUR_HIDDEN);
    item(s, ID_CURSOR + CUR_CROSSHAIR, "Crosshair", opt.cursor == CUR_CROSSHAIR);
    item(s, ID_CURSOR + CUR_POINTER, "Mouse pointer", opt.cursor == CUR_POINTER);
    sub(m, s, "&Cursor");
    s = CreatePopupMenu();
    item(s, ID_SINDEN + 0, "Off", opt.sinden == 0);
    item(s, ID_SINDEN + 1, "Thin", opt.sinden == 1);
    item(s, ID_SINDEN + 2, "Medium", opt.sinden == 2);
    item(s, ID_SINDEN + 3, "Thick", opt.sinden == 3);
    sub(m, s, "&Sinden light gun border");
    sep(m);
    note(m, "Mouse: left fires, right reloads (off screen), middle pumps");
    note(m, "Keys: Space fires, Shift reloads, 5/6 coin, 1/2 start, 9 service credit");
    note(m, "F2 test switch, Tab (held) fast forward");
    note(m, "Gamepad: left stick aims, A/RT fire, B/LT reload, X pump, Start, Back = coin");
    note(m, "Sinden: run its software in mouse mode, off-screen reload = right click");
    sub(bar, m, "&Controls");

    m = CreatePopupMenu();
    for (int i = 0; i < k_cheat_count && i < 32; i++)
        item(m, ID_CHEAT + i, k_cheats[i].label, !k_cheats[i].is_action && (g_cheats >> i & 1));
    if (!k_cheat_count) note(m, "No cheats yet");
    if (net && netplay_role() == 2) note(m, "(during netplay the host's cheats apply)");
    for (int i = 0; i < k_option_count && i < MAX_OPTIONS; i++) {
        const option_def* o = &k_options[i];
        if (i == 0) sep(m);
        s = CreatePopupMenu();
        for (int c = 0; c < o->count && c < 64; c++)
            AppendMenuA(s, MF_STRING | (g_run_options[i] == c ? MF_CHECKED : 0) | (net ? MF_GRAYED : 0),
                        ID_OPTION + i * 64 + c, o->choices[c]);
        if (net) note(s, "(fixed for a netplay session)");
        snprintf(b, sizeof b, o->restart ? "%s  (restarts the game)" : "%s", o->label);
        sub(m, s, b);
    }
    sub(bar, m, "C&heats");

    m = CreatePopupMenu(); s = CreatePopupMenu();
    AppendMenuA(m, MF_STRING | (net ? MF_GRAYED : 0), ID_HOST, "&Host a game...");
    AppendMenuA(m, MF_STRING | (net ? MF_GRAYED : 0), ID_JOIN, "&Join a game...");
    AppendMenuA(m, MF_STRING | (net ? 0 : MF_GRAYED), ID_DISCONNECT, "&Disconnect");
    for (int i = 1; i <= 8; i++) {
        snprintf(b, sizeof b, "%d frame%s%s", i, i == 1 ? "" : "s", i <= 2 ? "  (LAN)" : i >= 5 ? "  (far away)" : "");
        item(s, ID_DELAY + i, b, opt.net_delay == i);
    }
    sub(m, s, "Input &delay when hosting");
    sep(m);
    note(m, net ? netplay_status() : "Not connected");
    note(m, "The host plays player 1; the game restarts for both when you connect");
    note(m, "LAN, a Tailscale address, or a forwarded TCP port all work");
    sub(bar, m, "&Multiplayer");

    HMENU old = g_menu;
    g_menu = bar;
    if (!g_is_fs) SetMenu(g_wnd, bar);
    if (old) DestroyMenu(old);
}

static void on_command(unsigned id) {
    int resize = 0;
    if (id == ID_RESET) relaunch(NULL, NULL, 0);
    else if (id == ID_QUIT) { g_quit = 1; PostQuitMessage(0); }
    else if (id > ID_SCALE && id <= ID_SCALE + 4) { opt.scale = (int)(id - ID_SCALE); opt.fullscreen = 0; resize = 1; }
    else if (id == ID_FULL) opt.fullscreen = !opt.fullscreen;
    else if (id == ID_SHARP) opt.smooth = 0;
    else if (id == ID_SMOOTH) opt.smooth = 1;
    else if (id == ID_SCAN) opt.scanlines = !opt.scanlines;
    else if (id == ID_ASPECT43) { opt.aspect43 = 1; resize = 1; }
    else if (id == ID_STRETCH) { opt.aspect43 = 0; resize = 1; }
    else if (id == ID_FASTBOOT) opt.fastboot = !opt.fastboot;
    else if (id == ID_MUTE) opt.mute = !opt.mute;
    else if (id >= ID_VOL && id < ID_VOL + 4) { opt.volume = k_volumes[id - ID_VOL]; opt.mute = 0; }
    else if (id == ID_MOUSE) opt.mouse = !opt.mouse;
    else if (id >= ID_PAD && id <= ID_PAD + 2) opt.pad = (int)(id - ID_PAD);
    else if (id >= ID_CURSOR && id <= ID_CURSOR + 2) opt.cursor = (int)(id - ID_CURSOR);
    else if (id >= ID_SINDEN && id <= ID_SINDEN + 3) opt.sinden = (int)(id - ID_SINDEN);
    else if (id > ID_DELAY && id <= ID_DELAY + 8) opt.net_delay = (int)(id - ID_DELAY);
    else if (id == ID_DISCONNECT) netplay_shutdown();
    else if (id == ID_HOST) {
        char port[32];
        snprintf(port, sizeof port, "%d", opt.net_port);
        if (prompt(L"Host a game", L"Port to listen on (TCP). The game restarts and waits for player 2.", port, sizeof port)
            && atoi(port) > 0) {
            opt.net_port = atoi(port);
            snprintf(port, sizeof port, "%d", opt.net_port);
            relaunch("--host", port, opt.net_delay);
        }
    } else if (id == ID_JOIN) {
        char addr[128];
        snprintf(addr, sizeof addr, "%s", opt.net_join[0] ? opt.net_join : "127.0.0.1:7777");
        if (prompt(L"Join a game", L"Host address and port, e.g. 192.168.1.20:7777 or a Tailscale name.", addr, sizeof addr)
            && addr[0]) {
            if (!strchr(addr, ':')) strncat(addr, ":7777", sizeof addr - strlen(addr) - 1);
            snprintf(opt.net_join, sizeof opt.net_join, "%s", addr);
            relaunch("--join", addr, 0);
        }
    } else if (id >= ID_CHEAT && id < ID_CHEAT + 32 && (int)(id - ID_CHEAT) < k_cheat_count) {
        int bit = (int)(id - ID_CHEAT);
        if (k_cheats[bit].is_action) g_cheat_pulse |= 1u << bit;
        else g_cheats ^= 1u << bit;
    } else if (id >= ID_OPTION && id < ID_OPTION + MAX_OPTIONS * 64 && !netplay_active()) {
        int i = (int)(id - ID_OPTION) / 64, v = (int)(id - ID_OPTION) % 64;
        if (i < k_option_count && v < k_options[i].count && g_options[i] != v) {
            g_options[i] = (uint8_t)v;
            if (k_options[i].restart) relaunch(NULL, NULL, 0);   /* the operator's way: change it, power cycle */
            g_run_options[i] = (uint8_t)v;
        }
    }
    settings_save();
    apply_video(resize);
    build_menu();
}

/* ======================================================================
 * Window procedure: records device state only
 * ====================================================================== */
static unsigned key_control(WPARAM vk) {
    switch (vk) {
        case '5': return C_COIN1;
        case '6': return C_COIN2;
        case '1': return C_START1;
        case '2': return C_START2;
        case '9': return C_SERVICE;
        case VK_SPACE: return C_FIRE;
        case VK_SHIFT: return C_RELOAD;
        case VK_TAB: return C_TURBO;
    }
    return 0;
}

static LRESULT CALLBACK wndproc(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
        case WM_KEYDOWN:
            if (l & 0x40000000) return 0;   /* auto-repeat */
            switch (w) {
                case VK_F2: g_dips ^= DIP_TEST_SWITCH; g_user_touched = 1; return 0;
                case VK_F9: on_command(ID_MUTE); return 0;
                case VK_F11: on_command(ID_FULL); return 0;
                case VK_ESCAPE:
                    if (opt.fullscreen) on_command(ID_FULL);
                    else { g_quit = 1; PostQuitMessage(0); }
                    return 0;
            }
            control(key_control(w), 1);
            return 0;
        case WM_KEYUP: control(key_control(w), 0); return 0;
        case WM_SYSKEYDOWN:
            if (w == VK_RETURN && !(l & 0x40000000)) { on_command(ID_FULL); return 0; }
            break;
        case WM_KILLFOCUS: g_held = 0; return 0;
        case WM_LBUTTONDOWN: SetCapture(h); control(C_FIRE, 1); return 0;
        case WM_RBUTTONDOWN: SetCapture(h); control(C_RELOAD, 1); return 0;
        case WM_MBUTTONDOWN: SetCapture(h); control(C_PUMP, 1); return 0;
        case WM_LBUTTONUP: control(C_FIRE, 0); goto release;
        case WM_RBUTTONUP: control(C_RELOAD, 0); goto release;
        case WM_MBUTTONUP: control(C_PUMP, 0);
        release:
            if (!(w & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON))) ReleaseCapture();
            return 0;
        case WM_SETCURSOR:
            if (LOWORD(l) == HTCLIENT) { SetCursor(opt.cursor == CUR_POINTER ? LoadCursor(NULL, IDC_ARROW) : NULL); return TRUE; }
            break;
        case WM_COMMAND:
            if (HIWORD(w) == 0) { on_command(LOWORD(w)); return 0; }
            break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC dc = BeginPaint(h, &ps);
            paint(dc);
            EndPaint(h, &ps);
            return 0;
        }
        case WM_ERASEBKGND: return 1;
        case WM_CLOSE: g_quit = 1; PostQuitMessage(0); return 0;
    }
    return DefWindowProcA(h, m, w, l);
}

static int pump_messages(void) {
    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) { g_quit = 1; return 0; }
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return !g_quit;
}

/* ======================================================================
 * This machine's controls for the frame
 * ====================================================================== */
static int stick(SHORT v) {
    return v > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || v < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
}

static void sample_pad(cabinet_inputs* in, unsigned* c) {
    XINPUT_STATE st;
    int found = -1;
    static unsigned tries;
    /* polling an absent pad is slow; look for a new one twice a second */
    if (g_pad_connected < 0 && tries++ % 30) return;
    for (int i = 0; i < 4 && found < 0; i++)
        if (XInputGetState((DWORD)i, &st) == ERROR_SUCCESS) found = i;
    if ((found >= 0) != (g_pad_connected >= 0)) { g_pad_connected = found; if (g_wnd) build_menu(); }
    g_pad_connected = found;
    if (found < 0 || !opt.pad) return;
    const XINPUT_GAMEPAD* p = &st.Gamepad;
    int pl = netplay_active() ? 0 : opt.pad - 1;   /* in a session this machine's player is always slot 1 */
    if (stick(p->sThumbLX)) g_pad_x += p->sThumbLX / 32768.0f * 0.012f;
    if (stick(p->sThumbLY)) g_pad_y -= p->sThumbLY / 32768.0f * 0.016f;
    if (g_pad_x < 0) g_pad_x = 0;
    if (g_pad_x > 0.9999f) g_pad_x = 0.9999f;
    if (g_pad_y < 0) g_pad_y = 0;
    if (g_pad_y > 0.9999f) g_pad_y = 0.9999f;
    gun_state* g = &in->gun[pl];
    g->x = (uint16_t)(g_pad_x * 65536);
    g->y = (uint16_t)(g_pad_y * 65536);
    if ((p->wButtons & XINPUT_GAMEPAD_A) || p->bRightTrigger > 128) g->buttons |= GUN_TRIGGER;
    if ((p->wButtons & XINPUT_GAMEPAD_B) || p->bLeftTrigger > 128) g->buttons |= GUN_TRIGGER | GUN_OFFSCREEN;
    if (p->wButtons & XINPUT_GAMEPAD_X) g->buttons |= GUN_PUMP;
    if (p->wButtons & XINPUT_GAMEPAD_START) *c |= pl ? C_START2 : C_START1;
    if (p->wButtons & XINPUT_GAMEPAD_BACK) *c |= pl ? C_COIN2 : C_COIN1;
    if (p->wButtons || p->bLeftTrigger > 128 || p->bRightTrigger > 128 || stick(p->sThumbLX) || stick(p->sThumbLY))
        g_user_touched = 1;
}

static cabinet_inputs local_inputs(void) {
    static const uint16_t k_sys[] = { SYS_COIN1, SYS_COIN2, SYS_START1, SYS_START2, SYS_SERVICE };
    cabinet_inputs in = { 0xFFFF, g_dips, { { 32768, 32768, 0 }, { 32768, 32768, 0 } }, 0 };
    unsigned c = g_held | g_latched;
    g_latched = 0;
    if (!g_wnd) return in;

    if (opt.mouse) {
        /* the gun sees where the pointer is inside the picture; outside it, nothing */
        POINT pt; GetCursorPos(&pt); ScreenToClient(g_wnd, &pt);
        RECT r = view_rect();
        gun_state* g = &in.gun[0];
        if (pt.x >= r.left && pt.x < r.right && pt.y >= r.top && pt.y < r.bottom) {
            g->x = (uint16_t)((int64_t)(pt.x - r.left) * 65536 / (r.right - r.left));
            g->y = (uint16_t)((int64_t)(pt.y - r.top) * 65536 / (r.bottom - r.top));
        } else g->buttons |= GUN_OFFSCREEN;
    }
    if (c & C_FIRE) in.gun[0].buttons |= GUN_TRIGGER;
    if (c & C_RELOAD) in.gun[0].buttons |= GUN_TRIGGER | GUN_OFFSCREEN;   /* reload: shoot off the screen */
    if (c & C_PUMP) in.gun[0].buttons |= GUN_PUMP;
    sample_pad(&in, &c);
    for (int i = 0; i < 5; i++)
        if (c & (1u << i)) in.system &= (uint16_t)~k_sys[i];
    g_turbo = (c & C_TURBO) != 0;
    return in;
}

/* ======================================================================
 * Input script for tests (--script FILE): lines of "frame action [args]".
 * Actions press what a player would press, at the vblank given:
 *   coin1 coin2 start1 start2 service   (held for 6 frames)
 *   gun P X Y        aim player P's gun (0..255 each axis)
 *   fire P / reload P / pump P   (held for 3 frames; reload = trigger off screen)
 *   dump FILE        write main RAM (8 MB)
 *   shot FILE        write the displayed frame (PPM)
 *   poke ADDR VAL / pokeb ADDR VAL   write RAM (hex; a test aid, not a player input)
 *   peek ADDR        print a RAM word (hex) to stderr
 *   quit
 * The script's presses are added to this machine's controls.
 * ====================================================================== */
typedef struct { int frame; char act[16]; int a, b, c; char path[260]; } script_line;
static script_line* g_script;
static int g_nscript, g_frame;
static cabinet_inputs g_sin = { 0xFFFF, 0, { { 32768, 32768, 0 }, { 32768, 32768, 0 } }, 0 };
static int g_saimed[2];
static struct { void* port; uint16_t bit; int until; } g_held_s[16];

static void set_bit(uint16_t* port, uint16_t bit, int pressed) {
    if (pressed) *port &= (uint16_t)~bit; else *port |= bit;
}

/* Gun buttons are active-high (port is a gun's 8-bit buttons); the system port is active-low. */
static void press(void* port, uint16_t bit, int down) {
    if (port == &g_sin.system) set_bit(&g_sin.system, bit, down);
    else { uint8_t* b = port; if (down) *b |= (uint8_t)bit; else *b &= (uint8_t)~bit; }
}

static void hold(void* port, uint16_t bit, int frames) {
    for (int i = 0; i < 16; i++)
        if (!g_held_s[i].port) { g_held_s[i].port = port; g_held_s[i].bit = bit; g_held_s[i].until = g_frame + frames; press(port, bit, 1); return; }
}

static void load_script(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) { fprintf(stderr, "cannot open script %s\n", path); exit(1); }
    char line[512];
    while (fgets(line, sizeof line, f)) {
        script_line s = { 0 };
        if (sscanf(line, "%d %15s", &s.frame, s.act) < 2 || line[0] == '#') continue;
        if (!strcmp(s.act, "dump") || !strcmp(s.act, "shot")) sscanf(line, "%*d %*s %259s", s.path);
        else if (!strncmp(s.act, "poke", 4) || !strcmp(s.act, "peek")) sscanf(line, "%*d %*s %x %x", (unsigned*)&s.a, (unsigned*)&s.b);
        else sscanf(line, "%*d %*s %d %d %d", &s.a, &s.b, &s.c);
        g_script = realloc(g_script, (size_t)(g_nscript + 1) * sizeof(s));
        g_script[g_nscript++] = s;
    }
    fclose(f);
}

static void run_script(cabinet_inputs* in) {
    for (int i = 0; i < 16; i++)
        if (g_held_s[i].port && g_frame >= g_held_s[i].until) { press(g_held_s[i].port, g_held_s[i].bit, 0); g_held_s[i].port = NULL; }
    for (int i = 0; i < g_nscript; i++) {
        script_line* s = &g_script[i];
        if (s->frame != g_frame) continue;
        gun_state* g = &g_sin.gun[s->a & 1];
        if (!strcmp(s->act, "coin1")) hold(&g_sin.system, SYS_COIN1, 6);
        else if (!strcmp(s->act, "coin2")) hold(&g_sin.system, SYS_COIN2, 6);
        else if (!strcmp(s->act, "start1")) hold(&g_sin.system, SYS_START1, 6);
        else if (!strcmp(s->act, "start2")) hold(&g_sin.system, SYS_START2, 6);
        else if (!strcmp(s->act, "service")) hold(&g_sin.system, SYS_SERVICE, 6);
        else if (!strcmp(s->act, "gun")) { g->x = (uint16_t)(s->b * 256); g->y = (uint16_t)(s->c * 256); g_saimed[s->a & 1] = 1; }
        else if (!strcmp(s->act, "fire")) hold(&g->buttons, GUN_TRIGGER, 3);
        else if (!strcmp(s->act, "reload")) hold(&g->buttons, GUN_TRIGGER | GUN_OFFSCREEN, 3);
        else if (!strcmp(s->act, "pump")) hold(&g->buttons, GUN_PUMP, 3);
        else if (!strcmp(s->act, "dump")) { FILE* f = fopen(s->path, "wb"); if (f) { fwrite(g_rdram, 1, RAM_SIZE, f); fclose(f); } }
        else if (!strcmp(s->act, "shot")) { const char* keep = g_shot; g_shot = s->path; save_shot(); g_shot = keep; }
        else if (!strcmp(s->act, "quit")) { hw_shutdown(); exit(0); }
        else if (!strcmp(s->act, "poke")) *(uint32_t*)(g_rdram + PHYS((uint32_t)s->a)) = (uint32_t)s->b;
        else if (!strcmp(s->act, "pokeb")) g_rdram[PHYS((uint32_t)s->a)] = (uint8_t)s->b;
        else if (!strcmp(s->act, "peek"))
            fprintf(stderr, "[peek] f%d %08X = %08X\n", g_frame, (uint32_t)s->a, *(uint32_t*)(g_rdram + PHYS((uint32_t)s->a)));
    }
    in->system &= g_sin.system;
    for (int p = 0; p < 2; p++) {
        if (g_saimed[p]) { in->gun[p].x = g_sin.gun[p].x; in->gun[p].y = g_sin.gun[p].y; in->gun[p].buttons &= (uint8_t)~GUN_OFFSCREEN; }
        in->gun[p].buttons |= g_sin.gun[p].buttons;
    }
}

/* ======================================================================
 * Once per frame (vblank)
 * ====================================================================== */
int video_pump(void) {
    g_frame++;
    audio_pump(!g_headless);
    if (g_frames_left > 0 && --g_frames_left == 0) {
        if (g_shot) save_shot();
        return 0;
    }
    if (g_wnd && !pump_messages()) return 0;

    cabinet_inputs local = local_inputs();
    local.cheats = g_cheats | g_cheat_pulse;
    g_cheat_pulse = 0;
    if (g_nscript) run_script(&local);

    uint64_t f = hw_frame();
    static int was_net;
    netplay_exchange(&local, &g_inputs, f);   /* offline (or the peer left): a copy */
    if (was_net != netplay_active()) { was_net = netplay_active(); if (g_wnd) build_menu(); }
    cheats_frame(&g_inputs, g_run_options, f);
    if (f % 120 == 0) netplay_check(f, hw_ram_hash());

    /* fast boot: flat out from power-on to attract mode, unless someone plays */
    if (opt.fastboot && !g_user_touched && f < FAST_BOOT_FRAMES && !netplay_active()) g_turbo = 1;
    if (g_headless) g_turbo = 1;

    if (g_wnd) {
        static ULONGLONG last;
        static uint64_t last_f;
        ULONGLONG now = GetTickCount64();
        if (now - last >= 1000) {
            update_title(last ? (double)(f - last_f) * 1000.0 / (double)(now - last) : 57.0);
            last = now; last_f = f;
        }
    }
    return 1;
}

/* ======================================================================
 * Netplay setup, before power-on: the setup blob is the host's CMOS image
 * and options, so both machines boot the same board.
 *   u32 cmos size, cmos bytes, MAX_OPTIONS option bytes
 * ====================================================================== */
static int net_pump(void) { return g_wnd ? pump_messages() : 1; }

static void net_fail(const char* what) {
    char b[300];
    snprintf(b, sizeof b, "%s\n\nThe game starts offline.", what);
    fprintf(stderr, "[netplay] %s\n", what);
    if (g_wnd && !g_quit) MessageBoxA(g_wnd, b, "CarnEvil", MB_ICONWARNING);
}

static void net_host(int port, int delay) {
    uint8_t* blob = NULL;
    uint32_t size = 0;
    FILE* f = fopen(g_nvram, "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        size = (uint32_t)ftell(f);
        fseek(f, 0, SEEK_SET);
    }
    blob = malloc(4 + size + MAX_OPTIONS);
    memcpy(blob, &size, 4);
    if (f) { size = (uint32_t)fread(blob + 4, 1, size, f); memcpy(blob, &size, 4); fclose(f); }
    memcpy(blob + 4 + size, g_run_options, MAX_OPTIONS);
    if (g_wnd) {
        char t[128];
        snprintf(t, sizeof t, "CarnEvil  -  Waiting for player 2 on port %d...", port);
        SetWindowTextA(g_wnd, t);
    }
    if (!netplay_host(port, delay, blob, 4 + size + MAX_OPTIONS, net_pump) && !g_quit) {
        char b[128];
        snprintf(b, sizeof b, "Hosting on port %d failed: no player 2 connected.", port);
        net_fail(b);
    }
    free(blob);
}

static void net_join(const char* addr) {
    void* setup = NULL;
    size_t n = 0;
    if (g_wnd) {
        char t[200];
        snprintf(t, sizeof t, "CarnEvil  -  Connecting to %s...", addr);
        SetWindowTextA(g_wnd, t);
    }
    if (!netplay_join(addr, &setup, &n, net_pump)) {
        if (!g_quit) {
            char b[200];
            snprintf(b, sizeof b, "Could not join a game at %s.", addr);
            net_fail(b);
        }
        return;
    }
    uint32_t size = 0;
    if (n >= 4) memcpy(&size, setup, 4);
    if (n < 4 || n != 4 + (size_t)size + MAX_OPTIONS) {
        free(setup);
        netplay_shutdown();
        net_fail("The host sent a setup this build does not understand.");
        return;
    }
    const uint8_t* b = setup;
    /* the host's CMOS goes to a session file: never overwrite this machine's own */
    remove(NET_NVRAM);
    if (size) {
        FILE* f = fopen(NET_NVRAM, "wb");
        if (f) { fwrite(b + 4, 1, size, f); fclose(f); }
    }
    g_nvram = NET_NVRAM;
    memcpy(g_run_options, b + 4 + size, MAX_OPTIONS);
    free(setup);
}

/* ====================================================================== */
int main(int argc, char** argv) {
    int host_port = 0, delay = 0;
    const char* join = NULL;
    g_argc = argc; g_argv = argv;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--frames") && i + 1 < argc) g_frames_left = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--shot") && i + 1 < argc) g_shot = argv[++i];
        else if (!strcmp(argv[i], "--headless")) g_headless = 1;
        else if (!strcmp(argv[i], "--nvram") && i + 1 < argc) g_nvram = argv[++i];
        else if (!strcmp(argv[i], "--script") && i + 1 < argc) load_script(argv[++i]);
        else if (!strcmp(argv[i], "--host") && i + 1 < argc) host_port = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--join") && i + 1 < argc) join = argv[++i];
        else if (!strcmp(argv[i], "--delay") && i + 1 < argc) delay = atoi(argv[++i]);
    }
    for (int i = 0; i < k_option_count && i < MAX_OPTIONS; i++) g_options[i] = (uint8_t)k_options[i].def;
    if (!g_headless) settings_load();   /* a headless run is a test: defaults only */
    memcpy(g_run_options, g_options, MAX_OPTIONS);
    g_dips = g_inputs.dips;
    g_pixels = calloc(640 * 480, 4);
    g_scan = calloc(640 * 480 * 2, 4);
    profile_start();

    if (!g_headless) {
        WNDCLASSA wc = { 0 };
        wc.lpfnWndProc = wndproc;
        wc.hInstance = GetModuleHandleA(NULL);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.lpszClassName = "carnevil";
        RegisterClassA(&wc);
        g_wnd = CreateWindowA("carnevil", "CarnEvil", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                              1024, 768, NULL, NULL, wc.hInstance, NULL);
        build_menu();
        int fs = opt.fullscreen;
        opt.fullscreen = 0;
        apply_video(1);                 /* the windowed size, kept for leaving fullscreen */
        ShowWindow(g_wnd, SW_SHOW);
        opt.fullscreen = fs;
        apply_video(0);
    } else {
        audio_set_volume(0);
    }

    /* A netplay session starts at power-on, so it is set up before the board boots. */
    if (host_port > 0) net_host(host_port, delay > 0 ? delay : opt.net_delay);
    else if (join) net_join(join);
    if (g_quit) return 0;
    if (g_wnd) build_menu();

    hw_config cfg = { "carnevil/carnevi1.img", "carnevil1_9.u32", g_nvram, "sound102.u95" };
    hw_init(&cfg);
    g_booted = 1;

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

    ConvertThreadToFiber(NULL);
    recomp_context ctx;
    recomp_ctx_init(&ctx);
    ctx.r29 = (gpr)(int32_t)0x80800000;
    g_cur_ctx = &ctx;
    rtos_80004000(g_rdram, &ctx);
    hw_fatal("RTOS entry returned");
    return 0;
}
