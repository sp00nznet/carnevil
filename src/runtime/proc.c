/*
 * CarnEvil's process system on host fibers.
 *
 * The game runs Midway's usual cooperative processes. A process struct lives
 * on the game heap:
 *   +0x00 next   +0x04 prev   +0x08 saved stack pointer
 *   +0x0C sleep ticks (s16)   +0x10 stack block / argument pointer
 *   +0x18 id     +0x1C name[8]   +0x24 entry function
 * The head of the list is at 0x801A25F0 and the running one at 0x801A25F8.
 *
 * func_80151718 is the dispatcher: wait for the vblank counter, then walk the
 * list, count each process's sleep down and resume the ones that reach zero.
 * func_80151618 is sleep. Both switch stacks by swapping $sp with $k1, which a
 * static recompile cannot do, so each process gets a host fiber instead and
 * these two functions are written here. Everything else about scheduling --
 * the list, the sleep counts, creation, killing -- is still the game's own
 * code working on its own memory.
 *
 * Registers: the real switch saves the dispatcher's registers around each
 * process and restores a process's callee-saved ones, but never touches $gp,
 * which the game uses as a live global (its display-list write pointer). So
 * $gp is handed from the dispatcher to each process and back.
 */
#include "recomp.h"
#include "hw.h"
#include <stdio.h>
#include <windows.h>

#define W(a) (*(uint32_t*)(g_rdram + PHYS(a)))
#define H(a) (*(int16_t*)(g_rdram + PHYS(a)))

#define PROC_HEAD    0x801A25F0u
#define PROC_CURRENT 0x801A25F8u
#define MAX_FIBERS   256

typedef struct { uint32_t proc; void* fiber; recomp_context* ctx; int dead; } proc_fiber;
static proc_fiber g_pf[MAX_FIBERS];
static void* g_dispatcher;             /* fiber running func_80151718 */
static recomp_context* g_dctx;         /* its context */
static int g_running = -1;             /* index into g_pf of the process on the CPU */

static void call3(uint32_t func, uint32_t a0, uint32_t a1, uint32_t a2) {
    g_dctx->r4 = (gpr)(int32_t)a0; g_dctx->r5 = (gpr)(int32_t)a1; g_dctx->r6 = (gpr)(int32_t)a2;
    get_function((int32_t)func)(g_rdram, g_dctx);
}
#define call0(f) call3((f), 0, 0, 0)

/* A new process starts in 0x8014A488: entry(arg), and when that returns the
 * exit path at 0x801516A8 -- kill self with func_801497B0(current, 1) and go
 * back to the dispatcher. */
static void CALLBACK process_main(void* param) {
    int idx = (int)(intptr_t)param;
    uint32_t p = g_pf[idx].proc;
    recomp_context c;
    recomp_ctx_init(&c);
    g_pf[idx].ctx = &c;
    c.r4 = (gpr)(int32_t)W(p + 0x10);
    c.r8 = (gpr)(int32_t)p;
    c.r29 = (gpr)(int32_t)W(p + 0x08);
    c.r28 = g_dctx->r28;
    g_cur_ctx = &c;
    get_function((int32_t)W(p + 0x24))(g_rdram, &c);

    c.r4 = (gpr)(int32_t)W(PROC_CURRENT);
    c.r5 = 1;
    get_function((int32_t)0x801497B0)(g_rdram, &c);
    g_pf[idx].dead = 1;
    g_dctx->r28 = c.r28;
    SwitchToFiber(g_dispatcher);
    hw_fatal("dead process 0x%08X resumed", p);
}

static int find_fiber(uint32_t p) {
    for (int i = 0; i < MAX_FIBERS; i++)
        if (g_pf[i].fiber && g_pf[i].proc == p) return i;
    return -1;
}

