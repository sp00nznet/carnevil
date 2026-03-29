/**
 * CarnEvil Input System
 *
 * Currently implements keyboard/mouse input.
 * Future: gamepad (SDL/XInput), Sinden lightgun (USB HID), network input.
 */

#include "input.h"
#include <string.h>
#include <stdio.h>

/* IOASIC button bit positions (active-low in hardware) */
#define BTN_P1_TRIGGER   (1 << 0)
#define BTN_P1_PUMP      (1 << 1)
#define BTN_P1_START     (1 << 2)
#define BTN_P2_TRIGGER   (1 << 4)
#define BTN_P2_PUMP      (1 << 5)
#define BTN_P2_START     (1 << 6)
#define BTN_VOLUME_UP    (1 << 7)
#define BTN_COIN1        (1 << 8)
#define BTN_COIN2        (1 << 9)
#define BTN_SERVICE_CRED (1 << 10)
#define BTN_TEST_MODE    (1 << 12)
#define BTN_TILT         (1 << 13)
#define BTN_COIN_DOOR    (1 << 14)
#define BTN_BILL_VALID   (1 << 15)

void input_init(void) {
    /* Platform-specific init will go here (SDL, Win32, etc.) */
}

uint8_t input_aim_to_gun(float normalized) {
    int val = (int)(normalized * 255.0f);
    if (val < 0) val = 0;
    if (val > 255) val = 255;
    return (uint8_t)val;
}

uint32_t input_build_ioasic_buttons(const input_state_t* state) {
    /* Start with all bits high (all switches open / released) */
    uint32_t reg = 0xFFFFFFFF;

    /* Clear bits for pressed buttons (active-low) */
    if (state->player[0].trigger) reg &= ~BTN_P1_TRIGGER;
    if (state->player[0].pump)    reg &= ~BTN_P1_PUMP;
    if (state->player[0].start)   reg &= ~BTN_P1_START;
    if (state->player[0].coin)    reg &= ~BTN_COIN1;

    if (state->player[1].trigger) reg &= ~BTN_P2_TRIGGER;
    if (state->player[1].pump)    reg &= ~BTN_P2_PUMP;
    if (state->player[1].start)   reg &= ~BTN_P2_START;
    if (state->player[1].coin)    reg &= ~BTN_COIN2;

    if (state->service_credit) reg &= ~BTN_SERVICE_CRED;
    if (state->test_mode)      reg &= ~BTN_TEST_MODE;

    return reg;
}

void input_write_to_ram(uint8_t* rdram, const input_state_t* state) {
    /* Write IOASIC button register to the game's global input state */
    /* Physical address of input_state: 0x002122E0 (from 0x802122E0 & 0x1FFFFFFF) */
    *(uint32_t*)(rdram + 0x002122E0) = state->ioasic_buttons;

    /* Lightgun positions are read via I/O at 0x16800000, handled in seattle_io_read32 */
}

void input_poll(input_state_t* state) {
    /* TODO: Poll actual input devices here.
     * For now, this is a stub. The platform layer (SDL/Win32)
     * will fill in player[0] and player[1] before calling this.
     *
     * Keyboard mapping (for initial testing without SDL):
     *   P1: Left click = trigger, Right click = pump, Enter = start, 5 = coin
     *   P2: Gamepad or network
     *   F1 = service credit, F2 = test mode
     *   Mouse position = P1 aim
     */

    /* Compute derived values */
    state->ioasic_buttons = input_build_ioasic_buttons(state);
    state->gun_x[0] = input_aim_to_gun(state->player[0].aim_x);
    state->gun_y[0] = input_aim_to_gun(state->player[0].aim_y);
    state->gun_x[1] = input_aim_to_gun(state->player[1].aim_x);
    state->gun_y[1] = input_aim_to_gun(state->player[1].aim_y);
}

/* Network serialization for multiplayer */
int input_serialize(const net_input_packet_t* packet, uint8_t* buf, int buf_size) {
    if (buf_size < 20) return -1;

    /* Simple binary format: frame(4) + player_idx(1) + aim_x(4) + aim_y(4) + buttons(4) + pad(3) */
    memcpy(buf + 0, &packet->frame_number, 4);
    buf[4] = packet->player_index;
    memcpy(buf + 5, &packet->player.aim_x, 4);
    memcpy(buf + 9, &packet->player.aim_y, 4);

    uint8_t buttons = 0;
    if (packet->player.trigger) buttons |= 0x01;
    if (packet->player.pump)    buttons |= 0x02;
    if (packet->player.start)   buttons |= 0x04;
    if (packet->player.coin)    buttons |= 0x08;
    buf[13] = buttons;
    buf[14] = buf[15] = buf[16] = buf[17] = buf[18] = buf[19] = 0; /* padding */

    return 20;
}

int input_deserialize(net_input_packet_t* packet, const uint8_t* buf, int buf_size) {
    if (buf_size < 20) return -1;

    memcpy(&packet->frame_number, buf + 0, 4);
    packet->player_index = buf[4];
    memcpy(&packet->player.aim_x, buf + 5, 4);
    memcpy(&packet->player.aim_y, buf + 9, 4);

    uint8_t buttons = buf[13];
    packet->player.trigger = (buttons & 0x01) ? 1 : 0;
    packet->player.pump    = (buttons & 0x02) ? 1 : 0;
    packet->player.start   = (buttons & 0x04) ? 1 : 0;
    packet->player.coin    = (buttons & 0x08) ? 1 : 0;
    packet->player.source  = 4; /* network */

    return 20;
}
