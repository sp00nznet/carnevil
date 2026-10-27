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

/* What the cabinet's controls feed the board. main.c changes it only between
 * frames (in video_pump), so a run is a pure function of these per frame --
 * which is what lets two machines play in lockstep. */
enum { GUN_TRIGGER = 1, GUN_PUMP = 2, GUN_OFFSCREEN = 4 };
typedef struct {
    uint16_t x, y;           /* aim, 0..65535 across the screen, left/top = 0 */
    uint8_t buttons;         /* GUN_* */
} gun_state;
typedef struct {
    uint16_t system;         /* I/O ASIC port 1, active low: coins, starts, service */
    uint16_t dips;           /* port 0, active low */
    gun_state gun[2];
    uint32_t cheats;         /* CHEAT_* bits (cheats.h), applied once a frame */
} cabinet_inputs;
extern cabinet_inputs g_inputs;

/* Frames (vblanks) since power-on. */
uint64_t hw_frame(void);

/* A hash of main RAM, for netplay's desync check (about 2 ms). */
uint32_t hw_ram_hash(void);

/* Run flat out instead of at the board's 57 Hz (fast forward). Emulated time
 * is unaffected; only the wait for the wall clock is skipped. */
extern int g_turbo;

/* System port bits */
enum { SYS_COIN1 = 0x0001, SYS_COIN2 = 0x0002, SYS_START1 = 0x0004, SYS_TILT = 0x0008,
       SYS_TEST = 0x0010, SYS_START2 = 0x0020, SYS_SERVICE = 0x0040 };
#define DIP_TEST_SWITCH 0x8000   /* active low: clear = test mode */

/* Video: called by hw.c on every vblank with the displayed buffer. */
void video_present(const uint16_t* fb, int stride, int width, int height);
int  video_pump(void);   /* returns 0 once the window was closed */

/* function lookup (rtos.c) */
void lookup_add(uint32_t vram, recomp_func_t* func);
void lookup_finish(void);
void rtos_register_slots(void);
void proc_register(void);

#endif
