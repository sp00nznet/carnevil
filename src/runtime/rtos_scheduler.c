/**
 * CarnEvil RTOS Cooperative Scheduler - Windows Fiber Implementation
 *
 * Uses Windows Fibers for cooperative multitasking between game tasks.
 */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "rtos_scheduler.h"
#include <stdio.h>
#include <string.h>

rtos_scheduler_t g_scheduler;

/* Forward declaration: fiber entry point */
static void CALLBACK fiber_entry(void* param);

void rtos_sched_init(rtos_scheduler_t* sched, uint8_t* rdram) {
    memset(sched, 0, sizeof(*sched));
    sched->rdram = rdram;
    sched->current_fiber = -1;

#ifdef _WIN32
    /* Convert the main thread to a fiber so we can switch away from it */
    sched->main_fiber = ConvertThreadToFiber(NULL);
    if (!sched->main_fiber) {
        fprintf(stderr, "[rtos] Failed to convert main thread to fiber (error %lu)\n", GetLastError());
    }
#endif
}

int rtos_sched_create_task(rtos_scheduler_t* sched, int task_id, uint32_t callback_vram) {
    if (sched->fiber_count >= RTOS_MAX_FIBERS) {
        /* Try to reactivate an existing inactive fiber with same callback */
        for (int i = 0; i < sched->fiber_count; i++) {
            if (sched->fibers[i].callback_vram == callback_vram && !sched->fibers[i].active) {
                sched->fibers[i].active = 1;
                sched->fibers[i].blocked = 0;
                sched->fibers[i].id = task_id;
                return i;
            }
        }
        return -1;
    }

    int idx = sched->fiber_count++;
    rtos_fiber_t* f = &sched->fibers[idx];
    f->id = task_id;
    f->callback_vram = callback_vram;
    f->active = 1;
    f->blocked = 0;

#ifdef _WIN32
    f->fiber = CreateFiber(RTOS_FIBER_STACK_SIZE, fiber_entry, (void*)(intptr_t)idx);
    if (!f->fiber) {
        fprintf(stderr, "[rtos] Failed to create fiber for task %d (error %lu)\n", task_id, GetLastError());
        f->active = 0;
        return -1;
    }
#endif

    return idx;
}

/* Fiber entry point: runs the task callback in a loop */
static void CALLBACK fiber_entry(void* param) {
    int idx = (int)(intptr_t)param;
    rtos_scheduler_t* sched = &g_scheduler;
    rtos_fiber_t* f = &sched->fibers[idx];

    /* Get the task's callback function */
    extern recomp_func_t* get_function(int32_t vram);
    recomp_func_t* callback = get_function((int32_t)f->callback_vram);

    if (!callback) {
        fprintf(stderr, "[rtos] Fiber %d: no callback for 0x%08X\n", idx, f->callback_vram);
        f->active = 0;
#ifdef _WIN32
        SwitchToFiber(sched->main_fiber);
#endif
        return;
    }

    /* Set up per-task stack in game RAM.
     * Use the very top of RAM (above heap's 0x807FBFF8 end).
     * Each task gets 4KB, stacks grow downward.
     * Total: 16 tasks * 4KB = 64KB at 0x807F0000-0x807FFFFF */
    uint32_t stack_top = 0x80800000 - (idx * 0x1000); /* 4KB per task */
    f->ctx_save.r29 = (gpr)stack_top;
    f->ctx_save.mips3_float_mode = 1;

    /* Initialize GP (r28) from the tracked render buffer address.
     * The rendering functions use GP as a write cursor for Voodoo DMA commands.
     * Also set the game's standard global pointer value for non-rendering code. */
    {
        extern uint32_t g_render_buffer_addr;
        if (g_render_buffer_addr != 0) {
            f->ctx_save.r28 = (gpr)g_render_buffer_addr;
        }
    }

    /* Set up per-task context data in game RAM.
     * Allocate 512 bytes per task below the stack area.
     * Pre-fill with indices so message_send passes non-zero values. */
    uint32_t task_ctx_addr = stack_top - 0x200; /* 512 bytes below stack top */

    /* Initialize the rendering context struct with Voodoo config data.
     * On real hardware, the RTOS boot code allocates DMA command lists
     * and stores pointers here. We populate the fields directly with
     * Voodoo register values that the DMA consumer will process. */
    rtos_init_task_context(sched->rdram, (task_ctx_addr & 0x1FFFFFFF));

    /* Run the callback repeatedly.
     * func_80144EB8 (exit_task) calls longjmp to return here,
     * bypassing the infinite loop in the recompiled code. */
    while (f->active) {
        f->ctx_save.r4 = (gpr)f->id;
        f->ctx_save.r5 = (gpr)task_ctx_addr;
        f->ctx_save.r29 = (gpr)(stack_top - 0x200);
        f->ctx_save.mips3_float_mode = 1;

        callback(sched->rdram, &f->ctx_save);

        /* Block until next frame */
        f->blocked = 1;
#ifdef _WIN32
        SwitchToFiber(sched->main_fiber);
#endif
    }

    /* If we get here, the fiber is done */
#ifdef _WIN32
    SwitchToFiber(sched->main_fiber);
#endif
}

