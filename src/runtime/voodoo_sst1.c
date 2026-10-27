/* SPDX-License-Identifier: BSD-3-Clause
 *
 * Ported from MAME (https://github.com/mamedev/mame):
 * src/devices/video/voodoo.cpp, voodoo.h, voodoo_regs.h,
 * voodoo_render.cpp/.h and voodoo_pci.cpp, src/devices/video/poly.h and
 * src/emu/video/rgbutil.h/.cpp, which carry this licence:
 *
 *   license:BSD-3-Clause
 *   copyright-holders:Aaron Giles, Ted Green (voodoo_pci), Ville Linde (poly.h), Vas Crabb and Ryan Holtz (rgbutil)
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
 * 3dfx Voodoo Graphics (SST-1) emulation: register file, FIFO/swap timing,
 * triangle setup, pixel pipeline, texture units, LFB and video DAC.
 *
 * This file is a C port that follows these MAME files, which are licensed
 * under BSD-3-Clause (copyright Aaron Giles; rgbutil: Vas Crabb, Ryan Holtz;
 * poly.h: Ville Linde, Aaron Giles):
 *   src/devices/video/voodoo.cpp, voodoo.h, voodoo_regs.h,
 *   src/devices/video/voodoo_render.cpp, voodoo_render.h,
 *   src/devices/video/poly.h (triangle scan conversion),
 *   src/emu/video/rgbutil.h, rgbutil.cpp (colour arithmetic)
 * See LICENSES/BSD-3-Clause.txt and NOTICE.
 *
 * Board configuration (Midway Seattle): 2 MB frame buffer, one TMU with 4 MB.
 * Everything runs synchronously on the caller's thread. Writes that real
 * hardware would hold in its FIFO behind a vsync'd swapbufferCMD are queued
 * here and executed when the swap happens at vblank.
 */

#include "voodoo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#if defined(_MSC_VER)
#include <intrin.h>
static int clz32(uint32_t v) { unsigned long i; return _BitScanReverse(&i, v) ? 31 - (int)i : 32; }
static int clz64(uint64_t v) { unsigned long i; return _BitScanReverse64(&i, v) ? 63 - (int)i : 64; }
#else
static int clz32(uint32_t v) { return v ? __builtin_clz(v) : 32; }
static int clz64(uint64_t v) { return v ? __builtin_clzll(v) : 64; }
#endif

#define FB_MB   2
#define TMU_MB  4
#define BIT(x, n, w) (((x) >> (n)) & ((1u << (w)) - 1))

static int32_t sext(uint32_t v, int bits) { return (int32_t)(v << (32 - bits)) >> (32 - bits); }
static int32_t clampi(int32_t v, int32_t lo, int32_t hi) { return v < lo ? lo : v > hi ? hi : v; }
static uint32_t bswap32(uint32_t v) { return (v >> 24) | ((v >> 8) & 0xff00) | ((v << 8) & 0xff0000) | (v << 24); }

/* ---------------------------------------------------------------------
 * Register numbers (word index into the 1 KB register space)
 * --------------------------------------------------------------------- */
enum {
    r_status = 0x000/4, r_vertexAx = 0x008/4, r_vertexAy, r_vertexBx, r_vertexBy, r_vertexCx, r_vertexCy,
    r_startR = 0x020/4, r_startG, r_startB, r_startZ, r_startA, r_startS, r_startT, r_startW,
    r_dRdX = 0x040/4, r_dGdX, r_dBdX, r_dZdX, r_dAdX, r_dSdX, r_dTdX, r_dWdX,
    r_dRdY = 0x060/4, r_dGdY, r_dBdY, r_dZdY, r_dAdY, r_dSdY, r_dTdY, r_dWdY,
    r_triangleCMD = 0x080/4,
    r_ftriangleCMD = 0x100/4, r_fbzColorPath, r_fogMode, r_alphaMode, r_fbzMode, r_lfbMode,
    r_clipLeftRight, r_clipLowYHighY, r_nopCMD, r_fastfillCMD, r_swapbufferCMD, r_fogColor,
    r_zaColor, r_chromaKey, r_chromaRange, r_userIntrCMD,
    r_stipple = 0x140/4, r_color0, r_color1, r_fbiPixelsIn, r_fbiChromaFail, r_fbiZfuncFail,
    r_fbiAfuncFail, r_fbiPixelsOut, r_fogTable,
    r_fbiInit4 = 0x200/4, r_vRetrace, r_backPorch, r_videoDimensions, r_fbiInit0, r_fbiInit1,
    r_fbiInit2, r_fbiInit3, r_hSync, r_vSync, r_clutData, r_dacData, r_maxRgbDelta,
    r_fbiSwapHistory = 0x258/4, r_fbiTrianglesOut = 0x25c/4,
    r_textureMode = 0x300/4, r_tLOD, r_tDetail, r_texBaseAddr, r_texBaseAddr_1, r_texBaseAddr_2,
    r_texBaseAddr_3_8, r_trexInit0, r_trexInit1, r_nccTable
};

/* first 64 registers as seen through the alternate (triangle-remap) mapping */
static const uint8_t s_alias_map[0x40] = {
    r_status, 0x004/4, r_vertexAx, r_vertexAy, r_vertexBx, r_vertexBy, r_vertexCx, r_vertexCy,
    r_startR, r_dRdX, r_dRdY, r_startG, r_dGdX, r_dGdY, r_startB, r_dBdX,
    r_dBdY, r_startZ, r_dZdX, r_dZdY, r_startA, r_dAdX, r_dAdY, r_startS,
    r_dSdX, r_dSdY, r_startT, r_dTdX, r_dTdY, r_startW, r_dWdX, r_dWdY,
    r_triangleCMD, 0x084/4, 0x088/4, 0x08c/4, 0x090/4, 0x094/4, 0x098/4, 0x09c/4,
    0x0a0/4, 0x0c0/4, 0x0e0/4, 0x0a4/4, 0x0c4/4, 0x0e4/4, 0x0a8/4, 0x0c8/4,
    0x0e8/4, 0x0ac/4, 0x0cc/4, 0x0ec/4, 0x0b0/4, 0x0d0/4, 0x0f0/4, 0x0b4/4,
    0x0d4/4, 0x0f4/4, 0x0b8/4, 0x0d8/4, 0x0f8/4, 0x0bc/4, 0x0dc/4, 0x0fc/4
};

/* register table: write mask, chips, FIFO flag, handlers */
enum { W_INV, W_UNIMP, W_PASS, W_FP4, W_FP12, W_ST, W_FST, W_W, W_FW, W_TRI, W_NOP, W_FASTFILL,
       W_SWAP, W_FOG, W_FBIINIT, W_VIDEO, W_CLUT, W_DAC, W_TEX, W_PAL };
enum { R_INV, R_PASS, R_STATUS, R_FBIINIT2, R_VRETRACE, R_STATS };
enum { C_FBI = 1, C_TREX = 6, C_ALL = 7 };

typedef struct { uint32_t mask; uint8_t chips, fifo, wk, rk; } regent_t;
static regent_t s_regs[256];

static void regdef(int byteoff, int bits, int chips, int fifo, int wk, int rk) {
    regent_t* e = &s_regs[byteoff / 4];
    e->mask = bits == 32 ? 0xffffffffu : (1u << bits) - 1;
    e->chips = (uint8_t)chips; e->fifo = (uint8_t)fifo; e->wk = (uint8_t)wk; e->rk = (uint8_t)rk;
}

static void init_regtable(void) {
    for (int i = 0; i < 256; i++) regdef(i * 4, 32, C_FBI, 1, W_INV, R_INV);
    regdef(0x000, 32, C_FBI, 1, W_UNIMP, R_STATUS);
    for (int o = 0x008; o <= 0x01c; o += 4) regdef(o, 16, C_ALL, 1, W_PASS, R_INV);
    for (int base = 0x020; base <= 0x060; base += 0x20) {          /* start, d?dX, d?dY */
        regdef(base + 0x00, 24, C_FBI, 1, W_PASS, R_INV);
        regdef(base + 0x04, 24, C_FBI, 1, W_PASS, R_INV);
        regdef(base + 0x08, 24, C_FBI, 1, W_PASS, R_INV);
        regdef(base + 0x0c, 32, C_FBI, 1, W_PASS, R_INV);
        regdef(base + 0x10, 24, C_FBI, 1, W_PASS, R_INV);
        regdef(base + 0x14, 32, C_TREX, 1, W_ST, R_INV);
        regdef(base + 0x18, 32, C_TREX, 1, W_ST, R_INV);
        regdef(base + 0x1c, 32, C_ALL, 1, W_W, R_INV);
        regdef(base + 0x80, 32, C_FBI, 1, W_FP12, R_INV);
        regdef(base + 0x84, 32, C_FBI, 1, W_FP12, R_INV);
        regdef(base + 0x88, 32, C_FBI, 1, W_FP12, R_INV);
        regdef(base + 0x8c, 32, C_FBI, 1, W_FP12, R_INV);
        regdef(base + 0x90, 32, C_FBI, 1, W_FP12, R_INV);
        regdef(base + 0x94, 32, C_TREX, 1, W_FST, R_INV);
        regdef(base + 0x98, 32, C_TREX, 1, W_FST, R_INV);
        regdef(base + 0x9c, 32, C_ALL, 1, W_FW, R_INV);
    }
    regdef(0x080, 32, C_ALL, 1, W_TRI, R_INV);
    for (int o = 0x088; o <= 0x09c; o += 4) regdef(o, 32, C_ALL, 1, W_FP4, R_INV);
    regdef(0x100, 32, C_ALL, 1, W_TRI, R_INV);
    regdef(0x104, 28, C_ALL, 1, W_PASS, R_PASS);
    regdef(0x108, 6, C_ALL, 1, W_PASS, R_PASS);
    regdef(0x10c, 32, C_ALL, 1, W_PASS, R_PASS);
    regdef(0x110, 21, C_ALL, 1, W_PASS, R_PASS);
    regdef(0x114, 17, C_ALL, 1, W_PASS, R_PASS);
    regdef(0x118, 26, C_ALL, 1, W_PASS, R_PASS);
    regdef(0x11c, 26, C_ALL, 1, W_PASS, R_PASS);
    regdef(0x120, 1, C_ALL, 1, W_NOP, R_INV);
    regdef(0x124, 0, C_FBI, 1, W_FASTFILL, R_INV);
    regdef(0x128, 9, C_FBI, 1, W_SWAP, R_INV);
    regdef(0x12c, 24, C_FBI, 1, W_PASS, R_INV);
    regdef(0x130, 32, C_FBI, 1, W_PASS, R_INV);
    regdef(0x134, 24, C_FBI, 1, W_PASS, R_INV);
    regdef(0x140, 32, C_FBI, 1, W_PASS, R_PASS);
    regdef(0x144, 32, C_FBI, 1, W_PASS, R_PASS);
    regdef(0x148, 32, C_FBI, 1, W_PASS, R_PASS);
    for (int o = 0x14c; o <= 0x15c; o += 4) regdef(o, 24, C_FBI, 0, W_INV, R_STATS);
    for (int o = 0x160; o <= 0x1dc; o += 4) regdef(o, 32, C_FBI, 1, W_FOG, R_INV);
    regdef(0x200, 28, C_FBI, 0, W_FBIINIT, R_PASS);
    regdef(0x204, 12, C_FBI, 0, W_INV, R_VRETRACE);
    regdef(0x208, 24, C_FBI, 0, W_VIDEO, R_PASS);
    regdef(0x20c, 26, C_FBI, 0, W_VIDEO, R_PASS);
    regdef(0x210, 31, C_FBI, 0, W_FBIINIT, R_PASS);
    regdef(0x214, 32, C_FBI, 0, W_FBIINIT, R_PASS);
    regdef(0x218, 32, C_FBI, 0, W_FBIINIT, R_FBIINIT2);
    regdef(0x21c, 32, C_FBI, 0, W_FBIINIT, R_PASS);
    regdef(0x220, 26, C_FBI, 0, W_VIDEO, R_INV);
    regdef(0x224, 28, C_FBI, 0, W_VIDEO, R_INV);
    regdef(0x228, 30, C_FBI, 0, W_CLUT, R_INV);
    regdef(0x22c, 12, C_FBI, 0, W_DAC, R_INV);
    regdef(0x230, 24, C_FBI, 0, W_UNIMP, R_INV);
    regdef(0x300, 32, C_TREX, 1, W_TEX, R_INV);
    regdef(0x304, 32, C_TREX, 1, W_TEX, R_INV);
    regdef(0x308, 17, C_TREX, 1, W_TEX, R_INV);
    for (int o = 0x30c; o <= 0x318; o += 4) regdef(o, 19, C_TREX, 1, W_TEX, R_INV);
    regdef(0x31c, 32, C_TREX, 1, W_PASS, R_INV);
    regdef(0x320, 32, C_TREX, 1, W_PASS, R_INV);
    for (int o = 0x324; o <= 0x380; o += 4) regdef(o, 32, C_TREX, 1, W_PAL, R_INV);
}

