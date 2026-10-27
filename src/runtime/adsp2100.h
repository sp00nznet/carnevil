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
 * ADSP-2100 family DSP core (the ADSP-2101/2115 flavour the DCS2 board uses).
 *
 * Follows MAME's src/devices/cpu/adsp2100/adsp2100.cpp, adsp2100.h and
 * 2100ops.hxx (Aaron Giles), used under the BSD-3-Clause license.
 *
 * One instance; the board supplies data memory, the serial-port transmit
 * hook and the timer-enable hook (see the externs at the bottom). Program
 * memory lives here: internal 0000-03ff, external 0800-3fff when prog_ext.
 */
#ifndef CARNEVIL_ADSP2100_H
#define CARNEVIL_ADSP2100_H

#include <stdint.h>

/* interrupt lines (ADSP-2101/2115 numbering) */
enum { ADSP_IRQ0 = 0, ADSP_IRQ1 = 1, ADSP_IRQ2 = 2, ADSP_SPORT0_RX = 3, ADSP_SPORT0_TX = 4, ADSP_TIMER = 5 };

typedef union { uint16_t u; int16_t s; } adsp_reg16;
typedef union { struct { adsp_reg16 sr0, sr1; } srx; uint32_t sr; } adsp_shift;               /* little-endian host */
typedef union { struct { adsp_reg16 mr0, mr1, mr2, mrzero; } mrx; uint64_t mr; } adsp_mac;

typedef struct {
    adsp_reg16 ax0, ax1, ay0, ay1, ar, af;
    adsp_reg16 mx0, mx1, my0, my1;
    adsp_mac mr;
    adsp_reg16 mf, si, se, sb;
    adsp_shift sr;
    adsp_reg16 zero;
} adsp_core;

typedef struct adsp2100 {
    uint32_t pc, ppc, loop, loop_condition, cntr;
    uint32_t astat, sstat, mstat, mstat_prev, astat_clear, idle;
    adsp_core core, alt;
    uint32_t i[8]; int32_t m[8]; uint32_t l[8], lmask[8], base[8];
    uint8_t px;
    uint32_t loop_stack[4], cntr_stack[4], pc_stack[16];
    uint16_t stat_stack[4][3];
    int32_t pc_sp, cntr_sp, stat_sp, loop_sp;
    uint8_t flagout, flagin, fl0, fl1, fl2;
    uint16_t imask; uint8_t icntl; uint16_t ifc;
    uint8_t irq_state[6], irq_latch[6];
    int icount;

    int16_t* read0_ptr[16];
    uint32_t* read1_ptr[16];
    uint32_t* read2_ptr[16];
    adsp_reg16 *alu_x[8], *alu_y[4], *mac_x[8], *mac_y[4], *shift_x[8];

    uint32_t prog[0x4000];   /* 24-bit words */
    int prog_ext;            /* external program memory (0800-3fff) mapped */

    uint8_t condition_table[0x1000];
    uint16_t mask_table[0x4000];
    uint16_t reverse_table[0x4000];
} adsp2100;

void adsp_init(adsp2100* c);
void adsp_reset(adsp2100* c);
void adsp_set_irq(adsp2100* c, int line, int state);
void adsp_check_irqs(adsp2100* c);
/* Run up to `cycles` (1 instruction = 1 cycle); returns when spent, or early
 * (icount forced to 0) on IDLE or a jump-to-self. */
void adsp_execute(adsp2100* c, int cycles);
/* ADSP-21xx 8-bit boot format: copy one boot page into internal program RAM */
void adsp_load_boot_data(adsp2100* c, const uint8_t* src);

/* board hooks */
uint16_t adsp_data_read(uint32_t addr);
void adsp_data_write(uint32_t addr, uint16_t data);
void adsp_sport_tx(int port, uint16_t data);
void adsp_timer_enable(int state);

#endif
