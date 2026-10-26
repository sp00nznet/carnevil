/*
 * CPU-level services the recompiled code calls back into: function lookup,
 * COP0, break/syscall, and the RTOS jump table the game copies into itself.
 */
#include "recomp.h"
#include "hw.h"
#include <stdio.h>

/* ---- function lookup ----------------------------------------------------
 * Built from the section tables MidwayRecomp emits for each binary
 * (lookup_game.c / lookup_rtos.c) plus the functions written by hand. */
typedef struct { uint32_t vram; recomp_func_t* func; } func_entry;
static func_entry* g_funcs;
static int g_nfuncs, g_capfuncs;

void lookup_add(uint32_t vram, recomp_func_t* func) {
    if (g_nfuncs == g_capfuncs) {
        g_capfuncs = g_capfuncs ? g_capfuncs * 2 : 4096;
        g_funcs = realloc(g_funcs, g_capfuncs * sizeof(func_entry));
    }
    g_funcs[g_nfuncs].vram = vram;
    g_funcs[g_nfuncs].func = func;
    g_nfuncs++;
}

static int cmp_entry(const void* a, const void* b) {
    uint32_t x = ((const func_entry*)a)->vram, y = ((const func_entry*)b)->vram;
    return x < y ? -1 : x > y;
}

void lookup_finish(void) {
    qsort(g_funcs, g_nfuncs, sizeof(func_entry), cmp_entry);
}

recomp_func_t* get_function(int32_t vram) {
    uint32_t v = (uint32_t)vram;
    if ((v & 0xE0000000u) == 0xA0000000u) v -= 0x20000000u; /* kseg1 -> kseg0 */
    int lo = 0, hi = g_nfuncs - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (g_funcs[mid].vram == v) return g_funcs[mid].func;
        if (g_funcs[mid].vram < v) lo = mid + 1; else hi = mid - 1;
    }
    hw_fatal("no recompiled function at 0x%08X (a jump through a pointer to an address the symbol file does not list)", v);
    return NULL;
}

/* ---- RTOS jump table -------------------------------------------------------
 * entry_point copies 72 two-word stubs (`j rtos_func; nop`) from the RTOS
 * image into 0x800C408C..0x800C42CB. GAME.EXE ships those slots empty, so the
 * recompiler sees nops; these functions run whatever the copy put there. */
static void run_slot(uint8_t* rdram, recomp_context* ctx, uint32_t slot) {
    uint32_t w = *(uint32_t*)(rdram + PHYS(slot));
    if ((w >> 26) != 2)
        hw_fatal("RTOS jump-table slot 0x%08X holds 0x%08X, not a j (called before entry_point copied the table?)", slot, w);
    get_function((int32_t)((slot & 0xF0000000u) | ((w & 0x3FFFFFFu) << 2)))(rdram, ctx);
}

#define RTOS_SLOTS(X) \
    X(800C408C) X(800C4094) X(800C409C) X(800C40A4) X(800C40AC) X(800C40B4) X(800C40BC) X(800C40C4) \
    X(800C40CC) X(800C40D4) X(800C40DC) X(800C40E4) X(800C40EC) X(800C40F4) X(800C40FC) X(800C4104) \
    X(800C410C) X(800C4114) X(800C411C) X(800C4124) X(800C412C) X(800C4134) X(800C413C) X(800C4144) \
    X(800C414C) X(800C4154) X(800C415C) X(800C4164) X(800C416C) X(800C4174) X(800C417C) X(800C4184) \
    X(800C418C) X(800C4194) X(800C419C) X(800C41A4) X(800C41AC) X(800C41B4) X(800C41BC) X(800C41C4) \
    X(800C41CC) X(800C41D4) X(800C41DC) X(800C41E4) X(800C41EC) X(800C41F4) X(800C41FC) X(800C4204) \
    X(800C420C) X(800C4214) X(800C421C) X(800C4224) X(800C422C) X(800C4234) X(800C423C) X(800C4244) \
    X(800C424C) X(800C4254) X(800C425C) X(800C4264) X(800C426C) X(800C4274) X(800C427C) X(800C4284) \
    X(800C428C) X(800C4294) X(800C429C) X(800C42A4) X(800C42AC) X(800C42B4) X(800C42BC) X(800C42C4)

