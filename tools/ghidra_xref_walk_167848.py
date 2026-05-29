# -*- coding: utf-8 -*-
# Walk callers of func_80167848 backward to depth N. Output the full
# reachability tree so we can see what static call-graph paths reach
# the DMA command builder, then compare against what our recomp executes.
#
# @category CarnEvil
# @runtime Jython

import os

TARGET   = 0x80167848
MAX_DEPTH = 8
OUTPATH  = "D:/recomp/arcade/carnevil/ghidra_xref_walk_167848.log"

fm = currentProgram.getFunctionManager()
af = currentProgram.getAddressFactory()
rm = currentProgram.getReferenceManager()

def get_func_containing(addr):
    return fm.getFunctionContaining(addr)

def callers_of(func_entry_offset):
    """Return list of (caller_func_entry_offset, ref_type_str, from_addr_str)."""
    addr = af.getAddress("%x" % func_entry_offset)
    refs = rm.getReferencesTo(addr)
    out = []
    for r in refs:
        rtype = r.getReferenceType()
        from_addr = r.getFromAddress()
        f = get_func_containing(from_addr)
        if f is not None:
            out.append((f.getEntryPoint().getOffset(),
                        str(rtype),
                        "0x%08X" % from_addr.getOffset()))
        else:
            out.append((None, str(rtype),
                        "0x%08X" % from_addr.getOffset()))
    return out

# BFS backward from target. Track depth at which each function is first reached.
first_depth = {TARGET: 0}
# edge: (callee_offset, caller_offset, ref_type, from_addr)
edges = []

frontier = [TARGET]
for depth in range(1, MAX_DEPTH + 1):
    next_frontier_set = set()
    for callee in frontier:
        for (caller, rtype, from_addr) in callers_of(callee):
            edges.append((callee, caller, rtype, from_addr, depth))
            if caller is None:
                continue
            if caller not in first_depth:
                first_depth[caller] = depth
                next_frontier_set.add(caller)
    frontier = list(next_frontier_set)
    if not frontier:
        break

# Write the result
fout = open(OUTPATH, "w")

def w(s):
    println(s)
    fout.write(s + "\n")

w("=" * 78)
w("Backward xref walk from 0x%08X (max depth %d)" % (TARGET, MAX_DEPTH))
w("=" * 78)
w("")
w("Reached %d distinct functions across %d edges" % (len(first_depth), len(edges)))
w("")

# Group edges by depth and print
edges_by_depth = {}
for (callee, caller, rtype, from_addr, depth) in edges:
    edges_by_depth.setdefault(depth, []).append((callee, caller, rtype, from_addr))

for depth in sorted(edges_by_depth.keys()):
    es = edges_by_depth[depth]
    w("")
    w("---- Depth %d (%d edges) ----" % (depth, len(es)))
    for (callee, caller, rtype, from_addr) in es:
        if caller is None:
            w("  [orphan ref] 0x%08X <- %s @ %s (no enclosing function)" %
              (callee, rtype, from_addr))
        else:
            new_marker = " *NEW*" if first_depth.get(caller) == depth else ""
            w("  0x%08X  <-  0x%08X  (%s @ %s)%s" %
              (callee, caller, rtype, from_addr, new_marker))

# Summary: list of all callers reached, with their first-discovered depth
w("")
w("=" * 78)
w("All reached functions (by depth)")
w("=" * 78)
for fn, d in sorted(first_depth.items(), key=lambda x: (x[1], x[0])):
    w("  depth %d  0x%08X" % (d, fn))

fout.close()
println("Wrote " + OUTPATH)
