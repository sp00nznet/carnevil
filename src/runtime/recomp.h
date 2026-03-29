/**
 * CarnEvil Runtime Header for MidwayRecomp
 *
 * Provides the types, macros, and functions needed by the statically
 * recompiled MIPS-IV output. Adapted for Midway Seattle (R5000 LE).
 *
 * Memory map:
 *   kseg0 0x80000000-0x9FFFFFFF -> physical 0x00000000 (cached)
 *   kseg1 0xA0000000-0xBFFFFFFF -> physical 0x00000000 (uncached)
 *   Both map to rdram via: physical = vaddr & 0x1FFFFFFF
 *
 *   RAM:      physical 0x00000000-0x007FFFFF (8MB)
 *   Galileo:  physical 0x14000000
 *   IOASIC:   physical 0x16000000
 *   Lightgun: physical 0x16800000
 *   Voodoo:   physical 0x16000000+ (PCI mapped)
 */

#ifndef CARNEVIL_RECOMP_H
#define CARNEVIL_RECOMP_H

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <fenv.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

/* ======================================================================
 * Compiler attributes
 * ====================================================================== */

#if defined(_MSC_VER) && !defined(__clang__)
    #define RECOMP_FUNC __declspec(noinline)
    #define SET_FENV_ACCESS() _Pragma("fenv_access(on)")
#elif defined(__clang__)
    #define RECOMP_FUNC extern inline __attribute__((weak,noinline))
    #define SET_FENV_ACCESS() _Pragma("STDC FENV_ACCESS ON")
#elif defined(__GNUC__)
    #define RECOMP_FUNC __attribute__((noipa, optimize("rounding-math")))
    #define SET_FENV_ACCESS()
#else
    #define RECOMP_FUNC
    #define SET_FENV_ACCESS()
#endif

/* ======================================================================
 * Basic types
 * ====================================================================== */

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
    uint32_t status_reg;
    uint8_t mips3_float_mode;
} recomp_context;

/* ======================================================================
 * Integer arithmetic
 * ====================================================================== */

#define SIGNED(val)  ((int64_t)(val))
#define ADD32(a, b)  ((gpr)(int32_t)((a) + (b)))
#define SUB32(a, b)  ((gpr)(int32_t)((a) - (b)))
#define S32(val)     ((int32_t)(val))
#define U32(val)     ((uint32_t)(val))
#define S64(val)     ((int64_t)(val))
#define U64(val)     ((uint64_t)(val))

/* ======================================================================
 * Memory access -- LITTLE ENDIAN with full MIPS address translation
 *
 * MIPS virtual to physical: strip kseg bits (mask with 0x1FFFFFFF)
 * This handles both kseg0 (0x80xxxxxx) and kseg1 (0xA0xxxxxx).
 *
 * Physical addresses beyond RAM (>8MB) are hardware I/O and handled
 * via the seattle_io_read/write functions.
 * ====================================================================== */

#define PHYS_MASK    0x1FFFFFFFU
#define RAM_SIZE_HW  0x00800000U  /* 8MB */

/* Declared in seattle_runtime.c */
#ifdef __cplusplus
extern "C" {
#endif
extern uint8_t* seattle_rdram;
uint32_t seattle_io_read32(uint32_t paddr);
void     seattle_io_write32(uint32_t paddr, uint32_t value);
uint16_t seattle_io_read16(uint32_t paddr);
void     seattle_io_write16(uint32_t paddr, uint16_t value);
uint8_t  seattle_io_read8(uint32_t paddr);
void     seattle_io_write8(uint32_t paddr, uint8_t value);
#ifdef __cplusplus
}
#endif

/* Translate MIPS virtual address to pointer into rdram.
 * Handles both kseg0 (0x80xxxxxx) and kseg1 (0xA0xxxxxx).
 * I/O addresses beyond RAM return a pointer into a scratch buffer
 * that the I/O subsystem manages separately. */
/* I/O write-back mechanism: the scratch buffer tracks pending I/O writes.
 * When mem_w_ptr is called, it flushes any previous I/O write first. */
static uint32_t io_wb_paddr = 0;
static int32_t io_wb_scratch = 0;
static int io_wb_pending = 0;

