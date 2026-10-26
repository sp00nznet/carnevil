#!/usr/bin/env python3
"""
Merge derived function names (ghidra_export/name_map.json) into
carnevil_syms.toml. This is the canonical naming source MidwayRecomp reads at
recompile time, so renamed entries flow into generated code on the next regen.

Safety: functions that have a hand-written override in src/runtime/*.c are
EXCLUDED (listed in ghidra_export/overridden_addrs.txt) — their generated
definition is renamed to *_original by rename_overrides.py and the override
keeps the func_XXXX name, so renaming them in syms would orphan the override.

The C++/CMake build does not read syms (regen is a separate manual step), so
applying this does not affect the current build; it takes effect on next regen.
Idempotent: only rewrites default func_/static_ names, never an already-derived
name.
"""
import json, re
from pathlib import Path

ROOT = Path("D:/recomp/arcade/carnevil")
SYMS = ROOT / "carnevil_syms.toml"
name_map = json.load(open(ROOT / "ghidra_export/name_map.json"))
overridden = set("0x" + l.strip() for l in open(ROOT / "ghidra_export/overridden_addrs.txt") if l.strip())

# addr-int -> new name, excluding overridden
safe = {}
for k, v in name_map.items():
    if k in overridden:
        continue
    safe[int(k, 16)] = v

lines = SYMS.read_text(encoding="utf-8").split("\n")
name_re = re.compile(r'^(\s*)name = "([^"]*)"\s*$')
vram_re = re.compile(r'^\s*vram = (0x[0-9A-Fa-f]+)\s*$')

applied = 0
pending_name_idx = None
pending_indent = None
for i, line in enumerate(lines):
    nm = name_re.match(line)
    if nm:
        pending_name_idx = i
        pending_indent = nm.group(1)
        continue
    vm = vram_re.match(line)
    if vm and pending_name_idx is not None:
        addr = int(vm.group(1), 16)
        if addr in safe:
            cur = name_re.match(lines[pending_name_idx]).group(2)
            # only rewrite default names
            if cur.startswith("func_") or cur.startswith("static_"):
                lines[pending_name_idx] = '%sname = "%s"' % (pending_indent, safe[addr])
                applied += 1
        pending_name_idx = None

SYMS.write_text("\n".join(lines), encoding="utf-8")
print("Applied %d / %d safe names to %s" % (applied, len(safe), SYMS.name))
