/* SPDX-License-Identifier: BSD-3-Clause
 *
 * Ported from MAME (https://github.com/mamedev/mame):
 * src/devices/cpu/adsp2100/adsp2100.cpp, adsp2100.h and 2100ops.hxx, which
 * carry this licence:
 *
 *   license:BSD-3-Clause
 *   copyright-holders:Aaron Giles
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions are
 *   met: (1) Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer. (2)
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *   (3) Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *   IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 *   TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 *   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The port and the changes made to it are distributed under the same terms.
 */
/*
 * ADSP-2100 family DSP core, ADSP-2101/2115 flavour.
 *
 * Follows MAME's src/devices/cpu/adsp2100/adsp2100.cpp, adsp2100.h and
 * 2100ops.hxx (copyright-holders: Aaron Giles), used under the BSD-3-Clause
 * license. Instruction semantics, flag computation, stacks, DAG addressing
 * and interrupt priorities are kept as MAME has them; the ADSP-2181-only
 * parts (IO space, overlays, constant operands) are left out.
 */
#include "adsp2100.h"
#include <stddef.h>
#include <string.h>

#define BITS(x, s, n) (((x) >> (s)) & ((1u << (n)) - 1))
#define BIT1(x, s) (((x) >> (s)) & 1)

enum { SSFLAG = 0x80, MVFLAG = 0x40, QFLAG = 0x20, SFLAG = 0x10, CFLAG = 0x08, VFLAG = 0x04, NFLAG = 0x02, ZFLAG = 0x01 };
enum { MSTAT_BANK = 0x01, MSTAT_REVERSE = 0x02, MSTAT_STICKYV = 0x04, MSTAT_SATURATE = 0x08,
       MSTAT_INTEGER = 0x10, MSTAT_TIMER = 0x20, MSTAT_GOMODE = 0x40 };
enum { PC_EMPTY = 0x01, PC_OVER = 0x02, COUNT_EMPTY = 0x04, COUNT_OVER = 0x08,
       STATUS_EMPTY = 0x10, STATUS_OVER = 0x20, LOOP_EMPTY = 0x40, LOOP_OVER = 0x80 };

#define ASTAT c->astat
#define GET_MV (ASTAT & MVFLAG)
#define GET_Q  (ASTAT & QFLAG)
#define GET_C  (ASTAT & CFLAG)
#define GET_V  (ASTAT & VFLAG)
#define GET_SS (ASTAT & SSFLAG)
#define SET_SS (ASTAT |= SSFLAG)
#define CLR_SS (ASTAT &= ~SSFLAG)
#define SET_MV (ASTAT |= MVFLAG)
#define CLR_MV (ASTAT &= ~MVFLAG)
#define SET_S  (ASTAT |= SFLAG)
#define CLR_S  (ASTAT &= ~SFLAG)
#define SET_C  (ASTAT |= CFLAG)
#define SET_V  (ASTAT |= VFLAG)
#define SET_N  (ASTAT |= NFLAG)
#define SET_Z  (ASTAT |= ZFLAG)
#define CLR_FLAGS (ASTAT &= c->astat_clear)
#define CALC_Z(r) (ASTAT |= ((r & 0xffff) == 0))
#define CALC_N(r) (ASTAT |= (r >> 14) & 0x02)
#define CALC_V(s, d, r) (ASTAT |= ((s ^ d ^ r ^ (r >> 1)) >> 13) & 0x04)
#define CALC_C(r) (ASTAT |= (r >> 13) & 0x08)
#define CALC_C_SUB(r) (ASTAT |= (~r >> 13) & 0x08)
#define CALC_NZ(r) CLR_FLAGS; CALC_N(r); CALC_Z(r)
#define CALC_NZVC(s, d, r) CLR_FLAGS; CALC_N(r); CALC_Z(r); CALC_V(s, d, r); CALC_C(r)
#define CALC_NZVC_SUB(s, d, r) CLR_FLAGS; CALC_N(r); CALC_Z(r); CALC_V(s, d, r); CALC_C_SUB(r)

static int32_t sext(uint32_t v, int bits) { return (int32_t)(v << (32 - bits)) >> (32 - bits); }

static int clz32(uint32_t x) {
    int n = 0;
    if (!x) return 32;
    while (!(x & 0x80000000u)) { x <<= 1; n++; }
    return n;
}

/* ---- memory ---- */
static inline uint32_t program_read(adsp2100* c, uint32_t a) {
    a &= 0x3fff;
    return (a < 0x400 || (a >= 0x800 && c->prog_ext)) ? c->prog[a] : 0xffffffffu;
}
static inline void program_write(adsp2100* c, uint32_t a, uint32_t v) {
    a &= 0x3fff;
    if (a < 0x400 || (a >= 0x800 && c->prog_ext)) c->prog[a] = v & 0xffffff;
}
#define data_read(a) adsp_data_read((a) & 0x3fff)
#define data_write(a, v) adsp_data_write((a) & 0x3fff, (uint16_t)(v))

/* ---- MSTAT ---- */
static void check_irqs(adsp2100* c);

static void update_mstat(adsp2100* c) {
    if ((c->mstat ^ c->mstat_prev) & MSTAT_BANK) {
        adsp_core t = c->core; c->core = c->alt; c->alt = t;
    }
    if ((c->mstat ^ c->mstat_prev) & MSTAT_TIMER) adsp_timer_enable((c->mstat & MSTAT_TIMER) != 0);
    c->astat_clear = (c->mstat & MSTAT_STICKYV) ? ~(uint32_t)(CFLAG | NFLAG | ZFLAG) : ~(uint32_t)(CFLAG | VFLAG | NFLAG | ZFLAG);
    c->mstat_prev = c->mstat;
}

