/*
 * Cheats and operator options: the Cheats menu.
 *
 * Cheats are bits in cabinet_inputs.cheats, so in a netplay session they
 * travel with the host's inputs and both machines apply them on the same
 * frame. Options are chosen in the menu, kept in carnevil.ini, sent to the
 * joiner at the start of a session, and fixed for the run (changing one that
 * needs it relaunches the game).
 */
#ifndef CARNEVIL_CHEATS_H
#define CARNEVIL_CHEATS_H

#include <stdint.h>
#include "hw.h"

#define MAX_OPTIONS 16

typedef struct {
    const char* label;            /* menu text, with & for the accelerator */
    int is_action;                /* 1: a one-shot (e.g. add credits), 0: a toggle */
} cheat_def;

typedef struct {
    const char* label;
    const char* const* choices;
    int count;
    int def;                      /* default choice */
    int restart;                  /* 1: takes effect from power-on (relaunch) */
} option_def;

extern const cheat_def k_cheats[];      /* index n is bit (1u << n) */
extern const int k_cheat_count;
extern const option_def k_options[];
extern const int k_option_count;

/* Called once a frame, after the frame's inputs are final and before the
 * board sees them. May adjust in->dips (e.g. free play) and the game's RAM.
 * Must be deterministic: only frame, inputs and options may matter. */
void cheats_frame(cabinet_inputs* in, const uint8_t options[MAX_OPTIONS], uint64_t frame);

#endif
