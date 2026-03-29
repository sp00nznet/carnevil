/**
 * CarnEvil DCS2 Sound System - Stub Implementation
 *
 * Currently returns success for all DCS commands so the game progresses.
 * Future: decode .BNK files and play audio via SDL_mixer or miniaudio.
 */

#include "dcs_sound.h"
#include <stdio.h>
#include <string.h>

void dcs_init(dcs_state_t* dcs) {
    memset(dcs, 0, sizeof(*dcs));
    dcs->version = 3; /* DCS version 3.0 */
    dcs->master_volume = 200;
    dcs->initialized = 1;
}

uint32_t dcs_command(dcs_state_t* dcs, uint32_t cmd_id, uint32_t data) {
    /* Log command */
    dcs->last_commands[dcs->cmd_log_pos & 15] = (uint16_t)cmd_id;
    dcs->cmd_log_pos++;

    switch (cmd_id) {
    case DCS_CMD_RESET:
        dcs->initialized = 1;
        return 0;

    case DCS_CMD_VERSION:
        return dcs->version;

    case DCS_CMD_STATUS:
    case DCS_CMD_READY:
        return 1; /* Ready */

    case DCS_CMD_PLAY:
    case DCS_CMD_PLAY_DATA:
        /* TODO: Translate DCS sound ID to audio file and play it.
         * The sound ID maps to an entry in the loaded .BNK file.
         * For now, just log it. */
        return 0;

    case DCS_CMD_VOLUME:
        dcs->master_volume = (uint8_t)(data & 0xFF);
        return 0;

    case DCS_CMD_CHAN_VOL:
        return 0;

    case DCS_CMD_GET_RESULT:
    case DCS_CMD_GET_DATA:
        return 0;

    case DCS_CMD_EXTENDED:
        return 0;

    case DCS_CMD_TEST:
    case DCS_CMD_DIAG:
        return 0;

    default:
        return 0;
    }
}

void dcs_load_bank(dcs_state_t* dcs, const char* bank_name) {
    /* TODO: Load .BNK file and index its sound entries.
     * BNK format: DCS compressed audio with a header table.
     * Each bank contains multiple sound effects/music tracks. */
    dcs->banks_loaded++;
}