/* ---------------------------------------------------------------------
 * Colour helpers (rgbaint_t equivalent: four signed ints)
 * --------------------------------------------------------------------- */
typedef struct { int32_t a, r, g, b; } rgba_t;

static rgba_t rgba_u32(uint32_t c) { rgba_t v = { (int32_t)(c >> 24), (int32_t)((c >> 16) & 0xff), (int32_t)((c >> 8) & 0xff), (int32_t)(c & 0xff) }; return v; }
static uint32_t rgba_pack(rgba_t c) { return ((uint32_t)(c.a & 0xff) << 24) | ((uint32_t)(c.r & 0xff) << 16) | ((uint32_t)(c.g & 0xff) << 8) | (uint32_t)(c.b & 0xff); }
static rgba_t rgba_all(int32_t v) { rgba_t c = { v, v, v, v }; return c; }
static rgba_t rgba_alpha(rgba_t c) { return rgba_all(c.a); }
static int32_t clamp8(int32_t v) { return v < 0 ? 0 : v > 255 ? 255 : v; }
static rgba_t rgba_clamp8(rgba_t c) { c.a = clamp8(c.a); c.r = clamp8(c.r); c.g = clamp8(c.g); c.b = clamp8(c.b); return c; }
static rgba_t scale_add_clamp(rgba_t c, rgba_t s, rgba_t add) {
    c.a = clamp8(((c.a * s.a) >> 8) + add.a); c.r = clamp8(((c.r * s.r) >> 8) + add.r);
    c.g = clamp8(((c.g * s.g) >> 8) + add.g); c.b = clamp8(((c.b * s.b) >> 8) + add.b);
    return c;
}

static uint8_t expand_bits(uint32_t v, int bits) {
    switch (bits) {
        case 1: return v ? 0xff : 0;
        case 2: return (uint8_t)(v * 0x55);
        case 3: return (uint8_t)((v << 5) | (v << 2) | (v >> 1));
        case 4: return (uint8_t)(v * 0x11);
        case 5: return (uint8_t)((v << 3) | (v >> 2));
        case 6: return (uint8_t)((v << 2) | (v >> 4));
        default: return (uint8_t)v;
    }
}

/* abits == 0: alpha comes from 'alpha' */
static uint32_t argb_expand(uint32_t d, int ab, int as, int rb, int rs, int gb, int gs, int bb, int bs, uint32_t alpha) {
    uint32_t a = ab ? expand_bits(BIT(d, as, ab), ab) : alpha;
    return (a << 24) | ((uint32_t)expand_bits(BIT(d, rs, rb), rb) << 16) |
           ((uint32_t)expand_bits(BIT(d, gs, gb), gb) << 8) | expand_bits(BIT(d, bs, bb), bb);
}

/* shared texel lookup tables */
static uint32_t t_rgb332[256], t_alpha8[256], t_int8[256], t_ai44[256];
static uint32_t t_rgb565[65536], t_argb1555[65536], t_argb4444[65536];
static const uint32_t* t_texel[16];

/* dithering (hardware-verified equations from MAME) */
static const uint8_t s_dither4x4[16] = { 0, 8, 2, 10, 12, 4, 14, 6, 3, 11, 1, 9, 15, 7, 13, 5 };
static const uint8_t s_dither2x2[16] = { 8, 10, 8, 10, 11, 9, 11, 9, 8, 10, 8, 10, 11, 9, 11, 9 };
static uint8_t s_dither4_lookup[256 * 4 * 4 * 2], s_dither2_lookup[256 * 4 * 4 * 2], s_nodither_lookup[256 * 4 * 2];

static void init_tables(void) {
    for (int v = 0; v < 256; v++) {
        t_rgb332[v] = argb_expand(v, 0, 0, 3, 5, 3, 2, 2, 0, 0xff);
        t_alpha8[v] = ((uint32_t)v << 24) | ((uint32_t)v << 16) | ((uint32_t)v << 8) | v;
        t_int8[v] = 0xff000000u | ((uint32_t)v << 16) | ((uint32_t)v << 8) | v;
        t_ai44[v] = argb_expand(v, 4, 4, 4, 0, 4, 0, 4, 0, 0);
    }
    for (int v = 0; v < 65536; v++) {
        t_rgb565[v] = argb_expand(v, 0, 0, 5, 11, 6, 5, 5, 0, 0xff);
        t_argb1555[v] = argb_expand(v, 1, 15, 5, 10, 5, 5, 5, 0, 0);
        t_argb4444[v] = argb_expand(v, 4, 12, 4, 8, 4, 4, 4, 0, 0);
    }
    t_texel[0] = t_rgb332; t_texel[2] = t_alpha8; t_texel[3] = t_int8; t_texel[4] = t_ai44;
    t_texel[8] = t_rgb332; t_texel[10] = t_rgb565; t_texel[11] = t_argb1555; t_texel[12] = t_argb4444;
    t_texel[13] = t_int8;

    for (int val = 0; val < 256 * 4 * 4 * 2; val++) {
        int color = val & 0xff, g = (val >> 8) & 1, x = (val >> 9) & 3, y = (val >> 11) & 3;
        int d4 = s_dither4x4[y * 4 + x], d2 = s_dither2x2[y * 4 + x];
        if (!g) {
            s_dither4_lookup[val] = (uint8_t)(((color << 1) - (color >> 4) + (color >> 7) + d4) >> 4);
            s_dither2_lookup[val] = (uint8_t)(((color << 1) - (color >> 4) + (color >> 7) + d2) >> 4);
        } else {
            s_dither4_lookup[val] = (uint8_t)(((color << 2) - (color >> 4) + (color >> 6) + d4) >> 4);
            s_dither2_lookup[val] = (uint8_t)(((color << 2) - (color >> 4) + (color >> 6) + d2) >> 4);
        }
    }
    for (int val = 0; val < 256 * 4 * 2; val++) {
        int color = val & 0xff, g = (val >> 8) & 1;
        s_nodither_lookup[val] = (uint8_t)(g ? color >> 2 : color >> 3);
    }
    init_regtable();
}

typedef struct { const uint8_t* lookup; const uint8_t* raw; const uint8_t* raw4x4; } dither_t;

static dither_t dither_make(int y, uint32_t fbzmode) {
    dither_t d;
    d.raw4x4 = &s_dither4x4[(y & 3) * 4];
    d.raw = NULL;
    if (!BIT(fbzmode, 8, 1)) d.lookup = s_nodither_lookup;
    else if (!BIT(fbzmode, 11, 1)) { d.lookup = &s_dither4_lookup[(y & 3) << 11]; d.raw = &s_dither4x4[(y & 3) * 4]; }
    else { d.lookup = &s_dither2_lookup[(y & 3) << 11]; d.raw = &s_dither2x2[(y & 3) * 4]; }
    return d;
}
static uint16_t dither_pixel(const dither_t* d, int x, int r, int g, int b) {
    const uint8_t* t = &d->lookup[(x & 3) << 9];
    return (uint16_t)((t[r] << 11) | (t[g + 256] << 5) | t[b]);
}

/* ---------------------------------------------------------------------
 * State
 * --------------------------------------------------------------------- */
typedef struct {
    uint32_t reg[256];
    int64_t starts, startt, startw, dsdx, dtdx, dwdx, dsdy, dtdy, dwdy;
} vregs_t;

typedef struct {
    vregs_t r;
    uint8_t* ram;
    uint32_t mask;
    int dirty;
    int pal_dirty[4];
    uint32_t palette[2][256];
    uint32_t ncc[2][256];
    /* decoded texture */
    const uint32_t* lookup;
    uint32_t wmask, hmask, detailscale, lodmask;
    int32_t lodmin, lodmax, lodbias, detailmax, detailbias;
    uint32_t lodoffset[9];
} tmu_t;

/* A colour combine unit, decoded once per triangle. fbzColorPath (from bit 8)
 * and textureMode (from bit 12) share the same 18-bit layout. */
typedef struct {
    uint8_t zero_rgb, zero_a, sub_rgb, sub_a, msel_rgb, msel_a, add_rgb, add_a;
    int32_t rev_rgb, rev_a, inv_rgb, inv_a;
    uint8_t kind;               /* COMB_GENERAL, or the result is simply c_local / c_other */
} comb_t;
enum { COMB_GENERAL, COMB_LOCAL, COMB_OTHER };

typedef struct {
    uint16_t* dest; uint16_t* depth;
    tmu_t* tex0;
    uint32_t fbzcp, fbzmode, alphamode, fogmode, texmode0;
    int32_t clipleft, clipright, cliptop, clipbottom;
    int32_t ax, ay;
    int32_t startr, startg, startb, starta, startz;
    int64_t startw;
    int32_t drdx, dgdx, dbdx, dadx, dzdx; int64_t dwdx;
    int32_t drdy, dgdy, dbdy, dady, dzdy; int64_t dwdy;
    int64_t starts0, startt0, startw0, ds0dx, dt0dx, dw0dx, ds0dy, dt0dy, dw0dy;
    uint32_t color0, color1, chromakey, fogcolor, zacolor, stipple, alpharef;
    int send_config;
    comb_t cc, tc;          /* colour path and texture combine units */
} poly_t;

struct voodoo_state {
    vregs_t fbi;
    tmu_t tmu;
    uint8_t* mem;
    uint8_t* fbram;
    uint32_t fbmask;
    uint32_t init_enable;

    uint32_t rgboffs[3], auxoffs;
    int frontbuf, backbuf;
    uint32_t rowpixels;
    int32_t yorigin;

    uint32_t width, height, vtotal;
    int vblank, vblank_count, swaps_pending;
    int swap_pending, swap_interval;
    /* optional profiling (env VOODOO_PROF) */
    int prof; double prof_busy, prof_t0; uint32_t prof_frames, prof_tris;
    double last_vblank_end;

    uint8_t dac_reg[32], dac_read_result;
    uint32_t clut[33];
    int clut_dirty;
    uint16_t pen[65536];

    uint8_t fogblend[64], fogdelta[64];

    /* commands held behind a vsync'd swap: (type|offset, data) pairs */
    uint32_t* q; size_t qhead, qtail, qcap;
    uint32_t memfifo_entries;

    uint16_t dummy_depth[4096];
    uint16_t out[640 * 480];
};

#define Q_REG 0x00000000u
#define Q_LFB 0x40000000u
#define Q_TEX 0x80000000u
#define Q_NO_16_31 0x20000000u
#define Q_NO_0_15  0x10000000u

/* Emulated time (hw.c): the beam position must follow the emulation, not the
 * host clock, or two runs of the same inputs would differ. */
double hw_time(void);

/* Host time, for the profiler only. */
static double now_seconds(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (double)ts.tv_sec + ts.tv_nsec * 1e-9;
}

/* ---------------------------------------------------------------------
 * Buffers
 * --------------------------------------------------------------------- */
static uint16_t* draw_buffer(voodoo_state_t* s, int idx) {
    if (s->rgboffs[idx] == ~0u) return NULL;
    return (uint16_t*)(s->fbram + s->rgboffs[idx]);
}
static uint16_t* aux_buffer(voodoo_state_t* s) { return s->auxoffs == ~0u ? NULL : (uint16_t*)(s->fbram + s->auxoffs); }
static uint16_t* ram_end(voodoo_state_t* s) { return (uint16_t*)(s->fbram + s->fbmask + 1); }
static uint16_t* draw_buffer_indirect(voodoo_state_t* s, int index) {
    switch (index) {
        case 0: return draw_buffer(s, s->frontbuf);
        case 1: return draw_buffer(s, s->backbuf);
        default: return NULL;
    }
}
static uint16_t* lfb_buffer_indirect(voodoo_state_t* s, int index) {
    if (index == 2) return aux_buffer(s);
    return draw_buffer_indirect(s, index);
}

static void recompute_video_memory(voodoo_state_t* s) {
    uint32_t f2 = s->fbi.reg[r_fbiInit2];
    uint32_t config = BIT(f2, 4, 1);
    uint32_t pages = BIT(f2, 11, 9);
    s->rgboffs[0] = 0;
    s->rgboffs[1] = pages * 0x1000;
    if (config == 0) { s->rgboffs[2] = ~0u; s->auxoffs = 2 * pages * 0x1000; }
    else { s->rgboffs[2] = 2 * pages * 0x1000; s->auxoffs = ~0u; }
    for (int b = 0; b < 3; b++)
        if (s->rgboffs[b] != ~0u && s->rgboffs[b] > s->fbmask) s->rgboffs[b] = s->fbmask;
    if (s->auxoffs != ~0u && s->auxoffs > s->fbmask) s->auxoffs = s->fbmask;
    if (s->rgboffs[2] == ~0u) {
        if (s->frontbuf == 2) s->frontbuf = 0;
        if (s->backbuf == 2) s->backbuf = 0;
    }
    s->rowpixels = BIT(s->fbi.reg[r_fbiInit1], 4, 4) * 64;
}