/* ---- stacks ---- */
static inline uint32_t pc_stack_top(adsp2100* c) { return c->pc_stack[c->pc_sp > 0 ? c->pc_sp - 1 : 0]; }
static inline void pc_stack_push_val(adsp2100* c, uint32_t v) {
    if (c->pc_sp < 16) { c->pc_stack[c->pc_sp++] = v; c->sstat &= ~PC_EMPTY; }
    else c->sstat |= PC_OVER;
}
static inline void pc_stack_push(adsp2100* c) { pc_stack_push_val(c, c->pc); }
static inline uint32_t pc_stack_pop_val(adsp2100* c) {
    if (c->pc_sp > 0) { c->pc_sp--; if (c->pc_sp == 0) c->sstat |= PC_EMPTY; }
    return c->pc_stack[c->pc_sp];
}
static inline void pc_stack_pop(adsp2100* c) { c->pc = pc_stack_pop_val(c); }

static inline void cntr_stack_push(adsp2100* c) {
    if (c->cntr_sp < 4) { c->cntr_stack[c->cntr_sp++] = c->cntr; c->sstat &= ~COUNT_EMPTY; }
    else c->sstat |= COUNT_OVER;
}
static inline void cntr_stack_pop(adsp2100* c) {
    if (c->cntr_sp > 0) { c->cntr_sp--; if (c->cntr_sp == 0) c->sstat |= COUNT_EMPTY; }
    c->cntr = c->cntr_stack[c->cntr_sp];
}

static inline void loop_stack_push(adsp2100* c, uint32_t v) {
    if (c->loop_sp < 4) {
        c->loop_stack[c->loop_sp++] = v;
        c->loop = v >> 4; c->loop_condition = v & 15;
        c->sstat &= ~LOOP_EMPTY;
    } else c->sstat |= LOOP_OVER;
}
static inline void loop_stack_pop(adsp2100* c) {
    if (c->loop_sp > 0) {
        c->loop_sp--;
        if (c->loop_sp == 0) { c->loop = 0xffff; c->loop_condition = 0; c->sstat |= LOOP_EMPTY; }
        else { c->loop = c->loop_stack[c->loop_sp - 1] >> 4; c->loop_condition = c->loop_stack[c->loop_sp - 1] & 15; }
    }
}

static inline void stat_stack_push(adsp2100* c) {
    if (c->stat_sp < 4) {
        c->stat_stack[c->stat_sp][0] = (uint16_t)c->mstat;
        c->stat_stack[c->stat_sp][1] = c->imask;
        c->stat_stack[c->stat_sp][2] = (uint16_t)c->astat;
        c->stat_sp++;
        c->sstat &= ~STATUS_EMPTY;
    } else c->sstat |= STATUS_OVER;
}
static void stat_stack_pop(adsp2100* c) {
    if (c->stat_sp > 0) { c->stat_sp--; if (c->stat_sp == 0) c->sstat |= STATUS_EMPTY; }
    c->mstat = c->stat_stack[c->stat_sp][0];
    update_mstat(c);
    c->imask = c->stat_stack[c->stat_sp][1];
    c->astat = c->stat_stack[c->stat_sp][2];
    check_irqs(c);
}

/* ---- conditions ---- */
static int slow_condition(adsp2100* c) {
    if ((int32_t)--c->cntr > 0) return 1;
    cntr_stack_pop(c);
    return 0;
}
#define condition(cc) (((cc) != 14) ? c->condition_table[((cc) << 8) | c->astat] : slow_condition(c))

/* ---- registers ---- */
static inline void update_i(adsp2100* c, int w) { c->base[w] = c->i[w] & c->lmask[w]; }
static inline void update_l(adsp2100* c, int w) { c->lmask[w] = c->mask_table[c->l[w] & 0x3fff]; c->base[w] = c->i[w] & c->lmask[w]; }

static void write_reg0(adsp2100* c, int r, int32_t v) {
    switch (r) {
        case 0x00: c->core.ax0.s = (int16_t)v; break;
        case 0x01: c->core.ax1.s = (int16_t)v; break;
        case 0x02: c->core.mx0.s = (int16_t)v; break;
        case 0x03: c->core.mx1.s = (int16_t)v; break;
        case 0x04: c->core.ay0.s = (int16_t)v; break;
        case 0x05: c->core.ay1.s = (int16_t)v; break;
        case 0x06: c->core.my0.s = (int16_t)v; break;
        case 0x07: c->core.my1.s = (int16_t)v; break;
        case 0x08: c->core.si.s = (int16_t)v; break;
        case 0x09: c->core.se.s = (int8_t)v; break;
        case 0x0a: c->core.ar.s = (int16_t)v; break;
        case 0x0b: c->core.mr.mrx.mr0.s = (int16_t)v; break;
        case 0x0c: c->core.mr.mrx.mr1.s = (int16_t)v; c->core.mr.mrx.mr2.s = (int16_t)v >> 15; break;
        case 0x0d: c->core.mr.mrx.mr2.s = (int8_t)v; break;
        case 0x0e: c->core.sr.srx.sr0.s = (int16_t)v; break;
        case 0x0f: c->core.sr.srx.sr1.s = (int16_t)v; break;
    }
}

static void write_reg12(adsp2100* c, int idx, int r, int32_t v) {
    int i = idx + (r & 3);
    switch (r >> 2) {
        case 0: c->i[i] = v & 0x3fff; update_i(c, i); break;
        case 1: c->m[i] = sext((uint32_t)v, 14); break;
        case 2: c->l[i] = v & 0x3fff; update_l(c, i); break;
        default: break;   /* invalid on the 2101/2115 */
    }
}
#define write_reg1(c, r, v) write_reg12(c, 0, r, v)
#define write_reg2(c, r, v) write_reg12(c, 4, r, v)

