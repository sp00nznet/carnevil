/* Cheats and operator options. See cheats.h.
 *
 * The game's own variables, found by diffing main-RAM dumps taken around an
 * event (a shot, a reload, a hit, a coin) in scripted runs, then read in the
 * disassembly and confirmed by forcing each one in a run and looking at the
 * screen:
 *
 *   players  one 0x118-byte record each, at 0x801E0440 (P1) and 0x801E0558
 *            (P2). +0x04 is the player's state: 4 while in play, 8 on the
 *            CONTINUE? countdown, 0 out of the game. +0x24 is health, a
 *            float, and +0x28 its maximum (900 at the factory setting);
 *            the green bar bottom left draws +0x24. +0x94 is rounds left in
 *            the gun, +0x98 the gun's capacity (6); the RELOAD! sign is up
 *            while +0x94 is 0.
 *   credits  a word at 0x801DDDB8, what the CREDITS: line shows (one per
 *            coin at USA 1; a game costs 2). The coin routine at 0x800C7200
 *            caps it so that it plus the word after it (service credits)
 *            stays within the maximum at 0x801E51F0 (32 at the factory
 *            setting), and keeps a copy in the CMOS audits.
 *   ride     the game loop (0x800C5830) puts the mode word 0x802122D4 at
 *            0x20 for the ride select screen and sets the chosen wave,
 *            0x801FA134, to -1; it waits until a shot or the timeout stores
 *            a wave there: 0x01020000 Haunted House, 0x01040000 Rickety
 *            Town, 0x01060000 Freak Show, 0x01070000 Big Top. 0x802122F4 is
 *            the rides finished, a bit each (0 at a game's start, 7 opens
 *            the Big Top).
 *
 * The operator adjustments (the test menu's GAME ADJUSTMENTS) live in the
 * CMOS, which has a checksum; the routine at 0x800F19F8 reads them at boot
 * into words the game then uses, so those are what is set, every frame once
 * the game is up (the mode word 0x802122D4 turns nonzero in the same frame,
 * frame 49, as the words are loaded). Ids, ranges and defaults are from the
 * table at 0x8018DF54 and the menu builder at 0x800F1C48:
 *
 *   0x8023673C  difficulty, id 48, 0..4 (very easy..very hard), default 2;
 *               read live against the spawn tables (0x800E8E54): the same
 *               scripted run scores 3800 at very easy, 5930 at very hard
 *   0x801DDD84  player starting health, id 49, 0..4, default 2; a player
 *               joins with 1300 - 200 * this (0x800E2B84), checked 1300/500
 *   0x80236778  monster strength, id 50, 0..4, default 2; read live
 *               (0x800FB05C): at very easy the same run gets through the
 *               Haunted House's yard markedly sooner
 *   0x801E51A8  violence, id 52: 0 none, 1 green blood, 2 red blood,
 *               3 full gore (default). Below 2 the game uses other assets
 *               (AAMANV.WMS rather than AAMAV.WMS, another sound bank), picked
 *               while the attract mode loads: changed after that, the next
 *               ride hangs at PLEASE WAIT. So it is a power-on option, set
 *               from frame 49 on, which works.
 *
 * Free play is the cabinet's way: the DIP switches. The routine at
 * 0x800C7658 reads them at boot ("Setting free play from dip"): Coinage
 * Source (0x0001) set to Dipswitch and Coinage (0x003E) all clear, free play.
 */
#include "cheats.h"
#include <string.h>

#define RAM(a) (g_rdram + ((a) & 0x7FFFFFu))

#define PLAYER(p)     (0x801E0440u + (p) * 0x118u)
#define P_STATE       0x04u
#define P_HEALTH      0x24u
#define P_HEALTH_MAX  0x28u
#define P_AMMO        0x94u
#define P_AMMO_CAP    0x98u
#define CREDITS       0x801DDDB8u
#define CREDITS_SVC   0x801DDDBCu
#define CREDITS_MAX   0x801E51F0u
#define MODE          0x802122D4u
#define MODE_SELECT   0x20u
#define WAVE          0x801FA134u
#define RIDES_DONE    0x802122F4u

enum { CHEAT_HEALTH_P1, CHEAT_HEALTH_P2, CHEAT_AMMO_P1, CHEAT_AMMO_P2, CHEAT_CREDITS };
enum { OPT_FREEPLAY, OPT_DIFFICULTY, OPT_HEALTH, OPT_MONSTER, OPT_VIOLENCE, OPT_RIDE };

