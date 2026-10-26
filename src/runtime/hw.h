/* The Midway Seattle board, as seen by the recompiled CPU code. */
#ifndef CARNEVIL_HW_H
#define CARNEVIL_HW_H

#include <stdint.h>
#include "recomp.h"

extern uint8_t* g_rdram;

/* Context of the code running right now (boot thread, a process fiber, or an
 * interrupt). Interrupts read the interrupted code's $gp from it. */
extern recomp_context* g_cur_ctx;

typedef struct {
    const char* disk;        /* raw hard disk image (carnevi1.img) */
    const char* bootrom;     /* carnevil1_9.u32, readable at 0x1FC00000 */
    const char* nvram;       /* 128 KB battery-backed CMOS, created if missing */
    const char* dcs_rom;     /* sound102.u95, the DCS2 board's boot ROM */
} hw_config;

void hw_init(const hw_config* cfg);
const uint8_t* hw_bootrom(void);   /* the 512 KB boot ROM image */
void hw_shutdown(void);
void hw_fatal(const char* fmt, ...);

/* CPU-wide COP0 state */
void     hw_set_status(uint32_t value);
uint32_t hw_get_status(void);
uint32_t hw_get_cause(void);
uint32_t hw_cp0_count(void);

/* Called from host-side wait loops that execute no MIPS loads. */
void hw_idle(void);

/* Inputs, set by the window (main.c); active-low like the IOASIC ports. */
extern uint16_t g_in_system, g_in_dips, g_in_p1, g_in_p2;
extern uint16_t g_gun_x[2], g_gun_y[2], g_gun_buttons;

/* Video: called by hw.c on every vblank with the displayed buffer. */
void video_present(const uint16_t* fb, int stride, int width, int height);
int  video_pump(void);   /* returns 0 once the window was closed */

/* function lookup (rtos.c) */
void lookup_add(uint32_t vram, recomp_func_t* func);
void lookup_finish(void);
void rtos_register_slots(void);
void proc_register(void);

#endif
