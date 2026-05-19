# -*- coding: utf-8 -*-
# Trace what FUN_800ECA7C is and who calls it
# @category CarnEvil
# @runtime Jython

fm = currentProgram.getFunctionManager()
af = currentProgram.getAddressFactory()
rm = currentProgram.getReferenceManager()
lis = currentProgram.getListing()

def find_func(name_or_addr):
    a = af.getAddress(name_or_addr)
    f = fm.getFunctionAt(a) or fm.getFunctionContaining(a)
    return f

println("=" * 70)
println("FUN_800ECA7C analysis")
println("=" * 70)
target = af.getAddress("800eca7c")
f = fm.getFunctionAt(target)
if f:
    println("Name: %s, entry: %s, body length: %d" %
            (f.getName(), f.getEntryPoint(),
             f.getBody().getNumAddresses()))
    println("")
    println("All refs TO FUN_800ECA7C:")
    refs = rm.getReferencesTo(target)
    n = 0
    for r in refs:
        n += 1
        fr = r.getFromAddress()
        cf = fm.getFunctionContaining(fr)
        cname = cf.getName() if cf else "<none>"
        println("  %-15s @%s in %s" %
                (str(r.getReferenceType()), str(fr), cname))
    println("Total: %d" % n)
else:
    println("Function not found at 0x800ECA7C")

println("")
println("=" * 70)
println("Other 'missing' functions Ghidra discovered (not in our syms.toml)")
println("=" * 70)
# Iterate Ghidra's functions; flag any that don't match our func_XXX naming
fns = fm.getFunctions(True)
n_total = 0
n_unnamed = 0
unnamed_examples = []
while fns.hasNext():
    fn = fns.next()
    n_total += 1
    name = fn.getName()
    if name.startswith("FUN_"):
        n_unnamed += 1
        if len(unnamed_examples) < 30:
            unnamed_examples.append(fn.getEntryPoint().getOffset())
println("Ghidra functions: %d" % n_total)
println("Unnamed (not in carnevil_syms.toml): %d" % n_unnamed)
println("First 30 unnamed function addresses (Ghidra found, we didn't):")
for addr in unnamed_examples:
    println("  0x%08X" % addr)