const cheat_def k_cheats[] = {
    { "Infinite health P&1", 0 },
    { "Infinite health P&2", 0 },
    { "Endless ammo P1", 0 },
    { "Endless ammo P2", 0 },
    { "Add &credits (to the maximum)", 1 },
};
const int k_cheat_count = sizeof k_cheats / sizeof k_cheats[0];

/* Choice 0 leaves the operator's setting (the test menu's) alone. */
static const char* const k_onoff[] = { "Off", "On" };
static const char* const k_level[] = { "As set in the test menu", "Very easy", "Easy", "Normal",
                                       "Hard", "Very hard" };
static const char* const k_health[] = { "As set in the test menu", "Very easy (1300)", "Easy (1100)",
                                        "Normal (900)", "Hard (700)", "Very hard (500)" };
static const char* const k_violence[] = { "As set in the test menu", "None", "Green blood", "Red blood",
                                          "Full gore" };
static const char* const k_ride[] = { "Normal (pick one)", "Haunted House", "Rickety Town", "Freak Show",
                                      "Big Top" };
static const uint32_t k_wave[] = { 0, 0x01020000u, 0x01040000u, 0x01060000u, 0x01070000u };

const option_def k_options[] = {
    { "Free play", k_onoff, 2, 0, 1 },
    { "Difficulty", k_level, 6, 0, 0 },
    { "Starting health", k_health, 6, 0, 0 },
    { "Monster strength", k_level, 6, 0, 0 },
    { "Violence", k_violence, 5, 0, 1 },
    { "Start at ride", k_ride, 5, 0, 0 },
};
const int k_option_count = sizeof k_options / sizeof k_options[0];

static uint32_t rd32(uint32_t a) { uint32_t v; memcpy(&v, RAM(a), 4); return v; }
static void wr32(uint32_t a, uint32_t v) { memcpy(RAM(a), &v, 4); }
static float rdf(uint32_t a) { float v; memcpy(&v, RAM(a), 4); return v; }
static void wrf(uint32_t a, float v) { memcpy(RAM(a), &v, 4); }

/* Only while the player is in play (not on the continue countdown, where
 * health 0 is what ends the game) and the record looks like one. */
static int in_play(int p) { return rd32(PLAYER(p) + P_STATE) == 4; }

static void top_up_health(int p) {
    uint32_t b = PLAYER(p);
    float h = rdf(b + P_HEALTH), max = rdf(b + P_HEALTH_MAX);
    if (in_play(p) && h > 0.0f && max > 0.0f && max <= 2000.0f && h < max) wrf(b + P_HEALTH, max);
}

static void refill_ammo(int p) {
    uint32_t b = PLAYER(p), cap = rd32(b + P_AMMO_CAP);
    if (in_play(p) && cap && cap <= 99u && rd32(b + P_AMMO) < cap) wr32(b + P_AMMO, cap);
}

static void set_adjustment(uint32_t addr, uint8_t choice) {
    if (choice) wr32(addr, choice - 1u);
}

void cheats_frame(cabinet_inputs* in, const uint8_t options[MAX_OPTIONS], uint64_t frame) {
    uint32_t on = in->cheats;

    /* Hardware, so from power-on: the game reads the switches at boot. */
    if (options[OPT_FREEPLAY]) in->dips = (uint16_t)((in->dips | 0x0001) & ~0x003E);

    if (rd32(MODE) == 0) return;   /* the game is not up yet */
    set_adjustment(0x8023673Cu, options[OPT_DIFFICULTY]);
    set_adjustment(0x801DDD84u, options[OPT_HEALTH]);
    set_adjustment(0x80236778u, options[OPT_MONSTER]);
    set_adjustment(0x801E51A8u, options[OPT_VIOLENCE]);

    /* The first ride of a game only: the select screen is waiting (wave -1)
     * and no ride is finished yet. Storing the wave is what a shot at the
     * attraction does. */
    if (options[OPT_RIDE] && options[OPT_RIDE] < 5 && rd32(MODE) == MODE_SELECT &&
        rd32(WAVE) == 0xFFFFFFFFu && rd32(RIDES_DONE) == 0)
        wr32(WAVE, k_wave[options[OPT_RIDE]]);

    if (on & (1u << CHEAT_HEALTH_P1)) top_up_health(0);
    if (on & (1u << CHEAT_HEALTH_P2)) top_up_health(1);
    if (on & (1u << CHEAT_AMMO_P1)) refill_ammo(0);
    if (on & (1u << CHEAT_AMMO_P2)) refill_ammo(1);
    if (on & (1u << CHEAT_CREDITS)) {
        uint32_t max = rd32(CREDITS_MAX), svc = rd32(CREDITS_SVC);
        if (max && max <= 99u && svc < max && frame >= 1300) wr32(CREDITS, max - svc);
    }
}