static void write_reg3(adsp2100* c, int r, int32_t v) {
    switch (r) {
        case 0x00: c->astat = v & 0xff; break;
        case 0x01: c->mstat = v & 0x7f; update_mstat(c); break;
        case 0x03: c->imask = v & 0x3f; check_irqs(c); break;
        case 0x04: c->icntl = v & 0x1f; check_irqs(c); break;
        case 0x05: cntr_stack_push(c); c->cntr = v & 0x3fff; break;
        case 0x06: c->core.sb.s = (int16_t)sext((uint32_t)v, 5); break;
        case 0x07: c->px = (uint8_t)v; break;
        case 0x09: adsp_sport_tx(0, (uint16_t)v); break;
        case 0x0b: adsp_sport_tx(1, (uint16_t)v); break;
        case 0x0c:
            c->ifc = (uint16_t)v;
            if (BIT1(v, 1)) c->irq_latch[ADSP_IRQ0] = 0;
            if (BIT1(v, 2)) c->irq_latch[ADSP_IRQ1] = 0;
            if (BIT1(v, 3)) c->irq_latch[ADSP_SPORT0_RX] = 0;
            if (BIT1(v, 4)) c->irq_latch[ADSP_SPORT0_TX] = 0;
            if (BIT1(v, 5)) c->irq_latch[ADSP_IRQ2] = 0;
            if (BIT1(v, 7)) c->irq_latch[ADSP_IRQ0] = 1;
            if (BIT1(v, 8)) c->irq_latch[ADSP_IRQ1] = 1;
            if (BIT1(v, 9)) c->irq_latch[ADSP_SPORT0_RX] = 1;
            if (BIT1(v, 10)) c->irq_latch[ADSP_SPORT0_TX] = 1;
            if (BIT1(v, 11)) c->irq_latch[ADSP_IRQ2] = 1;
            check_irqs(c);
            break;
        case 0x0d: c->cntr = v & 0x3fff; break;
        case 0x0f: pc_stack_push_val(c, v & 0x3fff); break;
        default: break;
    }
}

static inline int32_t read_reg0(adsp2100* c, int r) { return *c->read0_ptr[r]; }
static inline int32_t read_reg1(adsp2100* c, int r) { return (int32_t)*c->read1_ptr[r]; }
static inline int32_t read_reg2(adsp2100* c, int r) { return (int32_t)*c->read2_ptr[r]; }
static int32_t read_reg3(adsp2100* c, int r) {
    switch (r) {
        case 0x00: return c->astat;
        case 0x01: return c->mstat;
        case 0x02: return c->sstat;
        case 0x03: return c->imask;
        case 0x04: return c->icntl;
        case 0x05: return c->cntr;
        case 0x06: return c->core.sb.s;
        case 0x07: return c->px;
        case 0x08: case 0x0a: return 0;   /* serial receive: nothing wired on DCS2 */
        case 0x0f: return pc_stack_pop_val(c);
        default: return 0;
    }
}

/* ---- DAGs ---- */
static inline void modify_address(adsp2100* c, uint32_t ireg, uint32_t mreg) {
    uint32_t base = c->base[ireg], i = c->i[ireg], l = c->l[ireg];
    i = (i + c->m[mreg]) & 0x3fff;
    if (i < base) i += l;
    else if (i >= base + l) i -= l;
    c->i[ireg] = i;
}

static inline uint32_t dag_addr(adsp2100* c, uint32_t ireg, uint32_t mreg) {
    uint32_t a = c->i[ireg];
    modify_address(c, ireg, mreg);
    return a;
}

static inline void data_write_dag1(adsp2100* c, uint32_t op, int32_t v) {
    uint32_t a = dag_addr(c, BITS(op, 2, 2), BITS(op, 0, 2));
    if (c->mstat & MSTAT_REVERSE) a = c->reverse_table[a & 0x3fff];
    data_write(a, v);
}
static inline uint32_t data_read_dag1(adsp2100* c, uint32_t op) {
    uint32_t a = dag_addr(c, BITS(op, 2, 2), BITS(op, 0, 2));
    if (c->mstat & MSTAT_REVERSE) a = c->reverse_table[a & 0x3fff];
    return data_read(a);
}
static inline void data_write_dag2(adsp2100* c, uint32_t op, int32_t v) {
    data_write(dag_addr(c, 4 + BITS(op, 2, 2), 4 + BITS(op, 0, 2)), v);
}
static inline uint32_t data_read_dag2(adsp2100* c, uint32_t op) {
    return data_read(dag_addr(c, 4 + BITS(op, 2, 2), 4 + BITS(op, 0, 2)));
}
static inline void pgm_write_dag2(adsp2100* c, uint32_t op, int32_t v) {
    program_write(c, dag_addr(c, 4 + BITS(op, 2, 2), 4 + BITS(op, 0, 2)), ((uint32_t)v << 8) | c->px);
}
static inline uint32_t pgm_read_dag2(adsp2100* c, uint32_t op) {
    uint32_t r = program_read(c, dag_addr(c, 4 + BITS(op, 2, 2), 4 + BITS(op, 0, 2)));
    c->px = (uint8_t)r;
    return r >> 8;
}

/* ---- ALU ---- */
#define XREG(x) (c->alu_x[x]->u)
#define YREG(y) (c->alu_y[y]->u)

static int32_t alu_op(adsp2100* c, uint32_t op) {
    int32_t xop = BITS(op, 8, 3), yop = BITS(op, 11, 2), res;
    switch (BITS(op, 13, 4)) {
        case 0x00: res = YREG(yop); CALC_NZ(res); break;
        case 0x01:
            yop = YREG(yop); res = yop + 1; CALC_NZ(res);
            if (yop == 0x7fff) SET_V; else if (yop == 0xffff) SET_C;
            break;
        case 0x02:
            xop = XREG(xop); yop = YREG(yop); yop += GET_C >> 3; res = xop + yop;
            CALC_NZVC(xop, yop, res); break;
        case 0x03:
            xop = XREG(xop); yop = YREG(yop); res = xop + yop;
            CALC_NZVC(xop, yop, res); break;
        case 0x04: res = YREG(yop) ^ 0xffff; CALC_NZ(res); break;
        case 0x05:
            yop = YREG(yop); res = -yop; CALC_NZ(res);
            if (yop == 0x8000) SET_V;
            if (yop == 0x0000) SET_C;
            break;
        case 0x06:
            xop = XREG(xop); yop = YREG(yop); res = xop - yop + (GET_C >> 3) - 1;
            CALC_NZVC_SUB(xop, yop, res); break;
        case 0x07:
            xop = XREG(xop); yop = YREG(yop); res = xop - yop;
            CALC_NZVC_SUB(xop, yop, res); break;
        case 0x08:
            yop = YREG(yop); res = yop - 1; CALC_NZ(res);
            if (yop == 0x8000) SET_V; else if (yop == 0x0000) SET_C;
            break;
        case 0x09:
            xop = XREG(xop); yop = YREG(yop); res = yop - xop;
            CALC_NZVC_SUB(yop, xop, res); break;
        case 0x0a:
            xop = XREG(xop); yop = YREG(yop); res = yop - xop + (GET_C >> 3) - 1;
            CALC_NZVC_SUB(yop, xop, res); break;
        case 0x0b: res = XREG(xop) ^ 0xffff; CALC_NZ(res); break;
        case 0x0c: xop = XREG(xop); yop = YREG(yop); res = xop & yop; CALC_NZ(res); break;
        case 0x0d: xop = XREG(xop); yop = YREG(yop); res = xop | yop; CALC_NZ(res); break;
        case 0x0e: xop = XREG(xop); yop = YREG(yop); res = xop ^ yop; CALC_NZ(res); break;
        default:   /* ABS X */
            xop = XREG(xop);
            res = (xop & 0x8000) ? -xop : xop;
            CLR_FLAGS; CLR_S;
            if (xop == 0) SET_Z;
            if (xop == 0x8000) { SET_N; SET_V; }
            if (xop & 0x8000) SET_S;
            break;
    }
    return res;
}

