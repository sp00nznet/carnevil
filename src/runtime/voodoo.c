/**
 * Minimal 3DFX Voodoo 1 register emulation for CarnEvil.
 *
 * This doesn't render anything yet -- it just responds to register
 * reads/writes so the game's Voodoo init sequence completes and the
 * game progresses past hardware checks.
 */

#include "voodoo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

void voodoo_init(voodoo_state_t* voodoo) {
    memset(voodoo, 0, sizeof(*voodoo));
    voodoo->width = 512;
    voodoo->height = 384;

    /* Default status: everything idle, FIFOs empty, VSync active */
    voodoo->status = VOODOO_STATUS_PCIFIFO_FREE_MASK  /* FIFO has 63 free slots */
                   | (0xFFFF << VOODOO_STATUS_MEMFIFO_FREE_SHIFT) /* Mem FIFO free */
                   | VOODOO_STATUS_RETRACE;  /* In retrace */

    /* Pre-configure fbiInit registers so the game thinks Voodoo is ready.
     * These match the values MAME's seattle.cpp sets during boot. */
    voodoo->fbiInit0 = 0x00000001; /* VGA passthrough disabled, FBI enabled */
    voodoo->fbiInit1 = 0x00100000; /* 2MB framebuffer */
    voodoo->fbiInit2 = 0x00000000; /* Normal swap buffer mode */
    voodoo->fbiInit3 = 0x00000040; /* Y-origin at top */
    voodoo->fbiInit4 = 0x00000000;

    /* Initialize Voodoo 1 registers for CarnEvil (512x384 @ 16bpp)
     * These mirror what the RTOS boot code writes to hardware. */

    /* Video dimensions: 512x384 */
    voodoo->videoDim = 512 | (384 << 16);

    /* fbiInit0: VGA passthrough disabled, FBI active, 16-bit output */
    voodoo->regs[VOODOO_FBIINIT0 / 4] = 0x00000411;

    /* fbiInit1: 8 tiles wide (512/64=8, stored as 8-1=7 in bits [7:4])
     * Bit 12: video output enable */
    voodoo->regs[VOODOO_FBIINIT1 / 4] = 0x00001070;

    /* fbiInit2: swap algorithm=auto(1), video buffer offset=0 */
    voodoo->regs[VOODOO_FBIINIT2 / 4] = 0x00000200;

    /* fbiInit3: yorigin=383 (bits [31:22]) for bottom-up rendering */
    voodoo->regs[VOODOO_FBIINIT3 / 4] = (383 << 22);

    /* fbiInit4: default */
    voodoo->regs[VOODOO_FBIINIT4 / 4] = 0x00000001;

    /* fbzMode: clip=1, depth_write=1, depth_func=less, dither=1, rgb_write=1,
     * draw_buffer_enable=1 */
    voodoo->regs[VOODOO_FBZMODE / 4] = 0x00004391;

    /* fbzColorPath: RGB from iterated, simple passthrough */
    voodoo->regs[VOODOO_FBZCOLORPATH / 4] = 0x00000009;

    /* Clip rect: full screen 512x384 */
    voodoo->regs[VOODOO_CLIPLR / 4] = 512;
    voodoo->regs[VOODOO_CLIPLOHI / 4] = 384;
    voodoo->initialized = 1;
}

