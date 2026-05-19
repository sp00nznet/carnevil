# -*- coding: utf-8 -*-
# Find what calls func_800E91E8 and decode its switch table
# @category CarnEvil
# @runtime Jython

fm = currentProgram.getFunctionManager()
af = currentProgram.getAddressFactory()
rm = currentProgram.getReferenceManager()
lis = currentProgram.getListing()

println("=" * 70)
println("All references to func_800E91E8 (any kind)")
println("=" * 70)
target = af.getAddress("800e91e8")
refs = rm.getReferencesTo(target)
n = 0
for r in refs:
    n += 1
    println("  %-15s @%s" % (str(r.getReferenceType()), str(r.getFromAddress())))
println("Total: %d" % n)

println("")
println("=" * 70)
println("What contains address 0x800ECD8C?")
println("=" * 70)
a = af.getAddress("800ecd8c")
f = fm.getFunctionContaining(a)
if f:
    println("  Function: %s at %s" % (f.getName(), f.getEntryPoint()))
else:
    println("  No function. Trying instruction:")
    instr = lis.getInstructionAt(a)
    if instr: println("    %s" % str(instr))
    # Walk backward to find a function boundary
    for back in range(0, 0x400, 4):
        ba = a.subtract(back)
        f2 = fm.getFunctionContaining(ba)
        if f2:
            println("    Nearest function backwards: %s at %s (offset -0x%X)" %
                    (f2.getName(), f2.getEntryPoint(), back))
            break

println("")
println("=" * 70)
println("Decoding func_800E91E8 type-switch comparisons")
println("=" * 70)
# Walk instructions in func_800E91E8 looking for bne $vN, ZERO, <addr> patterns
# and ADDIU constants that look like type IDs (0x201X range)
import re
fn = fm.getFunctionAt(af.getAddress("800e91e8"))
if fn:
    body = fn.getBody()
    insts = lis.getInstructions(body, True)
    seen = set()
    type_consts = []
    while insts.hasNext():
        i = insts.next()
        mn = i.getMnemonicString()
        if mn == "addiu" or mn == "li" or mn == "ori":
            try:
                ops = i.toString()
                m = re.search(r"0x([0-9a-fA-F]{3,5})\b", ops)
                if m:
                    val = int(m.group(1), 16)
                    if 0x2010 <= val <= 0x2080 and val not in seen:
                        seen.add(val)
                        type_consts.append((str(i.getAddress()), val, ops))
            except:
                pass
    for addr, val, ops in type_consts:
        println("  @%s  val=0x%04X  %s" % (addr, val, ops))

println("")
println("=" * 70)
println("Find what address loads/calls func_8013934C from <no function>")
println("=" * 70)
# Search code refs more broadly: get all memory operations referring to 8013934C
mem_target = af.getAddress("8013934c")
all_refs = rm.getReferencesTo(mem_target)
for r in all_refs:
    fr = r.getFromAddress()
    f3 = fm.getFunctionContaining(fr)
    instr = lis.getInstructionAt(fr)
    fname = f3.getName() if f3 else "<none>"
    println("  ref @%s in %s  instr=%s" % (
        str(fr), fname, str(instr) if instr else "?"))
