"""Scan the MAME RAM dump for 32-bit values in the RTOS code range
(0x80000000-0x8001C000 — RTOS loads at 0x80000000, size ~0x15C00).
These are likely fiber-resume entry points stored in scene nodes / task
contexts. Cross-reference against rtos_syms.toml to find which addresses
are MID-FUNCTION (not start-of-function) and need to be added as new
recompiler entry points."""
import struct, re

with open("D:/recomp/mame/attract_sdram.bin", "rb") as f:
    ram = f.read()

# Parse rtos_syms.toml for existing function entries
known = {}  # addr -> (name, size)
with open("rtos_syms.toml") as f:
    txt = f.read()
for m in re.finditer(r'name = "([^"]+)"\s*\n\s*vram = (0x[0-9A-Fa-f]+)\s*\n\s*size = (0x[0-9A-Fa-f]+)', txt):
    known[int(m.group(2), 0)] = (m.group(1), int(m.group(3), 0))

print(f"Loaded {len(known)} RTOS functions from rtos_syms.toml")

# Collect every 32-bit value in the dump that's in the RTOS range
RTOS_MIN, RTOS_MAX = 0x80000000, 0x8001C000
counts = {}
locations = {}
for off in range(0, len(ram) - 3, 4):
    w = struct.unpack_from("<I", ram, off)[0]
    if RTOS_MIN <= w < RTOS_MAX:
        counts[w] = counts.get(w, 0) + 1
        locations.setdefault(w, []).append(off)

print(f"\nUnique RTOS addresses referenced as 32-bit values: {len(counts)}")
print(f"Total references: {sum(counts.values())}")

# Classify each address as start-of-function or mid-function
def find_containing(addr):
    """Return (start_addr, name, offset_from_start) for the function
    containing addr, or None if none found."""
    best = None
    for fa, (fn, sz) in known.items():
        if fa <= addr < fa + sz:
            if best is None or fa > best[0]:
                best = (fa, fn, addr - fa)
    return best

# Group by start-of-function vs mid-function
starts = []
mids = []
unknowns = []
for addr, cnt in counts.items():
    c = find_containing(addr)
    if c is None:
        unknowns.append((addr, cnt))
    elif c[2] == 0:
        starts.append((addr, cnt, c[1]))
    else:
        mids.append((addr, cnt, c[1], c[2]))

starts.sort(key=lambda x: -x[1])
mids.sort(key=lambda x: -x[1])
unknowns.sort(key=lambda x: -x[1])

print(f"\nSTART of function ({len(starts)} unique, top 15 by ref count):")
for addr, cnt, name in starts[:15]:
    print(f"  0x{addr:08X}  x{cnt}  {name}")

print(f"\nMID-function ({len(mids)} unique, top 30 by ref count):")
for addr, cnt, name, off in mids[:30]:
    print(f"  0x{addr:08X}  x{cnt}  {name}+0x{off:X}")

print(f"\nUNKNOWN (not inside any known function, {len(unknowns)} unique, top 10):")
for addr, cnt in unknowns[:10]:
    print(f"  0x{addr:08X}  x{cnt}")
