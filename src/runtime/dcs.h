/*
 * DCS2 sound board (ADSP-2115), as the I/O ASIC sees it.
 *
 * hw.c calls these from the I/O ASIC's sound registers; the board itself runs
 * from dcs_run(). Implemented in dcs.c (board) and adsp2100.c (the DSP).
 */
#ifndef CARNEVIL_DCS_H
#define CARNEVIL_DCS_H

#include <stdint.h>
#include <stddef.h>

/* Called once. rom is the board's boot ROM (sound102.u95, 32 KB). */
void dcs_init(const uint8_t* rom, size_t rom_size);

/* I/O ASIC SOUNDCTL bit 0: 0 holds the DSP in reset, 1 runs it. */
void dcs_reset_w(int state);

/* Main CPU -> DSP data latch (SOUNDOUT write). */
void dcs_data_w(uint16_t data);

/* DSP -> main CPU data latch (SOUNDIN read), and the read acknowledge. */
uint16_t dcs_data_r(void);
void dcs_ack_w(void);

/* Latch status for SOUNDSTAT: the bits MAME calls m_latch_control (0x800 output
 * empty, 0x400 input empty, as the ADSP sees them). */
int dcs_control_r(void);

/* Upper byte of SOUNDSTAT (DCS output_control). */
int dcs_data2_r(void);

/* I/O ASIC sound FIFO (CarnEvil streams sample data through it). The FIFO
 * itself lives in hw.c; the DSP reads it through these callbacks. */
typedef struct {
    uint16_t (*fifo_read)(void);
    uint16_t (*fifo_status)(void);   /* 0x08 empty, 0x10 half full, 0x20 full */
    void (*input_empty)(int state);  /* DSP took the main CPU's word: I/O ASIC irq bit 0x80 */
    void (*output_full)(int state);  /* DSP wrote a word for the main CPU: irq bit 0x40 */
} dcs_callbacks;
void dcs_set_callbacks(const dcs_callbacks* cb);
void dcs_fifo_notify(int count, int max);

/* Run the DSP for this many seconds of emulated time (called from hw_tick). */
void dcs_run(double seconds);

/* Pull rendered stereo samples (interleaved int16); returns frames written. */
int dcs_audio(int16_t* out, int max_frames, int* sample_rate);

#endif