void rtos_sched_run_frame(rtos_scheduler_t* sched, recomp_context* ctx) {
    /* Unblock all active fibers at frame start */
    extern uint32_t g_render_buffer_addr;
    for (int i = 0; i < sched->fiber_count; i++) {
        if (sched->fibers[i].active) {
            sched->fibers[i].blocked = 0;
            sched->fibers[i].ctx_save.mips3_float_mode = ctx->mips3_float_mode;
            /* Reset GP to rendering buffer start each frame.
             * The rendering functions advance GP as they write commands. */
            if (g_render_buffer_addr != 0) {
                sched->fibers[i].ctx_save.r28 = (gpr)g_render_buffer_addr;
            }
        }
    }

    /* Run fibers round-robin until all are blocked */
    int max_iterations = sched->fiber_count * 4; /* Safety limit */
    int iterations = 0;

    while (iterations < max_iterations) {
        int any_ran = 0;

        for (int i = 0; i < sched->fiber_count; i++) {
            rtos_fiber_t* f = &sched->fibers[i];
            if (!f->active || f->blocked) continue;

            sched->current_fiber = i;
            any_ran = 1;
            iterations++;

#ifdef _WIN32
            SwitchToFiber(f->fiber);
#endif
        }

        if (!any_ran) break; /* All fibers blocked */
    }

    sched->current_fiber = -1;
}

void rtos_sched_yield(rtos_scheduler_t* sched, int channel) {
    int idx = sched->current_fiber;
    if (idx < 0 || idx >= sched->fiber_count) return;

    rtos_fiber_t* f = &sched->fibers[idx];
    f->blocked = 1;
    f->blocked_channel = channel;

#ifdef _WIN32
    SwitchToFiber(sched->main_fiber);
#endif
}

void rtos_sched_wake(rtos_scheduler_t* sched, int channel) {
    /* Wake any fiber blocked on this channel */
    for (int i = 0; i < sched->fiber_count; i++) {
        rtos_fiber_t* f = &sched->fibers[i];
        if (f->active && f->blocked && f->blocked_channel == channel) {
            f->blocked = 0;
        }
    }
}

/* Context struct field mapping (from func_800C47E0 disassembly):
 *
 * Offset -> Channel -> Purpose (Voodoo DMA)
 * +0x10  -> 0x85    -> fbzColorPath (color pipeline config)
 * +0x18  -> 0x86    -> fogMode
 * +0x20  -> 0x87    -> alphaMode
 * +0x28  -> 0x88    -> fbzMode (framebuffer/Z config)
 * +0x30  -> 0x89    -> lfbMode (linear framebuffer access mode)
 * +0x38  -> 0x8A    -> clipLeftRight
 * +0x40  -> 0x8B    -> clipLowYHighY
 * +0x48  -> 0x8C    -> nopCMD (FIFO sync/flush)
 * +0x50  -> 0x8D    -> fogColor
 * +0x58  -> 0x8E    -> zaColor (Z/alpha reference)
 * +0x60  -> 0x8F    -> chromaKey
 * +0x68  -> 0x90    -> chromaRange
 * +0x70  -> 0x91    -> stipple
 * +0x78  -> 0x92    -> color0
 * +0xC0  -> 0x93    -> color1
 * +0xC8  -> 0x94    -> fbiPixelsIn (perf counter - clear)
 * +0x80  -> 0x95    -> textureMode (TMU0)
 * +0x88  -> 0x96    -> tLOD (TMU0 LOD control)
 * +0x90  -> 0x97    -> tDetail (TMU0 detail)
 * +0x98  -> 0x98    -> texBaseAddr (TMU0 base address)
 * +0xA0  -> 0x99    -> texBaseAddr_1 (TMU0 LOD 1)
 * +0xA8  -> 0x9A    -> texBaseAddr_2 (TMU0 LOD 2)
 * +0xB0  -> 0x9B    -> texBaseAddr_3_8 (TMU0 LOD 3-8)
 * +0xB8  -> 0x9C    -> trexInit0 (TMU init)
 * +0xF0  -> 0x9D    -> trexInit1 (TMU init)
 * +0xE8  -> 0x9E    -> reserved/nop
 * +0xF8  -> 0x9F    -> reserved/nop
 * +0x110 -> dynamic -> task-specific data (display list pointer)
 *
 * Channel 0xA2 gets its value from a global, not the context struct.
 */

