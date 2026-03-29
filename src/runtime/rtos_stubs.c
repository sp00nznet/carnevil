/**
 * Stubs for RTOS functions that the recompiler references but didn't generate.
 * These are typically exception handlers, TLB routines, or interrupt code
 * that doesn't apply to the recompiled environment.
 */

#include "recomp.h"
#include <stdio.h>

/* Exception/interrupt handlers - not needed in recomp */
RECOMP_FUNC void static_0_80005020(uint8_t* rdram, recomp_context* ctx) {
    /* RTOS interrupt enable/disable */
    ctx->r2 = 0;
}

RECOMP_FUNC void static_0_80004FF0(uint8_t* rdram, recomp_context* ctx) {
    /* RTOS critical section enter */
    ctx->r2 = 0;
}

RECOMP_FUNC void static_0_80004FF8(uint8_t* rdram, recomp_context* ctx) {
    /* RTOS critical section exit */
    ctx->r2 = 0;
}

RECOMP_FUNC void static_0_800042D0(uint8_t* rdram, recomp_context* ctx) {
    /* RTOS memset-like function */
    /* a0=dest, a1=fill_byte, a2=count */
    uint32_t dest = (uint32_t)ctx->r4 & 0x1FFFFFFF;
    uint8_t fill = (uint8_t)ctx->r5;
    uint32_t count = (uint32_t)ctx->r6;
    if (dest + count <= 0x00800000) {
        memset(rdram + dest, fill, count);
    }
}

RECOMP_FUNC void static_0_8000BAC0(uint8_t* rdram, recomp_context* ctx) {
    /* RTOS DMA transfer stub */
    ctx->r2 = 0;
}

RECOMP_FUNC void static_0_80004DD0(uint8_t* rdram, recomp_context* ctx) {
    /* RTOS timer/delay */
    ctx->r2 = 0;
}

RECOMP_FUNC void static_0_80004D68(uint8_t* rdram, recomp_context* ctx) {
    /* RTOS timer read */
    static uint32_t timer = 0;
    timer += 1000;
    ctx->r2 = (gpr)(int32_t)timer;
}

RECOMP_FUNC void static_0_80011B9C(uint8_t* rdram, recomp_context* ctx) {
    /* RTOS video sync stub */
    ctx->r2 = 0;
}

RECOMP_FUNC void static_0_80004E78(uint8_t* rdram, recomp_context* ctx) {
    /* RTOS sleep/yield */
    ctx->r2 = 0;
}

/* Additional RTOS stubs for vector table targets that weren't in the recompiled output */
RECOMP_FUNC void rtos_80004E3C(uint8_t* rdram, recomp_context* ctx) {
    /* Timer/delay function */
    ctx->r2 = 0;
}

RECOMP_FUNC void rtos_80005048(uint8_t* rdram, recomp_context* ctx) {
    /* Interrupt enable/disable */
    ctx->r2 = 0;
}

/* These "DMA" functions are actually RTOS callback/interrupt registration:
 * rtos_800060BC: Register a callback function on a channel
 *   a0 = channel/slot number
 *   a1 = callback function pointer
 *   a2 = enable flag (1=enable, 0=register-only)
 * rtos_80006200: Register frame-done callback
 * rtos_80006210: Register DMA-complete callback
 *
 * We store the callbacks and invoke them during frame processing. */

extern recomp_func_t* get_function(int32_t vram);

#define MAX_RTOS_CALLBACKS 8
#define MAX_CBS_PER_SLOT 4
static struct {
    uint32_t func_vram[MAX_CBS_PER_SLOT];
    int count;
    int enabled;
} rtos_callbacks[MAX_RTOS_CALLBACKS];
static uint32_t rtos_frame_done_cb = 0;
static uint32_t rtos_dma_done_cb = 0;