static void alu_op_ar(adsp2100* c, uint32_t op) {
    int32_t res = alu_op(c, op);
    if ((c->mstat & MSTAT_SATURATE) && GET_V) res = GET_C ? -32768 : 32767;
    c->core.ar.u = (uint16_t)res;
}
static void alu_op_af(adsp2100* c, uint32_t op) { c->core.af.u = (uint16_t)alu_op(c, op); }

/* ---- MAC ---- */
#define MXS(x) ((int32_t)c->mac_x[x]->s)
#define MXU(x) ((int32_t)c->mac_x[x]->u)
#define MYS(y) ((int32_t)c->mac_y[y]->s)
#define MYU(y) ((int32_t)c->mac_y[y]->u)

/* returns 0 for the no-op form */
static int mac_op(adsp2100* c, uint32_t op, int64_t* out) {
    int shift = (c->mstat & MSTAT_INTEGER) ? 0 : 1;
    int xr = BITS(op, 8, 3), yr = BITS(op, 11, 2), f = BITS(op, 13, 4);
    int32_t xop, yop, temp;
    int64_t res, mr = (int64_t)c->core.mr.mr;
    if (f == 0) return 0;
    switch (f) {
        case 0x05: case 0x09: case 0x0d: xop = MXS(xr); yop = MYU(yr); break;    /* SU */
        case 0x06: case 0x0a: case 0x0e: xop = MXU(xr); yop = MYS(yr); break;    /* US */
        case 0x07: case 0x0b: case 0x0f: xop = MXU(xr); yop = MYU(yr); break;    /* UU */
        default:                         xop = MXS(xr); yop = MYS(yr); break;    /* SS, RND */
    }
    temp = (int32_t)(((uint32_t)xop * (uint32_t)yop) << shift);
    switch (f) {
        case 0x01: case 0x04: case 0x05: case 0x06: case 0x07: res = (int64_t)temp; break;
        case 0x02: case 0x08: case 0x09: case 0x0a: case 0x0b: res = (int64_t)((uint64_t)mr + (uint64_t)(int64_t)temp); break;
        default: res = (int64_t)((uint64_t)mr - (uint64_t)(int64_t)temp); break;
    }
    if (f <= 3) {   /* RND */
        temp &= 0xffff;
        res += 0x8000;
        if (temp == 0x8000) res &= ~(int64_t)0x10000;
    }
    *out = res;
    return 1;
}

static void mac_op_mr(adsp2100* c, uint32_t op) {
    int64_t res;
    if (!mac_op(c, op, &res)) return;
    uint32_t t = (uint32_t)((uint64_t)res >> 31) & 0x1ff;
    CLR_MV;
    if (t != 0x000 && t != 0x1ff) SET_MV;
    c->core.mr.mr = (uint64_t)res;
}
static void mac_op_mf(adsp2100* c, uint32_t op) {
    int64_t res;
    if (!mac_op(c, op, &res)) return;
    c->core.mf.u = (uint16_t)((uint32_t)res >> 16);
}

