# -*- coding: utf-8 -*-
# Find every lui+addiu (or lui+ori) pair in .text that constructs the
# address of any depth-1 caller of func_80167848. These are "address-of"
# operations that load a function pointer into a register — most likely
# to store it into a dispatch table or use it as a call target via jalr.
#
# Pair-finding rules: in MIPS, a LUI sets the high 16 bits of a register,
# then a nearby ADDIU adds the (sign-extended) low 16 bits. If
# (high << 16) + (signed low) == some depth-1 caller, record it.
#
# @category CarnEvil
# @runtime Jython

DEPTH1 = [
    0x800D1408, 0x800D16F0, 0x800D1A44, 0x800D1E2C,
    0x800D2074, 0x800D231C, 0x800D2654, 0x800D2A8C,
    0x800D2CA8, 0x800D2F24, 0x800D31E0,
    0x8015E2F4, 0x8015E3E4, 0x8015E658, 0x8015E70C,
    0x8015EC70, 0x8015ED34, 0x8015EE64, 0x8015F178,
    0x8015F264, 0x8015F308, 0x8015F3C4, 0x8015F498,
    0x8015F558, 0x8015F7F8, 0x8015F8D4, 0x8015F9A8,
    0x8015FA3C, 0x80167D7C,
]
TARGET_SET = set(DEPTH1)
OUTPATH = "D:/recomp/arcade/carnevil/ghidra_lui_addiu_scan.log"

fm = currentProgram.getFunctionManager()
af = currentProgram.getAddressFactory()
mem = currentProgram.getMemory()
listing = currentProgram.getListing()

fout = open(OUTPATH, "w")
def w(s):
    println(s)
    fout.write(s + "\n")

# Find the .text block
text_block = None
for b in mem.getBlocks():
    if b.getName() == ".text":
        text_block = b
        break

if text_block is None:
    println("ERROR: no .text block found")
    fout.close()
    raise SystemExit()

start_off = text_block.getStart().getOffset()
end_off   = text_block.getEnd().getOffset()
w("Scanning .text [0x%08X..0x%08X] for lui+addiu/ori pairs constructing depth-1 callers" % (start_off, end_off))
w("Targets: %d distinct" % len(TARGET_SET))
w("")

def sign_ext_16(v):
    v = v & 0xFFFF
    return v if v < 0x8000 else v - 0x10000

# Per-register, track the LUI most recently observed: reg -> (lui_addr, high_imm)
last_lui = {}
hits = []  # (lui_addr, addiu_addr, target, reg, kind)

addr = text_block.getStart()
count_inst = 0
while addr.getOffset() <= end_off - 3:
    try:
        w_word = mem.getInt(addr) & 0xFFFFFFFF
    except:
        addr = addr.add(4)
        continue
    opcode = (w_word >> 26) & 0x3F
    rs     = (w_word >> 21) & 0x1F
    rt     = (w_word >> 16) & 0x1F
    imm    =  w_word        & 0xFFFF

    if opcode == 0x0F and rs == 0:
        # LUI rt, imm
        last_lui[rt] = (addr.getOffset(), imm)
    elif opcode == 0x09 or opcode == 0x0D:
        # ADDIU (0x09) or ORI (0x0D)
        # form: opcode rt, rs, imm  (for ADDIU)
        # We pair with last_lui[rs] (LUI loaded high into rs, then addiu rt, rs, imm)
        if rs in last_lui:
            lui_addr, high_imm = last_lui[rs]
            if opcode == 0x09:
                low = sign_ext_16(imm)
            else:
                low = imm  # ORI is zero-extended
            target = ((high_imm << 16) + low) & 0xFFFFFFFF
            if target in TARGET_SET:
                kind = "addiu" if opcode == 0x09 else "ori"
                hits.append((lui_addr, addr.getOffset(), target, rs, kind))
                # don't pop the LUI — could be used again
    # Some instructions clobber the LUI destination — track common clobbers
    # (but for now, leave last_lui sticky; spurious double-counts are OK)
    addr = addr.add(4)
    count_inst += 1

w("Scanned %d instructions" % count_inst)
w("Found %d lui+addiu/ori pairs constructing depth-1 caller addresses" % len(hits))
w("")

# Group by target
by_target = {}
for h in hits:
    by_target.setdefault(h[2], []).append(h)

# Function containing helper
def func_at(off):
    a = af.getAddress("%x" % off)
    f = fm.getFunctionContaining(a)
    if f is None:
        return ("?", 0)
    return (f.getName(), f.getEntryPoint().getOffset())

w("=" * 78)
w("Hits grouped by target (depth-1 caller of func_80167848)")
w("=" * 78)
for t in sorted(by_target.keys()):
    rows = by_target[t]
    w("")
    w("  TARGET 0x%08X (%d address-of constructions):" % (t, len(rows)))
    for (lui_addr, addiu_addr, target, reg, kind) in rows:
        fname, fentry = func_at(lui_addr)
        w("    lui@0x%08X %s@0x%08X  reg=$%d  in %s @ 0x%08X" % (
            lui_addr, kind, addiu_addr, reg, fname, fentry))

# Summary: distinct enclosing functions
distinct_funcs = set()
for (lui_addr, addiu_addr, target, reg, kind) in hits:
    fname, fentry = func_at(lui_addr)
    distinct_funcs.add(fentry)
w("")
w("=" * 78)
w("Distinct enclosing functions (%d):" % len(distinct_funcs))
w("=" * 78)
for f in sorted(distinct_funcs):
    w("  0x%08X" % f)

fout.close()
println("Wrote " + OUTPATH)