uint32_t voodoo_read(voodoo_state_t* voodoo, uint32_t offset) {
    uint32_t reg = offset & 0x3FF;  /* Registers are in first 1KB */

    switch (reg) {
    case VOODOO_STATUS: {
        voodoo->frame_count++;
        if (voodoo->frame_count & 1)
            voodoo->status |= VOODOO_STATUS_RETRACE;
        else
            voodoo->status &= ~VOODOO_STATUS_RETRACE;
        voodoo->status &= ~(VOODOO_STATUS_FBI_BUSY | VOODOO_STATUS_TREX_BUSY |
                           VOODOO_STATUS_SST_BUSY | VOODOO_STATUS_SWAPBUF_PEND);
        voodoo->status |= VOODOO_STATUS_PCIFIFO_FREE_MASK |
                          (0xFFFF << VOODOO_STATUS_MEMFIFO_FREE_SHIFT);
        return voodoo->status;
    }

    case VOODOO_FBIINIT0: return voodoo->fbiInit0;
    case VOODOO_FBIINIT1: return voodoo->fbiInit1;
    case VOODOO_FBIINIT2: return voodoo->fbiInit2;
    case VOODOO_FBIINIT3: return voodoo->fbiInit3;
    case VOODOO_FBIINIT4: return voodoo->fbiInit4;
    case VOODOO_VIDEODIM:  return voodoo->videoDim;
    case VOODOO_BACKPORCH: return voodoo->backporch;

    default:
        ; /* empty statement for MSVC C11 compliance */
        if ((reg >> 2) < 256) return voodoo->regs[reg >> 2];
        return 0;
    }
}

static int vwrite_log = 0;
static uint32_t vwrite_total = 0;
static uint32_t vwrite_nonzero = 0;
uint64_t g_tex_writes = 0;
int g_texbound_this_frame = 0;
uint32_t g_tex_first_off = 0, g_tex_last_off = 0;

uint32_t voodoo_get_write_count(void) { return vwrite_total; }
uint32_t voodoo_get_nonzero_count(void) { return vwrite_nonzero; }

