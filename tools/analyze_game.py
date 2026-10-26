#!/usr/bin/env python3
"""
Deep game architecture analysis of CarnEvil GAME.bin.
Map the call graph from entry_point and main_loop to understand
the game's major subsystems.
"""
import struct
from collections import defaultdict

BIN = 'D:/recomp/arcade/carnevil/extracted/GAME.bin'
LOAD_ADDR = 0x800C4000

with open(BIN, 'rb') as f:
    data = f.read()

# Build complete function map
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

# Build function ranges
func_ranges = {}
for i, va in enumerate(sorted_funcs):
    end = sorted_funcs[i+1] if i+1 < len(sorted_funcs) else LOAD_ADDR + len(data)
    func_ranges[va] = end

# Build call graph
call_graph = defaultdict(set)
callers = defaultdict(set)

for va in sorted_funcs:
    end = func_ranges[va]
    off = va - LOAD_ADDR
    end_off = end - LOAD_ADDR
    for j in range(off, min(end_off, off + 0x10000), 4):
        if j + 4 > len(data):
            break
        w = struct.unpack_from('<I', data, j)[0]
        if (w >> 26) == 0x03:
            target = ((w & 0x03FFFFFF) << 2) | 0x80000000
            if LOAD_ADDR <= target < LOAD_ADDR + len(data):
                call_graph[va].add(target)
                callers[target].add(va)

# Find all strings and map to functions
strings = {}
idx = 0
while idx < len(data):
    if data[idx] == 0:
        idx += 1
        continue
    end = data.find(b'\x00', idx)
    if end == -1:
        break
    s = data[idx:end]
    if len(s) >= 4 and all(32 <= b < 127 for b in s):
        strings[LOAD_ADDR + idx] = s.decode('ascii')
    idx = end + 1

func_strings = defaultdict(list)
for sva, s in strings.items():
    for va in sorted_funcs:
        if va <= sva < func_ranges[va]:
            func_strings[va].append(s)
            break

# Name functions
func_names = {
    0x800C4000: 'entry_point',
    0x800C4524: 'main_loop',
    0x80140E28: 'main_init',
    0x80143A10: 'sys_init',
}

naming_map = {
    'main_init_sound': 'main_init_sound',
    'CARNEVIL V%d': 'print_version_info',
    'FATAL ERROR': 'fatal_error',
    'INSERT COIN': 'attract_coin_prompt',
    'GAME STARTS': 'game_start_handler',
    'COIN AUDITS': 'coin_audit_display',
    'GAME AUDITS': 'game_audit_display',
    'GUN CALIBRATION': 'gun_calibration_menu',
    'CALIBRATE GUNS': 'gun_calibrate',
    'CALIBRATE MONITOR': 'monitor_calibrate',
    'HIGH SCORES': 'high_score_display',
    'FACTORY SETTINGS': 'factory_settings_restore',
    'DEBUGGING CHAR': 'debug_character_mode',
    'ADDRESS ERROR': 'exception_address_error',
    'BUS ERROR': 'exception_bus_error',
    'generic1': 'load_level_generic',
    'tokken': 'load_level_tokken',
    'hhou': 'load_level_hauntedhouse',
    'attract': 'attract_mode',
    'select1': 'level_select',
    'SHOTGUN': 'weapon_select',
    'HEALTH UP': 'health_pickup',
    'FREE PLAY': 'freeplay_check',
    'RTP %d DMA': 'render_stats_display',
    'Setting ': 'video_mode_setup',
    'gunsmoke': 'gunsmoke_effect',
    'SERIAL NUMBER': 'show_serial_number',
    'COINAGE:': 'coinage_setup',
    'Loading banks': 'sound_load_banks',
    'Copyright': 'show_copyright',
    'Mother GOOSE': 'boot_banner',
    'ZA DISK FAILURE': 'disk_error_handler',
    'Special Instruction': 'service_mode_menu',
    'D - Debugger': 'debug_service_menu',
}

for va in sorted_funcs:
    if va in func_names:
        continue
    for s in func_strings.get(va, []):
        for pattern, name in naming_map.items():
            if pattern in s:
                func_names[va] = name
                break
        if va in func_names:
            break

# Also find LUI references to hardware I/O to name hardware functions
for va in sorted_funcs:
    if va in func_names:
        continue
    off = va - LOAD_ADDR
    end_off = func_ranges[va] - LOAD_ADDR
    has_voodoo = False
    has_ioasic = False
    has_galileo = False
    for j in range(off, min(end_off, off + 0x4000), 4):
        if j + 4 > len(data):
            break
        w = struct.unpack_from('<I', data, j)[0]
        if (w >> 26) == 0x0F:
            imm = w & 0xFFFF
            if imm == 0xB600: has_voodoo = True
            elif imm == 0x1600 or imm == 0xB616: has_ioasic = True
            elif imm == 0xB000: has_galileo = True

    if has_voodoo and not has_ioasic:
        func_names[va] = f'voodoo_func_{va:08X}'
    elif has_ioasic:
        func_names[va] = f'ioasic_func_{va:08X}'
    elif has_galileo:
        func_names[va] = f'galileo_func_{va:08X}'


