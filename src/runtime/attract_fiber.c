/*
 * Attract-mode main-loop fiber -- see attract_fiber.h for the rationale.
 */
#include "attract_fiber.h"
#include "recomp.h"
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

/* Default OFF: the build keeps the proven escape-based path until this is
 * validated. Flip to 1 (here or at runtime before the frame loop) to drive
 * func_800C50AC through the fiber instead. */
int g_use_attract_fiber = 0;

#ifdef _WIN32

/* Host stack for the attract fiber. The recompiled attract state machine
 * recurses (state -> func_800F1690 -> func_800F1550 -> event_wait), so give it
 * a comfortable host stack -- 1 MB, 4x the per-task fiber stacks. */
#define ATTRACT_FIBER_STACK (1024 * 1024)

/* Dedicated MIPS stack top for the attract loop, in game RAM. Sits below the
 * callback stack (0x807EF000) and the per-task fiber stacks (0x807F0000+), and
 * well above the heap head (~0x806Fxxxx), so it does not collide. */
#define ATTRACT_MIPS_SP  0x807E0000u

static void*   s_fiber   = NULL;   /* the attract loop fiber */
static void*   s_resumer = NULL;   /* fiber to switch back to (the main fiber) */
static int     s_in_fiber = 0;     /* 1 while the attract fiber is executing */
static int     s_done     = 0;     /* func_800C50AC returned (it normally won't) */
static int     s_started  = 0;     /* fiber created + ctx initialised */
static recomp_context s_ctx;       /* persistent MIPS context for the loop */
static uint8_t* s_rdram   = NULL;

extern void func_800C50AC(uint8_t* rdram, recomp_context* ctx);
extern uint32_t g_render_buffer_addr;

static void CALLBACK attract_entry(void* param) {
    (void)param;
    /* Runs the attract state machine. Normally never returns -- it suspends
     * each frame via attract_fiber_yield() (called from the func_80151618
     * yield override). If it ever does return, mark done and hand control back
     * so the caller stops resuming us. */
    func_800C50AC(s_rdram, &s_ctx);
    s_done = 1;
    SwitchToFiber(s_resumer);
}

void attract_fiber_resume(uint8_t* rdram) {
    if (s_done) return;
    s_rdram = rdram;

    if (!s_started) {
        s_started = 1;
        memset(&s_ctx, 0, sizeof(s_ctx));
        s_ctx.r29 = (gpr)(int32_t)ATTRACT_MIPS_SP;       /* MIPS stack pointer */
        s_ctx.r28 = (gpr)g_render_buffer_addr;            /* GP / render cursor */
        s_ctx.mips3_float_mode = 1;
        s_fiber = CreateFiber(ATTRACT_FIBER_STACK, attract_entry, NULL);
        if (!s_fiber) {
            fprintf(stderr, "[attract] CreateFiber failed (error %lu)\n", GetLastError());
            s_done = 1;
            return;
        }
        fprintf(stderr, "[attract] fiber created (MIPS sp=0x%08X)\n", ATTRACT_MIPS_SP);
    }

    /* Refresh GP each frame (the render buffer cursor resets per frame, the
     * same treatment rtos_sched_run_frame gives the task fibers). */
    s_ctx.r28 = (gpr)g_render_buffer_addr;

    s_resumer = GetCurrentFiber();
    s_in_fiber = 1;
    SwitchToFiber(s_fiber);   /* run until the loop's next yield (or completion) */
    s_in_fiber = 0;
}

int attract_fiber_in_context(void) { return s_in_fiber; }

int attract_fiber_yield(void) {
    if (!s_in_fiber) return 0;
    SwitchToFiber(s_resumer);   /* suspend; resumed by the next attract_fiber_resume */
    return 1;
}

int attract_fiber_is_done(void) { return s_done; }

#else  /* non-Windows: fiber path unsupported, stays disabled */

void attract_fiber_resume(uint8_t* rdram) { (void)rdram; }
int  attract_fiber_in_context(void) { return 0; }
int  attract_fiber_yield(void) { return 0; }
int  attract_fiber_is_done(void) { return 1; }

#endif
