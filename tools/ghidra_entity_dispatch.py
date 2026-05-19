# -*- coding: utf-8 -*-
# Ghidra Jython script: find dispatch chain to entity-draw functions
# @category CarnEvil
# @runtime Jython

TARGETS = [
    0x800D7600,
    0x800ED458, 0x801014C8, 0x80102268, 0x80102A10,
    0x80103120, 0x80103848, 0x80103FD0, 0x80104780,
    0x801079F0, 0x801098AC, 0x8010D3D0, 0x8010D4FC,
    0x8012CFF8, 0x8013934C,
    0x800CC1E4,
    0x800E91E8,
]

fm = currentProgram.getFunctionManager()
af = currentProgram.getAddressFactory()
rm = currentProgram.getReferenceManager()
lis = currentProgram.getListing()

def get_func_at(addr):
    return fm.getFunctionContaining(addr)

println("=" * 70)
println("Cross-references to entity-dispatch targets")
println("=" * 70)
for t in TARGETS:
    addr = af.getAddress("%x" % t)
    refs = rm.getReferencesTo(addr)
    rows = []
    for r in refs:
        rtype = r.getReferenceType()
        from_addr = r.getFromAddress()
        f = get_func_at(from_addr)
        fname = f.getName() if f else "<no function>"
        fvram = "0x%08X" % f.getEntryPoint().getOffset() if f else "?"
        rows.append((from_addr, rtype, fname, fvram))
    println("")
    println("TARGET 0x%08X (%d refs):" % (t, len(rows)))
    if not rows:
        println("  (no references)")
        continue
    for from_addr, rtype, fname, fvram in rows:
        println("  %-15s @%s in %s @%s" % (
            str(rtype), str(from_addr), fname, fvram))

println("")
println("=" * 70)
println("Data-reference scan (function pointers in data)")
println("=" * 70)
data_iter = lis.getDefinedData(True)
target_set = set(TARGETS)
hits = 0
while data_iter.hasNext():
    d = data_iter.next()
    try:
        v = d.getValue()
        if hasattr(v, 'getOffset'):
            off = v.getOffset()
            if off in target_set:
                println("  Pointer to 0x%08X at %s" % (off, d.getAddress()))
                hits += 1
    except:
        pass
println("Data pointer hits: %d" % hits)
