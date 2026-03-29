/**
 * CarnEvil Input System
 *
 * Abstracts input from multiple sources (mouse, gamepad, Sinden gun, network)
 * into the arcade's native format (IOASIC button register + analog lightgun).
 *
 * Designed for networked multiplayer: each player has an input_state struct
 * that can be filled from any local or remote source.
 *
 * Arcade input format (active-low, 0=pressed):
 *   bit 0:  P1 trigger       bit 4:  P2 trigger
 *   bit 1:  P1 pump/reload   bit 5:  P2 pump/reload
 *   bit 2:  P1 start         bit 6:  P2 start
 *   bit 7:  volume up        bit 8:  coin 1
 *   bit 9:  coin 2           bit 10: service credit
 *   bit 12: test mode        bit 13: tilt
 *   bit 14: coin door        bit 15: bill validator
 *
 * Lightgun: 0-255 for X and Y per player (0x16800000-0x1680000F)
 */

#ifndef CARNEVIL_INPUT_H
#define CARNEVIL_INPUT_H

#include <stdint.h>

/* Per-player input state (device-agnostic) */
typedef struct {
    /* Lightgun aim position (0.0 - 1.0, normalized) */
    float aim_x;
    float aim_y;

    /* Buttons (1 = pressed, 0 = released) -- opposite of hardware! */
    uint8_t trigger;
    uint8_t pump;       /* Reload / shotgun pump */
    uint8_t start;
    uint8_t coin;

    /* Input source identifier */
    int source;  /* 0=none, 1=mouse, 2=gamepad, 3=sinden, 4=network */
} player_input_t;

/* System-level inputs */
typedef struct {
    player_input_t player[2];

    uint8_t service_mode;
    uint8_t test_mode;
    uint8_t service_credit;

    /* Raw IOASIC register value (computed from above) */
    uint32_t ioasic_buttons;

    /* Raw lightgun values (0-255) */
    uint8_t gun_x[2];
    uint8_t gun_y[2];
} input_state_t;

/* Initialize input system */
void input_init(void);

/* Poll all input sources and update state */
void input_poll(input_state_t* state);

/* Convert normalized aim (0-1) to arcade gun register (0-255) */
uint8_t input_aim_to_gun(float normalized);

/* Build the IOASIC button register from player inputs */
uint32_t input_build_ioasic_buttons(const input_state_t* state);

/* Write input state to game RAM */
void input_write_to_ram(uint8_t* rdram, const input_state_t* state);

/* Network multiplayer: serialize/deserialize player input for transmission */
typedef struct {
    uint32_t frame_number;
    player_input_t player;
    uint8_t player_index;
} net_input_packet_t;

int  input_serialize(const net_input_packet_t* packet, uint8_t* buf, int buf_size);
int  input_deserialize(net_input_packet_t* packet, const uint8_t* buf, int buf_size);

#endif