# Output
print('=' * 70)
print('CarnEvil Game Architecture Analysis')
print('=' * 70)

# Boot sequence
print('\n=== BOOT SEQUENCE ===')
print('entry_point (0x800C4000):')
for callee in sorted(call_graph.get(0x800C4000, [])):
    name = func_names.get(callee, f'func_{callee:08X}')
    print(f'  -> {name} ({callee:#010x})')

# Main init
print('\nmain_init (0x80140E28):')
for callee in sorted(call_graph.get(0x80140E28, [])):
    name = func_names.get(callee, f'func_{callee:08X}')
    subcalls = len(call_graph.get(callee, set()))
    strs = func_strings.get(callee, [])
    hint = f'  "{strs[0][:40]}"' if strs else ''
    print(f'  -> {name} ({callee:#010x}) [{subcalls} subcalls]{hint}')

# Main loop - depth 2
print('\n=== MAIN LOOP ===')
ml = 0x800C4524
ml_callees = sorted(call_graph.get(ml, []))
print(f'main_loop ({ml:#010x}) - {len(ml_callees)} direct calls:')
for callee in ml_callees:
    name = func_names.get(callee, f'func_{callee:08X}')
    subcalls = call_graph.get(callee, set())
    strs = func_strings.get(callee, [])
    hint = f'  "{strs[0][:50]}"' if strs else ''
    print(f'  -> {name} ({callee:#010x}) [{len(subcalls)} subcalls]{hint}')

# Biggest functions
print('\n=== LARGEST FUNCTIONS (top 30) ===')
func_sizes = [(va, func_ranges[va] - va) for va in sorted_funcs]
func_sizes.sort(key=lambda x: x[1], reverse=True)
for va, size in func_sizes[:30]:
    name = func_names.get(va, f'func_{va:08X}')
    ncalls = len(call_graph.get(va, set()))
    ncallers = len(callers.get(va, set()))
    strs = func_strings.get(va, [])
    hint = f'  "{strs[0][:35]}"' if strs else ''
    print(f'  {va:#010x} {name:<35s} {size:>6}B  out={ncalls:<3} in={ncallers:<3}{hint}')

# Most-called
print('\n=== MOST-CALLED FUNCTIONS (top 25) ===')
by_callers = sorted(callers.items(), key=lambda x: len(x[1]), reverse=True)
for va, caller_set in by_callers[:25]:
    name = func_names.get(va, f'func_{va:08X}')
    size = func_ranges[va] - va
    strs = func_strings.get(va, [])
    hint = f'  "{strs[0][:30]}"' if strs else ''
    print(f'  {va:#010x} {name:<35s} {len(caller_set):>4} callers  {size:>5}B{hint}')

# Named functions by category
categories = defaultdict(list)
for va, name in sorted(func_names.items()):
    nl = name.lower()
    if 'init' in nl or 'boot' in nl or 'entry' in nl or 'main_loop' == nl:
        categories['Core/Init'].append((va, name))
    elif 'level' in nl or 'attract' in nl or 'select' in nl or 'game_start' in nl:
        categories['Game Flow'].append((va, name))
    elif 'weapon' in nl or 'health' in nl or 'gun' in nl or 'gunsmoke' in nl:
        categories['Gameplay'].append((va, name))
    elif 'coin' in nl or 'audit' in nl or 'service' in nl or 'factory' in nl or 'coinage' in nl or 'freeplay' in nl or 'serial' in nl:
        categories['Operator/Service'].append((va, name))
    elif 'video' in nl or 'render' in nl or 'display' in nl or 'print' in nl or 'show' in nl or 'player_display' in nl or 'score' in nl:
        categories['Display/Video'].append((va, name))
    elif 'sound' in nl or 'music' in nl:
        categories['Audio'].append((va, name))
    elif 'voodoo' in nl or 'ioasic' in nl or 'galileo' in nl or 'disk' in nl:
        categories['Hardware I/O'].append((va, name))
    elif 'exception' in nl or 'error' in nl or 'debug' in nl or 'fatal' in nl:
        categories['Error/Debug'].append((va, name))
    else:
        categories['Uncategorized'].append((va, name))

print('\n=== NAMED FUNCTIONS BY SUBSYSTEM ===')
for cat in ['Core/Init', 'Game Flow', 'Gameplay', 'Audio', 'Display/Video',
            'Hardware I/O', 'Operator/Service', 'Error/Debug', 'Uncategorized']:
    funcs = categories.get(cat, [])
    if funcs:
        print(f'\n  [{cat}] ({len(funcs)} functions)')
        for va, name in funcs:
            size = func_ranges[va] - va
            print(f'    {va:#010x}: {name:<35s} ({size}B)')

# Summary stats
named = sum(1 for v in sorted_funcs if v in func_names)
print(f'\n=== SUMMARY ===')
print(f'  Total functions: {len(sorted_funcs)}')
print(f'  Named: {named} ({100*named/len(sorted_funcs):.1f}%)')
print(f'  Call edges: {sum(len(v) for v in call_graph.values())}')
print(f'  Entry points (no callers): {sum(1 for v in sorted_funcs if v not in callers)}')
print(f'  Leaf functions (no calls): {sum(1 for v in sorted_funcs if v not in call_graph)}')