void voodoo_write(voodoo_state_t* voodoo, uint32_t offset, uint32_t value) {
    uint32_t reg = offset & 0x3FF;
    vwrite_total++;
    if (value != 0) vwrite_nonzero++;

    /* Memory-mapped regions (TMU texture memory at 0x800000+, LFB framebuffer
     * at 0x400000-0x800000) are NOT registers. They must be handled before the
     * register switch and return early: otherwise reg=offset&0x3FF aliases real
     * command/vertex registers -- a texel whose offset&0x3FF==0x100 would fire
     * an FTRIANGLE render mid-upload, and every texel/pixel write clobbers
     * regs[offset&0x3FF] (this is exactly what was trashing texMode/texBase/S-T
     * to garbage between texture load and render). */
    if (offset >= VOODOO_TEX_OFFSET) {
        uint32_t toff = offset - VOODOO_TEX_OFFSET;
        if (g_tex_writes == 0) g_tex_first_off = toff;
        g_tex_last_off = toff;
        g_tex_writes++;
        if (toff + 4 <= sizeof(voodoo->texmem))
            *(uint32_t*)(voodoo->texmem + toff) = value;
        return;
    }
    if (offset >= VOODOO_LFB_OFFSET) {
        uint32_t fb_offset = offset - VOODOO_LFB_OFFSET;
        uint32_t pixel_index = fb_offset / 2;   /* 32-bit write = two RGB565 px */
        if (pixel_index < 640 * 480) {
            voodoo->framebuffer[pixel_index] = (uint16_t)value;
            if (pixel_index + 1 < 640 * 480)
                voodoo->framebuffer[pixel_index + 1] = (uint16_t)(value >> 16);
        }
        return;
    }

    if (reg == VOODOO_TEXBASE && value != 0) {
        extern int g_texbound_this_frame; g_texbound_this_frame = 1;
        static int tb=0;
        if (tb++ < 8) fprintf(stderr, "[TEXBIND] texBaseAddr = 0x%08X (g_tex=%llu swap=%d)\n",
                              value, (unsigned long long)g_tex_writes, voodoo->swap_count);
    }

    /* Log unique non-zero register writes */
    if (value != 0 && vwrite_log < 50) {
        static uint32_t seen_regs[64] = {0};
        static int seen_count = 0;
        int is_new = 1;
        for (int i = 0; i < seen_count && i < 64; i++) {
            if (seen_regs[i] == reg) { is_new = 0; break; }
        }
        if (is_new && seen_count < 64) {
            seen_regs[seen_count++] = reg;
            vwrite_log++;
            fprintf(stderr, "[voodoo] NEW reg write: offset=0x%08X reg=0x%03X val=0x%08X (write #%u)\n",
                    offset, reg, value, vwrite_total);
        }
    }

    switch (reg) {
    case VOODOO_FBIINIT0:
        voodoo->fbiInit0 = value;
        /* Extract resolution from fbiInit1/fbiInit2 if set */
        break;

    case VOODOO_FBIINIT1:
        voodoo->fbiInit1 = value;
        break;

    case VOODOO_FBIINIT2:
        voodoo->fbiInit2 = value;
        break;

    case VOODOO_FBIINIT3:
        voodoo->fbiInit3 = value;
        break;

    case VOODOO_FBIINIT4:
        voodoo->fbiInit4 = value;
        break;

    case VOODOO_VIDEODIM:
        voodoo->videoDim = value;
        /* Rev1 Voodoo: bits [9:0] = xwidth, bits [25:16] = yheight */
        voodoo->width = value & 0x3FF;
        voodoo->height = (value >> 16) & 0x3FF;
        if (voodoo->width == 0) voodoo->width = 512;
        if (voodoo->height == 0) voodoo->height = 384;
        fprintf(stderr, "[voodoo] Video dimensions: %dx%d\n",
                voodoo->width, voodoo->height);
        break;

    case VOODOO_BACKPORCH:
        voodoo->backporch = value;
        break;

    case VOODOO_SWAPBUFCMD:
        voodoo->swap_count++;
        /* Copy back → front. Front buffer is what gets dumped. */
        memcpy(voodoo->framebuffer, voodoo->backbuffer,
               sizeof(uint16_t) * 640 * 480);
        /* TEXDBG: dump a snapshot whenever a texture was bound this frame so
         * the (rare, transient) textured attract frames can be inspected. */
        if (getenv("CARNEVIL_DUMP_FRAMES")) {
            extern int g_texbound_this_frame;
            /* Dump on texture-bind frames AND periodically, to trace the attract
             * sequence and catch transient textured frames. */
            int periodic = (voodoo->swap_count % 500 == 0);
            if (g_texbound_this_frame || periodic) {
                char nm[64]; snprintf(nm, sizeof(nm), "seq_%05d%s.ppm", voodoo->swap_count,
                                      g_texbound_this_frame ? "_TEX" : "");
                FILE* tf = fopen(nm, "wb");
                if (tf) {
                    fprintf(tf, "P6\n640 480\n255\n");
                    for (int i = 0; i < 640*480; i++) {
                        uint16_t p = voodoo->framebuffer[i];
                        fputc((p>>11)<<3, tf); fputc(((p>>5)&0x3F)<<2, tf); fputc((p&0x1F)<<3, tf);
                    }
                    fclose(tf);
                    fprintf(stderr, "[seqframe] dumped %s\n", nm);
                }
            }
            g_texbound_this_frame = 0;
        }
        /* Reset the depth buffer to "far" for the next frame so the first
         * fragment at each pixel always passes a less/lessequal test. */
        for (int i = 0; i < 640 * 480; i++) voodoo->zbuffer[i] = 1e30f;
        if (voodoo->swap_count <= 5 || voodoo->swap_count % 100 == 0) {
            fprintf(stderr, "[voodoo] SwapBuffers #%d\n", voodoo->swap_count);
        }
        break;

    case VOODOO_NOPCOMMAND:
        break;

    /* Vertex setup registers — store AND log. The previous version only
     * logged, which meant the rasterizer below read whatever stale value
     * was in the register slot (usually 0). */
    case VOODOO_SVX: case VOODOO_SVY:
    case VOODOO_SRED: case VOODOO_SGREEN: case VOODOO_SBLUE: case VOODOO_SALPHA: {
        voodoo->regs[reg >> 2] = value;
        static int vtx_log = 0;
        vtx_log++;
        if (vtx_log <= 30) {
            fprintf(stderr, "[voodoo] VERTEX INT reg 0x%03X = 0x%08X (%.3f)\n",
                    reg, value, *(float*)&value);
        }
        break;
    }
    case VOODOO_FVX: case VOODOO_FVY:
    case VOODOO_FRED: case VOODOO_FGREEN: case VOODOO_FBLUE: case VOODOO_FALPHA: {
        voodoo->regs[reg >> 2] = value;
        static int fvtx_log = 0;
        fvtx_log++;
        if (fvtx_log <= 30) {
            fprintf(stderr, "[voodoo] VERTEX FLOAT reg 0x%03X = %.3f\n",
                    reg, *(float*)&value);
        }
        break;
    }

    case VOODOO_TRIANGLECMD:
    case VOODOO_FTRIANGLE: {
        static int tri_count = 0;
        tri_count++;

        /* Read the three vertices. Voodoo 1's triangle setup stores
         * three vertices A, B, C at fixed register offsets. The float
         * variant (FTRIANGLE) uses IEEE registers at 0x180/0x184 for
         * vertex A; the original setup writes B and C via SetupDX/DY
         * coefficients. For a minimal flat-shaded rasterizer that's
         * usable as a smoke test, we read three vertices directly from
         * adjacent register slots and fill the triangle with color1. */
        float ax, ay, bx, by, cx, cy;
        if (reg == VOODOO_FTRIANGLE) {
            /* CarnEvil's float triangle setup stores the three IEEE-float
             * vertices contiguously at 0x88..0x9C (verified by dumping the
             * register window at FTRIANGLE time): A=(0x88,0x8C), B=(0x90,0x94),
             * C=(0x98,0x9C). (The 0x180/0x1A8/0x1D0 SetupX/Y slots this used to
             * read are never written -- hence the old all-zero triangles.) */
            ax = *(float*)&voodoo->regs[(0x88) >> 2];
            ay = *(float*)&voodoo->regs[(0x8C) >> 2];
            bx = *(float*)&voodoo->regs[(0x90) >> 2];
            by = *(float*)&voodoo->regs[(0x94) >> 2];
            cx = *(float*)&voodoo->regs[(0x98) >> 2];
            cy = *(float*)&voodoo->regs[(0x9C) >> 2];
        } else {
            /* Fixed-point 12.4 -> float */
            ax = (int32_t)voodoo->regs[(0x048) >> 2] / 16.0f;
            ay = (int32_t)voodoo->regs[(0x04C) >> 2] / 16.0f;
            bx = (int32_t)voodoo->regs[(0x070) >> 2] / 16.0f;
            by = (int32_t)voodoo->regs[(0x074) >> 2] / 16.0f;
            cx = (int32_t)voodoo->regs[(0x098) >> 2] / 16.0f;
            cy = (int32_t)voodoo->regs[(0x09C) >> 2] / 16.0f;
        }

        /* --- Pixel-pipeline setup ---------------------------------------
         * Verified CarnEvil register map = standard SST-1 layout + 0x80 (the
         * float-format bit): vertices 0x88..0x9C, iterator START values at
         * 0xA0..0xBC (R,G,B,Z,A,S,T,W) and per-X / per-Y GRADIENTS at
         * 0xC0..0xDC / 0xE0..0xFC. All iterated relative to vertex A. */
        #define RF(o) (*(float*)&voodoo->regs[(o) >> 2])
        uint32_t color1   = voodoo->regs[VOODOO_COLOR1 >> 2];
        uint32_t cpath    = voodoo->regs[VOODOO_FBZCOLORPATH >> 2];
        uint32_t fbzmode  = voodoo->regs[VOODOO_FBZMODE >> 2];
        uint32_t texmode  = voodoo->regs[VOODOO_TEXMODE >> 2];   /* 0x300 */
        int   rgbselect   = cpath & 0x3;            /* 0=iterated 1=texture 2=color1 */
        int   depth_en    = (fbzmode >> 4) & 1;     /* enable depth buffer */
        int   depth_func  = (fbzmode >> 5) & 7;     /* depth compare function */
        int   depth_wmask = (fbzmode >> 10) & 1;    /* depth write mask (1=write) */
        /* Texturing is selected by fbzColorPath rgbselect==1; whether a texture
         * is actually *bound* is indicated by a non-zero texBaseAddr pointing at
         * uploaded data. Gate on that: it's the semantically-correct condition
         * (texMode bit 0 is perspective-enable, not texture-enable) and it keeps
         * non-textured geometry on the flat color1 path. In the current attract
         * state the game never binds a texture for rendering (texBase stays 0 --
         * verified by instrumenting reg 0x30C across 3000 frames), so this draws
         * the same clean UI; it lights up once the attract submits real textured
         * geometry. The uploaded font itself is verified correct (8-bit intensity,
         * 256-wide, glyphs recoverable from texmem -- see project memory). */
        uint32_t texbase  = voodoo->regs[VOODOO_TEXBASE >> 2];
        int   tex_en      = (g_tex_writes > 0) && (texbase != 0);

        float sR=RF(0xA0), sG=RF(0xA4), sB=RF(0xA8), sZ=RF(0xAC);
        float sS=RF(0xB4), sT=RF(0xB8), sW=RF(0xBC);
        float dRdX=RF(0xC0), dGdX=RF(0xC4), dBdX=RF(0xC8), dZdX=RF(0xCC);
        float dSdX=RF(0xD4), dTdX=RF(0xD8), dWdX=RF(0xDC);
        float dRdY=RF(0xE0), dGdY=RF(0xE4), dBdY=RF(0xE8), dZdY=RF(0xEC);
        float dSdY=RF(0xF4), dTdY=RF(0xF8), dWdY=RF(0xFC);

        /* color1 components (RGB888) -- the flat-shade source, preserved for
         * the color1 path and as the texture/iterated fallback. */
        int c1r=(color1>>16)&0xFF, c1g=(color1>>8)&0xFF, c1b=color1&0xFF;
        uint16_t c1_565 = (uint16_t)(((color1>>8)&0xF800)|((color1>>5)&0x07E0)|((color1>>3)&0x001F));
        if (c1_565 == 0) c1_565 = 0xFFFF;   /* same black->white fallback as before */

        /* texture geometry from texMode/LOD (size = 256>>lod, square). */
        uint32_t tlod = voodoo->regs[VOODOO_TLOD >> 2];
        int tex_w = 256 >> (tlod & 0xF); if (tex_w < 1) tex_w = 1;
        int tex_h = tex_w;
        int tex_fmt = (texmode >> 8) & 0xF;          /* texture format */

        /* Triangle bounding box, clipped to 640x480 */
        int xmin = (int)floorf(fminf(ax, fminf(bx, cx)));
        int ymin = (int)floorf(fminf(ay, fminf(by, cy)));
        int xmax = (int)ceilf(fmaxf(ax, fmaxf(bx, cx)));
        int ymax = (int)ceilf(fmaxf(ay, fmaxf(by, cy)));
        if (xmin < 0) xmin = 0;
        if (ymin < 0) ymin = 0;
        if (xmax > 640) xmax = 640;
        if (ymax > 480) ymax = 480;

        /* TEXDBG: profile textured triangles -- how many select texture, how many
         * actually bind one, and the S/T range they sample (degenerate S/T ->
         * flat-looking output). Gated by CARNEVIL_TEXPROF. */
        if (getenv("CARNEVIL_TEXPROF") && rgbselect == 1) {
            static long n_sel=0, n_bound=0; n_sel++;
            if (tex_en) {
                n_bound++;
                float srange = fabsf(dSdX*(xmax-xmin)) + fabsf(dSdY*(ymax-ymin));
                float trange = fabsf(dTdX*(xmax-xmin)) + fabsf(dTdY*(ymax-ymin));
                static int lg=0;
                if (lg++ < 14)
                    fprintf(stderr, "[texprof] swap=%d texBase=0x%X fmt=%d tex_w=%d sW=%.3f S_range=%.2f T_range=%.2f c1=0x%06X\n",
                            voodoo->swap_count, texbase, tex_fmt, tex_w, sW, srange, trange, color1 & 0xFFFFFF);
            }
            if ((n_sel % 20000)==0)
                fprintf(stderr, "[texprof] cumulative: rgbsel==1 tris=%ld, of which texBase-bound=%ld\n", n_sel, n_bound);
        }

        if (tri_count <= 20 || tri_count % 1000 == 0) {
            fprintf(stderr, "[voodoo] Triangle #%d A=(%.1f,%.1f) B=(%.1f,%.1f) C=(%.1f,%.1f) bbox=(%d,%d)-(%d,%d) rgbsel=%d depth=%d tex=%d c1=0x%04X\n",
                    tri_count, ax, ay, bx, by, cx, cy, xmin, ymin, xmax, ymax, rgbselect, depth_en, tex_en, c1_565);
        }

        /* Edge-function (signed-area) rasterizer. A pixel is inside the
         * triangle iff it has consistent sign on all three edges. */
        float e_abc = (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
        if (e_abc == 0) break;  /* degenerate */
        int pixels_drawn = 0;
        for (int y = ymin; y < ymax; y++) {
            float py = y + 0.5f;
            for (int x = xmin; x < xmax; x++) {
                float px = x + 0.5f;
                float w0 = (bx - ax) * (py - ay) - (by - ay) * (px - ax);
                float w1 = (cx - bx) * (py - by) - (cy - by) * (px - bx);
                float w2 = (ax - cx) * (py - cy) - (ay - cy) * (px - cx);
                int inside = (e_abc > 0)
                    ? (w0 >= 0 && w1 >= 0 && w2 >= 0)
                    : (w0 <= 0 && w1 <= 0 && w2 <= 0);
                if (!inside) continue;

                int idx = y * 640 + x;
                float fx = px - ax, fy = py - ay;   /* relative to start vertex A */

                /* --- Depth test (only when the game enables it) --- */
                float z = 0.0f;
                if (depth_en) {
                    z = sZ + dZdX * fx + dZdY * fy;
                    float zb = voodoo->zbuffer[idx];
                    int pass;
                    switch (depth_func) {
                        case 0: pass = 0; break;            /* never */
                        case 1: pass = (z < zb); break;     /* less */
                        case 2: pass = (z == zb); break;    /* equal */
                        case 3: pass = (z <= zb); break;    /* lessequal */
                        case 4: pass = (z > zb); break;     /* greater */
                        case 5: pass = (z != zb); break;    /* notequal */
                        case 6: pass = (z >= zb); break;    /* greaterequal */
                        default: pass = 1; break;           /* always */
                    }
                    if (!pass) continue;
                }

                /* --- Colour select --- */
                int r, g, b;
                if (rgbselect == 0) {
                    /* iterated RGB (Gouraud) */
                    int ir = (int)(sR + dRdX * fx + dRdY * fy);
                    int ig = (int)(sG + dGdX * fx + dGdY * fy);
                    int ib = (int)(sB + dBdX * fx + dBdY * fy);
                    r = ir < 0 ? 0 : ir > 255 ? 255 : ir;
                    g = ig < 0 ? 0 : ig > 255 ? 255 : ig;
                    b = ib < 0 ? 0 : ib > 255 ? 255 : ib;
                } else if (rgbselect == 1 && tex_en) {
                    /* perspective-correct texture sample. S/T are S*W,T*W; W is
                     * 1/w iterated, so s=S/W,t=T/W gives texel coordinates. */
                    float w = sW + dWdX * fx + dWdY * fy;
                    float inv = (w != 0.0f) ? 1.0f / w : 0.0f;
                    float s = (sS + dSdX * fx + dSdY * fy) * inv;
                    float t = (sT + dTdX * fx + dTdY * fy) * inv;
                    int ti = ((int)s) & (tex_w - 1);
                    int tj = ((int)t) & (tex_h - 1);
                    uint32_t texel = (uint32_t)(tj * tex_w + ti);
                    /* texBaseAddr is the texture's byte offset within TMU memory
                     * (the allocator hands the upload a base; the render binds it
                     * via reg 0x30C). Fonts upload as 8-bit intensity, 256-wide. */
                    uint32_t base = texbase & (sizeof(voodoo->texmem) - 1);
                    if (tex_fmt < 8 /* 8-bit-per-texel formats: intensity/palette */) {
                        uint32_t a = (base + texel) & (sizeof(voodoo->texmem) - 1);
                        int v = voodoo->texmem[a];
                        /* Intensity textures (fonts) modulate the iterated/color1
                         * tint, so a white glyph takes the UI element's colour. */
                        r = (v * c1r) / 255;
                        g = (v * c1g) / 255;
                        b = (v * c1b) / 255;
                    } else {
                        /* 16-bit-per-texel formats: sample as RGB565. */
                        uint32_t a = (base + texel * 2) & (sizeof(voodoo->texmem) - 2);
                        uint16_t tx = *(uint16_t*)(voodoo->texmem + a);
                        r = ((tx >> 11) & 0x1F) << 3;
                        g = ((tx >> 5) & 0x3F) << 2;
                        b = (tx & 0x1F) << 3;
                    }
                } else {
                    /* color1 (rgbselect==2) or texture-without-texture fallback:
                     * exactly the previous flat behaviour. */
                    voodoo->backbuffer[idx] = c1_565;
                    if (depth_en && depth_wmask) voodoo->zbuffer[idx] = z;
                    pixels_drawn++;
                    continue;
                }

                voodoo->backbuffer[idx] = (uint16_t)(((r << 8) & 0xF800) | ((g << 3) & 0x07E0) | (b >> 3));
                if (depth_en && depth_wmask) voodoo->zbuffer[idx] = z;
                pixels_drawn++;
            }
        }
        #undef RF
        if (tri_count <= 5) {
            fprintf(stderr, "[voodoo] Triangle #%d drew %d pixels\n", tri_count, pixels_drawn);
        }
        break;
    }

    case VOODOO_FASTFILLCMD: {
        /* FastFill: fill the clip rectangle with color1 */
        uint32_t clip_lr = voodoo->regs[VOODOO_CLIPLR >> 2];
        uint32_t clip_tb = voodoo->regs[VOODOO_CLIPLOHI >> 2];
        uint32_t color1 = voodoo->regs[VOODOO_COLOR1 >> 2];
        int left = (clip_lr >> 16) & 0x3FF;
        int right = clip_lr & 0x3FF;
        int top = (clip_tb >> 16) & 0x3FF;
        int bottom = clip_tb & 0x3FF;
        uint16_t fill_color = (uint16_t)(
            ((color1 >> 8) & 0xF800) |   /* R */
            ((color1 >> 5) & 0x07E0) |   /* G */
            ((color1 >> 3) & 0x001F));    /* B */
        if (right > 640) right = 640;
        if (bottom > 480) bottom = 480;
        static int fill_count = 0;
        fill_count++;
        if (fill_count <= 5) {
            fprintf(stderr, "[voodoo] FastFill: (%d,%d)-(%d,%d) color=0x%08X -> 0x%04X\n",
                    left, top, right, bottom, color1, fill_color);
        }
        /* FastFill writes to BACK buffer.
         * Skip black fills — they erase LFB-rendered content.
         * The game clears before rendering, but our single-pass loop
         * means the clear happens AFTER the last LFB write. */
        if (fill_color == 0x0000) break;
        for (int y = top; y < bottom; y++) {
            for (int x = left; x < right; x++) {
                voodoo->backbuffer[y * 640 + x] = fill_color;
            }
        }
        break;
    }

    default:
        if ((reg >> 2) < 256) {
            voodoo->regs[reg >> 2] = value;
            /* Log unique register writes for analysis */
            static uint32_t logged_regs[256] = {0};
            static int log_count = 0;
            uint32_t reg_idx = reg >> 2;
            if (reg_idx < 256 && !logged_regs[reg_idx] && log_count < 30) {
                logged_regs[reg_idx] = 1;
                log_count++;
                fprintf(stderr, "[voodoo] First write to reg 0x%03X = 0x%08X\n", reg, value);
            }
        }
        break;
    }
}

void voodoo_test_pattern(voodoo_state_t* voodoo) {
    int w = voodoo->width > 0 ? voodoo->width : 512;
    int h = voodoo->height > 0 ? voodoo->height : 384;
    if (w > 640) w = 640;
    if (h > 480) h = 480;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            uint16_t pixel;
            /* Draw color bars with a border */
            if (x < 2 || x >= w-2 || y < 2 || y >= h-2) {
                /* White border */
                pixel = 0xFFFF;
            } else {
                /* 8 vertical color bars */
                int bar = (x * 8) / w;
                switch (bar) {
                    case 0: pixel = 0xFFFF; break; /* white */
                    case 1: pixel = 0xFFE0; break; /* yellow */
                    case 2: pixel = 0x07FF; break; /* cyan */
                    case 3: pixel = 0x07E0; break; /* green */
                    case 4: pixel = 0xF81F; break; /* magenta */
                    case 5: pixel = 0xF800; break; /* red */
                    case 6: pixel = 0x001F; break; /* blue */
                    default: pixel = 0x0000; break; /* black */
                }
                /* Gradient overlay in bottom quarter */
                if (y > h * 3 / 4) {
                    int gray = (x * 31) / w;
                    pixel = (gray << 11) | (gray * 2 << 5) | gray;
                }
            }
            voodoo->framebuffer[y * 640 + x] = pixel;
        }
    }
    fprintf(stderr, "[voodoo] Test pattern written: %dx%d\n", w, h);
}