static void recompute_fbmem_fifo(voodoo_state_t* s) {
    uint32_t f4 = s->fbi.reg[r_fbiInit4];
    uint32_t last = BIT(f4, 18, 10), start = BIT(f4, 8, 10);
    if (last > s->fbmask / 0x1000) last = s->fbmask / 0x1000;
    if (start <= last && BIT(s->fbi.reg[r_fbiInit0], 13, 1)) {
        uint32_t size = (last + 1 - start) * 0x1000 / 4;
        s->memfifo_entries = size < 65536 * 2 ? size : 65536 * 2;
    } else
        s->memfifo_entries = 0;
}

/* Clip a scanline [*sx,*ex) so that base[row*rowpixels + x] stays in frame-buffer RAM. */
static int clip_row(voodoo_state_t* s, const uint16_t* base, int32_t scry, int32_t* sx, int32_t* ex) {
    int64_t row = (int64_t)(base - (const uint16_t*)s->fbram) + (int64_t)scry * s->rowpixels;
    int64_t lo = -row, hi = (int64_t)((s->fbmask + 1) / 2) - row;
    if (*sx < lo) *sx = (int32_t)lo;
    if (*ex > hi) *ex = (int32_t)(hi < -0x7fffffff ? -0x7fffffff : hi);
    return *sx < *ex;
}

/* ---------------------------------------------------------------------
 * Textures
 * --------------------------------------------------------------------- */
static void compute_ncc(uint32_t* out, const uint32_t* regs) {
    const uint8_t* ybase = (const uint8_t*)regs;    /* little-endian host */
    for (int index = 0; index < 256; index++) {
        int32_t y = ybase[BIT(index, 4, 4)];
        int32_t i = (int32_t)regs[4 + BIT(index, 2, 2)];
        int32_t q = (int32_t)regs[8 + BIT(index, 0, 2)];
        int32_t r = clampi(y + sext((uint32_t)i >> 18, 9) + sext((uint32_t)q >> 18, 9), 0, 255);
        int32_t g = clampi(y + sext((uint32_t)i >> 9, 9) + sext((uint32_t)q >> 9, 9), 0, 255);
        int32_t b = clampi(y + sext((uint32_t)i, 9) + sext((uint32_t)q, 9), 0, 255);
        out[index] = 0xff000000u | ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
    }
}

static void ncc_w(tmu_t* t, uint32_t regnum, uint32_t data) {
    uint32_t regindex = regnum - r_nccTable;
    if (BIT(data, 31, 1) && regindex >= 4 && regindex < 12) {
        int index = (int)((BIT(data, 24, 7) << 1) | BIT(regindex, 0, 1));
        uint32_t rgb = 0xff000000u | data;
        uint32_t argb = argb_expand(data, 6, 18, 6, 12, 6, 6, 6, 0, 0);
        if (t->palette[0][index] != rgb) { t->palette[0][index] = rgb; t->pal_dirty[0] = 1; }
        if (t->palette[1][index] != argb) { t->palette[1][index] = argb; t->pal_dirty[1] = 1; }
        return;
    }
    if (t->r.reg[regnum] == data) return;
    t->r.reg[regnum] = data;
    t->pal_dirty[2 + regindex / 12] = 1;
}

static void tex_recompute(tmu_t* t) {
    uint32_t lod = t->r.reg[r_tLOD], mode = t->r.reg[r_textureMode];
    uint32_t fmt = BIT(mode, 8, 4);
    const uint32_t* lookup = t_texel[fmt];
    if (!lookup) {
        if ((fmt & 7) == 1) {
            int sel = (int)BIT(mode, 5, 1);
            if (t->pal_dirty[2 + sel]) { compute_ncc(t->ncc[sel], &t->r.reg[r_nccTable + 12 * sel]); t->pal_dirty[2 + sel] = 0; }
            lookup = t->ncc[sel];
        } else
            lookup = t->palette[fmt == 6 ? 1 : 0];
    }
    t->lookup = lookup;

    t->lodmin = (int32_t)(BIT(lod, 0, 6) << 6);
    t->lodmax = (int32_t)((BIT(lod, 6, 6) < 32 ? BIT(lod, 6, 6) : 32) << 6);
    t->lodbias = (int32_t)(int8_t)(BIT(lod, 12, 6) << 2) << 4;
    t->lodmask = 0x1ff;
    if (BIT(lod, 19, 1)) t->lodmask = BIT(lod, 18, 1) ? 0x0aa : 0x155;
    t->wmask = t->hmask = 0xff;
    if (BIT(lod, 20, 1)) t->hmask >>= BIT(lod, 21, 2);
    else t->wmask >>= BIT(lod, 21, 2);

    int bppscale = (int)(fmt >> 3);
    uint32_t lodstart[9], offset = 0;
    for (int l = 0; l <= 8; l++) {
        lodstart[l] = offset;
        if (BIT(t->lodmask, l, 1)) {
            uint32_t size = ((t->wmask >> l) + 1) * ((t->hmask >> l) + 1);
            if (l >= 3 && size < 4) size = 4;
            offset += size << bppscale;
        }
    }
    uint32_t lodbase[9];
    for (int l = 0; l <= 8; l++) lodbase[l] = t->r.reg[r_texBaseAddr];
    if (BIT(lod, 24, 1) && BIT(lod, 28, 4) == 0) {
        lodbase[1] = t->r.reg[r_texBaseAddr_1];
        lodbase[2] = t->r.reg[r_texBaseAddr_2];
        for (int l = 3; l <= 8; l++) lodbase[l] = t->r.reg[r_texBaseAddr_3_8];
    }
    for (int l = 0; l <= 8; l++)
        t->lodoffset[l] = (((lodbase[l] & 0xfffff) << 3) + lodstart[l]) & t->mask;

    uint32_t det = t->r.reg[r_tDetail];
    t->detailmax = (int32_t)BIT(det, 0, 8);
    t->detailbias = (int32_t)(int8_t)(BIT(det, 8, 6) << 2) << 6;
    t->detailscale = BIT(det, 14, 3);
}

static void prepare_texture(tmu_t* t) {
    uint32_t mode = t->r.reg[r_textureMode], fmt = BIT(mode, 8, 4);
    int ncc_dirty = !t_texel[fmt] && (fmt & 7) == 1 && t->pal_dirty[2 + BIT(mode, 5, 1)];
    if (t->dirty || ncc_dirty) { tex_recompute(t); t->dirty = 0; }
}

static uint32_t lookup_texel(const tmu_t* t, uint32_t fmt, uint32_t base, int32_t s, int32_t tt) {
    if (fmt < 8) return t->lookup[t->ram[(base + tt + s) & t->mask]];
    uint16_t v = *(const uint16_t*)&t->ram[(base + 2 * (tt + s)) & t->mask];
    if (fmt >= 10 && fmt <= 12) return t->lookup[v];
    return (t->lookup[v & 0xff] & 0xffffff) | ((uint32_t)(v & 0xff00) << 16);
}

static int32_t fast_log2(double value, int fracbits) {
    static const uint8_t tbl[128] = {
          0,   2,   5,   8,  11,  14,  16,  19,  22,  25,  27,  30,  33,  35,  38,  40,
         43,  46,  48,  51,  53,  56,  58,  61,  63,  65,  68,  70,  73,  75,  77,  80,
         82,  84,  87,  89,  91,  93,  96,  98, 100, 102, 104, 106, 109, 111, 113, 115,
        117, 119, 121, 123, 125, 127, 129, 132, 134, 136, 138, 140, 141, 143, 145, 147,
        149, 151, 153, 155, 157, 159, 161, 162, 164, 166, 168, 170, 172, 173, 175, 177,
        179, 181, 182, 184, 186, 188, 189, 191, 193, 194, 196, 198, 200, 201, 203, 205,
        206, 208, 209, 211, 213, 214, 216, 218, 219, 221, 222, 224, 225, 227, 229, 230,
        232, 233, 235, 236, 238, 239, 241, 242, 244, 245, 247, 248, 250, 251, 253, 254 };
    if (value < 0) return 0;
    uint64_t bits; memcpy(&bits, &value, 8);
    uint32_t ival = (uint32_t)(bits >> 45);
    int32_t exp = (int32_t)(ival >> 7) - 1023 - fracbits;
    return (int32_t)((uint32_t)exp << 8) | tbl[ival & 127];
}

static int32_t compute_lodbase(int64_t dsdx, int64_t dsdy, int64_t dtdx, int64_t dtdy) {
    double fdsdx = (double)dsdx, fdsdy = (double)dsdy, fdtdx = (double)dtdx, fdtdy = (double)dtdy;
    double texdx = fdsdx * fdsdx + fdtdx * fdtdx;
    double texdy = fdsdy * fdsdy + fdtdy * fdtdy;
    return fast_log2(texdx > texdy ? texdx : texdy, 64) / 2;
}

static rgba_t bilinear(uint32_t c00, uint32_t c01, uint32_t c10, uint32_t c11, uint32_t u, uint32_t v) {
    uint32_t ag00 = ((c00 >> 8) & 0x00ff00ff) * (256 - u), rb00 = (c00 & 0x00ff00ff) * (256 - u);
    uint32_t ag10 = ((c10 >> 8) & 0x00ff00ff) * (256 - u), rb10 = (c10 & 0x00ff00ff) * (256 - u);
    uint32_t ag01 = ((c01 >> 8) & 0x00ff00ff) * u, rb01 = (c01 & 0x00ff00ff) * u;
    uint32_t ag11 = ((c11 >> 8) & 0x00ff00ff) * u, rb11 = (c11 & 0x00ff00ff) * u;
    uint32_t ag0x = ((ag00 >> 1) & 0x7fff7fff) + ((ag01 >> 1) & 0x7fff7fff);
    uint32_t rb0x = ((rb00 >> 1) & 0x7fff7fff) + ((rb01 >> 1) & 0x7fff7fff);
    uint32_t ag1x = ((ag10 >> 1) & 0x7fff7fff) + ((ag11 >> 1) & 0x7fff7fff);
    uint32_t rb1x = ((rb10 >> 1) & 0x7fff7fff) + ((rb11 >> 1) & 0x7fff7fff);
    rgba_t c;
    c.a = (int32_t)((((ag0x >> 16) * (256 - v)) + ((ag1x >> 16) * v)) >> 15);
    c.r = (int32_t)((((rb0x >> 16) * (256 - v)) + ((rb1x >> 16) * v)) >> 15);
    c.g = (int32_t)((((ag0x & 0xffff) * (256 - v)) + ((ag1x & 0xffff) * v)) >> 15);
    c.b = (int32_t)((((rb0x & 0xffff) * (256 - v)) + ((rb1x & 0xffff) * v)) >> 15);
    return c;
}

#define BILINEAR_MASK 0xf0      /* Voodoo 1 resolves 4 bits of bilinear fraction */

static rgba_t fetch_texel(const tmu_t* t, uint32_t mode, const dither_t* d, int32_t x,
                          double iters, double itert, double iterw, int32_t* plod) {
    int32_t s, tt, lod = *plod;
    if (BIT(mode, 0, 1)) {
        double recip = 256.0 / iterw;
        s = (int32_t)(iters * recip);
        tt = (int32_t)(itert * recip);
        if (t->lodmin < t->lodmax) lod -= fast_log2(iterw, 32);   /* else the clamp below decides */
    } else {
        s = (int32_t)(iters * (1.0 / (1 << 24)));
        tt = (int32_t)(itert * (1.0 / (1 << 24)));
    }
    if (BIT(mode, 3, 1) && iterw < 0) s = tt = 0;

    lod += t->lodbias;
    if (BIT(mode, 4, 1)) lod += d->raw4x4[x & 3] << 4;
    if (lod < t->lodmin) lod = t->lodmin;
    if (lod > t->lodmax) lod = t->lodmax;
    *plod = lod;

    int32_t ilod = lod >> 8;
    ilod += (int32_t)((~t->lodmask >> ilod) & 1);
    {
        int32_t maxlod = (t->lodmask & 0x100) ? 8 : 7;
        if (ilod > maxlod) ilod = maxlod;
    }
    uint32_t base = t->lodoffset[ilod];
    int32_t smax = (int32_t)(t->wmask >> ilod), tmax = (int32_t)(t->hmask >> ilod);
    uint32_t fmt = BIT(mode, 8, 4);

    if ((lod == t->lodmin && !BIT(mode, 2, 1)) || (lod != t->lodmin && !BIT(mode, 1, 1))) {
        s >>= ilod + 8;
        tt >>= ilod + 8;
        if (BIT(mode, 6, 1)) s = clampi(s, 0, smax);
        if (BIT(mode, 7, 1)) tt = clampi(tt, 0, tmax);
        s &= smax; tt &= tmax;
        tt *= smax + 1;
        return rgba_u32(lookup_texel(t, fmt, base, s, tt));
    } else {
        s >>= ilod; tt >>= ilod;
        s -= 0x80; tt -= 0x80;
        uint32_t sfrac = (uint32_t)s & BILINEAR_MASK, tfrac = (uint32_t)tt & BILINEAR_MASK;
        s >>= 8; tt >>= 8;
        int32_t s1 = s + 1, t1 = tt + 1;
        if (BIT(mode, 6, 1)) {
            if (s < 0) s = s1 = 0;
            else if (s >= smax) s = s1 = smax;
        }
        s &= smax; s1 &= smax;
        if (BIT(mode, 7, 1)) {
            if (tt < 0) tt = t1 = 0;
            else if (tt >= tmax) tt = t1 = tmax;
        }
        tt &= tmax; t1 &= tmax;
        tt *= smax + 1; t1 *= smax + 1;
        return bilinear(lookup_texel(t, fmt, base, s, tt), lookup_texel(t, fmt, base, s1, tt),
                        lookup_texel(t, fmt, base, s, t1), lookup_texel(t, fmt, base, s1, t1), sfrac, tfrac);
    }
}