RECOMP_FUNC void rtos_800060BC(uint8_t* rdram, recomp_context* ctx) {
    uint32_t slot = (uint32_t)ctx->r4;
    uint32_t func = (uint32_t)ctx->r5;
    uint32_t enable = (uint32_t)ctx->r6;

    static int c = 0; c++;
    if (c <= 10) fprintf(stderr, "[rtos_cb] Register slot %d: func=0x%08X enable=%d\n",
        slot, func, enable);

    /* Store callback in RDRAM at 0x800146E0 + slot*4 (matching real RTOS behavior).
     * The real rtos_800060BC stores at: *(0x80010000 + 18144 + slot*4) = func */
    if (slot < 8) {
        uint32_t tbl_phys = 0x000146E0 + slot * 4;
        if (tbl_phys + 4 < 0x00800000) {
            *(uint32_t*)(rdram + tbl_phys) = func;
        }
    }

    /* Also store in our C-side tracking array */
    if (slot < MAX_RTOS_CALLBACKS) {
        int idx = rtos_callbacks[slot].count;
        if (idx < MAX_CBS_PER_SLOT) {
            rtos_callbacks[slot].func_vram[idx] = func;
            rtos_callbacks[slot].count++;
        }
        if (enable) rtos_callbacks[slot].enabled = 1;
    }
    ctx->r2 = 0;
}

/* rtos_8000619C exists in RTOS recomp - stub removed */

RECOMP_FUNC void rtos_80006200(uint8_t* rdram, recomp_context* ctx) {
    rtos_frame_done_cb = (uint32_t)ctx->r4;
    static int c = 0; c++;
    if (c <= 3) fprintf(stderr, "[rtos_cb] Frame-done callback: 0x%08X\n", rtos_frame_done_cb);
    ctx->r2 = 0;
}

RECOMP_FUNC void rtos_80006210(uint8_t* rdram, recomp_context* ctx) {
    rtos_dma_done_cb = (uint32_t)ctx->r4;
    static int c = 0; c++;
    if (c <= 3) fprintf(stderr, "[rtos_cb] DMA-done callback: 0x%08X\n", rtos_dma_done_cb);
    ctx->r2 = 0;
}

/* Call all registered rendering callbacks. Called from the frame loop. */
void rtos_run_callbacks(uint8_t* rdram) {
    recomp_context cb_ctx = {0};
    static int run_count = 0;
    run_count++;

    /* Call enabled slot callbacks (all registered functions per slot) */
    for (int i = 0; i < MAX_RTOS_CALLBACKS; i++) {
        if (rtos_callbacks[i].enabled) {
            for (int j = 0; j < rtos_callbacks[i].count; j++) {
                uint32_t fv = rtos_callbacks[i].func_vram[j];
                if (fv) {
                    recomp_func_t* f = get_function(fv);
                    if (f) {
                        cb_ctx.r4 = (gpr)i;
                        f(rdram, &cb_ctx);
                    }
                }
            }
        }
    }

    /* Also call VEC[64]-registered callbacks from the RTOS function table at 0x800146A0.
     * These are VBlank/timer callbacks that the game needs for state machine transitions. */
    for (int i = 0; i < 8; i++) {
        uint32_t fv = *(uint32_t*)(rdram + 0x000146A0 + i * 4);
        if (fv >= 0x800C0000 && fv < 0x801C0000) {
            recomp_func_t* f = get_function(fv);
            if (f) {
                cb_ctx.r4 = (gpr)i;
                f(rdram, &cb_ctx);
            }
        }
    }

    /* Call frame-done callback */
    if (rtos_frame_done_cb) {
        recomp_func_t* f = get_function(rtos_frame_done_cb);
        if (f) {
            if (run_count <= 3) fprintf(stderr, "[rtos_cb] Calling frame-done 0x%08X\n", rtos_frame_done_cb);
            f(rdram, &cb_ctx);
        }
    }
}

/* rtos_80006220 exists in RTOS recomp - stub removed */

/* rtos_80006754 exists in RTOS recomp - stub removed */

/* rtos_80009A80 exists in RTOS recomp - stub removed */

/* rtos_80009C9C exists in RTOS recomp - stub removed */

/* rtos_80009CBC exists in RTOS recomp - stub removed */