static inline void io_wb_flush(void) {
    if (io_wb_pending) {
        seattle_io_write32(io_wb_paddr, (uint32_t)io_wb_scratch);
        io_wb_pending = 0;
    }
}

static inline int32_t* mem_w_ptr(uint8_t* rdram, gpr addr) {
    uint32_t paddr = (uint32_t)addr & PHYS_MASK;

    /* Flush any pending I/O write from a previous MEM_W assignment */
    io_wb_flush();

    if (paddr < RAM_SIZE_HW) {
        return (int32_t*)(rdram + paddr);
    }

    /* I/O region: set up write-back tracking */
    io_wb_paddr = paddr;
    io_wb_scratch = (int32_t)seattle_io_read32(paddr);
    io_wb_pending = 1; /* Will be flushed on next mem_w_ptr or io_wb_flush call */
    return &io_wb_scratch;
}

static inline int16_t* mem_h_ptr(uint8_t* rdram, gpr addr) {
    io_wb_flush();
    uint32_t paddr = (uint32_t)addr & PHYS_MASK;
    if (paddr < RAM_SIZE_HW) {
        return (int16_t*)(rdram + paddr);
    }
    static int16_t io_h_scratch;
    io_h_scratch = (int16_t)seattle_io_read16(paddr);
    return &io_h_scratch;
}

static inline int8_t* mem_b_ptr(uint8_t* rdram, gpr addr) {
    io_wb_flush();
    uint32_t paddr = (uint32_t)addr & PHYS_MASK;
    if (paddr < RAM_SIZE_HW) {
        return (int8_t*)(rdram + paddr);
    }
    static int8_t io_b_scratch;
    io_b_scratch = (int8_t)seattle_io_read8(paddr);
    return &io_b_scratch;
}

static inline void mem_write32(uint8_t* rdram, gpr addr, int32_t val) {
    io_wb_flush();
    uint32_t paddr = (uint32_t)addr & PHYS_MASK;
    if (paddr < RAM_SIZE_HW) {
        *(int32_t*)(rdram + paddr) = val;
    } else {
        seattle_io_write32(paddr, (uint32_t)val);
    }
}

/* Memory access macros -- work as both lvalue and rvalue */
#define MEM_W(offset, reg)   (*mem_w_ptr(rdram, (reg) + (offset)))
#define MEM_H(offset, reg)   (*mem_h_ptr(rdram, (reg) + (offset)))
#define MEM_HU(offset, reg)  (*(uint16_t*)mem_h_ptr(rdram, (reg) + (offset)))
#define MEM_B(offset, reg)   (*mem_b_ptr(rdram, (reg) + (offset)))
#define MEM_BU(offset, reg)  (*(uint8_t*)mem_b_ptr(rdram, (reg) + (offset)))

/* Doubleword access */
static inline uint64_t load_doubleword(uint8_t* rdram, gpr offset, gpr reg) {
    uint64_t lo = (uint64_t)(uint32_t)MEM_W(offset + 0, reg);
    uint64_t hi = (uint64_t)(uint32_t)MEM_W(offset + 4, reg);
    return lo | (hi << 32);
}

#define LD(offset, reg) load_doubleword(rdram, offset, reg)

#define SD(val, offset, reg) { \
    mem_write32(rdram, (reg) + (offset) + 0, (uint32_t)((gpr)(val) >> 0)); \
    mem_write32(rdram, (reg) + (offset) + 4, (uint32_t)((gpr)(val) >> 32)); \
}

/* Unaligned word access (LE versions) */
static inline gpr do_lwl(uint8_t* rdram, gpr initial_value, gpr offset, gpr reg) {
    gpr address = offset + reg;
    gpr word_address = address & ~0x3;
    uint32_t loaded_value = MEM_W(0, word_address);
    gpr misalignment = address & 0x3;
    gpr masked_value = initial_value & (uint32_t)(~(0xFFFFFFFFu << ((3 - misalignment) * 8)));
    loaded_value <<= ((3 - misalignment) * 8);
    return (gpr)(int32_t)(uint32_t)(masked_value | loaded_value);
}

