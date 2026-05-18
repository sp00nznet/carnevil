#!/usr/bin/env python3
"""Trace LUI+ADDIU pairs to map string references to functions, then analyze game structure."""
import struct
from collections import defaultdict

BIN = 'D:/recomp/arcade/carnevil/extracted/GAME.bin'
LOAD_ADDR = 0x800C4000

with open(BIN, 'rb') as f:
    data = f.read()

# Build function map
func_starts = set()
for i in range(0, len(data) - 4, 4):
    w = struct.unpack_from('<I', data, i)[0]
    if (w >> 16) == 0x27BD:
        imm = struct.unpack('<h', struct.pack('<H', w & 0xFFFF))[0]
        if imm < 0:
            func_starts.add(LOAD_ADDR + i)
    if (w >> 26) == 0x03:
        target = ((w & 0x03FFFFFF) << 2) | 0x80000000
        if LOAD_ADDR <= target < LOAD_ADDR + len(data):
            func_starts.add(target)
func_starts.add(LOAD_ADDR)
sorted_funcs = sorted(func_starts)

func_ranges = {}
for i, va in enumerate(sorted_funcs):
    end = sorted_funcs[i+1] if i+1 < len(sorted_funcs) else LOAD_ADDR + len(data)
    func_ranges[va] = end

# Find all strings
strings = {}
idx = 0
while idx < len(data):
    if data[idx] == 0:
        idx += 1
        continue
    end_idx = data.find(b'\x00', idx)
    if end_idx == -1:
        break
    s = data[idx:end_idx]
    if len(s) >= 4 and all(32 <= b < 127 for b in s):
        strings[LOAD_ADDR + idx] = s.decode('ascii')
    idx = end_idx + 1

# Trace LUI+ADDIU for string references
func_string_refs = defaultdict(list)
for func_va in sorted_funcs:
    func_end = min(func_ranges[func_va], func_va + 0x4000)
    off = func_va - LOAD_ADDR
    end_off = func_end - LOAD_ADDR
    lui_vals = {}
    for j in range(off, min(end_off, len(data) - 4), 4):
        w = struct.unpack_from('<I', data, j)[0]
        op = (w >> 26) & 0x3F
        rt = (w >> 16) & 0x1F
        rs = (w >> 21) & 0x1F
        imm = w & 0xFFFF
        if op == 0x0F:
            lui_vals[rt] = imm << 16
        elif op == 0x09 and rs in lui_vals:
            addr = (lui_vals[rs] + struct.unpack('<h', struct.pack('<H', imm))[0]) & 0xFFFFFFFF
            if addr in strings:
                func_string_refs[func_va].append(strings[addr])
        elif op == 0x0D and rs in lui_vals:
            addr = lui_vals[rs] | imm
            if addr in strings:
                func_string_refs[func_va].append(strings[addr])

# Build call graph
call_graph = defaultdict(set)
callers_map = defaultdict(set)
for va in sorted_funcs:
    end = func_ranges[va]
    off = va - LOAD_ADDR
    for j in range(off, min(end - LOAD_ADDR, off + 0x10000), 4):
        if j + 4 > len(data):
            break
        w = struct.unpack_from('<I', data, j)[0]
        if (w >> 26) == 0x03:
            target = ((w & 0x03FFFFFF) << 2) | 0x80000000
            if LOAD_ADDR <= target < LOAD_ADDR + len(data):
                call_graph[va].add(target)
                callers_map[target].add(va)

# Name functions
func_names = {
    0x800C4000: 'entry_point',
    0x800C4524: 'main_loop',
    0x80140E28: 'main_init',
    0x80143A10: 'sys_init',
}

