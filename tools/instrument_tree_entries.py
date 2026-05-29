#!/usr/bin/env python3
"""
Insert an entry-hit tally call `g_fn_hit(0xADDR);` at the top of every
function body in recomp_out/funcs that is one of the 314 functions in the
backward-reachability tree of func_80167848. Lets us see which of those
functions our recomp actually executes — the boundary between "runs" and
"never runs" is the dispatch break that keeps rendering dormant.

Idempotent-ish: only inserts where not already present.
"""
import re
from pathlib import Path

ROOT = Path("D:/recomp/arcade/carnevil/recomp_out/funcs")
TREE = Path("D:/recomp/arcade/carnevil/tree_funcs.txt")

addrs = set()
for line in TREE.read_text().split("\n"):
    line = line.strip()
    if line.startswith("0x"):
        addrs.add(int(line, 16))

# Match function definitions: RECOMP_FUNC void func_XXXXXXXX(uint8_t* rdram, recomp_context* ctx) {
defn_re = re.compile(
    r"^(RECOMP_FUNC\s+void\s+func_([0-9A-Fa-f]{8})\s*\(uint8_t\*\s*rdram,\s*recomp_context\*\s*ctx\)\s*\{)\s*$"
)

patched = 0
files_changed = 0
found_addrs = set()
for fp in sorted(ROOT.glob("funcs_*.c")):
    lines = fp.read_text(encoding="utf-8", errors="replace").split("\n")
    out = []
    changed = False
    for i, line in enumerate(lines):
        out.append(line)
        m = defn_re.match(line)
        if m:
            faddr = int(m.group(2), 16)
            if faddr in addrs:
                # avoid double-insert
                nxt = lines[i+1] if i+1 < len(lines) else ""
                if "g_fn_hit(" not in nxt:
                    out.append(f"    g_fn_hit(0x{faddr:08X});")
                    patched += 1
                    found_addrs.add(faddr)
                    changed = True
    if changed:
        fp.write_text("\n".join(out), encoding="utf-8")
        files_changed += 1

print(f"Inserted {patched} entry tallies across {files_changed} files")
print(f"Tree addrs: {len(addrs)}, matched in recomp: {len(found_addrs)}")
missing = addrs - found_addrs
print(f"Not found as RECOMP_FUNC defs: {len(missing)}")
if missing:
    for a in sorted(missing)[:40]:
        print(f"  missing 0x{a:08X}")