static inline gpr do_lwr(uint8_t* rdram, gpr initial_value, gpr offset, gpr reg) {
    gpr address = offset + reg;
    gpr word_address = address & ~0x3;
    uint32_t loaded_value = MEM_W(0, word_address);
    gpr misalignment = address & 0x3;
    gpr masked_value = initial_value & (uint32_t)(~(0xFFFFFFFFu >> (misalignment * 8)));
    loaded_value >>= (misalignment * 8);
    return (gpr)(int32_t)(uint32_t)(masked_value | loaded_value);
}

static inline void do_swl(uint8_t* rdram, gpr offset, gpr reg, gpr val) {
    gpr address = offset + reg;
    gpr word_address = address & ~0x3;
    uint32_t initial_value = MEM_W(0, word_address);
    gpr misalignment = address & 0x3;
    uint32_t masked = initial_value & ~(0xFFFFFFFFu >> ((3 - misalignment) * 8));
    uint32_t shifted = ((uint32_t)val) >> ((3 - misalignment) * 8);
    mem_write32(rdram, word_address, (int32_t)(masked | shifted));
}

static inline void do_swr(uint8_t* rdram, gpr offset, gpr reg, gpr val) {
    gpr address = offset + reg;
    gpr word_address = address & ~0x3;
    uint32_t initial_value = MEM_W(0, word_address);
    gpr misalignment = address & 0x3;
    uint32_t masked = initial_value & ~(0xFFFFFFFFu << (misalignment * 8));
    uint32_t shifted = ((uint32_t)val) << (misalignment * 8);
    mem_write32(rdram, word_address, (int32_t)(masked | shifted));
}

static inline gpr do_ldl(uint8_t* rdram, gpr initial_value, gpr offset, gpr reg) {
    gpr address = offset + reg;
    gpr dword_address = address & ~0x7;
    uint64_t loaded_value = load_doubleword(rdram, 0, dword_address);
    gpr misalignment = address & 0x7;
    gpr masked_value = initial_value & ~(0xFFFFFFFFFFFFFFFFu << ((7 - misalignment) * 8));
    loaded_value <<= ((7 - misalignment) * 8);
    return masked_value | loaded_value;
}

static inline gpr do_ldr(uint8_t* rdram, gpr initial_value, gpr offset, gpr reg) {
    gpr address = offset + reg;
    gpr dword_address = address & ~0x7;
    uint64_t loaded_value = load_doubleword(rdram, 0, dword_address);
    gpr misalignment = address & 0x7;
    gpr masked_value = initial_value & ~(0xFFFFFFFFFFFFFFFFu >> (misalignment * 8));
    loaded_value >>= (misalignment * 8);
    return masked_value | loaded_value;
}

static inline void do_sdl(uint8_t* rdram, gpr offset, gpr reg, gpr val) {
    gpr address = offset + reg;
    gpr dword_address = address & ~0x7;
    uint64_t iv = load_doubleword(rdram, 0, dword_address);
    gpr mis = address & 0x7;
    uint64_t result = (iv & ~(0xFFFFFFFFFFFFFFFFu >> ((7 - mis) * 8))) | (val >> ((7 - mis) * 8));
    mem_write32(rdram, dword_address + 0, (int32_t)(uint32_t)result);
    mem_write32(rdram, dword_address + 4, (int32_t)(uint32_t)(result >> 32));
}

static inline void do_sdr(uint8_t* rdram, gpr offset, gpr reg, gpr val) {
    gpr address = offset + reg;
    gpr dword_address = address & ~0x7;
    uint64_t iv = load_doubleword(rdram, 0, dword_address);
    gpr mis = address & 0x7;
    uint64_t result = (iv & ~(0xFFFFFFFFFFFFFFFFu << (mis * 8))) | (val << (mis * 8));
    mem_write32(rdram, dword_address + 0, (int32_t)(uint32_t)result);
    mem_write32(rdram, dword_address + 4, (int32_t)(uint32_t)(result >> 32));
}

/* ======================================================================
 * 64-bit multiply/divide
 * ====================================================================== */