static void comb_decode(comb_t* c, uint32_t v) {
    c->zero_rgb = (uint8_t)BIT(v, 0, 1); c->sub_rgb = (uint8_t)BIT(v, 1, 1); c->msel_rgb = (uint8_t)BIT(v, 2, 3);
    c->rev_rgb = BIT(v, 5, 1) ? 0 : 0xff; c->add_rgb = (uint8_t)BIT(v, 6, 2); c->inv_rgb = BIT(v, 8, 1) ? 0xff : 0;
    c->zero_a = (uint8_t)BIT(v, 9, 1); c->sub_a = (uint8_t)BIT(v, 10, 1); c->msel_a = (uint8_t)BIT(v, 11, 3);
    c->rev_a = BIT(v, 14, 1) ? 0 : 0xff; c->add_a = (uint8_t)BIT(v, 15, 2); c->inv_a = BIT(v, 17, 1) ? 0xff : 0;
    c->kind = COMB_GENERAL;
    if (c->inv_rgb || c->inv_a) return;
    /* other is zeroed and not subtracted from: result = clamp(add) */
    if (c->zero_rgb && c->zero_a && !c->sub_rgb && !c->sub_a && c->add_rgb == 1 && c->add_a)
        c->kind = COMB_LOCAL;
    /* factor is exactly 256 and nothing is added: result = other */
    else if (!c->zero_rgb && !c->zero_a && !c->sub_rgb && !c->sub_a && c->msel_rgb == 0 && c->msel_a == 0 &&
             c->rev_rgb && c->rev_a && (c->add_rgb == 0 || c->add_rgb == 3) && !c->add_a)
        c->kind = COMB_OTHER;
}

/* x4/x5: the unit-specific factor sources for mselect 4 and 5 */
static rgba_t comb_apply(const comb_t* c, rgba_t local, rgba_t other, rgba_t x4, rgba_t x5) {
    rgba_t b = other, f, add;
    if (c->zero_rgb) b.r = b.g = b.b = 0;
    if (c->zero_a) b.a = 0;
    if (c->sub_rgb) { b.r -= local.r; b.g -= local.g; b.b -= local.b; }
    if (c->sub_a) b.a -= local.a;
    switch (c->msel_rgb) {
        case 1: f = local; break;
        case 2: f = rgba_all(other.a); break;
        case 3: f = rgba_all(local.a); break;
        case 4: f = x4; break;
        case 5: f = x5; break;
        default: f = rgba_all(0); break;
    }
    switch (c->msel_a) {
        case 1: case 3: f.a = local.a; break;
        case 2: f.a = other.a; break;
        case 4: f.a = x4.a; break;
        case 5: f.a = x5.a; break;
        default: f.a = 0; break;
    }
    f.r = (f.r ^ c->rev_rgb) + 1; f.g = (f.g ^ c->rev_rgb) + 1; f.b = (f.b ^ c->rev_rgb) + 1;
    f.a = (f.a ^ c->rev_a) + 1;
    switch (c->add_rgb) {
        case 1: add = local; break;
        case 2: add = rgba_all(local.a); break;
        default: add = rgba_all(0); break;
    }
    add.a = c->add_a ? local.a : 0;
    b = scale_add_clamp(b, f, add);
    b.r ^= c->inv_rgb; b.g ^= c->inv_rgb; b.b ^= c->inv_rgb; b.a ^= c->inv_a;
    return b;
}

static rgba_t combine_texture(const tmu_t* t, const comb_t* c, rgba_t c_local, rgba_t c_other, int32_t lod) {
    if (c->kind == COMB_LOCAL) return c_local;
    int32_t detail = 0;
    if (c->msel_rgb == 4 || c->msel_a == 4) {
        if (t->detailbias > lod) {
            uint32_t tmp = (uint8_t)(((t->detailbias - lod) << t->detailscale) >> 8);
            detail = (int32_t)(tmp > (uint32_t)t->detailmax ? (uint32_t)t->detailmax : tmp);
        }
    }
    return comb_apply(c, c_local, c_other, rgba_all(detail), rgba_all(lod & 0xff));
}

/* ---------------------------------------------------------------------
 * Pixel pipeline
 * --------------------------------------------------------------------- */
static int32_t compute_wfloat(int64_t iterw) {
    int exp = clz64((uint64_t)iterw) - 16;
    if (exp < 0) return 0x0000;
    if (exp >= 16) return 0xffff;
    return ((exp << 12) | (int32_t)((iterw >> (35 - exp)) ^ 0x1fff)) + 1;
}

static int32_t clamped_z(int32_t iterz, uint32_t fbzcp) {
    if (BIT(fbzcp, 28, 1)) return clampi(iterz >> 12, 0, 0xffff);
    uint32_t r = (uint32_t)iterz >> 12;
    if (r == 0xfffff) return 0;
    if (r == 0x10000) return 0xffff;
    return (int32_t)(r & 0xffff);
}

static int32_t clamped_w(int64_t iterw, uint32_t fbzcp) {
    if (BIT(fbzcp, 28, 1)) return clampi((int32_t)(iterw >> 48), 0, 0xff);
    uint32_t r = (uint32_t)((uint64_t)iterw >> 48);
    if (r == 0xffff) return 0;
    if (r == 0x100) return 0xff;
    return (int32_t)(r & 0xff);
}

static int32_t clamp_iter(int32_t v, int clamp) {      /* v is 12.20 */
    v >>= 20;
    if (clamp) return clamp8(v);
    v &= 0xfff;
    if (v == 0xfff) return 0;
    if (v == 0x100) return 0xff;
    return v & 0xff;
}

static int depth_test(uint32_t fbzmode, int32_t dst, int32_t src) {
    switch (BIT(fbzmode, 5, 3)) {
        case 0: return 0;
        case 1: return src < dst;
        case 2: return src == dst;
        case 3: return src <= dst;
        case 4: return src > dst;
        case 5: return src != dst;
        case 6: return src >= dst;
        default: return 1;
    }
}

static int alpha_test(uint32_t alphamode, int32_t a, int32_t ref) {
    switch (BIT(alphamode, 1, 3)) {
        case 0: return 0;
        case 1: return a < ref;
        case 2: return a == ref;
        case 3: return a <= ref;
        case 4: return a > ref;
        case 5: return a != ref;
        case 6: return a >= ref;
        default: return 1;
    }
}

/* chromaRange is Voodoo 2 only; on Voodoo 1 only the exact match exists */
static int chroma_pass(rgba_t c, uint32_t key) { return ((rgba_pack(c) ^ key) & 0xffffff) != 0; }

static int stipple_test(uint32_t fbzmode, int32_t x, int32_t scry, uint32_t* stipple) {
    if (!BIT(fbzmode, 12, 1)) {
        *stipple = (*stipple >> 1) | (*stipple << 31);
        return (int32_t)*stipple < 0;
    }
    return BIT(*stipple, ((scry & 3) << 3) | (~x & 7), 1);
}

static int32_t compute_depthval(uint32_t fbzmode, uint32_t fbzcp, uint32_t zacolor, int32_t wfloat, int32_t iterz) {
    int32_t r = BIT(fbzmode, 3, 1) ? wfloat : clamped_z(iterz, fbzcp);
    if (BIT(fbzmode, 16, 1)) r = clampi(r + (int16_t)zacolor, 0, 0xffff);
    return r;
}

static void apply_fog(voodoo_state_t* s, rgba_t* color, uint32_t fogcolor, uint32_t depthbias, uint32_t fbzmode,
                      uint32_t fogmode, uint32_t fbzcp, int32_t wfloat, int32_t iterz, int64_t iterw, int32_t itera) {
    rgba_t fc = rgba_u32(fogcolor);
    if (BIT(fogmode, 5, 1)) {
        if (!BIT(fogmode, 2, 1)) { fc.r += color->r; fc.g += color->g; fc.b += color->b; fc = rgba_clamp8(fc); }
    } else {
        int32_t fogblend = 0;
        if (BIT(fogmode, 1, 1)) fc = rgba_all(0);
        if (!BIT(fogmode, 2, 1)) { fc.r -= color->r; fc.g -= color->g; fc.b -= color->b; fc.a -= color->a; }
        switch (BIT(fogmode, 3, 2)) {
            case 0: {
                int32_t fd = wfloat;
                if (BIT(fbzmode, 16, 1)) fd = clampi(fd + (int16_t)depthbias, 0, 0xffff);
                int32_t delta = s->fogdelta[fd >> 10];
                int32_t dv = (delta & 0xff) * ((fd >> 2) & 0xff);
                dv >>= 6;
                dv >>= 4;
                fogblend = s->fogblend[fd >> 10] + dv;
                break;
            }
            case 1: fogblend = itera; break;
            case 2: fogblend = clamped_z(iterz, fbzcp) >> 8; break;
            case 3: fogblend = clamped_w(iterw, fbzcp); break;
        }
        fogblend++;
        fogblend = (int16_t)fogblend;
        fc.r = clamp8((fc.r * fogblend) >> 8); fc.g = clamp8((fc.g * fogblend) >> 8);
        fc.b = clamp8((fc.b * fogblend) >> 8);
        if (!BIT(fogmode, 2, 1)) { fc.r += color->r; fc.g += color->g; fc.b += color->b; fc = rgba_clamp8(fc); }
    }
    fc.a = color->a;
    *color = fc;
}

static rgba_t blend_factor(int mode, int32_t sa, int32_t da, rgba_t other, rgba_t prefog, int is_src) {
    rgba_t f;
    int32_t ta;
    switch (mode) {
        case 1: f = rgba_all(sa + 1); break;
        case 2: f = other; f.a++; f.r++; f.g++; f.b++; break;
        case 3: f = rgba_all(da + 1); break;
        case 4: f = rgba_all(256); break;
        case 5: f = rgba_all(0x100 - sa); break;
        case 6: f.a = 0x100 - other.a; f.r = 0x100 - other.r; f.g = 0x100 - other.g; f.b = 0x100 - other.b; break;
        case 7: f = rgba_all(0x100 - da); break;
        case 15:
            if (is_src) { ta = 0x100 - da; if (sa < ta) ta = sa; f = rgba_all(ta + 1); }
            else { f = prefog; f.a++; f.r++; f.g++; f.b++; }
            break;
        default: f = rgba_all(0); break;
    }
    return f;
}

static void alpha_blend(rgba_t* color, uint32_t fbzmode, uint32_t alphamode, int32_t x, const dither_t* d,
                        uint16_t dpix, const uint16_t* depth, rgba_t prefog) {
    rgba_t dst = rgba_u32(t_rgb565[dpix]);
    int32_t da = 0xff;
    if (BIT(fbzmode, 18, 1)) dst.a = da = depth[x];
    if (BIT(fbzmode, 19, 1)) {
        int32_t dith = d->raw ? (15 - d->raw[x & 3]) >> 1 : 0;
        dst.r += dith; dst.g += dith >> 1; dst.b += dith;
    }
    int32_t sa = color->a;
    if ((alphamode & 0xff00) == 0x5100) {              /* src*a + dst*(1-a): the common case */
        int32_t fs = sa + 1, fd = 0x100 - sa;
        int32_t as = BIT(alphamode, 16, 4) == 4 ? 256 : 0, ad = BIT(alphamode, 20, 4) == 4 ? 256 : 0;
        color->a = clamp8((color->a * as + dst.a * ad) >> 8);
        color->r = clamp8((color->r * fs + dst.r * fd) >> 8);
        color->g = clamp8((color->g * fs + dst.g * fd) >> 8);
        color->b = clamp8((color->b * fs + dst.b * fd) >> 8);
        return;
    }
    rgba_t ss = blend_factor((int)BIT(alphamode, 8, 4), sa, da, dst, prefog, 1);
    ss.a = BIT(alphamode, 16, 4) == 4 ? 256 : 0;
    rgba_t ds = blend_factor((int)BIT(alphamode, 12, 4), sa, da, *color, prefog, 0);
    ds.a = BIT(alphamode, 20, 4) == 4 ? 256 : 0;
    color->a = clamp8((color->a * ss.a + dst.a * ds.a) >> 8);
    color->r = clamp8((color->r * ss.r + dst.r * ds.r) >> 8);
    color->g = clamp8((color->g * ss.g + dst.g * ds.g) >> 8);
    color->b = clamp8((color->b * ss.b + dst.b * ds.b) >> 8);
}

enum { ST_IN, ST_CHROMA, ST_ZFUNC, ST_AFUNC, ST_OUT };