/* ---- shifter ---- */
static void shift_core(adsp2100* c, uint32_t op, int sc, int imm) {
    int xr = BITS(op, 8, 3);
    int32_t xop;
    uint32_t res;
    switch (BITS(op, 11, 4)) {
        case 0x00: case 0x01:   /* LSHIFT (HI) */
            xop = (int32_t)((uint32_t)c->shift_x[xr]->u << 16);
            if (sc > 0) res = (sc < 32) ? ((uint32_t)xop << sc) : 0;
            else res = (sc > -32) ? ((uint32_t)xop >> -sc) : 0;
            if (op & 0x800) c->core.sr.sr |= res; else c->core.sr.sr = res;
            break;
        case 0x02: case 0x03:   /* LSHIFT (LO) */
            xop = c->shift_x[xr]->u;
            if (sc > 0) res = (sc < 32) ? ((uint32_t)xop << sc) : 0;
            else res = (sc > -32) ? ((uint32_t)xop >> -sc) : 0;
            if (op & 0x800) c->core.sr.sr |= res; else c->core.sr.sr = res;
            break;
        case 0x04: case 0x05:   /* ASHIFT (HI) */
            xop = (int32_t)((uint32_t)(int32_t)c->shift_x[xr]->s << 16);
            if (sc > 0) res = (sc < 32) ? ((uint32_t)xop << sc) : 0;
            else res = (sc > -32) ? (uint32_t)(xop >> -sc) : (uint32_t)(xop >> 31);
            if (op & 0x800) c->core.sr.sr |= res; else c->core.sr.sr = res;
            break;
        case 0x06: case 0x07:   /* ASHIFT (LO) */
            xop = c->shift_x[xr]->s;
            if (sc > 0) res = (sc < 32) ? ((uint32_t)xop << sc) : 0;
            else res = (sc > -32) ? (uint32_t)(xop >> -sc) : (uint32_t)(xop >> 31);
            if (op & 0x800) c->core.sr.sr |= res; else c->core.sr.sr = res;
            break;
        case 0x08: case 0x09:   /* NORM (HI) */
            xop = (int32_t)((uint32_t)(int32_t)c->shift_x[xr]->s << 16);
            if (sc > 0) {
                xop = (int32_t)(((uint32_t)xop >> 1) | ((c->astat & CFLAG) << 28));
                res = (uint32_t)(xop >> (sc - 1 < 31 ? sc - 1 : 31));
            } else res = (sc > -32) ? ((uint32_t)xop << -sc) : 0;
            if (op & 0x800) c->core.sr.sr |= res; else c->core.sr.sr = res;
            break;
        case 0x0a: case 0x0b:   /* NORM (LO) */
            xop = c->shift_x[xr]->u;
            if (sc > 0) res = (sc < 32) ? ((uint32_t)xop >> sc) : 0;
            else res = (sc > -32) ? ((uint32_t)xop << -sc) : 0;
            if (op & 0x800) c->core.sr.sr |= res; else c->core.sr.sr = res;
            break;
        case 0x0c:              /* EXP (HI) */
            if (imm) break;
            xop = c->shift_x[xr]->s;
            if (xop < 0) { SET_SS; res = clz32(~(uint32_t)xop) - 16 - 1; }
            else { CLR_SS; res = clz32((uint32_t)xop) - 16 - 1; }
            c->core.se.s = (int16_t)-(int32_t)res;
            break;
        case 0x0d:              /* EXP (HIX) */
            if (imm) break;
            xop = c->shift_x[xr]->s;
            if (GET_V) {
                c->core.se.s = 1;
                if (xop < 0) CLR_SS; else SET_SS;
            } else {
                if (xop < 0) { SET_SS; res = clz32(~(uint32_t)xop) - 16 - 1; }
                else { CLR_SS; res = clz32((uint32_t)xop) - 16 - 1; }
                c->core.se.s = (int16_t)-(int32_t)res;
            }
            break;
        case 0x0e:              /* EXP (LO) */
            if (imm) break;
            if (c->core.se.s == -15) {
                xop = c->shift_x[xr]->s;
                res = clz32((uint32_t)((GET_SS ? ~xop : xop) & 0xffff)) - 1;
                c->core.se.s = (int16_t)-(int32_t)res;
            }
            break;
        default: {              /* EXPADJ */
            int32_t r;
            if (imm) break;
            xop = c->shift_x[xr]->s;
            r = clz32((uint32_t)((xop < 0) ? ~xop : xop)) - 16 - 1;
            if (r < -c->core.sb.s) c->core.sb.s = (int16_t)-r;
            break;
        }
    }
}
static void shift_op(adsp2100* c, uint32_t op) { shift_core(c, op, (int8_t)c->core.se.s, 0); }
static void shift_op_imm(adsp2100* c, uint32_t op) { shift_core(c, op, (int8_t)op, 1); }

/* ---- interrupts (ADSP-2101 family priorities) ---- */
static int generate_irq(adsp2100* c, int which, int indx) {
    if (!(c->imask & (0x20 >> indx))) return 0;
    c->irq_latch[which] = 0;
    pc_stack_push(c);
    stat_stack_push(c);
    c->pc = 0x04 + indx * 4;
    c->idle = 0;
    if (c->icntl & 0x10) c->imask &= ~(0x3f >> indx);
    else c->imask &= ~0x3f;
    return 1;
}

static void check_irqs(adsp2100* c) {
    uint8_t chk;
    chk = (c->icntl & 4) ? c->irq_latch[ADSP_IRQ2] : c->irq_state[ADSP_IRQ2];
    if (chk && generate_irq(c, ADSP_IRQ2, 0)) return;
    chk = c->irq_latch[ADSP_SPORT0_TX];
    if (chk && generate_irq(c, ADSP_SPORT0_TX, 1)) return;
    chk = c->irq_latch[ADSP_SPORT0_RX];
    if (chk && generate_irq(c, ADSP_SPORT0_RX, 2)) return;
    chk = (c->icntl & 2) ? c->irq_latch[ADSP_IRQ1] : c->irq_state[ADSP_IRQ1];
    if (chk && generate_irq(c, ADSP_IRQ1, 3)) return;
    chk = (c->icntl & 1) ? c->irq_latch[ADSP_IRQ0] : c->irq_state[ADSP_IRQ0];
    if (chk && generate_irq(c, ADSP_IRQ0, 4)) return;
    chk = c->irq_latch[ADSP_TIMER];
    if (chk && generate_irq(c, ADSP_TIMER, 5)) return;
}
void adsp_check_irqs(adsp2100* c) { check_irqs(c); }

void adsp_set_irq(adsp2100* c, int line, int state) {
    if (state && !c->irq_state[line]) c->irq_latch[line] = 1;
    c->irq_state[line] = (uint8_t)(state != 0);
}

