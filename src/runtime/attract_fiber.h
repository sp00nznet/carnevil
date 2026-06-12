/*
 * Attract-mode main-loop fiber.
 *
 * func_800C50AC is the attract-mode STATE MACHINE: it registers the entity
 * render callback, then flows through states servicing RTOS channels, advancing
 * one iteration per frame at its yield point (func_80151618). It is infinite by
 * design -- on real hardware it runs as an RTOS task that suspends each frame.
 *
 * Our earlier approach cold-called it once and longjmp-escaped the infinite
 * loop, which left the attract state machine mid-traversal (and the entity it
 * was building half-initialised -> the render handler then NULL-derefs). This
 * module instead runs func_800C50AC in its own Windows fiber that we resume
 * once per frame: its per-frame yield (func_80151618) suspends the fiber back
 * to the caller, so the state machine advances across frames with consistent
 * state -- the architecturally-correct model.
 *
 * Gated behind g_use_attract_fiber (default 0) so the existing escape-based
 * path stays the build default until this is proven.
 */
#ifndef CARNEVIL_ATTRACT_FIBER_H
#define CARNEVIL_ATTRACT_FIBER_H

#include <stdint.h>

/* Master switch. 0 = legacy cold-call + yield-escape (current default).
 * 1 = run func_800C50AC as a per-frame-resumed fiber (this module). */
extern int g_use_attract_fiber;

/* Resume the attract fiber for one frame's worth of work. Creates the fiber on
 * the first call. Returns when the fiber next suspends (its func_80151618 yield)
 * or completes. No-op once the fiber has returned. */
void attract_fiber_resume(uint8_t* rdram);

/* 1 while code is executing inside the attract fiber (used by the yield /
 * event_wait overrides to route a yield back to the resumer). */
int attract_fiber_in_context(void);

/* Suspend the attract fiber back to its resumer. Returns 1 if handled (we were
 * in the attract fiber), 0 otherwise. Call from the func_80151618 yield. */
int attract_fiber_yield(void);

/* 1 once func_800C50AC has actually returned (it normally never does). */
int attract_fiber_is_done(void);

#endif