naming_rules = [
    ('main_init_sound', 'main_init_sound'),
    ('CARNEVIL V%d', 'print_version'),
    ('FATAL ERROR: U96', 'fatal_error'),
    ('INSERT COIN', 'attract_coin_insert'),
    ('COIN AUDITS', 'menu_coin_audits'),
    ('GAME AUDITS', 'menu_game_audits'),
    ('GUN CALIBRATION', 'menu_gun_cal'),
    ('CALIBRATE MONITOR', 'menu_monitor_cal'),
    ('HIGH SCORES', 'menu_high_scores'),
    ('FACTORY SETTINGS', 'menu_factory_reset'),
    ('DEBUGGING CHAR', 'debug_characters'),
    ('ADDRESS ERROR', 'exc_handler'),
    ('generic1', 'scene_generic'),
    ('tokken', 'scene_tokken'),
    ('hhou', 'scene_hauntedhouse'),
    ('freaksho', 'scene_freakshow'),
    ('attract', 'attract_mode'),
    ('select1', 'level_select'),
    ('SHOTGUN X 10', 'weapon_setup'),
    ('HEALTH UP', 'item_health'),
    ('FREE PLAY', 'check_freeplay'),
    ('RTP %d DMA', 'render_stats'),
    ('Setting ', 'video_init'),
    ('gunsmoke', 'fx_gunsmoke'),
    ('SERIAL NUMBER', 'menu_serial'),
    ('COINAGE:', 'menu_coinage'),
    ('Loading banks', 'sound_load_banks'),
    ('Copyright', 'boot_copyright'),
    ('Mother GOOSE', 'boot_platform'),
    ('ZA DISK FAILURE', 'disk_error'),
    ('Special Instruction', 'service_menu'),
    ('GAME DIFFICULTY', 'menu_difficulty'),
    ('INITIALS ENTERED', 'hiscore_entry'),
    ('VOLUME LEVELS', 'menu_volume'),
    ('SOUND TEST', 'menu_sound_test'),
    ('This should never happen', 'assert_fail'),
    ('wavinmus', 'sound_music'),
    ('PLEASE TURN OFF', 'prompt_poweroff'),
    ('GRAPHIC TEST', 'menu_gfx_test'),
    ('SWITCH TEST', 'menu_switch_test'),
    ('SCREEN POSI', 'menu_screen_pos'),
    ('COLOR BARS', 'menu_color_bars'),
    ('ROM CHECKSUM', 'menu_rom_check'),
    ('#PLAYERS:%d', 'game_players'),
    ('Carnevil', 'title_screen'),
    ('GAME OVER', 'game_over'),
    ('CONTINUE', 'continue_screen'),
    ('SMIL.ZM', 'model_load'),
    ('GAME STARTS', 'stat_game_starts'),
    ('.PTH', 'path_load'),
    ('.WMS', 'texture_load'),
    ('.BNK', 'bank_load'),
    ('bigtop', 'scene_bigtop'),
    ('intro', 'scene_intro'),
    ('bob', 'npc_bob'),
    ('ozob', 'scene_ozob'),
    ('smil', 'npc_smil'),
    ('eyeclops', 'boss_eyeclops'),
    ('junior', 'npc_junior'),
    ('krampus', 'boss_krampus'),
    ('clwn', 'npc_clown'),
    ('baby', 'npc_baby'),
    ('acid', 'npc_acid'),
    ('zomb', 'npc_zombie'),
    ('tort', 'npc_tort'),
    ('dino', 'npc_dino'),
    ('mari', 'npc_mari'),
    ('hand', 'npc_hand'),
    ('pood', 'npc_pood'),
    ('krik', 'npc_krik'),
    ('smeek', 'npc_smeek'),
    ('skull', 'npc_skull'),
    ('mime', 'npc_mime'),
    ('klott', 'npc_klott'),
    ('knif', 'npc_knife'),
    ('teen', 'npc_teen'),
    ('rick', 'npc_rickity'),
    ('frek', 'npc_frek'),
]

for va in sorted_funcs:
    if va in func_names:
        continue
    refs = func_string_refs.get(va, [])
    for s in refs:
        for pattern, name in naming_rules:
            if pattern.lower() in s.lower():
                if va not in func_names:
                    func_names[va] = name
                break

# Output
print('=' * 70)
print('CarnEvil Game Architecture')
print('=' * 70)

print('\n--- BOOT SEQUENCE ---')
print('entry_point -> sys_init -> main_init -> main_loop')
for callee in sorted(call_graph.get(0x80140E28, [])):
    n = func_names.get(callee, f'func_{callee:08X}')
    refs = func_string_refs.get(callee, [])
    h = f'  "{refs[0][:40]}"' if refs else ''
    print(f'  main_init -> {n} ({callee:#x}){h}')
    for sub in sorted(call_graph.get(callee, []))[:5]:
        sn = func_names.get(sub, f'func_{sub:08X}')
        sr = func_string_refs.get(sub, [])
        sh = f' "{sr[0][:30]}"' if sr else ''
        print(f'    -> {sn}{sh}')

print('\n--- MAIN LOOP (per-frame update) ---')
for callee in sorted(call_graph.get(0x800C4524, [])):
    n = func_names.get(callee, f'func_{callee:08X}')
    size = func_ranges.get(callee, callee+4) - callee
    sc = len(call_graph.get(callee, set()))
    refs = func_string_refs.get(callee, [])
    h = f'  "{refs[0][:40]}"' if refs else ''
    print(f'  {callee:#010x} {n:<30s} {size:>5}B {sc:>2} calls{h}')

print('\n--- NAMED FUNCTIONS ({}) ---'.format(len(func_names)))
for va, name in sorted(func_names.items()):
    size = func_ranges.get(va, va+4) - va
    nc = len(callers_map.get(va, set()))
    refs = func_string_refs.get(va, [])
    h = f'  "{refs[0][:35]}"' if refs else ''
    print(f'  {va:#010x} {name:<30s} {size:>5}B  {nc:>3} callers{h}')

print('\n--- MOST-CALLED (engine core) ---')
by_callers = sorted(callers_map.items(), key=lambda x: len(x[1]), reverse=True)
for va, cs in by_callers[:15]:
    n = func_names.get(va, f'func_{va:08X}')
    size = func_ranges.get(va, va+4) - va
    refs = func_string_refs.get(va, [])
    h = f'  "{refs[0][:30]}"' if refs else ''
    print(f'  {va:#010x} {n:<30s} {len(cs):>4} callers {size:>5}B{h}')

print(f'\nTotal: {len(sorted_funcs)} functions, {len(func_names)} named ({100*len(func_names)/len(sorted_funcs):.0f}%)')
