"""Find functions whose real start is a few instructions before the listed one.

The compiler often schedules loads above a function's stack-frame setup, so a
disassembler that starts functions at the `addiu sp, sp, -N` misses them:

    80167D74: jr   ra          <- end of the previous function
    80167D78: nop
    80167D7C: lui  v0, 0x801A  <- real entry: callers jal here
    80167D80: lw   v0, 0x2810(v0)
    80167D84: addiu sp, sp, -0x20   <- where the symbol file starts it

Prints NAME:VRAM:SIZE entries (for syms_add.py) for each listed function with
1-8 non-nop instructions between the previous jr ra's delay slot and its start.

usage: find_hoisted.py IMAGE.bin BASE SYMS.toml CODE_END
"""
import re, struct, sys

image, base, syms, code_end = sys.argv[1:5]
data = open(image, "rb").read()
base, code_end = int(base, 16), int(code_end, 16)
word = lambda a: struct.unpack_from("<I", data, a - base)[0]
known = sorted({int(m, 16) for m in re.findall(r"vram = (0x[0-9A-Fa-f]+)", open(syms).read())})
known_set = set(known)
prefix = "rtos" if base == 0x80000000 else "func"

def extent(a):
    reach, p = a, a
    while p < code_end:
        w = word(p); op = w >> 26
        if op in (4, 5, 6, 7, 20, 21, 22, 23) or (op == 1 and ((w >> 16) & 0x1F) in (0, 1, 2, 3)) or \
           (op == 0x11 and ((w >> 21) & 0x1F) == 8):
            off = w & 0xFFFF
            off -= 0x10000 if off & 0x8000 else 0
            reach = max(reach, p + 4 + off * 4)
        if w == 0x03E00008 and p >= reach:
            return p + 8 - a
        if p - a > 0x8000:
            return None
        p += 4
    return None

out = []
for s in known:
    if s - base < 24 or s >= code_end:
        continue
    for n in range(1, 9):
        start = s - 4 * n
        if word(start - 8) == 0x03E00008:            # jr ra two words back: start follows its delay slot
            body = [word(start + 4 * i) for i in range(n)]
            if all(w != 0 for w in body) and start not in known_set:
                size = extent(start)
                if size:
                    out.append(f"{prefix}_{start:08X}:{start:08X}:{size:X}")
            break
        if word(start) == 0:                         # padding: not a hoisted prologue
            break
print(" ".join(out))
print(f"{len(out)} entries", file=sys.stderr)