static void write_pixel(uint32_t* st, uint32_t fbzmode, const dither_t* d, uint16_t* dest, uint16_t* depth,
                        int32_t x, rgba_t c, int32_t depthval) {
    if (BIT(fbzmode, 9, 1)) dest[x] = dither_pixel(d, x, c.r, c.g, c.b);
    if (BIT(fbzmode, 10, 1)) depth[x] = (uint16_t)(BIT(fbzmode, 18, 1) ? c.a : depthval);
    st[ST_OUT]++;
}

/* colour combine unit; returns 0 if the pixel is rejected */
static int combine_color(uint32_t* st, rgba_t* color, const poly_t* p, rgba_t texel, int32_t iterz, int64_t iterw) {
    uint32_t cp = p->fbzcp, fm = p->fbzmode;
    rgba_t c_other, c_local;
    switch (BIT(cp, 0, 2)) {
        case 0: c_other = *color; break;
        case 1: c_other = texel; break;
        case 2: c_other = rgba_u32(p->color1); break;
        default: c_other = rgba_all(0); break;
    }
    if (BIT(fm, 1, 1) && !chroma_pass(c_other, p->chromakey)) { st[ST_CHROMA]++; return 0; }
    switch (BIT(cp, 2, 2)) {
        case 0: c_other.a = color->a; break;
        case 1: c_other.a = texel.a; break;
        case 2: c_other.a = (int32_t)(p->color1 >> 24); break;
        default: c_other.a = 0; break;
    }
    if (BIT(fm, 13, 1) && !(c_other.a & 1)) { st[ST_AFUNC]++; return 0; }
    if (p->cc.kind == COMB_OTHER) { *color = c_other; return 1; }

    if (!BIT(cp, 7, 1)) c_local = BIT(cp, 4, 1) ? rgba_u32(p->color0) : *color;
    else c_local = (texel.a & 0x80) ? rgba_u32(p->color0) : *color;
    switch (BIT(cp, 5, 2)) {
        case 0: c_local.a = color->a; break;
        case 1: c_local.a = (int32_t)(p->color0 >> 24); break;
        case 2: c_local.a = (uint8_t)(clamped_z(iterz, cp) >> 8); break;
        case 3: c_local.a = (uint8_t)clamped_w(iterw, cp); break;
    }
    *color = comb_apply(&p->cc, c_local, c_other, rgba_alpha(texel), texel);
    return 1;
}

/* one scanline of a triangle */
static void raster_scanline(voodoo_state_t* s, const poly_t* pp, int32_t y, int32_t startx, int32_t stopx) {
    const poly_t lp = *pp, *p = &lp;
    uint32_t fm = p->fbzmode, cp = p->fbzcp, am = p->alphamode, fog = p->fogmode;
    int32_t scry = BIT(fm, 17, 1) ? s->yorigin - y : y;
    uint32_t stipple = p->stipple;

    s->fbi.reg[r_fbiPixelsIn] += stopx - startx;
    uint32_t st[5] = { 0, 0, 0, 0, 0 };
    if (BIT(fm, 0, 1)) {
        if (scry < p->cliptop || scry >= p->clipbottom) return;
        if (startx >= p->clipright) return;
        if (stopx > p->clipright) stopx = p->clipright;
        if (startx < p->clipleft) startx = p->clipleft;
    }
    if (!clip_row(s, p->dest, scry, &startx, &stopx)) return;
    uint16_t* dest = p->dest + (int64_t)scry * s->rowpixels;
    uint16_t* depth;
    if (p->depth) {
        if (!clip_row(s, p->depth, scry, &startx, &stopx)) return;
        depth = p->depth + (int64_t)scry * s->rowpixels;
    } else
        depth = s->dummy_depth - startx;          /* triple-buffered: no aux buffer */
    if (stopx - startx > 4096) stopx = startx + 4096;

    int32_t dx = startx - (p->ax >> 4), dy = y - (p->ay >> 4);
    int32_t iterr = (p->startr + dy * p->drdy + dx * p->drdx) << 8;
    int32_t iterg = (p->startg + dy * p->dgdy + dx * p->dgdx) << 8;
    int32_t iterb = (p->startb + dy * p->dbdy + dx * p->dbdx) << 8;
    int32_t itera = (p->starta + dy * p->dady + dx * p->dadx) << 8;
    int32_t iterz = p->startz + dy * p->dzdy + dx * p->dzdx;
    int64_t iterw = (p->startw + dy * p->dwdy + dx * p->dwdx) << 16;
    int64_t iterw_d = p->dwdx << 16;
    int32_t dr = p->drdx << 8, dg = p->dgdx << 8, db = p->dbdx << 8, da = p->dadx << 8;
    double iters0 = 0, itert0 = 0, iterw0 = 0, ds0 = 0, dt0 = 0, dw0 = 0;
    int32_t lodbase0 = 0;
    if (p->tex0) {
        ds0 = (double)p->ds0dx; dt0 = (double)p->dt0dx; dw0 = (double)p->dw0dx;
        iters0 = (double)(p->starts0 + dy * p->ds0dy + dx * p->ds0dx);
        itert0 = (double)(p->startt0 + dy * p->dt0dy + dx * p->dt0dx);
        iterw0 = (double)(p->startw0 + dy * p->dw0dy + dx * p->dw0dx);
        lodbase0 = compute_lodbase(p->ds0dx, p->ds0dy, p->dt0dx, p->dt0dy);
    }
    int clamp = (int)BIT(cp, 28, 1);
    dither_t d = dither_make(scry, fm);

    for (int32_t x = startx; x < stopx; x++) {
        do {
            if (BIT(fm, 2, 1) && !stipple_test(fm, x, scry, &stipple)) break;
            int32_t wfloat = compute_wfloat(iterw);
            int32_t depthval = compute_depthval(fm, cp, p->zacolor, wfloat, iterz);
            if (BIT(fm, 4, 1) && !depth_test(fm, depth[x], BIT(fm, 20, 1) ? (uint16_t)p->zacolor : depthval)) {
                st[ST_ZFUNC]++;
                break;
            }
            rgba_t texel = rgba_all(0);
            if (p->tex0) {
                if (!p->send_config) {
                    int32_t lod0 = lodbase0;
                    rgba_t t0 = fetch_texel(p->tex0, p->texmode0, &d, x, iters0, itert0, iterw0, &lod0);
                    texel = combine_texture(p->tex0, &p->tc, t0, texel, lod0);
                } else
                    texel = rgba_u32(0x11);      /* TMU configuration byte: one TMU */
            }
            rgba_t color = { clamp_iter(itera, clamp), clamp_iter(iterr, clamp), clamp_iter(iterg, clamp), clamp_iter(iterb, clamp) };
            int32_t itera8 = color.a;
            if (!combine_color(st, &color, p, texel, iterz, iterw)) break;
            if (BIT(am, 0, 1) && !alpha_test(am, color.a, (int32_t)p->alpharef)) { st[ST_AFUNC]++; break; }
            rgba_t prefog = color;
            if (BIT(fog, 0, 1)) apply_fog(s, &color, p->fogcolor, p->zacolor, fm, fog, cp, wfloat, iterz, iterw, itera8);
            if (BIT(am, 4, 1)) alpha_blend(&color, fm, am, x, &d, dest[x], depth, prefog);
            write_pixel(st, fm, &d, dest, depth, x, color, depthval);
        } while (0);
        iterr += dr; iterg += dg; iterb += db; itera += da;
        iterz += p->dzdx;
        iterw += iterw_d;
        iters0 += ds0; itert0 += dt0; iterw0 += dw0;
    }
    for (int i = 1; i < 5; i++) s->fbi.reg[r_fbiPixelsIn + i] += st[i];
}

static int32_t round_coord(float v) {
    if (v >= 2147483647.0f) return 0x7fffffff;
    float ip = floorf(v);
    if (ip < -2147483648.0f) return (int32_t)0x80000000;
    return (int32_t)ip + ((v - ip) > 0.5f ? 1 : 0);
}

static int texture_used(uint32_t cp) {
    return BIT(cp, 0, 2) == 1 || BIT(cp, 2, 2) == 1 || BIT(cp, 7, 1) ||
           BIT(cp, 10, 3) == 4 || BIT(cp, 10, 3) == 5 || BIT(cp, 19, 3) == 4;
}

static void triangle(voodoo_state_t* s) {
    vregs_t* f = &s->fbi;
    tmu_t* t = &s->tmu;
    poly_t p;
    memset(&p, 0, sizeof(p));
    p.fbzcp = f->reg[r_fbzColorPath]; p.fbzmode = f->reg[r_fbzMode];
    p.alphamode = f->reg[r_alphaMode]; p.fogmode = f->reg[r_fogMode];
    p.dest = draw_buffer_indirect(s, (int)BIT(p.fbzmode, 14, 2));
    f->reg[r_fbiTrianglesOut]++;
    if (!p.dest) return;
    p.depth = aux_buffer(s);
    p.clipleft = (int32_t)BIT(f->reg[r_clipLeftRight], 16, 10); p.clipright = (int32_t)BIT(f->reg[r_clipLeftRight], 0, 10);
    p.cliptop = (int32_t)BIT(f->reg[r_clipLowYHighY], 16, 10); p.clipbottom = (int32_t)BIT(f->reg[r_clipLowYHighY], 0, 10);

    p.ax = (int16_t)f->reg[r_vertexAx]; p.ay = (int16_t)f->reg[r_vertexAy];
    p.startr = sext(f->reg[r_startR], 24); p.startg = sext(f->reg[r_startG], 24);
    p.startb = sext(f->reg[r_startB], 24); p.starta = sext(f->reg[r_startA], 24);
    p.startz = (int32_t)f->reg[r_startZ]; p.startw = f->startw;
    p.drdx = sext(f->reg[r_dRdX], 24); p.dgdx = sext(f->reg[r_dGdX], 24); p.dbdx = sext(f->reg[r_dBdX], 24);
    p.dadx = sext(f->reg[r_dAdX], 24); p.dzdx = (int32_t)f->reg[r_dZdX]; p.dwdx = f->dwdx;
    p.drdy = sext(f->reg[r_dRdY], 24); p.dgdy = sext(f->reg[r_dGdY], 24); p.dbdy = sext(f->reg[r_dBdY], 24);
    p.dady = sext(f->reg[r_dAdY], 24); p.dzdy = (int32_t)f->reg[r_dZdY]; p.dwdy = f->dwdy;

    if (BIT(p.fbzcp, 26, 1)) {                        /* subpixel adjust, done in the registers */
        int32_t dx = 8 - (p.ax & 15), dy = 8 - (p.ay & 15);
        f->reg[r_startR] = (uint32_t)(p.startr += (dy * p.drdy + dx * p.drdx) >> 4);
        f->reg[r_startG] = (uint32_t)(p.startg += (dy * p.dgdy + dx * p.dgdx) >> 4);
        f->reg[r_startB] = (uint32_t)(p.startb += (dy * p.dbdy + dx * p.dbdx) >> 4);
        f->reg[r_startA] = (uint32_t)(p.starta += (dy * p.dady + dx * p.dadx) >> 4);
        f->reg[r_startZ] = (uint32_t)(p.startz += (dy * p.dzdy + dx * p.dzdx) >> 4);
        f->startw = p.startw += (dy * p.dwdy + dx * p.dwdx) >> 4;
        t->r.startw += (dy * t->r.dwdy + dx * t->r.dwdx) >> 4;
        t->r.starts += (dy * t->r.dsdy + dx * t->r.dsdx) >> 4;
        t->r.startt += (dy * t->r.dtdy + dx * t->r.dtdx) >> 4;
    }

    if (!BIT(f->reg[r_fbiInit3], 6, 1) && BIT(t->r.reg[r_tLOD], 0, 6) <= 32 && texture_used(p.fbzcp)) {
        p.tex0 = t;
        p.texmode0 = t->r.reg[r_textureMode];
        p.send_config = (int)BIT(t->r.reg[r_trexInit1], 18, 1);
        comb_decode(&p.tc, p.texmode0 >> 12);
        p.starts0 = t->r.starts; p.startt0 = t->r.startt; p.startw0 = t->r.startw;
        p.ds0dx = t->r.dsdx; p.dt0dx = t->r.dtdx; p.dw0dx = t->r.dwdx;
        p.ds0dy = t->r.dsdy; p.dt0dy = t->r.dtdy; p.dw0dy = t->r.dwdy;
        prepare_texture(t);
    }
    p.color0 = f->reg[r_color0]; p.color1 = f->reg[r_color1];
    p.chromakey = f->reg[r_chromaKey]; p.fogcolor = f->reg[r_fogColor];
    p.zacolor = f->reg[r_zaColor]; p.stipple = f->reg[r_stipple];
    p.alpharef = BIT(f->reg[r_alphaMode], 24, 8);
    comb_decode(&p.cc, p.fbzcp >> 8);

    /* scan conversion: vertices are 12.4 */
    float vx[3] = { (int16_t)f->reg[r_vertexAx] / 16.0f, (int16_t)f->reg[r_vertexBx] / 16.0f, (int16_t)f->reg[r_vertexCx] / 16.0f };
    float vy[3] = { (int16_t)f->reg[r_vertexAy] / 16.0f, (int16_t)f->reg[r_vertexBy] / 16.0f, (int16_t)f->reg[r_vertexCy] / 16.0f };
    int i1 = 0, i2 = 1, i3 = 2, tmp;
    if (vy[i2] < vy[i1]) { tmp = i1; i1 = i2; i2 = tmp; }
    if (vy[i3] < vy[i2]) { tmp = i2; i2 = i3; i3 = tmp; if (vy[i2] < vy[i1]) { tmp = i1; i1 = i2; i2 = tmp; } }
    float x1 = vx[i1], y1 = vy[i1], x2 = vx[i2], y2 = vy[i2], x3 = vx[i3], y3 = vy[i3];
    int32_t v1y = round_coord(y1), v3y = round_coord(y3);
    if (v1y < -4096) v1y = -4096;
    if (v3y > 4096) v3y = 4096;
    float d12 = (y2 == y1) ? 0.0f : (x2 - x1) / (y2 - y1);
    float d13 = (y3 == y1) ? 0.0f : (x3 - x1) / (y3 - y1);
    float d23 = (y3 == y2) ? 0.0f : (x3 - x2) / (y3 - y2);
    for (int32_t y = v1y; y < v3y; y++) {
        float fy = (float)y + 0.5f;
        float sx = x1 + (fy - y1) * d13;
        float ex = (fy < y2) ? x1 + (fy - y1) * d12 : x2 + (fy - y2) * d23;
        int32_t is = round_coord(sx), ie = round_coord(ex);
        if (is > ie) { tmp = is; is = ie; ie = tmp; }
        if (is < -4096) is = -4096;
        if (ie > 4096) ie = 4096;
        if (is < ie) raster_scanline(s, &p, y, is, ie);
    }
}

