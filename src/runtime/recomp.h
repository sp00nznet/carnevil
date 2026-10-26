/*
 * Support header for MidwayRecomp output (MIPS R5000, little-endian).
 *
 * Every MIPS load and store goes through mem_read / mem_write below. RAM is a
 * flat 8 MB array; anything else is a device and goes to hw_read / hw_write
 * (hw.c), exactly once per access, so device side effects (FIFOs, acks, IDE
 * data ports) behave like the real bus.
 *
 * Loads also drive emulated time: every HW_TICK_LOADS loads the hardware gets
 * a tick (hw_tick), which advances the video timing and delivers pending
 * interrupts. That is what lets the game's own spin-waits on interrupt-driven
 * counters finish, the way they do on the board.
 */
#ifndef CARNEVIL_RECOMP_H
#define CARNEVIL_RECOMP_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fenv.h>
#include <assert.h>

#if defined(_MSC_VER) && !defined(__clang__)
    #define RECOMP_FUNC __declspec(noinline)
#elif defined(__clang__)
    #define RECOMP_FUNC extern inline __attribute__((weak,noinline))
#elif defined(__GNUC__)
    #define RECOMP_FUNC __attribute__((noipa, optimize("rounding-math")))
#else
    #define RECOMP_FUNC
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t gpr;

typedef union {
    double d;
    struct { float fl; float fh; };
    struct { uint32_t u32l; uint32_t u32h; };
    uint64_t u64;
} fpr;

typedef struct {
    gpr r0,  r1,  r2,  r3,  r4,  r5,  r6,  r7,
        r8,  r9,  r10, r11, r12, r13, r14, r15,
        r16, r17, r18, r19, r20, r21, r22, r23,
        r24, r25, r26, r27, r28, r29, r30, r31;
    fpr f0,  f1,  f2,  f3,  f4,  f5,  f6,  f7,
        f8,  f9,  f10, f11, f12, f13, f14, f15,
        f16, f17, f18, f19, f20, f21, f22, f23,
        f24, f25, f26, f27, f28, f29, f30, f31;
    uint64_t hi, lo;
    uint32_t* f_odd;
    uint32_t status_reg;     /* unused: Status is CPU-global, see hw.c */
    uint8_t mips3_float_mode;
} recomp_context;

/* A fresh context for calling recompiled code from the host. */
static inline void recomp_ctx_init(recomp_context* c) {
    memset(c, 0, sizeof(*c));
    c->f_odd = &c->f0.u32h;
    c->mips3_float_mode = 1;
}

typedef void (recomp_func_t)(uint8_t* rdram, recomp_context* ctx);

/* ---- integer helpers -------------------------------------------------- */
#define SIGNED(val)  ((int64_t)(val))
#define ADD32(a, b)  ((gpr)(int32_t)((a) + (b)))
#define SUB32(a, b)  ((gpr)(int32_t)((a) - (b)))
#define S32(val)     ((int32_t)(val))
#define U32(val)     ((uint32_t)(val))
#define S64(val)     ((int64_t)(val))
#define U64(val)     ((uint64_t)(val))

/* ---- memory ------------------------------------------------------------ */
#define RAM_SIZE      0x00800000u
#define PHYS(a)       ((uint32_t)(a) & 0x1FFFFFFFu)
#define HW_TICK_LOADS 4096

extern int32_t g_hw_budget;
void     hw_tick(void);
uint32_t hw_read(uint32_t paddr, int size);
void     hw_write(uint32_t paddr, uint32_t value, int size);

static inline uint32_t mem_read(uint8_t* rdram, gpr addr, int size) {
    uint32_t p = PHYS(addr);
    if (--g_hw_budget < 0) hw_tick();
    if (p < RAM_SIZE) {
        switch (size) {
            case 1: return rdram[p];
            case 2: return *(uint16_t*)(rdram + p);
            default: return *(uint32_t*)(rdram + p);
        }
    }
    return hw_read(p, size);
}

static inline void mem_write(uint8_t* rdram, gpr addr, uint32_t value, int size) {
    uint32_t p = PHYS(addr);
    if (p < RAM_SIZE) {
        switch (size) {
            case 1: rdram[p] = (uint8_t)value; break;
            case 2: *(uint16_t*)(rdram + p) = (uint16_t)value; break;
            default: *(uint32_t*)(rdram + p) = value; break;
        }
        return;
    }
    hw_write(p, value, size);
}