static void resume(uint32_t p) {
    int idx = find_fiber(p);
    if (idx < 0) {
        for (idx = 0; idx < MAX_FIBERS && g_pf[idx].fiber; idx++) {}
        if (idx == MAX_FIBERS) hw_fatal("more than %d live processes", MAX_FIBERS);
        g_pf[idx].proc = p;
        g_pf[idx].dead = 0;
        g_pf[idx].ctx = NULL;
        g_pf[idx].fiber = CreateFiberEx(64 * 1024, 1024 * 1024, 0, process_main, (void*)(intptr_t)idx);
        if (!g_pf[idx].fiber) hw_fatal("CreateFiberEx failed");
    } else {
        g_pf[idx].ctx->r28 = g_dctx->r28;
        g_cur_ctx = g_pf[idx].ctx;
    }
    g_running = idx;
    SwitchToFiber(g_pf[idx].fiber);
    g_running = -1;
    g_cur_ctx = g_dctx;
    if (g_pf[idx].dead) {
        DeleteFiber(g_pf[idx].fiber);
        memset(&g_pf[idx], 0, sizeof(g_pf[idx]));
    }
}

/* func_80148FD4 frees a process struct; every kill path ends there. A process
 * killed by another one is parked in sleep, so its fiber can go now. One that
 * killed itself is still running; resume() deletes it once it switches out. */
void proc_freed(uint8_t* rdram, recomp_context* ctx) {
    (void)rdram;
    int idx = find_fiber((uint32_t)ctx->r4);
    if (idx < 0) return;
    if (idx == g_running) { g_pf[idx].dead = 1; return; }
    DeleteFiber(g_pf[idx].fiber);
    memset(&g_pf[idx], 0, sizeof(g_pf[idx]));
}

/* sleep(ticks): add to the running process's sleep count and yield. */
RECOMP_FUNC void func_80151618(uint8_t* rdram, recomp_context* ctx) {
    (void)rdram;
    if (g_running < 0)
        hw_fatal("process sleep called outside a process (ra=0x%08X)", (uint32_t)ctx->r31);
    uint32_t p = W(PROC_CURRENT);
    H(p + 0x0C) |= (int16_t)ctx->r4;
    W(p + 0x08) = (uint32_t)ctx->r29;
    g_dctx->r28 = ctx->r28;
    SwitchToFiber(g_dispatcher);
}

/* The dispatcher, transcribed from 0x80151718. Never returns. */
RECOMP_FUNC void func_80151718(uint8_t* rdram, recomp_context* ctx) {
    (void)rdram;
    g_dispatcher = GetCurrentFiber();
    g_dctx = ctx;
    W(0x801A24C8) = 0;
    for (;;) {
        if (W(0x801A2B98)) call0(0x8014F6D0);
        W(0x801A2BAC) = 0;
        W(0x801E039C) = W(0x801A24C8);
        /* wait for the vblank interrupt to advance the frame counter */
        do {
            W(0x801A2BAC)++;
            if ((int32_t)W(0x801A2BAC) > 100000000) call0(W(0x801A2BA8));
            hw_idle();
        } while ((int32_t)W(0x801A24C8) < (int32_t)W(0x801A2B9C));
        W(0x801A24C8) = 0;
        if (W(0x801A2BA4) == 1) call3(0x800C4154, 5, 0x7403, 0);
        if (W(0x801A2B98)) call0(0x80167D7C);
        call0(0x80151DE0);

        W(PROC_CURRENT) = W(PROC_HEAD);
        while (W(PROC_CURRENT)) {
            uint32_t p = W(PROC_CURRENT);
            if (H(p + 0x0C) > 0) {
                H(p + 0x0C) = (int16_t)(H(p + 0x0C) - 1);
                if (H(p + 0x0C) == 0) resume(p);
            }
            W(PROC_CURRENT) = W(PROC_CURRENT) ? W(W(PROC_CURRENT)) : W(PROC_HEAD);
        }

        call0(0x80151E24);
        if (W(0x801A2B98)) { call0(0x80148714); call0(0x8014C7EC); }
        call0(0x8015336C);
        call0(0x80149C4C);
        if (W(0x801A2BA4) == 1) call3(0x800C4154, 5, 0x7407, 0x801A2BA0);
    }
}

void proc_register(void) {
    lookup_add(0x80151618u, func_80151618);
    lookup_add(0x80151718u, func_80151718);
}
