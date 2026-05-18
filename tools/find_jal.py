"""Search a flat MIPS binary for JAL instructions targeting given addresses.

JAL encoding: 0x0C000000 | ((target >> 2) & 0x03FFFFFF)

Usage: python tools/find_jal.py <bin> <base_vaddr> <target_addr> [target_addr...]
"""
import sys, struct

if len(sys.argv) < 4:
    print(__doc__); sys.exit(1)

bin_path = sys.argv[1]
base = int(sys.argv[2], 0)
targets = [int(a, 0) for a in sys.argv[3:]]

# Compute JAL encoded values for each target
jal_for = {}
for t in targets:
    jal = 0x0C000000 | ((t >> 2) & 0x03FFFFFF)
    jal_for[jal] = t

with open(bin_path, 'rb') as f: data = f.read()

print(f"Scanning {bin_path} for JAL to {[hex(t) for t in targets]}")
hits = 0
for off in range(0, len(data) - 3, 4):
    word = struct.unpack_from('<I', data, off)[0]
    if word in jal_for:
        vaddr = base + off
        print(f"  HIT at 0x{vaddr:08X}: JAL 0x{jal_for[word]:08X}")
        hits += 1
print(f"total hits: {hits}")