/* ---- setup ---- */
void adsp_init(adsp2100* c) {
    memset(c, 0, sizeof(*c));
    for (int i = 0; i < 0x4000; i++) {
        uint16_t d = 0;
        for (int b = 0; b < 14; b++) if (i & (1 << b)) d |= (uint16_t)(1 << (13 - b));
        c->reverse_table[i] = d;
    }
    for (int i = 0; i < 0x4000; i++) {
        uint16_t m = 0x3fff;
        for (int b = 13; b >= 0; b--) if (i > (1 << b)) { m = (uint16_t)((0x3fff << (b + 1)) & 0x3fff); break; }
        c->mask_table[i] = m;
    }
    for (int i = 0; i < 0x100; i++) {
        int az = (i & ZFLAG) != 0, an = (i & NFLAG) != 0, av = (i & VFLAG) != 0;
        int ac = (i & CFLAG) != 0, mv = (i & MVFLAG) != 0, as = (i & SFLAG) != 0;
        c->condition_table[i | 0x000] = (uint8_t)az;
        c->condition_table[i | 0x100] = (uint8_t)!az;
        c->condition_table[i | 0x200] = (uint8_t)!((an ^ av) | az);
        c->condition_table[i | 0x300] = (uint8_t)((an ^ av) | az);
        c->condition_table[i | 0x400] = (uint8_t)(an ^ av);
        c->condition_table[i | 0x500] = (uint8_t)!(an ^ av);
        c->condition_table[i | 0x600] = (uint8_t)av;
        c->condition_table[i | 0x700] = (uint8_t)!av;
        c->condition_table[i | 0x800] = (uint8_t)ac;
        c->condition_table[i | 0x900] = (uint8_t)!ac;
        c->condition_table[i | 0xa00] = (uint8_t)as;
        c->condition_table[i | 0xb00] = (uint8_t)!as;
        c->condition_table[i | 0xc00] = (uint8_t)mv;
        c->condition_table[i | 0xd00] = (uint8_t)!mv;
        c->condition_table[i | 0xf00] = 1;
    }
    adsp_core* k = &c->core;
    int16_t* r0[16] = { &k->ax0.s, &k->ax1.s, &k->mx0.s, &k->mx1.s, &k->ay0.s, &k->ay1.s, &k->my0.s, &k->my1.s,
                        &k->si.s, &k->se.s, &k->ar.s, &k->mr.mrx.mr0.s, &k->mr.mrx.mr1.s, &k->mr.mrx.mr2.s,
                        &k->sr.srx.sr0.s, &k->sr.srx.sr1.s };
    memcpy(c->read0_ptr, r0, sizeof(r0));
    for (int i = 0; i < 4; i++) {
        c->read1_ptr[0x00 + i] = &c->i[i];     c->read2_ptr[0x00 + i] = &c->i[4 + i];
        c->read1_ptr[0x04 + i] = (uint32_t*)&c->m[i]; c->read2_ptr[0x04 + i] = (uint32_t*)&c->m[4 + i];
        c->read1_ptr[0x08 + i] = &c->l[i];     c->read2_ptr[0x08 + i] = &c->l[4 + i];
        c->read1_ptr[0x0c + i] = &c->l[i];     c->read2_ptr[0x0c + i] = &c->l[4 + i];
    }
    adsp_reg16* ax[8] = { &k->ax0, &k->ax1, &k->ar, &k->mr.mrx.mr0, &k->mr.mrx.mr1, &k->mr.mrx.mr2, &k->sr.srx.sr0, &k->sr.srx.sr1 };
    adsp_reg16* ay[4] = { &k->ay0, &k->ay1, &k->af, &k->zero };
    adsp_reg16* mx[8] = { &k->mx0, &k->mx1, &k->ar, &k->mr.mrx.mr0, &k->mr.mrx.mr1, &k->mr.mrx.mr2, &k->sr.srx.sr0, &k->sr.srx.sr1 };
    adsp_reg16* my[4] = { &k->my0, &k->my1, &k->mf, &k->zero };
    adsp_reg16* sx[8] = { &k->si, &k->si, &k->ar, &k->mr.mrx.mr0, &k->mr.mrx.mr1, &k->mr.mrx.mr2, &k->sr.srx.sr0, &k->sr.srx.sr1 };
    memcpy(c->alu_x, ax, sizeof(ax)); memcpy(c->alu_y, ay, sizeof(ay));
    memcpy(c->mac_x, mx, sizeof(mx)); memcpy(c->mac_y, my, sizeof(my));
    memcpy(c->shift_x, sx, sizeof(sx));
}

void adsp_reset(adsp2100* c) {
    c->core.zero.u = c->alt.zero.u = 0;
    for (int i = 0; i < 4; i++) {
        write_reg1(c, 0x08 + i, c->l[i]);     write_reg1(c, 0x00 + i, c->i[i]);
        write_reg2(c, 0x08 + i, c->l[4 + i]); write_reg2(c, 0x00 + i, c->i[4 + i]);
    }
    c->pc = c->ppc = 0;
    c->loop = 0xffff; c->loop_condition = 0;
    c->astat_clear = ~(uint32_t)(CFLAG | VFLAG | NFLAG | ZFLAG);
    c->mstat = 0; c->sstat = 0x55; c->idle = 0;
    update_mstat(c);
    c->pc_sp = c->cntr_sp = c->stat_sp = c->loop_sp = 0;
    c->flagout = c->flagin = c->fl0 = c->fl1 = c->fl2 = 0;
    c->imask = 0;
    memset(c->irq_state, 0, sizeof(c->irq_state));
    memset(c->irq_latch, 0, sizeof(c->irq_latch));
}

void adsp_load_boot_data(adsp2100* c, const uint8_t* src) {
    int len = (src[3] + 1) * 8;
    for (int i = 0; i < len && i < 0x400; i++)
        c->prog[i] = ((uint32_t)src[i * 4 + 0] << 16) | ((uint32_t)src[i * 4 + 1] << 8) | src[i * 4 + 2];
}

/* ---- execution ---- */
static void compute(adsp2100* c, uint32_t op) {
    switch (BITS(op, 17, 2)) {
        case 0: mac_op_mr(c, op); break;
        case 1: alu_op_ar(c, op); break;
        case 2: mac_op_mf(c, op); break;
        default: alu_op_af(c, op); break;
    }
}