#define MEM_W(offset, reg)   ((int32_t)mem_read(rdram, (reg) + (offset), 4))
#define MEM_WU(offset, reg)  ((uint32_t)mem_read(rdram, (reg) + (offset), 4))
#define MEM_H(offset, reg)   ((int16_t)mem_read(rdram, (reg) + (offset), 2))
#define MEM_HU(offset, reg)  ((uint16_t)mem_read(rdram, (reg) + (offset), 2))
#define MEM_B(offset, reg)   ((int8_t)mem_read(rdram, (reg) + (offset), 1))
#define MEM_BU(offset, reg)  ((uint8_t)mem_read(rdram, (reg) + (offset), 1))

#define SW(val, offset, reg) mem_write(rdram, (reg) + (offset), (uint32_t)(val), 4)
#define SH(val, offset, reg) mem_write(rdram, (reg) + (offset), (uint32_t)(val), 2)
#define SB(val, offset, reg) mem_write(rdram, (reg) + (offset), (uint32_t)(val), 1)

/* Doublewords are two words, low word first (little-endian). */
static inline uint64_t load_doubleword(uint8_t* rdram, gpr offset, gpr reg) {
    uint64_t lo = mem_read(rdram, reg + offset, 4);
    uint64_t hi = mem_read(rdram, reg + offset + 4, 4);
    return lo | (hi << 32);
}
#define LD(offset, reg) load_doubleword(rdram, (offset), (reg))
#define SD(val, offset, reg) do { \
    gpr sd_v_ = (gpr)(val); \
    mem_write(rdram, (reg) + (offset), (uint32_t)sd_v_, 4); \
    mem_write(rdram, (reg) + (offset) + 4, (uint32_t)(sd_v_ >> 32), 4); \
} while (0)

/* Unaligned accesses, little-endian semantics. */
static inline gpr do_lwl(uint8_t* rdram, gpr initial_value, gpr offset, gpr reg) {
    gpr address = offset + reg;
    uint32_t loaded = mem_read(rdram, address & ~3ull, 4);
    uint32_t mis = (uint32_t)address & 3;
    uint32_t keep = (uint32_t)initial_value & ~(0xFFFFFFFFu << ((3 - mis) * 8));
    return (gpr)(int32_t)(keep | (loaded << ((3 - mis) * 8)));
}
static inline gpr do_lwr(uint8_t* rdram, gpr initial_value, gpr offset, gpr reg) {
    gpr address = offset + reg;
    uint32_t loaded = mem_read(rdram, address & ~3ull, 4);
    uint32_t mis = (uint32_t)address & 3;
    uint32_t keep = (uint32_t)initial_value & ~(0xFFFFFFFFu >> (mis * 8));
    return (gpr)(int32_t)(keep | (loaded >> (mis * 8)));
}
static inline void do_swl(uint8_t* rdram, gpr offset, gpr reg, gpr val) {
    gpr address = offset + reg;
    uint32_t mis = (uint32_t)address & 3;
    uint32_t old = mem_read(rdram, address & ~3ull, 4);
    uint32_t v = (old & ~(0xFFFFFFFFu >> ((3 - mis) * 8))) | ((uint32_t)val >> ((3 - mis) * 8));
    mem_write(rdram, address & ~3ull, v, 4);
}
static inline void do_swr(uint8_t* rdram, gpr offset, gpr reg, gpr val) {
    gpr address = offset + reg;
    uint32_t mis = (uint32_t)address & 3;
    uint32_t old = mem_read(rdram, address & ~3ull, 4);
    uint32_t v = (old & ~(0xFFFFFFFFu << (mis * 8))) | ((uint32_t)val << (mis * 8));
    mem_write(rdram, address & ~3ull, v, 4);
}
static inline gpr do_ldl(uint8_t* rdram, gpr initial_value, gpr offset, gpr reg) {
    gpr address = offset + reg;
    uint64_t loaded = load_doubleword(rdram, 0, address & ~7ull);
    uint32_t mis = (uint32_t)address & 7;
    return (initial_value & ~(~0ull << ((7 - mis) * 8))) | (loaded << ((7 - mis) * 8));
}
static inline gpr do_ldr(uint8_t* rdram, gpr initial_value, gpr offset, gpr reg) {
    gpr address = offset + reg;
    uint64_t loaded = load_doubleword(rdram, 0, address & ~7ull);
    uint32_t mis = (uint32_t)address & 7;
    return (initial_value & ~(~0ull >> (mis * 8))) | (loaded >> (mis * 8));
}
static inline void do_sdl(uint8_t* rdram, gpr offset, gpr reg, gpr val) {
    gpr address = offset + reg;
    uint32_t mis = (uint32_t)address & 7;
    uint64_t old = load_doubleword(rdram, 0, address & ~7ull);
    SD((old & ~(~0ull >> ((7 - mis) * 8))) | (val >> ((7 - mis) * 8)), 0, address & ~7ull);
}
static inline void do_sdr(uint8_t* rdram, gpr offset, gpr reg, gpr val) {
    gpr address = offset + reg;
    uint32_t mis = (uint32_t)address & 7;
    uint64_t old = load_doubleword(rdram, 0, address & ~7ull);
    SD((old & ~(~0ull << (mis * 8))) | (val << (mis * 8)), 0, address & ~7ull);
}

