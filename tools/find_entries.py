"""List function entry points a symbol file is missing.

Code reached only through pointers (driver tables, handlers passed to the RTOS)
never shows up as a jal target, so the recompiler has no function there and a
jump through the pointer fails at runtime. Candidates are
  - words anywhere in the image that hold an address inside the code, and
  - addresses built in code with lui+addiu/ori,
kept when the previous instruction pair ends a function (jr ra + delay slot,
or an unconditional j). Each one's extent runs to the jr ra after its last
forward branch.

usage: find_entries.py IMAGE.bin BASE SYMS.toml CODE_END [--any-pointer]
  -> NAME:VRAM:SIZE ...
--any-pointer keeps every pointer target with a sensible extent (fine for a
small table-driven image like the RTOS, where handlers can follow data).
--statics FUNCS.H adds the static_0_XXXXXXXX functions the recompiler made up
for jal targets the symbol file lacks; as statics they are missing from the
address lookup, so code that reaches them through a pointer cannot.
(feed the output to syms_add.py)
"""
import re, struct, sys

image, base, syms, code_end = sys.argv[1], int(sys.argv[2], 16), sys.argv[3], int(sys.argv[4], 16)
any_pointer = "--any-pointer" in sys.argv
statics = set()
if "--statics" in sys.argv:
    hdr = open(sys.argv[sys.argv.index("--statics") + 1]).read()
    statics = {int(a, 16) for a in re.findall(r"void static_\d+_([0-9A-F]{8})\(", hdr)}
data = open(image, "rb").read()
end = min(base + len(data), code_end)
prefix = "rtos" if base == 0x80000000 else "func"

known = {int(m.group(1), 16) for m in re.finditer(r"vram = (0x[0-9A-Fa-f]+)", open(syms).read())}

def word(a):
    return struct.unpack_from("<I", data, a - base)[0]

def is_jr_ra(w):
    return w == 0x03E00008

def ends_function(a, strict):
    """Previous pair closes a function: jr ra, or (for addresses built in
    code) an unconditional j. Switch cases usually end in j, so a bare data
    word needs the jr ra."""
    if a - 8 < base:
        return False
    w = word(a - 8)
    return is_jr_ra(w) or (not strict and (w >> 26) == 2)

cands = set()      # built in code: strong evidence
from_data = set()  # seen as a data word: may be a switch-table case label
for off in range(0, len(data) - 3, 4):
    v = struct.unpack_from("<I", data, off)[0]
    if base <= v < end and v % 4 == 0:
        from_data.add(v)

lui = {}
for a in range(base, end, 4):
    w = word(a)
    op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
    if op == 0x0F:
        lui[rt] = ((w & 0xFFFF) << 16, a)
    elif op in (0x09, 0x0D) and rs in lui and a - lui[rs][1] <= 32:
        imm = w & 0xFFFF
        if op == 0x09 and imm & 0x8000:
            imm -= 0x10000
        v = (lui[rs][0] + imm) & 0xFFFFFFFF if op == 0x09 else lui[rs][0] | imm
        if base <= v < end and v % 4 == 0:
            cands.add(v)

def extent(a):
    """Bytes from a to the delay slot of the jr ra that closes the function."""
    reach = a
    p = a
    while p < end:
        w = word(p)
        op = w >> 26
        if op in (4, 5, 6, 7, 20, 21, 22, 23) or (op == 1 and ((w >> 16) & 0x1F) in (0, 1, 2, 3)) or \
           (op == 0x11 and ((w >> 21) & 0x1F) == 8):
            off = w & 0xFFFF
            if off & 0x8000:
                off -= 0x10000
            reach = max(reach, p + 4 + off * 4)
        if is_jr_ra(w) and p >= reach:
            return p + 8 - a
        if p - a > 0x8000:
            return None
        p += 4
    return None

out = []
if statics:
    cands, from_data, any_pointer = statics, set(), True
for a in sorted((cands | from_data) - known):
    if not any_pointer and not ends_function(a, strict=a not in cands):
        continue
    n = extent(a)
    if n:
        out.append(f"{prefix}_{a:08X}:{a:08X}:{n:X}")
print(" ".join(out))
print(f"{len(out)} entries", file=sys.stderr)