#if defined(__SIZEOF_INT128__)
static inline void DMULT(int64_t a, int64_t b, int64_t* lo64, int64_t* hi64) {
    __int128 full = ((__int128)a) * ((__int128)b);
    *hi64 = (int64_t)(full >> 64); *lo64 = (int64_t)(full >> 0);
}
static inline void DMULTU(uint64_t a, uint64_t b, uint64_t* lo64, uint64_t* hi64) {
    unsigned __int128 full = ((unsigned __int128)a) * ((unsigned __int128)b);
    *hi64 = (uint64_t)(full >> 64); *lo64 = (uint64_t)(full >> 0);
}
#elif defined(_MSC_VER)
#include <intrin.h>
#pragma intrinsic(_mul128)
#pragma intrinsic(_umul128)
static inline void DMULT(int64_t a, int64_t b, int64_t* lo64, int64_t* hi64) { *lo64 = _mul128(a, b, hi64); }
static inline void DMULTU(uint64_t a, uint64_t b, uint64_t* lo64, uint64_t* hi64) { *lo64 = _umul128(a, b, hi64); }
#else
#error "128-bit integer type not found"
#endif

static inline void DDIV(int64_t a, int64_t b, int64_t* quot, int64_t* rem) {
    int overflow = ((uint64_t)a == 0x8000000000000000ull) && (b == -1ll);
    *quot = overflow ? a : (a / b); *rem = overflow ? 0 : (a % b);
}
static inline void DDIVU(uint64_t a, uint64_t b, uint64_t* quot, uint64_t* rem) {
    *quot = a / b; *rem = a % b;
}

/* ======================================================================
 * Floating point
 * ====================================================================== */

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

static inline int32_t do_cvt_w_s(float val)  { return (int32_t)lrintf(val); }
static inline int32_t do_cvt_w_d(double val) { return (int32_t)lrint(val);  }
static inline int64_t do_cvt_l_s(float val)  { return (int64_t)llrintf(val); }
static inline int64_t do_cvt_l_d(double val) { return (int64_t)llrint(val);  }

#define CVT_W_S(val)  do_cvt_w_s(val)
#define CVT_W_D(val)  do_cvt_w_d(val)
#define CVT_L_S(val)  do_cvt_l_s(val)
#define CVT_L_D(val)  do_cvt_l_d(val)

static inline uint32_t get_cop1_cs(void) {
    switch (fegetround()) {
        case FE_TONEAREST:  return 0;
        case FE_TOWARDZERO: return 1;
        case FE_UPWARD:     return 2;
        case FE_DOWNWARD:   return 3;
        default:            return 0;
    }
}

static inline void set_cop1_cs(uint32_t val) {
    int round;
    switch (val & 0x3) {
        case 0: round = FE_TONEAREST;  break;
        case 1: round = FE_TOWARDZERO; break;
        case 2: round = FE_UPWARD;     break;
        case 3: round = FE_DOWNWARD;   break;
        default: round = FE_TONEAREST; break;
    }
    fesetround(round);
}

#define NAN_CHECK(val)     assert((val) == (val))
#define CHECK_FR(ctx, idx) assert(((idx) & 1) == 0 || (ctx)->mips3_float_mode)

/* ======================================================================
 * System / runtime callbacks
 * ====================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

void cop0_status_write(recomp_context* ctx, gpr value);
gpr  cop0_status_read(recomp_context* ctx);

typedef void (recomp_func_t)(uint8_t* rdram, recomp_context* ctx);
recomp_func_t* get_function(int32_t vram);
#define LOOKUP_FUNC(val)  get_function((int32_t)(val))

void switch_error(const char* func, uint32_t vram, uint32_t jtbl);
void do_break(uint32_t vram);
void recomp_syscall_handler(uint8_t* rdram, recomp_context* ctx, int32_t instruction_vram);
void pause_self(uint8_t* rdram);

extern int32_t* section_addresses;
#define LO16(x)  ((x) & 0xFFFF)
#define HI16(x)  (((x) >> 16) + (((x) >> 15) & 1))
#define RELOC_HI16(section_index, offset)  HI16(section_addresses[section_index] + (offset))
#define RELOC_LO16(section_index, offset)  LO16(section_addresses[section_index] + (offset))

#ifdef __cplusplus
}
#endif

#endif /* CARNEVIL_RECOMP_H */