/* ---- 64-bit multiply / divide ------------------------------------------ */
#if defined(__SIZEOF_INT128__)
static inline void DMULT(int64_t a, int64_t b, int64_t* lo64, int64_t* hi64) {
    __int128 full = (__int128)a * (__int128)b; *hi64 = (int64_t)(full >> 64); *lo64 = (int64_t)full;
}
static inline void DMULTU(uint64_t a, uint64_t b, uint64_t* lo64, uint64_t* hi64) {
    unsigned __int128 full = (unsigned __int128)a * b; *hi64 = (uint64_t)(full >> 64); *lo64 = (uint64_t)full;
}
#elif defined(_MSC_VER)
#include <intrin.h>
static inline void DMULT(int64_t a, int64_t b, int64_t* lo64, int64_t* hi64) { *lo64 = _mul128(a, b, hi64); }
static inline void DMULTU(uint64_t a, uint64_t b, uint64_t* lo64, uint64_t* hi64) { *lo64 = _umul128(a, b, hi64); }
#endif
static inline void DDIV(int64_t a, int64_t b, int64_t* quot, int64_t* rem) {
    int overflow = ((uint64_t)a == 0x8000000000000000ull) && (b == -1ll);
    *quot = overflow ? a : (a / b); *rem = overflow ? 0 : (a % b);
}
static inline void DDIVU(uint64_t a, uint64_t b, uint64_t* quot, uint64_t* rem) { *quot = a / b; *rem = a % b; }

/* ---- floating point ----------------------------------------------------- */
#define MUL_S(a, b)    ((a) * (b))
#define MUL_D(a, b)    ((a) * (b))
#define DIV_S(a, b)    ((a) / (b))
#define DIV_D(a, b)    ((a) / (b))
#define CVT_S_W(val)   ((float)((int32_t)(val)))
#define CVT_D_W(val)   ((double)((int32_t)(val)))
#define CVT_D_L(val)   ((double)((int64_t)(val)))
#define CVT_S_L(val)   ((float)((int64_t)(val)))
#define CVT_D_S(val)   ((double)(val))
#define CVT_S_D(val)   ((float)(val))
#define TRUNC_W_S(val) ((int32_t)(val))
#define TRUNC_W_D(val) ((int32_t)(val))
#define TRUNC_L_S(val) ((int64_t)(val))
#define TRUNC_L_D(val) ((int64_t)(val))
#define DEFAULT_ROUNDING_MODE 0
#define CVT_W_S(val)   ((int32_t)lrintf(val))
#define CVT_W_D(val)   ((int32_t)lrint(val))
#define CVT_L_S(val)   ((int64_t)llrintf(val))
#define CVT_L_D(val)   ((int64_t)llrint(val))

static inline uint32_t get_cop1_cs(void) {
    switch (fegetround()) {
        case FE_TOWARDZERO: return 1;
        case FE_UPWARD:     return 2;
        case FE_DOWNWARD:   return 3;
        default:            return 0;
    }
}
static inline void set_cop1_cs(uint32_t val) {
    static const int modes[4] = { FE_TONEAREST, FE_TOWARDZERO, FE_UPWARD, FE_DOWNWARD };
    fesetround(modes[val & 3]);
}
#define NAN_CHECK(val)     ((void)0)
#define CHECK_FR(ctx, idx) ((void)0)

/* ---- CPU services (rtos.c) ---------------------------------------------- */
void cop0_status_write(recomp_context* ctx, gpr value);
gpr  cop0_status_read(recomp_context* ctx);
void cop0_reg_write(recomp_context* ctx, int reg, gpr value);
gpr  cop0_reg_read(recomp_context* ctx, int reg);

recomp_func_t* get_function(int32_t vram);
#define LOOKUP_FUNC(val) get_function((int32_t)(val))

void switch_error(const char* func, uint32_t vram, uint32_t jtbl);
void do_break(uint8_t* rdram, recomp_context* ctx, uint32_t vram);
void recomp_syscall_handler(uint8_t* rdram, recomp_context* ctx, int32_t instruction_vram);
void pause_self(uint8_t* rdram);

/* Called from the hook on func_80148FD4 (process free), see proc.c. */
void proc_freed(uint8_t* rdram, recomp_context* ctx);

#ifdef __cplusplus
}
#endif

#endif