int voodoo_process_cmdlist(voodoo_state_t* voodoo, const uint8_t* rdram,
                           uint32_t list_phys, int max_cmds) {
    int count = 0;
    /* DMA command list format (Midway Seattle):
     * Word 0: number of register writes (N)
     * Words 1..2N: pairs of (register_byte_offset, value) */
    if (list_phys >= 0x00800000 - 8) return 0;

    uint32_t n_cmds = *(uint32_t*)(rdram + list_phys);
    if (n_cmds == 0 || n_cmds > (uint32_t)max_cmds) {
        /* Might be raw register/value pairs terminated by sentinel */
        /* Try alternative format: pairs until 0xFFFFFFFF or max */
        for (int i = 0; i < max_cmds; i++) {
            uint32_t off = list_phys + i * 8;
            if (off + 8 > 0x00800000) break;
            uint32_t reg = *(uint32_t*)(rdram + off);
            uint32_t val = *(uint32_t*)(rdram + off + 4);
            if (reg == 0xFFFFFFFF) break;
            if (reg < 0x400) { /* Valid Voodoo register range */
                voodoo_write(voodoo, reg, val);
                count++;
            } else {
                break; /* Invalid register = end of list */
            }
        }
        return count;
    }

    /* Standard format: count + pairs */
    for (uint32_t i = 0; i < n_cmds && i < (uint32_t)max_cmds; i++) {
        uint32_t off = list_phys + 4 + i * 8;
        if (off + 8 > 0x00800000) break;
        uint32_t reg = *(uint32_t*)(rdram + off);
        uint32_t val = *(uint32_t*)(rdram + off + 4);
        voodoo_write(voodoo, reg, val);
        count++;
    }
    return count;
}
