/**
 * CarnEvil RTOS Cooperative Scheduler
 *
 * Implements cooperative multitasking using Windows fibers (or POSIX ucontext).
 * Each RTOS task runs as a fiber that yields when calling event_wait.
 * The scheduler switches between fibers when a task blocks.
 *
 * This allows the game's producer/consumer rendering pipeline to work:
 * - Task A sends a message (render command)
 * - Task A calls event_wait (blocks/yields)
 * - Scheduler switches to Task B
 * - Task B receives the message and processes it (renders)
 * - Task B calls event_wait (blocks/yields)
 * - Scheduler switches back to Task A
 */

#ifndef CARNEVIL_RTOS_SCHEDULER_H
#define CARNEVIL_RTOS_SCHEDULER_H

#include "recomp.h"
#include <stdint.h>
#include <setjmp.h>

#define RTOS_MAX_FIBERS 32
#define RTOS_FIBER_STACK_SIZE (256 * 1024)  /* 256KB per fiber */

typedef struct {
    int id;
    uint32_t callback_vram;
    void* fiber;              /* Windows fiber or ucontext */
    int active;
    int blocked;              /* Waiting on event_wait */
    int blocked_channel;      /* Which channel it's waiting on */
    recomp_context ctx_save;  /* Saved register state */
    jmp_buf restart_jmp;      /* For exit_task to restart the callback loop */
} rtos_fiber_t;

typedef struct {
    rtos_fiber_t fibers[RTOS_MAX_FIBERS];
    int fiber_count;
    int current_fiber;        /* Index of currently running fiber */
    void* main_fiber;         /* The main thread's fiber */
    uint8_t* rdram;           /* Shared RAM pointer */
    int frame_done;           /* Set when all tasks have yielded */
} rtos_scheduler_t;

/* Initialize the scheduler */
void rtos_sched_init(rtos_scheduler_t* sched, uint8_t* rdram);

/* Create a new fiber for a task */
int rtos_sched_create_task(rtos_scheduler_t* sched, int task_id, uint32_t callback_vram);

/* Run one frame: schedule all fibers until they all block or complete */
void rtos_sched_run_frame(rtos_scheduler_t* sched, recomp_context* ctx);

/* Called by event_wait to yield the current fiber */
void rtos_sched_yield(rtos_scheduler_t* sched, int channel);

/* Called by message_send to wake a fiber waiting on a channel */
void rtos_sched_wake(rtos_scheduler_t* sched, int channel);

/* Initialize the per-task rendering context struct in game RAM.
 * Replicates RTOS boot: builds Voodoo init command lists in RAM
 * and stores pointers at the context struct offsets (+0x10..+0xF8)
 * that the task callback reads and sends on channels 0x85-0x9F. */
void rtos_init_task_context(uint8_t* rdram, uint32_t ctx_phys);

/* Global scheduler instance */
extern rtos_scheduler_t g_scheduler;

#endif