static void fastfill(voodoo_state_t* s) {
    vregs_t* f = &s->fbi;
    uint32_t fm = f->reg[r_fbzMode];
    uint16_t* dest = draw_buffer_indirect(s, (int)BIT(fm, 14, 2));
    uint16_t* depth = aux_buffer(s);
    if (!dest || (!BIT(fm, 9, 1) && !BIT(fm, 10, 1))) return;
    int32_t x0 = (int32_t)BIT(f->reg[r_clipLeftRight], 16, 10), x1 = (int32_t)BIT(f->reg[r_clipLeftRight], 0, 10);
    int32_t y0 = (int32_t)BIT(f->reg[r_clipLowYHighY], 16, 10), y1 = (int32_t)BIT(f->reg[r_clipLowYHighY], 0, 10);
    if (y1 < y0) { int32_t t = y0; y0 = y1; y1 = t; }
    if (x0 > x1) return;
    uint16_t pat[16];
    uint32_t c1 = f->reg[r_color1];
    for (int y = 0; y < 4; y++) {
        dither_t d = dither_make(y, fm);
        for (int x = 0; x < 4; x++)
            pat[y * 4 + x] = dither_pixel(&d, x, (c1 >> 16) & 0xff, (c1 >> 8) & 0xff, c1 & 0xff);
    }
    uint16_t z = (uint16_t)f->reg[r_zaColor];
    for (int32_t y = y0; y < y1; y++) {
        int32_t scry = BIT(fm, 17, 1) ? s->yorigin - y : y;
        if (BIT(fm, 9, 1)) {
            int32_t sx = x0, ex = x1;
            if (clip_row(s, dest, scry, &sx, &ex)) {
                uint16_t* row = dest + (int64_t)scry * s->rowpixels;
                const uint16_t* dr = &pat[(y & 3) * 4];
                for (int32_t x = sx; x < ex; x++) row[x] = dr[x & 3];
            }
            f->reg[r_fbiPixelsOut] += x1 - x0;
        }
        if (BIT(fm, 10, 1) && depth) {
            int32_t sx = x0, ex = x1;
            if (clip_row(s, depth, scry, &sx, &ex)) {
                uint16_t* row = depth + (int64_t)scry * s->rowpixels;
                for (int32_t x = sx; x < ex; x++) row[x] = z;
            }
        }
    }
}

/* LFB writes with the pixel pipeline enabled */
static void pixel_pipeline(voodoo_state_t* s, uint16_t* dest, uint16_t* depth, int32_t x, int32_t scry, uint32_t src, uint16_t sz) {
    vregs_t* f = &s->fbi;
    uint32_t fm = f->reg[r_fbzMode];
    f->reg[r_fbiPixelsIn]++;
    if (BIT(fm, 0, 1)) {
        if (x < (int32_t)BIT(f->reg[r_clipLeftRight], 16, 10) || x >= (int32_t)BIT(f->reg[r_clipLeftRight], 0, 10) ||
            scry < (int32_t)BIT(f->reg[r_clipLowYHighY], 16, 10) || scry >= (int32_t)BIT(f->reg[r_clipLowYHighY], 0, 10))
            return;
    }
    if (BIT(fm, 2, 1)) {
        uint32_t st = f->reg[r_stipple];
        if (!stipple_test(fm, x, scry, &st)) return;
    }
    int32_t depthval = sz;
    if (BIT(fm, 4, 1) && !depth_test(fm, depth[x], BIT(fm, 20, 1) ? (uint16_t)f->reg[r_zaColor] : depthval)) {
        f->reg[r_fbiZfuncFail]++;
        return;
    }
    rgba_t color = rgba_u32(src);
    if (BIT(fm, 1, 1) && !chroma_pass(color, f->reg[r_chromaKey])) { f->reg[r_fbiChromaFail]++; return; }
    if (BIT(fm, 13, 1) && !(color.a & 1)) { f->reg[r_fbiAfuncFail]++; return; }
    uint32_t am = f->reg[r_alphaMode];
    if (BIT(am, 0, 1) && !alpha_test(am, color.a, (int32_t)BIT(am, 24, 8))) { f->reg[r_fbiAfuncFail]++; return; }
    uint32_t fog = f->reg[r_fogMode];
    dither_t d = dither_make(scry, fm);
    rgba_t prefog = color;
    if (BIT(fog, 0, 1)) {
        int32_t iterz = sz << 12;
        int64_t iterw = BIT(f->reg[r_lfbMode], 14, 1) ? (uint32_t)(f->reg[r_zaColor] << 16) : (uint32_t)(sz << 16);
        apply_fog(s, &color, f->reg[r_fogColor], f->reg[r_zaColor], fm, fog, f->reg[r_fbzColorPath], depthval, iterz, iterw, 0);
    }
    if (BIT(am, 4, 1)) alpha_blend(&color, fm, am, x, &d, dest[x], depth, prefog);
    uint32_t st[5] = { 0, 0, 0, 0, 0 };
    write_pixel(st, fm, &d, dest, depth, x, color, depthval);
    f->reg[r_fbiPixelsOut] += st[ST_OUT];
}

/* ---------------------------------------------------------------------
 * LFB and texture memory
 * --------------------------------------------------------------------- */
#define LFB_RGB0 0x01
#define LFB_ALPHA0 0x02
#define LFB_DEPTH0 0x04
#define LFB_DEPTHMSW0 0x08
#define LFB_RGB1 0x10
#define LFB_ALPHA1 0x20
#define LFB_DEPTH1 0x40

static uint32_t expand_lfb(voodoo_state_t* s, uint32_t mode, uint32_t d, uint32_t col[2], uint16_t dep[2]) {
    uint32_t za = s->fbi.reg[r_zaColor], a = za >> 24;
    dep[0] = dep[1] = (uint16_t)za;
    switch (16 * BIT(mode, 9, 2) + BIT(mode, 0, 4)) {
        case 16*0 + 0: case 16*2 + 0:
            col[0] = argb_expand(d, 0, 0, 5, 11, 6, 5, 5, 0, a); col[1] = argb_expand(d, 0, 0, 5, 27, 6, 21, 5, 16, a);
            return LFB_RGB0 | LFB_RGB1;
        case 16*1 + 0: case 16*3 + 0:
            col[0] = argb_expand(d, 0, 0, 5, 0, 6, 5, 5, 11, a); col[1] = argb_expand(d, 0, 0, 5, 16, 6, 21, 5, 27, a);
            return LFB_RGB0 | LFB_RGB1;
        case 16*0 + 1:
            col[0] = argb_expand(d, 0, 0, 5, 10, 5, 5, 5, 0, a); col[1] = argb_expand(d, 0, 0, 5, 26, 5, 21, 5, 16, a);
            return LFB_RGB0 | LFB_RGB1;
        case 16*1 + 1:
            col[0] = argb_expand(d, 0, 0, 5, 0, 5, 5, 5, 10, a); col[1] = argb_expand(d, 0, 0, 5, 16, 5, 21, 5, 26, a);
            return LFB_RGB0 | LFB_RGB1;
        case 16*2 + 1:
            col[0] = argb_expand(d, 0, 0, 5, 11, 5, 6, 5, 1, a); col[1] = argb_expand(d, 0, 0, 5, 27, 5, 22, 5, 17, a);
            return LFB_RGB0 | LFB_RGB1;
        case 16*3 + 1:
            col[0] = argb_expand(d, 0, 0, 5, 1, 5, 6, 5, 11, a); col[1] = argb_expand(d, 0, 0, 5, 17, 5, 22, 5, 27, a);
            return LFB_RGB0 | LFB_RGB1;
        case 16*0 + 2:
            col[0] = argb_expand(d, 1, 15, 5, 10, 5, 5, 5, 0, 0); col[1] = argb_expand(d, 1, 31, 5, 26, 5, 21, 5, 16, 0);
            return LFB_RGB0 | LFB_ALPHA0 | LFB_RGB1 | LFB_ALPHA1;
        case 16*1 + 2:
            col[0] = argb_expand(d, 1, 15, 5, 0, 5, 5, 5, 10, 0); col[1] = argb_expand(d, 1, 31, 5, 16, 5, 21, 5, 26, 0);
            return LFB_RGB0 | LFB_ALPHA0 | LFB_RGB1 | LFB_ALPHA1;
        case 16*2 + 2:
            col[0] = argb_expand(d, 1, 0, 5, 11, 5, 6, 5, 1, 0); col[1] = argb_expand(d, 1, 16, 5, 27, 5, 22, 5, 17, 0);
            return LFB_RGB0 | LFB_ALPHA0 | LFB_RGB1 | LFB_ALPHA1;
        case 16*3 + 2:
            col[0] = argb_expand(d, 1, 0, 5, 1, 5, 6, 5, 11, 0); col[1] = argb_expand(d, 1, 16, 5, 17, 5, 22, 5, 27, 0);
            return LFB_RGB0 | LFB_ALPHA0 | LFB_RGB1 | LFB_ALPHA1;
        case 16*0 + 4: col[0] = argb_expand(d, 0, 0, 8, 16, 8, 8, 8, 0, a); return LFB_RGB0;
        case 16*1 + 4: col[0] = argb_expand(d, 0, 0, 8, 0, 8, 8, 8, 16, a); return LFB_RGB0;
        case 16*2 + 4: col[0] = argb_expand(d, 0, 0, 8, 24, 8, 16, 8, 8, a); return LFB_RGB0;
        case 16*3 + 4: col[0] = argb_expand(d, 0, 0, 8, 8, 8, 16, 8, 24, a); return LFB_RGB0;
        case 16*0 + 5: col[0] = argb_expand(d, 8, 24, 8, 16, 8, 8, 8, 0, 0); return LFB_RGB0 | LFB_ALPHA0;
        case 16*1 + 5: col[0] = argb_expand(d, 8, 24, 8, 0, 8, 8, 8, 16, 0); return LFB_RGB0 | LFB_ALPHA0;
        case 16*2 + 5: col[0] = argb_expand(d, 8, 0, 8, 24, 8, 16, 8, 8, 0); return LFB_RGB0 | LFB_ALPHA0;
        case 16*3 + 5: col[0] = argb_expand(d, 8, 0, 8, 8, 8, 16, 8, 24, 0); return LFB_RGB0 | LFB_ALPHA0;
        case 16*0 + 12: case 16*2 + 12:
            col[0] = argb_expand(d, 0, 0, 5, 11, 6, 5, 5, 0, a); dep[0] = (uint16_t)(d >> 16); return LFB_RGB0 | LFB_DEPTHMSW0;
        case 16*1 + 12: case 16*3 + 12:
            col[0] = argb_expand(d, 0, 0, 5, 0, 6, 5, 5, 11, a); dep[0] = (uint16_t)(d >> 16); return LFB_RGB0 | LFB_DEPTHMSW0;
        case 16*0 + 13: col[0] = argb_expand(d, 0, 0, 5, 10, 5, 5, 5, 0, a); dep[0] = (uint16_t)(d >> 16); return LFB_RGB0 | LFB_DEPTHMSW0;
        case 16*1 + 13: col[0] = argb_expand(d, 0, 0, 5, 0, 5, 5, 5, 10, a); dep[0] = (uint16_t)(d >> 16); return LFB_RGB0 | LFB_DEPTHMSW0;
        case 16*2 + 13: col[0] = argb_expand(d, 0, 0, 5, 11, 5, 6, 5, 1, a); dep[0] = (uint16_t)(d >> 16); return LFB_RGB0 | LFB_DEPTHMSW0;
        case 16*3 + 13: col[0] = argb_expand(d, 0, 0, 5, 1, 5, 6, 5, 11, a); dep[0] = (uint16_t)(d >> 16); return LFB_RGB0 | LFB_DEPTHMSW0;
        case 16*0 + 14: col[0] = argb_expand(d, 1, 15, 5, 10, 5, 5, 5, 0, 0); dep[0] = (uint16_t)(d >> 16); return LFB_RGB0 | LFB_ALPHA0 | LFB_DEPTHMSW0;
        case 16*1 + 14: col[0] = argb_expand(d, 1, 15, 5, 0, 5, 5, 5, 10, 0); dep[0] = (uint16_t)(d >> 16); return LFB_RGB0 | LFB_ALPHA0 | LFB_DEPTHMSW0;
        case 16*2 + 14: col[0] = argb_expand(d, 1, 0, 5, 11, 5, 6, 5, 1, 0); dep[0] = (uint16_t)(d >> 16); return LFB_RGB0 | LFB_ALPHA0 | LFB_DEPTHMSW0;
        case 16*3 + 14: col[0] = argb_expand(d, 1, 0, 5, 1, 5, 6, 5, 11, 0); dep[0] = (uint16_t)(d >> 16); return LFB_RGB0 | LFB_ALPHA0 | LFB_DEPTHMSW0;
        case 16*0 + 15: case 16*1 + 15: case 16*2 + 15: case 16*3 + 15:
            dep[0] = (uint16_t)d; dep[1] = (uint16_t)(d >> 16);
            return LFB_DEPTH0 | LFB_DEPTH1;
        default:
            return 0;
    }
}