void adsp_execute(adsp2100* c, int cycles) {
    c->icount = cycles;
    check_irqs(c);
    if (c->idle) { c->icount = 0; return; }
    do {
        uint32_t op, temp;
        c->ppc = c->pc;
        op = program_read(c, c->pc);
        if (c->pc != c->loop) c->pc++;
        else if (condition(c->loop_condition)) c->pc = pc_stack_top(c);
        else { loop_stack_pop(c); pc_stack_pop_val(c); c->pc++; }

        switch (BITS(op, 16, 8)) {
            case 0x00: case 0x01: case 0x08: break;
            case 0x02:
                if (BIT1(op, 15)) { c->idle = 1; c->icount = 0; }
                else if (condition(BITS(op, 0, 4))) {
                    if (BIT1(op, 5)) c->flagout = 0;
                    if (BIT1(op, 4)) c->flagout ^= 1;
                    if (BIT1(op, 7)) c->fl0 = 0;
                    if (BIT1(op, 6)) c->fl0 ^= 1;
                    if (BIT1(op, 9)) c->fl1 = 0;
                    if (BIT1(op, 8)) c->fl1 ^= 1;
                    if (BIT1(op, 11)) c->fl2 = 0;
                    if (BIT1(op, 10)) c->fl2 ^= 1;
                }
                break;
            case 0x03:
                if (BIT1(op, 1) ? c->flagin : !c->flagin) {
                    if (BIT1(op, 0)) pc_stack_push(c);
                    c->pc = BITS(op, 4, 12) | BITS(op, 2, 2) << 12;
                }
                break;
            case 0x04:
                if (BIT1(op, 4)) pc_stack_pop_val(c);
                if (BIT1(op, 3)) loop_stack_pop(c);
                if (BIT1(op, 2)) cntr_stack_pop(c);
                if (BIT1(op, 1)) { if (BIT1(op, 0)) stat_stack_pop(c); else stat_stack_push(c); }
                break;
            case 0x05:
                if (GET_MV) {
                    if (c->core.mr.mrx.mr2.u & 0x80) { c->core.mr.mrx.mr2.u = 0xffff; c->core.mr.mrx.mr1.u = 0x8000; c->core.mr.mrx.mr0.u = 0x0000; }
                    else { c->core.mr.mrx.mr2.u = 0x0000; c->core.mr.mrx.mr1.u = 0x7fff; c->core.mr.mrx.mr0.u = 0xffff; }
                }
                break;
            case 0x06: {   /* DIVS */
                uint32_t xop = XREG(BITS(op, 8, 3)), yop = YREG(BITS(op, 11, 2));
                temp = xop ^ yop;
                c->astat = (c->astat & ~QFLAG) | ((temp >> 10) & QFLAG);
                c->core.af.u = (uint16_t)((yop << 1) | (c->core.ay0.u >> 15));
                c->core.ay0.u = (uint16_t)((c->core.ay0.u << 1) | (temp >> 15));
                break;
            }
            case 0x07: {   /* DIVQ */
                uint32_t xop = XREG(BITS(op, 8, 3));
                int32_t res = GET_Q ? (int32_t)c->core.af.u + (int32_t)xop : (int32_t)c->core.af.u - (int32_t)xop;
                temp = (uint32_t)res ^ xop;
                c->astat = (c->astat & ~QFLAG) | ((temp >> 10) & QFLAG);
                c->core.af.u = (uint16_t)(((uint32_t)res << 1) | (c->core.ay0.u >> 15));
                c->core.ay0.u = (uint16_t)((c->core.ay0.u << 1) | ((~temp >> 15) & 1));
                break;
            }
            case 0x09:
                temp = BITS(op, 2, 3);
                modify_address(c, temp, (temp & 4) | (op & 3));
                break;
            case 0x0a:
                if (condition(BITS(op, 0, 4))) {
                    pc_stack_pop(c);
                    if (BIT1(op, 4)) stat_stack_pop(c);
                }
                break;
            case 0x0b:
                if (condition(BITS(op, 0, 4))) {
                    if (BIT1(op, 4)) pc_stack_push(c);
                    c->pc = c->i[4 + BITS(op, 6, 2)] & 0x3fff;
                }
                break;
            case 0x0c:
                if (BIT1(op, 3)) c->mstat = BIT1(op, 2) ? (c->mstat | MSTAT_GOMODE) : (c->mstat & ~MSTAT_GOMODE);
                if (BIT1(op, 13)) c->mstat = BIT1(op, 12) ? (c->mstat | MSTAT_INTEGER) : (c->mstat & ~MSTAT_INTEGER);
                if (BIT1(op, 15)) c->mstat = BIT1(op, 14) ? (c->mstat | MSTAT_TIMER) : (c->mstat & ~MSTAT_TIMER);
                if (BIT1(op, 5)) c->mstat = BIT1(op, 4) ? (c->mstat | MSTAT_BANK) : (c->mstat & ~MSTAT_BANK);
                if (BIT1(op, 7)) c->mstat = BIT1(op, 6) ? (c->mstat | MSTAT_REVERSE) : (c->mstat & ~MSTAT_REVERSE);
                if (BIT1(op, 9)) c->mstat = BIT1(op, 8) ? (c->mstat | MSTAT_STICKYV) : (c->mstat & ~MSTAT_STICKYV);
                if (BIT1(op, 11)) c->mstat = BIT1(op, 10) ? (c->mstat | MSTAT_SATURATE) : (c->mstat & ~MSTAT_SATURATE);
                update_mstat(c);
                break;
            case 0x0d: {
                int s = BITS(op, 0, 4), d = BITS(op, 4, 4);
                int32_t v;
                switch (BITS(op, 8, 2)) {
                    case 0: v = read_reg0(c, s); break;
                    case 1: v = read_reg1(c, s); break;
                    case 2: v = read_reg2(c, s); break;
                    default: v = read_reg3(c, s); break;
                }
                switch (BITS(op, 10, 2)) {
                    case 0: write_reg0(c, d, v); break;
                    case 1: write_reg1(c, d, v); break;
                    case 2: write_reg2(c, d, v); break;
                    default: write_reg3(c, d, v); break;
                }
                break;
            }
            case 0x0e: if (condition(BITS(op, 0, 4))) shift_op(c, op); break;
            case 0x0f: shift_op_imm(c, op); break;
            case 0x10:
                shift_op(c, op);
                write_reg0(c, BITS(op, 4, 4), read_reg0(c, BITS(op, 0, 4)));
                break;
            case 0x11:
                if (BIT1(op, 15)) { pgm_write_dag2(c, op, read_reg0(c, BITS(op, 4, 4))); shift_op(c, op); }
                else { shift_op(c, op); write_reg0(c, BITS(op, 4, 4), pgm_read_dag2(c, op)); }
                break;
            case 0x12:
                if (BIT1(op, 15)) { data_write_dag1(c, op, read_reg0(c, BITS(op, 4, 4))); shift_op(c, op); }
                else { shift_op(c, op); write_reg0(c, BITS(op, 4, 4), data_read_dag1(c, op)); }
                break;
            case 0x13:
                if (BIT1(op, 15)) { data_write_dag2(c, op, read_reg0(c, BITS(op, 4, 4))); shift_op(c, op); }
                else { shift_op(c, op); write_reg0(c, BITS(op, 4, 4), data_read_dag2(c, op)); }
                break;
            case 0x14: case 0x15: case 0x16: case 0x17:
                loop_stack_push(c, op & 0x3ffff);
                pc_stack_push(c);
                break;
            case 0x18: case 0x19: case 0x1a: case 0x1b:
                if (condition(BITS(op, 0, 4))) {
                    c->pc = BITS(op, 4, 14);
                    if (c->pc == c->ppc) c->icount = 0;   /* jump to self: nothing changes until an event */
                }
                break;
            case 0x1c: case 0x1d: case 0x1e: case 0x1f:
                if (condition(BITS(op, 0, 4))) { pc_stack_push(c); c->pc = BITS(op, 4, 14); }
                break;
            case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
                if (condition(BITS(op, 0, 4))) compute(c, op);
                break;
            case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c: case 0x2d: case 0x2e: case 0x2f:
                temp = (uint32_t)read_reg0(c, BITS(op, 0, 4));
                compute(c, op);
                write_reg0(c, BITS(op, 4, 4), (int32_t)temp);
                break;
            case 0x30: case 0x31: case 0x32: case 0x33: write_reg0(c, BITS(op, 0, 4), sext(op >> 4, 14)); break;
            case 0x34: case 0x35: case 0x36: case 0x37: write_reg1(c, BITS(op, 0, 4), sext(op >> 4, 14)); break;
            case 0x38: case 0x39: case 0x3a: case 0x3b: write_reg2(c, BITS(op, 0, 4), sext(op >> 4, 14)); break;
            case 0x3c: case 0x3d: case 0x3e: case 0x3f: write_reg3(c, BITS(op, 0, 4), sext(op >> 4, 14)); break;
            case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
            case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4e: case 0x4f:
                write_reg0(c, BITS(op, 0, 4), (int32_t)BITS(op, 4, 16));
                break;
            case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
                compute(c, op);
                write_reg0(c, BITS(op, 4, 4), (int32_t)pgm_read_dag2(c, op));
                break;
            case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5e: case 0x5f:
                pgm_write_dag2(c, op, read_reg0(c, BITS(op, 4, 4)));
                compute(c, op);
                break;
            case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
                compute(c, op);
                write_reg0(c, BITS(op, 4, 4), (int32_t)data_read_dag1(c, op));
                break;
            case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6e: case 0x6f:
                data_write_dag1(c, op, read_reg0(c, BITS(op, 4, 4)));
                compute(c, op);
                break;
            case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
                compute(c, op);
                write_reg0(c, BITS(op, 4, 4), (int32_t)data_read_dag2(c, op));
                break;
            case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7e: case 0x7f:
                data_write_dag2(c, op, read_reg0(c, BITS(op, 4, 4)));
                compute(c, op);
                break;
            case 0x80: case 0x81: case 0x82: case 0x83: write_reg0(c, BITS(op, 0, 4), data_read(BITS(op, 4, 14))); break;
            case 0x84: case 0x85: case 0x86: case 0x87: write_reg1(c, BITS(op, 0, 4), data_read(BITS(op, 4, 14))); break;
            case 0x88: case 0x89: case 0x8a: case 0x8b: write_reg2(c, BITS(op, 0, 4), data_read(BITS(op, 4, 14))); break;
            case 0x8c: case 0x8d: case 0x8e: case 0x8f: write_reg3(c, BITS(op, 0, 4), data_read(BITS(op, 4, 14))); break;
            case 0x90: case 0x91: case 0x92: case 0x93: data_write(BITS(op, 4, 14), read_reg0(c, BITS(op, 0, 4))); break;
            case 0x94: case 0x95: case 0x96: case 0x97: data_write(BITS(op, 4, 14), read_reg1(c, BITS(op, 0, 4))); break;
            case 0x98: case 0x99: case 0x9a: case 0x9b: data_write(BITS(op, 4, 14), read_reg2(c, BITS(op, 0, 4))); break;
            case 0x9c: case 0x9d: case 0x9e: case 0x9f: data_write(BITS(op, 4, 14), read_reg3(c, BITS(op, 0, 4))); break;
            default:
                if (op < 0xb00000) {
                    if (op < 0xa00000) break;   /* unreachable */
                    data_write_dag1(c, op, (int32_t)BITS(op, 4, 16));
                } else if (op < 0xc00000) {
                    data_write_dag2(c, op, (int32_t)BITS(op, 4, 16));
                } else {
                    /* 11PPDDAx: ALU/MAC to AR/MR with DM read via DAG1 and PM read via DAG2 */
                    static const int dd_ofs[4] = { offsetof(adsp_core, ax0), offsetof(adsp_core, ax1), offsetof(adsp_core, mx0), offsetof(adsp_core, mx1) };
                    static const int pd_ofs[4] = { offsetof(adsp_core, ay0), offsetof(adsp_core, ay1), offsetof(adsp_core, my0), offsetof(adsp_core, my1) };
                    if (BIT1(op, 17)) alu_op_ar(c, op); else mac_op_mr(c, op);
                    ((adsp_reg16*)((uint8_t*)&c->core + dd_ofs[BITS(op, 18, 2)]))->u = (uint16_t)data_read_dag1(c, op);
                    ((adsp_reg16*)((uint8_t*)&c->core + pd_ofs[BITS(op, 20, 2)]))->u = (uint16_t)pgm_read_dag2(c, op >> 4);
                }
                break;
        }
        c->icount--;
    } while (c->icount > 0);
}
