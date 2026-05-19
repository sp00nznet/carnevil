"""Generate rtos_syms.toml additions for the RTOS mid-function entry
points found in the MAME live dump. These are fiber-resume PCs the
recompiler needs to know about as separate function entry points so
get_function(addr) can dispatch to them."""
import struct, re

with open("D:/recomp/mame/attract_sdram.bin", "rb") as f:
    ram = f.read()
with open("rtos_syms.toml") as f:
    txt = f.read()

known = {}
for m in re.finditer(r'name = "([^"]+)"\s*\n\s*vram = (0x[0-9A-Fa-f]+)\s*\n\s*size = (0x[0-9A-Fa-f]+)', txt):
    known[int(m.group(2), 0)] = (m.group(1), int(m.group(3), 0))

RTOS_MIN, RTOS_MAX = 0x80000000, 0x8001C000
counts = {}
for off in range(0, len(ram) - 3, 4):
    w = struct.unpack_from("<I", ram, off)[0]
    if RTOS_MIN <= w < RTOS_MAX:
        counts[w] = counts.get(w, 0) + 1

def find_containing(addr):
    best = None
    for fa, (fn, sz) in known.items():
        if fa <= addr < fa + sz:
            if best is None or fa > best[0]:
                best = (fa, fn, sz, addr - fa)
    return best

# For each mid-function entry, compute the size: address of next-greater
# RTOS symbol or end of containing function, minus this addr.
sorted_known_addrs = sorted(known.keys())

def size_from(addr):
    """Estimate the function size if we register a new function at addr —
    extend to the start of the next known function (or the end of the
    containing function)."""
    c = find_containing(addr)
    if c is None: return 0
    fa, fn, sz, off = c
    end = fa + sz
    return end - addr

# Collect ALL mid-entries with at least 1 reference
mids = []
for addr, cnt in counts.items():
    if addr & 3:  # word-aligned only — non-aligned addresses aren't code targets
        continue
    c = find_containing(addr)
    if c is None: continue
    if c[3] == 0: continue  # start of function, already known
    size = size_from(addr)
    if size <= 0: continue
    mids.append((addr, cnt, c[1], c[3], size))

mids.sort(key=lambda x: -x[1])
print(f"Generating entries for {len(mids)} mid-function points")
print()

# Emit toml entries
out = []
for addr, cnt, parent, off_in_parent, size in mids:
    name = "rtos_%08X" % addr
    out.append(f"""
  [[section.functions]]
  name = "{name}"
  vram = 0x{addr:08X}
  size = 0x{size:X}""")

with open("rtos_resume_additions.toml", "w") as f:
    f.write("# Auto-generated from MAME dump analysis. "
            "Insert these into rtos_syms.toml.\n")
    for s in out:
        f.write(s + "\n")
print(f"Wrote {len(out)} entries to rtos_resume_additions.toml")
print()
print("Top 10 by reference count:")
for addr, cnt, parent, off, size in mids[:10]:
    print(f"  0x{addr:08X} (refs={cnt}, parent={parent}+0x{off:X}, size=0x{size:X})")
