# -*- coding: utf-8 -*-
# Export Ghidra analysis to three JSON files for offline name derivation:
#   functions.json  : [{addr,name,size,signature,callees[],string_refs[]}]
#   symbols.json    : {labels:[{addr,name,type,source}], strings:[{addr,value}]}
#   decompiled.json : {"%08X": "<decompiled C>"}
#
# String_refs are the key naming signal: CarnEvil routes debug output through
# printf-like loggers, so a function that references "adjust.fmt" / "startadjust"
# can be named after that string. We collect them via the reference manager
# (fast) rather than parsing decompiled C.
#
# Decompilation of all funcs is slow; pass a positive DECOMP_LIMIT (env or edit)
# to cap it for a quick pass, or 0 for all.
#
# @category CarnEvil
# @runtime Jython

import json
import os

OUT_DIR = "D:/recomp/arcade/carnevil/ghidra_export"
DECOMP_LIMIT = 0          # 0 = decompile every function
DECOMP_TIMEOUT = 60       # seconds per function

try:
    os.makedirs(OUT_DIR)
except:
    pass

from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor

fm = currentProgram.getFunctionManager()
st = currentProgram.getSymbolTable()
listing = currentProgram.getListing()
rm = currentProgram.getReferenceManager()

def is_string_data(d):
    try:
        n = d.getDataType().getName().lower()
    except:
        return False
    return ("string" in n) or ("char" in n and d.isArray())

# ---- Build string table + map each string address to a python value ----
println("Collecting defined strings...")
strings = []
str_value_by_addr = {}
di = listing.getDefinedData(True)
while di.hasNext():
    d = di.next()
    if is_string_data(d):
        try:
            val = d.getValue()
            if val is not None:
                val = unicode(val)
        except:
            try:
                val = d.getDefaultValueRepresentation()
            except:
                val = None
        off = d.getAddress().getOffset()
        strings.append({"addr": off, "value": val})
        str_value_by_addr[off] = val
println("  %d strings" % len(strings))

# ---- For each string, find referencing functions -> function string_refs ----
println("Mapping string references to functions...")
func_string_refs = {}   # func_entry_offset -> set(values)
for s in strings:
    saddr = toAddr(s["addr"])
    refs = rm.getReferencesTo(saddr)
    for r in refs:
        f = fm.getFunctionContaining(r.getFromAddress())
        if f is None:
            continue
        fe = f.getEntryPoint().getOffset()
        func_string_refs.setdefault(fe, [])
        if s["value"] is not None and s["value"] not in func_string_refs[fe]:
            func_string_refs[fe].append(s["value"])

# ---- functions.json ----
println("Exporting functions.json...")
funcs = []
fit = fm.getFunctions(True)
all_funcs = []
while fit.hasNext():
    all_funcs.append(fit.next())

for f in all_funcs:
    fe = f.getEntryPoint().getOffset()
    # callees
    callees = []
    try:
        for callee in f.getCalledFunctions(ConsoleTaskMonitor()):
            callees.append(callee.getEntryPoint().getOffset())
    except:
        pass
    try:
        sig = f.getPrototypeString(False, False)
    except:
        sig = ""
    funcs.append({
        "addr": fe,
        "name": f.getName(),
        "size": f.getBody().getNumAddresses(),
        "signature": sig,
        "callees": callees,
        "string_refs": func_string_refs.get(fe, []),
    })

with open(os.path.join(OUT_DIR, "functions.json"), "w") as fh:
    json.dump(funcs, fh, indent=1)
println("  %d functions" % len(funcs))

# ---- symbols.json ----
println("Exporting symbols.json...")
labels = []
sit = st.getAllSymbols(True)
while sit.hasNext():
    s = sit.next()
    labels.append({
        "addr": s.getAddress().getOffset(),
        "name": s.getName(),
        "type": str(s.getSymbolType()),
        "source": str(s.getSource()),
    })
with open(os.path.join(OUT_DIR, "symbols.json"), "w") as fh:
    json.dump({"labels": labels, "strings": strings}, fh, indent=1)
println("  %d symbols, %d strings" % (len(labels), len(strings)))

# ---- decompiled.json ----
println("Decompiling functions (this is the slow part)...")
ifc = DecompInterface()
ifc.openProgram(currentProgram)
monitor = ConsoleTaskMonitor()
decomp = {}
count = 0
for f in all_funcs:
    if DECOMP_LIMIT and count >= DECOMP_LIMIT:
        break
    res = ifc.decompileFunction(f, DECOMP_TIMEOUT, monitor)
    key = "%08X" % f.getEntryPoint().getOffset()
    if res is not None and res.decompileCompleted():
        try:
            decomp[key] = res.getDecompiledFunction().getC()
        except:
            decomp[key] = ""
    else:
        decomp[key] = ""
    count += 1
    if count % 200 == 0:
        println("  decompiled %d/%d" % (count, len(all_funcs)))

with open(os.path.join(OUT_DIR, "decompiled.json"), "w") as fh:
    json.dump(decomp, fh, indent=0)
println("  decompiled %d functions" % len(decomp))

println("DONE. Wrote functions.json, symbols.json, decompiled.json to " + OUT_DIR)