static void lfb_w(voodoo_state_t* s, uint32_t offset, uint32_t data, uint32_t mem_mask) {
    uint32_t mode = s->fbi.reg[r_lfbMode];
    if (BIT(mode, 12, 1)) { data = bswap32(data); mem_mask = bswap32(mem_mask); }
    if (BIT(mode, 11, 1)) { data = (data << 16) | (data >> 16); mem_mask = (mem_mask << 16) | (mem_mask >> 16); }
    uint32_t col[2] = { 0, 0 };
    uint16_t dep[2];
    uint32_t mask = expand_lfb(s, mode, data, col, dep);
    if (mask & 0x70) offset <<= 1;
    if (!(mem_mask & 0x0000ffff)) mask &= ~(uint32_t)(0x0f - LFB_DEPTHMSW0);
    if (!(mem_mask & 0xffff0000)) mask &= ~(uint32_t)(0x70 + LFB_DEPTHMSW0);
    int32_t x = (int32_t)(offset & 1023), y = (int32_t)((offset >> 10) & 0x3ff);
    uint16_t* dest = draw_buffer_indirect(s, (int)BIT(mode, 4, 2));
    if (!dest) return;
    uint16_t* depth = aux_buffer(s);
    uint16_t* start = (uint16_t*)s->fbram;
    uint16_t* end = ram_end(s);
    uint32_t fm = s->fbi.reg[r_fbzMode];

    if (!BIT(mode, 8, 1)) {
        int32_t scry = BIT(mode, 13, 1) ? s->yorigin - y : y;
        dest += (int64_t)scry * s->rowpixels + x;
        if (depth) depth += (int64_t)scry * s->rowpixels + x;
        dither_t d = dither_make(scry, fm);
        for (int pix = 0; mask; pix++, x++, mask >>= 4) {
            if (!(mask & 0x0f)) continue;
            uint32_t c = col[pix];
            if ((mask & LFB_RGB0) && dest + pix >= start && dest + pix < end)
                dest[pix] = dither_pixel(&d, x, (c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff);
            if (depth && depth + pix >= start && depth + pix < end) {
                if (BIT(fm, 18, 1)) { if (mask & LFB_ALPHA0) depth[pix] = (uint16_t)(c >> 24); }
                else if (mask & (LFB_DEPTH0 | LFB_DEPTHMSW0)) depth[pix] = dep[pix];
            }
            s->fbi.reg[r_fbiPixelsOut]++;
        }
    } else {
        int32_t scry = BIT(fm, 17, 1) ? s->yorigin - y : y;
        int32_t sx = x, ex = x + 2;
        if (!clip_row(s, dest, scry, &sx, &ex)) return;
        if (depth && !clip_row(s, depth, scry, &sx, &ex)) return;
        dest += (int64_t)scry * s->rowpixels;
        depth = depth ? depth + (int64_t)scry * s->rowpixels : s->dummy_depth - x;
        for (int pix = 0; mask; pix++, x++, mask >>= 4)
            if ((mask & 0x0f) && x >= sx && x < ex) pixel_pipeline(s, dest, depth, x, scry, col[pix], dep[pix]);
    }
}

static uint32_t lfb_r(voodoo_state_t* s, uint32_t offset) {
    offset <<= 1;
    int32_t x = (int32_t)(offset & 1023), scry = (int32_t)((offset >> 10) & 0x3ff);
    uint32_t mode = s->fbi.reg[r_lfbMode];
    if (BIT(mode, 13, 1)) scry = s->yorigin - scry;
    uint16_t* buf = lfb_buffer_indirect(s, (int)BIT(mode, 6, 2));
    if (!buf) return 0xffffffff;
    buf += (int64_t)scry * s->rowpixels + x;
    if (buf < (uint16_t*)s->fbram || buf + 1 >= ram_end(s)) return 0xffffffff;
    uint32_t data = buf[0] | ((uint32_t)buf[1] << 16);
    if (BIT(mode, 15, 1)) data = (data << 16) | (data >> 16);
    if (BIT(mode, 16, 1)) data = bswap32(data);
    return data;
}

static void texture_w(voodoo_state_t* s, uint32_t offset, uint32_t data) {
    if (BIT(offset, 19, 2) != 0) return;               /* only TMU 0 is fitted */
    tmu_t* t = &s->tmu;
    uint32_t lod = t->r.reg[r_tLOD], mode = t->r.reg[r_textureMode];
    prepare_texture(t);
    if (BIT(lod, 25, 1)) data = bswap32(data);
    if (BIT(lod, 26, 1)) data = (data >> 16) | (data << 16);
    uint32_t bpt = BIT(mode, 8, 4) < 8 ? 1 : 2;
    uint32_t l = BIT(offset, 15, 4), tt = BIT(offset, 7, 8);
    uint32_t ts = (offset << ((BIT(mode, 31, 1) && bpt == 1) ? 2 : 1)) & 0xff;
    if (l > 8) return;
    uint32_t offs = tt * ((t->wmask >> l) + 1) + ts;
    uint8_t* dest = t->ram + ((t->lodoffset[l] + ((bpt * offs) & ~3u)) & t->mask);
    memcpy(dest, &data, 4);                            /* little-endian host */
}

/* ---------------------------------------------------------------------
 * Video timing, swaps, DAC
 * --------------------------------------------------------------------- */
static void drain_queue(voodoo_state_t* s);

static void swap_buffers(voodoo_state_t* s) {
    s->fbi.reg[r_fbiSwapHistory] = (s->fbi.reg[r_fbiSwapHistory] << 4) | (uint32_t)(s->vblank_count < 15 ? s->vblank_count : 15);
    int buffers = s->rgboffs[2] == ~0u ? 2 : 3;
    s->frontbuf = (s->frontbuf + 1) % buffers;
    s->backbuf = (s->frontbuf + 1) % buffers;
    if (s->swaps_pending) s->swaps_pending--;
    s->vblank_count = 0;
    s->swap_pending = 0;
    drain_queue(s);
}

static void reset_counters(voodoo_state_t* s) {
    s->fbi.reg[r_fbiPixelsIn] = s->fbi.reg[r_fbiChromaFail] = s->fbi.reg[r_fbiZfuncFail] = 0;
    s->fbi.reg[r_fbiAfuncFail] = s->fbi.reg[r_fbiPixelsOut] = 0;
}

static void soft_reset(voodoo_state_t* s) {
    reset_counters(s);
    s->fbi.reg[r_fbiTrianglesOut] = 0;
    s->qhead = s->qtail = 0;
}

static void dac_w(voodoo_state_t* s, uint32_t data) {
    uint32_t reg = BIT(data, 8, 3);
    if (!BIT(data, 11, 1)) { s->dac_reg[reg] = (uint8_t)data; return; }
    s->dac_read_result = s->dac_reg[reg];
    switch (s->dac_reg[7]) {                           /* indexed ID reads used by DAC detection */
        case 0x01: s->dac_read_result = 0x55; break;
        case 0x07: s->dac_read_result = 0x71; break;
        case 0x0b: s->dac_read_result = 0x79; break;
    }
}

static void video_w(voodoo_state_t* s) {
    uint32_t hs = s->fbi.reg[r_hSync], vs = s->fbi.reg[r_vSync];
    uint32_t dim = s->fbi.reg[r_videoDimensions], bp = s->fbi.reg[r_backPorch];
    if (hs && vs && dim && bp) {
        s->width = BIT(dim, 0, 10);
        s->height = BIT(dim, 16, 10);
        s->vtotal = BIT(vs, 0, 12) + BIT(vs, 16, 12);
    }
}

/* ---------------------------------------------------------------------
 * Register access
 * --------------------------------------------------------------------- */
static int32_t float_to_int32(uint32_t data, int fixedbits) {
    int exponent = (int)((data >> 23) & 0xff) - 127 - 23 + fixedbits;
    int32_t r = (int32_t)((data & 0x7fffff) | 0x800000);
    if (exponent < 0) r = exponent > -32 ? r >> -exponent : 0;
    else r = exponent < 32 ? (int32_t)((uint32_t)r << exponent) : 0x7fffffff;
    return (data & 0x80000000u) ? -r : r;
}

static int64_t float_to_int64(uint32_t data, int fixedbits) {
    int exponent = (int)((data >> 23) & 0xff) - 127 - 23 + fixedbits;
    int64_t r = (data & 0x7fffff) | 0x800000;
    if (exponent < 0) r = exponent > -64 ? r >> -exponent : 0;
    else r = exponent < 64 ? (int64_t)((uint64_t)r << exponent) : 0x7fffffffffffffffll;
    return (data & 0x80000000u) ? -r : r;
}

static int64_t* st_target(vregs_t* r, uint32_t regnum) {
    switch (regnum & 0x1f) {
        case 0x0d: return &r->starts;  case 0x0e: return &r->startt;  case 0x0f: return &r->startw;
        case 0x15: return &r->dsdx;    case 0x16: return &r->dtdx;    case 0x17: return &r->dwdx;
        case 0x1d: return &r->dsdy;    case 0x1e: return &r->dtdy;    default:   return &r->dwdy;
    }
}

static void passive_w(voodoo_state_t* s, uint32_t chip, uint32_t regnum, uint32_t data) {
    if (chip & 1) s->fbi.reg[regnum] = data;
    if (chip & 2) s->tmu.r.reg[regnum] = data;
}

/* returns 1 if the command leaves the chip busy until a vblank (sync'd swap) */
static int reg_exec(voodoo_state_t* s, uint32_t chipmask, uint32_t regnum, uint32_t data) {
    const regent_t* e = &s_regs[regnum];
    uint32_t chip = chipmask & e->chips;
    data &= e->mask;
    switch (e->wk) {
        case W_PASS: passive_w(s, chip, regnum, data); break;
        case W_FP4: passive_w(s, chip, regnum - 0x20, (uint32_t)float_to_int32(data, 4)); break;
        case W_FP12: passive_w(s, chip, regnum - 0x20, (uint32_t)float_to_int32(data, 12)); break;
        case W_ST: case W_W: case W_FST: case W_FW: {
            int64_t v = (e->wk == W_ST) ? (int64_t)(int32_t)data << 14 :
                        (e->wk == W_W) ? (int64_t)(int32_t)data << 2 : float_to_int64(data, 32);
            if (chip & 1) *st_target(&s->fbi, regnum) = v;
            if (chip & 2) *st_target(&s->tmu.r, regnum) = v;
            break;
        }
        case W_TRI:
            if (s->prof) { double t0 = now_seconds(); triangle(s); s->prof_busy += now_seconds() - t0; s->prof_tris++; }
            else triangle(s);
            break;
        case W_NOP:
            if (data & 1) reset_counters(s);
            if (data & 2) s->fbi.reg[r_fbiTrianglesOut] = 0;
            break;
        case W_FASTFILL:
            if (s->prof) { double t0 = now_seconds(); fastfill(s); s->prof_busy += now_seconds() - t0; }
            else fastfill(s);
            break;
        case W_SWAP:
            s->swap_interval = (int)BIT(data, 1, 8);
            if (!(data & 1)) { s->swap_pending = 1; swap_buffers(s); return 0; }
            s->swap_pending = 1;
            return 1;
        case W_FOG:
            if (chip & 1) {
                uint32_t b = 2 * (regnum - r_fogTable);
                s->fogdelta[b] = (uint8_t)data; s->fogblend[b] = (uint8_t)(data >> 8);
                s->fogdelta[b + 1] = (uint8_t)(data >> 16); s->fogblend[b + 1] = (uint8_t)(data >> 24);
            }
            break;
        case W_FBIINIT:
            if ((chip & 1) && (s->init_enable & 1)) {
                s->fbi.reg[regnum] = data;
                if (regnum == r_fbiInit0 && (BIT(data, 1, 1) || BIT(data, 2, 1))) soft_reset(s);
                if (regnum == r_fbiInit0 || regnum == r_fbiInit4) recompute_fbmem_fifo(s);
                if (regnum == r_fbiInit1 || regnum == r_fbiInit2) recompute_video_memory(s);
                if (regnum == r_fbiInit3) s->yorigin = (int32_t)BIT(data, 22, 10);
            }
            break;
        case W_VIDEO: if (chip & 1) { s->fbi.reg[regnum] = data; video_w(s); } break;
        case W_CLUT:
            if ((chip & 1) && !BIT(s->fbi.reg[r_fbiInit1], 8, 1)) {
                uint32_t idx = BIT(data, 24, 8);
                if (idx <= 32 && s->clut[idx] != data) { s->clut[idx] = data; s->clut_dirty = 1; }
            }
            break;
        case W_DAC: if (chip & 1) dac_w(s, data); break;
        case W_TEX:
            if ((chip & 2) && s->tmu.r.reg[regnum] != data) { s->tmu.r.reg[regnum] = data; s->tmu.dirty = 1; }
            break;
        case W_PAL: if (chip & 2) ncc_w(&s->tmu, regnum, data); break;
        default: break;                                /* invalid/unimplemented: ignored */
    }
    return 0;
}

static void queue_add(voodoo_state_t* s, uint32_t a, uint32_t d) {
    if (s->qtail + 2 > s->qcap) {
        if (s->qhead) {                                /* compact */
            memmove(s->q, s->q + s->qhead, (s->qtail - s->qhead) * 4);
            s->qtail -= s->qhead; s->qhead = 0;
        }
        if (s->qtail + 2 > s->qcap) {
            s->qcap = s->qcap ? s->qcap * 2 : 65536;
            s->q = (uint32_t*)realloc(s->q, s->qcap * 4);
            if (!s->q) { fprintf(stderr, "[voodoo] out of memory\n"); exit(1); }
        }
    }
    s->q[s->qtail++] = a;
    s->q[s->qtail++] = d;
}

static void drain_queue(voodoo_state_t* s) {
    while (s->qhead < s->qtail && !s->swap_pending) {
        uint32_t a = s->q[s->qhead++], d = s->q[s->qhead++];
        switch (a & 0xc0000000u) {
            case Q_REG: reg_exec(s, BIT(a, 8, 4), a & 0xff, d); break;
            case Q_LFB:
                lfb_w(s, a & 0x0fffffff, d, ((a & Q_NO_16_31) ? 0 : 0xffff0000u) | ((a & Q_NO_0_15) ? 0 : 0xffffu));
                break;
            default:    texture_w(s, a & 0x0fffffff, d); break;
        }
    }
    if (s->qhead == s->qtail) s->qhead = s->qtail = 0;
}

static int queueing(voodoo_state_t* s) { return s->swap_pending && (s->init_enable & 2); }

static uint32_t status_r(voodoo_state_t* s) {
    uint32_t queued = (uint32_t)((s->qtail - s->qhead) / 2);
    uint32_t r = 0, pcifree = 0x3f, memfree = 0xffff;
    if (BIT(s->fbi.reg[r_fbiInit0], 13, 1)) {
        int32_t m = ((int32_t)s->memfifo_entries - 1) / 2 - (int32_t)queued;
        memfree = (uint32_t)clampi(m, 0, 0xffff);
    } else
        pcifree = (uint32_t)clampi(0x3f - (int32_t)queued, 0, 0x3f);
    r |= pcifree;
    r |= (uint32_t)s->vblank << 6;
    if (s->swap_pending || queued) r |= (1u << 7) | (1u << 8) | (1u << 9);
    r |= (uint32_t)s->frontbuf << 10;
    r |= memfree << 12;
    r |= (uint32_t)(s->swaps_pending < 7 ? s->swaps_pending : 7) << 28;
    return r;
}

static uint32_t reg_read(voodoo_state_t* s, uint32_t regnum) {
    const regent_t* e = &s_regs[regnum];
    uint32_t v = 0;
    switch (e->rk) {
        case R_PASS: case R_STATS: v = s->fbi.reg[regnum]; break;
        case R_STATUS: v = status_r(s); break;
        case R_FBIINIT2: v = (s->init_enable & 4) ? s->dac_read_result : s->fbi.reg[regnum]; break;
        case R_VRETRACE:
            if (!s->vblank) {
                uint32_t vt = s->vtotal ? s->vtotal : 400;
                double line = (hw_time() - s->last_vblank_end) * 57.0 * vt;
                v = line < 1 ? 1 : line >= vt ? vt - 1 : (uint32_t)line;
            }
            break;
        default: v = 0; break;
    }
    return v & e->mask;
}

/* ---------------------------------------------------------------------
 * Public interface
 * --------------------------------------------------------------------- */
voodoo_state_t* voodoo_create(void) {
    static int tables_ready;
    if (!tables_ready) { init_tables(); tables_ready = 1; }
    voodoo_state_t* s = (voodoo_state_t*)calloc(1, sizeof(*s));
    if (!s) return NULL;
    s->mem = (uint8_t*)calloc(1, (FB_MB + TMU_MB) * 1024 * 1024 + 8);
    s->fbram = s->mem;
    s->fbmask = FB_MB * 1024 * 1024 - 1;
    s->tmu.ram = s->mem + FB_MB * 1024 * 1024;
    s->tmu.mask = TMU_MB * 1024 * 1024 - 1;
    s->tmu.dirty = 1;
    s->tmu.pal_dirty[0] = s->tmu.pal_dirty[1] = s->tmu.pal_dirty[2] = s->tmu.pal_dirty[3] = 1;
    s->rgboffs[0] = s->rgboffs[1] = s->rgboffs[2] = 0;
    s->auxoffs = ~0u;
    s->frontbuf = 0; s->backbuf = 1;
    s->width = 512; s->height = 384;
    for (int pen = 0; pen < 32; pen++) {
        uint32_t c = expand_bits(pen, 5);
        s->clut[pen] = ((uint32_t)pen << 24) | (c << 16) | (c << 8) | c;
    }
    s->clut[32] = (32u << 24) | 0xffffff;
    s->clut_dirty = 1;
    /* power-on fbiInit values */
    s->fbi.reg[r_fbiInit0] = (1 << 4) | (0x10 << 6);
    s->fbi.reg[r_fbiInit1] = (1 << 1) | (1 << 8) | (1 << 12) | (2 << 20);
    s->fbi.reg[r_fbiInit2] = (1 << 6) | (0x100u << 23);
    s->fbi.reg[r_fbiInit3] = (2 << 13) | (0xf << 17);
    s->fbi.reg[r_fbiInit4] = 1;
    s->last_vblank_end = hw_time();
    s->prof_t0 = now_seconds();
    s->prof = getenv("VOODOO_PROF") != NULL;
    return s;
}

uint32_t voodoo_read(voodoo_state_t* s, uint32_t offset) {
    offset &= 0xffffff;
    if (offset < 0x400000) return reg_read(s, (offset >> 2) & 0xff);
    if (offset < 0x800000) return lfb_r(s, (offset - 0x400000) >> 2);
    return 0xffffffff;
}

/* Not in voodoo.h yet: a write with byte lanes. 'offset' is word aligned, 'value'
 * sits in its lanes (little-endian bus) and mem_mask selects them. Only LFB
 * writes honour partial masks, as on the chip; a 16-bit LFB write in a 16-bit
 * format writes one pixel. */
void voodoo_write_masked(voodoo_state_t* s, uint32_t offset, uint32_t value, uint32_t mem_mask) {
    offset &= 0xfffffc;
    if (offset >= 0x400000 && offset < 0x800000 && mem_mask != 0xffffffffu) {
        uint32_t word = (offset - 0x400000) >> 2;
        uint32_t a = word | ((mem_mask & 0xffff0000u) ? 0 : Q_NO_16_31) | ((mem_mask & 0xffffu) ? 0 : Q_NO_0_15);
        if (queueing(s)) queue_add(s, Q_LFB | a, value);
        else lfb_w(s, word, value, mem_mask);
        return;
    }
    voodoo_write(s, offset, value);
}

void voodoo_write(voodoo_state_t* s, uint32_t offset, uint32_t value) {
    offset &= 0xffffff;
    uint32_t word = offset >> 2;
    if (offset < 0x400000) {
        uint32_t chipmask = BIT(word, 8, 4);
        if (!chipmask) chipmask = 0xf;
        chipmask &= 3;                                 /* FBI + TMU0 */
        uint32_t regnum = word & 0xff;
        if (BIT(offset, 20, 1) && BIT(s->fbi.reg[r_fbiInit0], 3, 1)) value = bswap32(value);
        if (BIT(offset, 21, 1) && BIT(s->fbi.reg[r_fbiInit3], 0, 1) && regnum < 0x40) regnum = s_alias_map[regnum];
        if (!s_regs[regnum].fifo) { reg_exec(s, chipmask, regnum, value); return; }
        if (regnum == r_swapbufferCMD) s->swaps_pending++;
        if (queueing(s)) { queue_add(s, Q_REG | (chipmask << 8) | regnum, value); return; }
        reg_exec(s, chipmask, regnum, value);
    } else if (offset < 0x800000) {
        if (queueing(s)) queue_add(s, Q_LFB | (word - 0x400000 / 4), value);
        else lfb_w(s, word - 0x400000 / 4, value, 0xffffffffu);
    } else {
        if (queueing(s)) queue_add(s, Q_TEX | (word - 0x800000 / 4), value);
        else texture_w(s, word - 0x800000 / 4, value);
    }
}

void voodoo_pci_config_write(voodoo_state_t* s, uint32_t offset, uint32_t value) {
    if (offset == 0x40) s->init_enable = value;
}

void voodoo_vblank(voodoo_state_t* s, int state) {
    if (state) {
        if (s->prof && ++s->prof_frames == 300) {
            double t = now_seconds();
            fprintf(stderr, "[voodoo] %.1f%% of wall time rasterizing, %u triangles/frame\n",
                    100.0 * s->prof_busy / (t - s->prof_t0), s->prof_tris / 300);
            s->prof_t0 = t; s->prof_busy = 0; s->prof_frames = s->prof_tris = 0;
        }
        if (s->vblank_count < 250) s->vblank_count++;
        if (s->swap_pending && s->vblank_count >= s->swap_interval) swap_buffers(s);
        s->vblank = 1;
    } else {
        s->vblank = 0;
        s->last_vblank_end = hw_time();
    }
}

const uint16_t* voodoo_display(voodoo_state_t* s, int* w, int* h, int* stride) {
    uint32_t width = s->width > 640 ? 640 : s->width, height = s->height > 480 ? 480 : s->height;
    if (!width || !height) { width = 512; height = 384; }
    *w = (int)width; *h = (int)height; *stride = (int)width;
    if (BIT(s->fbi.reg[r_fbiInit1], 12, 1)) {          /* software blank */
        memset(s->out, 0, sizeof(s->out));
        return s->out;
    }
    if (s->clut_dirty) {
        uint32_t* c = s->clut;
        if ((c[32] & 0xffffff) == 0 && (c[31] & 0xffffff) != 0) c[32] = 0x20ffffff;
        uint8_t rt[32], gt[64], bt[32];
        for (uint32_t i = 0; i < 32; i++) {
            uint32_t v = expand_bits(i, 5);
            rt[i] = (uint8_t)((((c[v >> 3] >> 16) & 0xff) * (8 - (v & 7)) + ((c[(v >> 3) + 1] >> 16) & 0xff) * (v & 7)) >> 3);
            bt[i] = (uint8_t)(((c[v >> 3] & 0xff) * (8 - (v & 7)) + (c[(v >> 3) + 1] & 0xff) * (v & 7)) >> 3);
        }
        for (uint32_t i = 0; i < 64; i++) {
            uint32_t v = expand_bits(i, 6);
            gt[i] = (uint8_t)((((c[v >> 3] >> 8) & 0xff) * (8 - (v & 7)) + ((c[(v >> 3) + 1] >> 8) & 0xff) * (v & 7)) >> 3);
        }
        for (uint32_t p = 0; p < 65536; p++)
            s->pen[p] = (uint16_t)(((rt[p >> 11] >> 3) << 11) | ((gt[(p >> 5) & 63] >> 2) << 5) | (bt[p & 31] >> 3));
        s->clut_dirty = 0;
    }
    const uint16_t* src = draw_buffer(s, s->frontbuf);
    const uint16_t* end = ram_end(s);
    for (uint32_t y = 0; y < height; y++) {
        const uint16_t* row = src + (size_t)y * s->rowpixels;
        uint16_t* o = s->out + (size_t)y * width;
        for (uint32_t x = 0; x < width; x++)
            o[x] = (row + x < end) ? s->pen[row[x]] : 0;
    }
    return s->out;
}