#define DEFINE_SLOT(a) \
    RECOMP_FUNC void func_##a(uint8_t* rdram, recomp_context* ctx) { run_slot(rdram, ctx, 0x##a##u); }
RTOS_SLOTS(DEFINE_SLOT)

void rtos_register_slots(void) {
#define ADD_SLOT(a) lookup_add(0x##a##u, func_##a);
    RTOS_SLOTS(ADD_SLOT)
}

/* ---- COP0 -------------------------------------------------------------------
 * Status and Cause are CPU-wide (hw.c); everything else just holds what was
 * written. There is no MMU: recompiled code addresses rdram directly, so the
 * TLB registers only need to read back. */
static gpr g_cp0[32];

void cop0_status_write(recomp_context* ctx, gpr value) { (void)ctx; hw_set_status((uint32_t)value); }
gpr  cop0_status_read(recomp_context* ctx) { (void)ctx; return (gpr)(int32_t)hw_get_status(); }

gpr cop0_reg_read(recomp_context* ctx, int reg) {
    (void)ctx;
    switch (reg) {
        case 9:  return (gpr)(int32_t)hw_cp0_count();   /* Count */
        case 12: return (gpr)(int32_t)hw_get_status();
        case 13: return (gpr)(int32_t)hw_get_cause();
        case 15: return 0x2320;                          /* PRId: R5000 rev 2.0 */
        default: return g_cp0[reg & 31];
    }
}

void cop0_reg_write(recomp_context* ctx, int reg, gpr value) {
    (void)ctx;
    if (reg == 12) { hw_set_status((uint32_t)value); return; }
    g_cp0[reg & 31] = value;
}

/* ---- break / syscall ----------------------------------------------------------
 * The RTOS offers services through the breakpoint exception: `break 0x400+n`
 * dispatches through the table at 0x80004684. They are a few instructions each
 * that edit the exception frame and eret, so they are implemented here. The
 * frame is the one the RTOS exception entry points k0 at. */
#define RTOS_K0_FRAME 0x800B86A8u
#define RTOS_EXC_TABLE 0x80014660u

void do_break(uint8_t* rdram, recomp_context* ctx, uint32_t vram) {
    uint32_t instr = *(uint32_t*)(rdram + PHYS(vram));
    uint32_t code = (instr >> 6) & 0xFFFFF;
    uint32_t* pass = (uint32_t*)(rdram + PHYS(RTOS_K0_FRAME + 0x338));
    uint32_t* pass_save = (uint32_t*)(rdram + PHYS(RTOS_K0_FRAME + 0x33C));
    switch (code) {
        case 0x403: case 0x406:           /* set the debugger pass-through mask */
            *pass = (uint32_t)ctx->r4;
            return;
        case 0x405:                       /* save the mask, pass everything but breakpoints */
            *pass_save = *pass;
            *pass = 0xFFFF0200u;
            return;
        case 0x408:
            return;
        case 0x409: {                     /* install exception handler: a0 = code, a1 = handler */
            uint32_t exc = (uint32_t)ctx->r4;
            if (exc < 16) {
                *pass &= ~(1u << exc);
                *(uint32_t*)(rdram + PHYS(RTOS_EXC_TABLE + exc * 4)) = (uint32_t)ctx->r5;
            }
            return;
        }
        default:
            fprintf(stderr, "[cpu] break 0x%X at 0x%08X (ra=0x%08X) -- debugger/fatal break, continuing\n",
                    code, vram, (uint32_t)ctx->r31);
            return;
    }
}

void recomp_syscall_handler(uint8_t* rdram, recomp_context* ctx, int32_t vram) {
    (void)rdram;
    fprintf(stderr, "[cpu] syscall at 0x%08X (v0=%u) ignored\n", (uint32_t)vram, (uint32_t)ctx->r2);
}

void switch_error(const char* func, uint32_t vram, uint32_t jtbl) {
    hw_fatal("jump table miss in %s at 0x%08X (table 0x%08X)", func, vram, jtbl);
}

void pause_self(uint8_t* rdram) { (void)rdram; hw_fatal("pause_self"); }
