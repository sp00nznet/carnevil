"""Scan a flat MIPS binary for 32-bit words matching given function addresses.
Reveals function-pointer tables / vtables that reference the target functions.

Usage: python tools/find_fnptrs.py <bin> <base_vaddr> <target_addr> [target_addr...]
"""
import sys, struct

if len(sys.argv) < 4:
    print(__doc__)
    sys.exit(1)

bin_path = sys.argv[1]
base = int(sys.argv[2], 0)
targets = set(int(a, 0) for a in sys.argv[3:])

with open(bin_path, 'rb') as f:
    data = f.read()

# Game binary is little-endian (Midway Seattle), so words are LE
print(f"Scanning {bin_path} ({len(data)} bytes), base=0x{base:08X}")
print(f"Targets: {[hex(t) for t in targets]}")
print()

for off in range(0, len(data) - 3, 4):
    word = struct.unpack_from('<I', data, off)[0]
    if word in targets:
        vaddr = base + off
        # Print context: 8 words around this hit
        ctx = []
        for i in range(-4, 5):
            o = off + i * 4
            if 0 <= o < len(data) - 3:
                w = struct.unpack_from('<I', data, o)[0]
                marker = ' <--' if i == 0 else ''
                ctx.append(f"  [0x{base + o:08X}] = 0x{w:08X}{marker}")
        print(f"HIT at 0x{vaddr:08X} = 0x{word:08X}")
        for line in ctx:
            print(line)
        print()
