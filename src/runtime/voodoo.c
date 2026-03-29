/**
 * Minimal 3DFX Voodoo 1 register emulation for CarnEvil.
 *
 * This doesn't render anything yet -- it just responds to register
 * reads/writes so the game's Voodoo init sequence completes and the
 * game progresses past hardware checks.
 */

#include "voodoo.h"
#include <stdio.h>
#include <string.h>

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

uint32_t voodoo_get_write_count(void) { return vwrite_total; }
uint32_t voodoo_get_nonzero_count(void) { return vwrite_nonzero; }

void voodoo_write(voodoo_state_t* voodoo, uint32_t offset, uint32_t value) {
    uint32_t reg = offset & 0x3FF;
    vwrite_total++;
    if (value != 0) vwrite_nonzero++;

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
        if (voodoo->swap_count <= 5 || voodoo->swap_count % 100 == 0) {
            fprintf(stderr, "[voodoo] SwapBuffers #%d\n", voodoo->swap_count);
        }
        break;

    case VOODOO_NOPCOMMAND:
        break;

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
        for (int y = top; y < bottom; y++) {
            for (int x = left; x < right; x++) {
                voodoo->framebuffer[y * 640 + x] = fill_color;
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

    /* Handle LFB writes (framebuffer access) */
    if (offset >= VOODOO_LFB_OFFSET && offset < VOODOO_TEX_OFFSET) {
        uint32_t fb_offset = offset - VOODOO_LFB_OFFSET;
        /* Each LFB write covers 2 pixels (32 bits = two 16-bit RGB565 pixels) */
        uint32_t pixel_index = fb_offset / 2;
        if (pixel_index < 640 * 480) {
            voodoo->framebuffer[pixel_index] = (uint16_t)value;
            if (pixel_index + 1 < 640 * 480) {
                voodoo->framebuffer[pixel_index + 1] = (uint16_t)(value >> 16);
            }
        }
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
