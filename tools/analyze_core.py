#!/usr/bin/env python3
"""Analyze CarnEvil engine core: most-called functions and main loop."""
import struct

BIN = 'D:/recomp/arcade/carnevil/extracted/GAME.bin'
LOAD_ADDR = 0x800C4000

with open(BIN, 'rb') as f:
    data = f.read()

def get_word(off):
    if off + 4 <= len(data):
        return struct.unpack_from('<I', data, off)[0]
    return 0

def disasm_one(w):
    op = (w >> 26) & 0x3F
    rs = (w >> 21) & 0x1F
    rt = (w >> 16) & 0x1F
    rd = (w >> 11) & 0x1F
    sa = (w >> 6) & 0x1F
    funct = w & 0x3F
    imm = w & 0xFFFF
    imm_s = struct.unpack('<h', struct.pack('<H', imm))[0]

    if w == 0: return 'nop'
    if w == 0x03e00008: return 'jr ra'
    if op == 0x0F: return f'lui r{rt}, 0x{imm:04x}'
    if op == 0x09: return f'addiu r{rt}, r{rs}, {imm_s}'
    if op == 0x0D: return f'ori r{rt}, r{rs}, 0x{imm:04x}'
    if op == 0x0C: return f'andi r{rt}, r{rs}, 0x{imm:04x}'
    if op == 0x23: return f'lw r{rt}, {imm_s}(r{rs})'
    if op == 0x2B: return f'sw r{rt}, {imm_s}(r{rs})'
    if op == 0x24: return f'lbu r{rt}, {imm_s}(r{rs})'
    if op == 0x25: return f'lhu r{rt}, {imm_s}(r{rs})'
    if op == 0x20: return f'lb r{rt}, {imm_s}(r{rs})'
    if op == 0x21: return f'lh r{rt}, {imm_s}(r{rs})'
    if op == 0x28: return f'sb r{rt}, {imm_s}(r{rs})'
    if op == 0x29: return f'sh r{rt}, {imm_s}(r{rs})'
    if op == 0x03: return f'jal 0x{((w & 0x03FFFFFF) << 2) | 0x80000000:08x}'
    if op == 0x02: return f'j 0x{((w & 0x03FFFFFF) << 2) | 0x80000000:08x}'
    if op == 0x04: return f'beq r{rs}, r{rt}, {imm_s}'
    if op == 0x05: return f'bne r{rs}, r{rt}, {imm_s}'
    if op == 0x06: return f'blez r{rs}, {imm_s}'
    if op == 0x07: return f'bgtz r{rs}, {imm_s}'
    if op == 0x0A: return f'slti r{rt}, r{rs}, {imm_s}'
    if op == 0x31: return f'lwc1 f{rt}, {imm_s}(r{rs})'
    if op == 0x39: return f'swc1 f{rt}, {imm_s}(r{rs})'
    if op == 0x11:
        fmt = rs
        if fmt == 0x04: return f'mtc1 r{rt}, f{rd}'
        if fmt == 0x00: return f'mfc1 r{rt}, f{rd}'
    if op == 0x00:
        if funct == 0x21: return f'addu r{rd}, r{rs}, r{rt}'
        if funct == 0x23: return f'subu r{rd}, r{rs}, r{rt}'
        if funct == 0x25: return f'or r{rd}, r{rs}, r{rt}'
        if funct == 0x24: return f'and r{rd}, r{rs}, r{rt}'
        if funct == 0x00 and w != 0: return f'sll r{rd}, r{rt}, {sa}'
        if funct == 0x02: return f'srl r{rd}, r{rt}, {sa}'
        if funct == 0x03: return f'sra r{rd}, r{rt}, {sa}'
        if funct == 0x18: return f'mult r{rs}, r{rt}'
        if funct == 0x19: return f'multu r{rs}, r{rt}'
        if funct == 0x12: return f'mflo r{rd}'
        if funct == 0x10: return f'mfhi r{rd}'
        if funct == 0x08: return f'jr r{rs}'
        if funct == 0x09: return f'jalr r{rs}'
        if funct == 0x0A: return f'movz r{rd}, r{rs}, r{rt}'
        if funct == 0x0B: return f'movn r{rd}, r{rs}, r{rt}'
    if op == 0x01:
        if rt == 1: return f'bgez r{rs}, {imm_s}'
        if rt == 0: return f'bltz r{rs}, {imm_s}'
    return f'0x{w:08x}'

def dump_func(va, max_instrs=25):
    off = va - LOAD_ADDR
    lines = []
    for i in range(max_instrs):
        w = get_word(off + i*4)
        m = disasm_one(w)
        lines.append((va + i*4, w, m))
        if w == 0x03e00008:  # jr ra
            # add delay slot
            w2 = get_word(off + (i+1)*4)
            lines.append((va + (i+1)*4, w2, disasm_one(w2)))
            break
    return lines

# Analyze each target function
targets = [
    (0x80166504, 113, 8,   'tiny_stub_113'),
    (0x80166448, 106, 8,   'tiny_stub_106'),
    (0x80141210, 100, 16,  'tiny_stub_100'),
    (0x80141260,  63, 12,  'tiny_stub_63'),
    (0x800D66BC,  62, 8,   'tiny_stub_62'),
    (0x8015E2F4,  88, 12,  'tiny_stub_88'),
    (0x800C4154,  52, 16,  'tiny_stub_52'),
    (0x801474C0, 105, 96,  'small_func_105'),
    (0x800CE9CC,  83, 80,  'small_func_83'),
    (0x800CDF4C,  81, 100, 'small_func_81'),
    (0x800EB964,  80, 60,  'small_func_80'),
    (0x800D64E4,  59, 72,  'small_func_59'),
    (0x800CE2F4,  56, 68,  'small_func_56'),
    (0x800EA210, 126, 208, 'medium_func_126'),
    (0x8015F178,  87, 196, 'medium_func_87'),
    (0x80143A40, 123, 360, 'called_from_mainloop_123'),
    (0x800D6868,  54, 320, 'medium_func_54'),
    (0x800CDB64,  52, 264, 'medium_func_52'),
    (0x80163CD8,  59, 556, 'medium_func_59b'),
    (0x80143E74, 209, 1088,'most_called_209'),
]

print('='*70)
print('CarnEvil Engine Core Analysis')
print('='*70)

for va, ncallers, size, label in targets:
    lines = dump_func(va, min(size//4, 30))
    print(f'\n{"="*60}')
    print(f'func_{va:08X} -- {ncallers} callers, {size} bytes')
    print(f'{"="*60}')
    for addr, w, m in lines:
        # Annotate interesting patterns
        note = ''
        if 'lui' in m:
            imm = w & 0xFFFF
            if imm >= 0x8000 and imm < 0x8100:
                note = '  // loading RAM address'
            elif imm == 0xB600:
                note = '  // VOODOO registers'
            elif imm == 0x1600:
                note = '  // IOASIC registers'
        if 'jal' in m:
            target = ((w & 0x03FFFFFF) << 2) | 0x80000000
            note = f'  // call func_{target:08X}'
        print(f'  {addr:#010x}: {m:<35s}{note}')

# Also dump the main_loop itself
print(f'\n{"="*60}')
print(f'main_loop (0x800C4524) -- game frame update')
print(f'{"="*60}')
lines = dump_func(0x800C4524, 60)
for addr, w, m in lines:
    note = ''
    if 'jal' in m:
        target = ((w & 0x03FFFFFF) << 2) | 0x80000000
        note = f'  // call func_{target:08X}'
    print(f'  {addr:#010x}: {m:<35s}{note}')