RECOMP_FUNC void rtos_8000EE30(uint8_t* rdram, recomp_context* ctx) {
    /* vec[53] = main_init: returns pointer to RTOS config struct at 0x800B6E30.
     * The entry point passes this to main_loop as a0.
     * The config struct contains display settings, device handles, etc.
     * Allocate it at physical 0x0B6E30 (RTOS BSS area). */
    uint32_t config_phys = 0x000B6E30;
    if (config_phys + 256 < 0x00800000) {
        /* Initialize with reasonable defaults if not already set */
        if (*(uint32_t*)(rdram + config_phys) == 0) {
            memset(rdram + config_phys, 0, 256);
            fprintf(stderr, "[rtos] rtos_8000EE30: initialized config at 0x800B6E30\n");
        }
    }
    ctx->r2 = (gpr)(int32_t)0x800B6E30;
}

/* Override PCI config write - bypass RTOS Galileo checks */
RECOMP_FUNC void rtos_8000F84C(uint8_t* rdram, recomp_context* ctx) {
    uint32_t addr_param = (uint32_t)ctx->r4;
    uint32_t value = (uint32_t)ctx->r6;
    uint32_t dev = (addr_param >> 11) & 0x1F;
    uint32_t reg = addr_param & 0xFC;
    uint32_t pci_addr = 0x80000000 | (dev << 11) | reg;

    static int c = 0; c++;
    if (c <= 20)
        fprintf(stderr, "[pci_ovr] Write: dev=%d reg=0x%02X val=0x%08X\n", dev, reg, value);

    extern void seattle_io_write32(uint32_t, uint32_t);
    seattle_io_write32(0x0C000CF8, pci_addr);
    seattle_io_write32(0x0C000CFC, value);
    ctx->r2 = 0;
}

/* Override PCI config read */
RECOMP_FUNC void rtos_8000F774(uint8_t* rdram, recomp_context* ctx) {
    uint32_t addr_param = (uint32_t)ctx->r4;
    uint32_t dev = (addr_param >> 11) & 0x1F;
    uint32_t reg = addr_param & 0xFC;
    uint32_t pci_addr = 0x80000000 | (dev << 11) | reg;

    extern void seattle_io_write32(uint32_t, uint32_t);
    extern uint32_t seattle_io_read32(uint32_t);
    seattle_io_write32(0x0C000CF8, pci_addr);
    ctx->r2 = (gpr)(int32_t)seattle_io_read32(0x0C000CFC);

    static int c = 0; c++;
    if (c <= 20)
        fprintf(stderr, "[pci_ovr] Read: dev=%d reg=0x%02X -> 0x%08X\n", dev, reg, (uint32_t)ctx->r2);
}

/* Instrumented wrapper for rtos_80006220 (VEC[64] = device callback registration)
 * This is the REAL function that populates 0x800146A0 function pointer table. */
extern RECOMP_FUNC void rtos_80006220(uint8_t* rdram, recomp_context* ctx);
void rtos_80006220_wrapper(uint8_t* rdram, recomp_context* ctx) {
    static int c = 0; c++;
    uint32_t slot = (uint32_t)ctx->r4;
    uint32_t func = (uint32_t)ctx->r5;
    
    /* Log before */
    if (c <= 10) {
        uint32_t tbl_before = *(uint32_t*)(rdram + 0x000146A0 + (slot < 8 ? slot * 4 : 0));
        fprintf(stderr, "[rtos_reg] VEC[64] slot=%d func=0x%08X (before: tbl=0x%08X) #%d\n",
                slot, func, tbl_before, c);
    }
    
    /* Call the real function */
    rtos_80006220(rdram, ctx);
    
    /* Log after */
    if (c <= 10) {
        uint32_t tbl_after = *(uint32_t*)(rdram + 0x000146A0 + (slot < 8 ? slot * 4 : 0));
        fprintf(stderr, "[rtos_reg] VEC[64] AFTER: tbl[%d]=0x%08X r2=0x%08X\n",
                slot, tbl_after, (uint32_t)ctx->r2);
    }
}
