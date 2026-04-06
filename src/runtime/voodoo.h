/**
 * 3DFX Voodoo 1 register definitions and minimal emulation.
 * Based on the Voodoo 1/2 register spec and MAME's voodoo_pci.cpp.
 */

#ifndef CARNEVIL_VOODOO_H
#define CARNEVIL_VOODOO_H

#include <stdint.h>

/* Register offsets (byte offsets from Voodoo PCI BAR0)
 * From MAME voodoo_regs.h - Voodoo 1 register map */
#define VOODOO_STATUS       0x000  /* R: status */
#define VOODOO_INTRCTRL     0x004  /* RW: interrupt control (V2+) */

/* Vertex/triangle setup registers (integer format) */
#define VOODOO_SVX          0x048  /* W: start vertex X (12.4 fixed) */
#define VOODOO_SVY          0x04C  /* W: start vertex Y (12.4 fixed) */
#define VOODOO_SRED         0x050  /* W: start Red (12.12 fixed) */
#define VOODOO_SGREEN       0x054  /* W: start Green */
#define VOODOO_SBLUE        0x058  /* W: start Blue */
#define VOODOO_SALPHA       0x05C  /* W: start Alpha */

/* Vertex/triangle setup registers (float format) */
#define VOODOO_FVX          0x180  /* W: start vertex X (IEEE float) */
#define VOODOO_FVY          0x184  /* W: start vertex Y */
#define VOODOO_FRED         0x188  /* W: start Red */
#define VOODOO_FGREEN       0x18C  /* W: start Green */
#define VOODOO_FBLUE        0x190  /* W: start Blue */
#define VOODOO_FALPHA       0x194  /* W: start Alpha */

/* Rendering command registers */
#define VOODOO_TRIANGLECMD  0x080  /* W: triangle command */
#define VOODOO_FTRIANGLE    0x100  /* W: float triangle command */
#define VOODOO_FBZCOLORPATH 0x104  /* RW: color path config */
#define VOODOO_FOGMODE      0x108  /* RW: fog mode */
#define VOODOO_ALPHAMODE    0x10C  /* RW: alpha test/blend */
#define VOODOO_FBZMODE      0x110  /* RW: framebuffer/Z config */
#define VOODOO_LFBMODE      0x114  /* RW: LFB access mode */
#define VOODOO_CLIPLR       0x118  /* RW: clip left/right */
#define VOODOO_CLIPLOHI     0x11C  /* RW: clip low/high Y */
#define VOODOO_NOPCOMMAND   0x120  /* W: NOP (FIFO flush) */
#define VOODOO_FASTFILLCMD  0x124  /* W: fast fill command */
#define VOODOO_SWAPBUFCMD   0x128  /* W: swap buffer command */
#define VOODOO_FOGCOLOR     0x12C  /* RW: fog color */
#define VOODOO_ZACOLOR      0x130  /* RW: Z/alpha color */
#define VOODOO_CHROMAKEY    0x134  /* RW: chroma key */
#define VOODOO_CHROMARANGE  0x138  /* RW: chroma range (V2+) */
#define VOODOO_STIPPLE      0x140  /* RW: stipple pattern */
#define VOODOO_COLOR0       0x144  /* RW: color 0 */
#define VOODOO_COLOR1       0x148  /* RW: color 1 */
#define VOODOO_FBIPIXELSIN  0x14C  /* R: pixels in counter */

/* Init registers */
#define VOODOO_FBIINIT4     0x200  /* RW: FBI init 4 */
#define VOODOO_VRETRACE     0x204  /* R: vertical retrace */
#define VOODOO_BACKPORCH    0x208  /* RW: back porch */
#define VOODOO_VIDEODIM     0x20C  /* RW: video dimensions */
#define VOODOO_FBIINIT0     0x210  /* RW: FBI init 0 */
#define VOODOO_FBIINIT1     0x214  /* RW: FBI init 1 */
#define VOODOO_FBIINIT2     0x218  /* RW: FBI init 2 */
#define VOODOO_FBIINIT3     0x21C  /* RW: FBI init 3 */
#define VOODOO_HSYNC        0x220  /* W: horizontal sync */
#define VOODOO_VSYNC        0x224  /* W: vertical sync */
#define VOODOO_CLUTDATA     0x228  /* W: CLUT data */
#define VOODOO_DACDATA      0x22C  /* W: DAC data */

/* TMU registers (offsets within TMU address space) */
#define VOODOO_TEXMODE      0x300  /* W: texture mode */
#define VOODOO_TLOD         0x304  /* W: texture LOD */
#define VOODOO_TDETAIL      0x308  /* W: texture detail */
#define VOODOO_TEXBASE      0x30C  /* W: texture base address */
#define VOODOO_TREXINIT0    0x31C  /* W: TMU init 0 */
#define VOODOO_TREXINIT1    0x320  /* W: TMU init 1 */

/* Framebuffer access: Voodoo LFB starts at base + 0x400000 (4MB offset) */
#define VOODOO_LFB_OFFSET   0x400000
/* Texture memory starts at base + 0x800000 (8MB offset) */
#define VOODOO_TEX_OFFSET   0x800000

/* Status register bits */
#define VOODOO_STATUS_FBI_BUSY     (1 << 7)
#define VOODOO_STATUS_TREX_BUSY    (1 << 8)
#define VOODOO_STATUS_SST_BUSY     (1 << 9)
#define VOODOO_STATUS_PCIFIFO_FREE_MASK  0x3F
#define VOODOO_STATUS_MEMFIFO_FREE_SHIFT 12
#define VOODOO_STATUS_RETRACE      (1 << 6)
#define VOODOO_STATUS_SWAPBUF_PEND (1 << 28)

/* Voodoo state */
typedef struct {
    /* Registers */
    uint32_t status;
    uint32_t fbiInit0;
    uint32_t fbiInit1;
    uint32_t fbiInit2;
    uint32_t fbiInit3;
    uint32_t fbiInit4;
    uint32_t videoDim;
    uint32_t backporch;
    uint32_t regs[256];  /* Generic register storage */

    /* Double-buffered framebuffer - 640x480 RGB565 */
    uint16_t framebuffer[640 * 480];   /* front buffer (displayed) */
    uint16_t backbuffer[640 * 480];    /* back buffer (rendered to) */
    int swap_count;
    int frame_count;

    /* Configuration */
    int width;
    int height;
    int initialized;
} voodoo_state_t;

void     voodoo_init(voodoo_state_t* voodoo);
uint32_t voodoo_read(voodoo_state_t* voodoo, uint32_t offset);
void     voodoo_write(voodoo_state_t* voodoo, uint32_t offset, uint32_t value);

/* Write a test pattern to the framebuffer to verify output pipeline */
void     voodoo_test_pattern(voodoo_state_t* voodoo);

/* Process a Voodoo DMA command list from game RAM.
 * Format: sequence of (register_offset, value) pairs, terminated by 0xFFFFFFFF.
 * Returns number of register writes performed. */
int      voodoo_process_cmdlist(voodoo_state_t* voodoo, const uint8_t* rdram,
                                uint32_t list_phys, int max_cmds);

#endif