void rtos_init_task_context(uint8_t* rdram, uint32_t ctx_phys) {
    if (ctx_phys + 0x120 > 0x00800000) return;
    uint32_t* ctx = (uint32_t*)(rdram + ctx_phys);

    /* CarnEvil: 512x384 @ 16bpp RGB565, Voodoo 1 with 2MB framebuffer
     * Framebuffer layout:
     *   Front: 0x000000  (512*384*2 = 0x60000)
     *   Back:  0x060000
     *   Aux/Z: 0x0C0000  (512*384*2 = 0x60000)
     * Stride: 512 pixels = 1024 bytes per row
     */

    /* +0x10 -> ch 0x85: fbzColorPath
     * Bits [1:0] = RGB select: 0=iterated, 1=texture, 2=color1
     * Bit 2 = alpha select: 0=iterated
     * Bits [7:4] = cc_localselect
     * Bits [16:14] = cca_localselect
     * Bit 13 = cc_zero_other: 1=start from zero, then add local*other
     * For basic rendering: use iterated colors */
    ctx[0x10/4] = 0x00000009;  /* RGB=iterated, cc_localselect=iterated, simple path */

    /* +0x18 -> ch 0x86: fogMode - disabled */
    ctx[0x18/4] = 0x00000000;

    /* +0x20 -> ch 0x87: alphaMode
     * Bits [3:0] = alpha test function (0=never, 4=less, 8=always)
     * Bits [7:4] = alpha test reference
     * Bits [12:8] = src blend factor (4=srcAlpha, 1=one)
     * Bits [16:13] = dst blend factor (5=1-srcAlpha, 0=zero)
     * Bit 0 = enable alpha test
     * For now: alpha test=always, blend disabled */
    ctx[0x20/4] = 0x00000000;

    /* +0x28 -> ch 0x88: fbzMode
     * Bit 0 = enable clipping
     * Bit 1 = enable chroma key
     * Bit 2 = enable stipple
     * Bit 4 = enable depth buffer writes
     * Bits [7:5] = depth function (4=less)
     * Bit 8 = enable dithering
     * Bit 9 = RGB buffer writes
     * For basic rendering: clip + depth(less) + dither + RGB writes */
    ctx[0x28/4] = 0x00000301;  /* clip=1, depth_write=0, dither=1, rgb_write=1 */

    /* +0x30 -> ch 0x89: lfbMode - LFB write format RGB565 */
    ctx[0x30/4] = 0x00000000;

    /* +0x38 -> ch 0x8A: clipLeftRight
     * Bits [9:0] = right clip (exclusive), Bits [25:16] = left clip */
    ctx[0x38/4] = (0 << 16) | 512;

    /* +0x40 -> ch 0x8B: clipLowYHighY
     * Bits [9:0] = high Y clip (exclusive), Bits [25:16] = low Y clip */
    ctx[0x40/4] = (0 << 16) | 384;

    /* +0x48 -> ch 0x8C: nopCMD */
    ctx[0x48/4] = 0x00000000;

    /* +0x50 -> ch 0x8D: fogColor */
    ctx[0x50/4] = 0x00000000;

    /* +0x58 -> ch 0x8E: zaColor */
    ctx[0x58/4] = 0x00000000;

    /* +0x60 -> ch 0x8F: chromaKey */
    ctx[0x60/4] = 0x00000000;

    /* +0x68 -> ch 0x90: chromaRange */
    ctx[0x68/4] = 0x00000000;

    /* +0x70 -> ch 0x91: stipple */
    ctx[0x70/4] = 0x00000000;

    /* +0x78 -> ch 0x92: color0 */
    ctx[0x78/4] = 0x00000000;

    /* +0xC0 -> ch 0x93: color1 */
    ctx[0xC0/4] = 0x00000000;

    /* +0xC8 -> ch 0x94: fbiPixelsIn (performance counter) */
    ctx[0xC8/4] = 0x00000000;

    /* +0x80 -> ch 0x95: textureMode TMU0 */
    ctx[0x80/4] = 0x00000000;

    /* +0x88 -> ch 0x96: tLOD */
    ctx[0x88/4] = 0x00000000;

    /* +0x90 -> ch 0x97: tDetail */
    ctx[0x90/4] = 0x00000000;

    /* +0x98 -> ch 0x98: texBaseAddr */
    ctx[0x98/4] = 0x00000000;

    /* +0xA0 -> ch 0x99: texBaseAddr_1 */
    ctx[0xA0/4] = 0x00000000;

    /* +0xA8 -> ch 0x9A: texBaseAddr_2 */
    ctx[0xA8/4] = 0x00000000;

    /* +0xB0 -> ch 0x9B: texBaseAddr_3_8 */
    ctx[0xB0/4] = 0x00000000;

    /* +0xB8 -> ch 0x9C: trexInit0 */
    ctx[0xB8/4] = 0x00000000;

    /* +0xF0 -> ch 0x9D: trexInit1 */
    ctx[0xF0/4] = 0x00000000;

    /* +0xE8 -> ch 0x9E: reserved */
    ctx[0xE8/4] = 0x00000000;

    /* +0xF8 -> ch 0x9F: reserved */
    ctx[0xF8/4] = 0x00000000;

    /* +0x110 -> dynamic channel value (display list pointer) */
    ctx[0x110/4] = 0x00000001; /* non-zero = "I have a valid context" */

    fprintf(stderr, "[rtos] Initialized task context at phys 0x%06X\n", ctx_phys);
}
