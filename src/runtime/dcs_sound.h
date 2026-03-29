/**
 * CarnEvil DCS2 Sound System
 *
 * The Midway DCS2 sound system uses an ADSP-2115 DSP with 2MB of sound RAM.
 * Sound data is stored in .BNK (bank) files on the game disk.
 *
 * For our recompilation, we intercept DCS commands at the IOASIC level
 * and translate them to modern audio playback (SDL_mixer, miniaudio, etc.)
 *
 * DCS command format (via IOASIC):
 *   0x6900: Reset sound system
 *   0x6901: Get sound board version
 *   0x6902: Get sound board status
 *   0x6903: Check if sound board is ready
 *   0x6904: Send sound command (play/stop sound)
 *   0x6905: Send sound command with data
 *   0x6906: Get sound result
 *   0x6907: Set volume
 *   0x6908: Set channel volume
 *   0x6909: Sound command (extended)
 *   0x690A: Get sound data
 *   0x690E: Sound test
 *   0x690F: Sound board diagnostic
 */

#ifndef CARNEVIL_DCS_SOUND_H
#define CARNEVIL_DCS_SOUND_H

#include <stdint.h>

/* DCS command IDs */
#define DCS_CMD_RESET       0x6900
#define DCS_CMD_VERSION     0x6901
#define DCS_CMD_STATUS      0x6902
#define DCS_CMD_READY       0x6903
#define DCS_CMD_PLAY        0x6904
#define DCS_CMD_PLAY_DATA   0x6905
#define DCS_CMD_GET_RESULT  0x6906
#define DCS_CMD_VOLUME      0x6907
#define DCS_CMD_CHAN_VOL     0x6908
#define DCS_CMD_EXTENDED    0x6909
#define DCS_CMD_GET_DATA    0x690A
#define DCS_CMD_TEST        0x690E
#define DCS_CMD_DIAG        0x690F

typedef struct {
    int initialized;
    int version;
    uint8_t master_volume;

    /* Sound command log for debugging */
    uint16_t last_commands[16];
    int cmd_log_pos;

    /* Bank loading state */
    int banks_loaded;
} dcs_state_t;

void     dcs_init(dcs_state_t* dcs);
uint32_t dcs_command(dcs_state_t* dcs, uint32_t cmd_id, uint32_t data);
void     dcs_load_bank(dcs_state_t* dcs, const char* bank_name);

#endif
