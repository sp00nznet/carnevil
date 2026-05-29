#!/usr/bin/env python3
"""
Patch every `func_80167848(rdram, ctx);` call site in recomp_out/funcs to
record the caller's jal PC into a global before the call. Lets the override
log exactly which static call site fired each invocation, which tells us
which depth-1 callers are active in our recomp vs which aren't.

Idempotent: writes patched files in-place, but skips lines already patched.
"""
import re
import sys
from pathlib import Path

ROOT = Path("D:/recomp/arcade/carnevil/recomp_out/funcs")
FILES = sorted(ROOT.glob("funcs_*.c"))

call_re = re.compile(r"^(\s*)func_80167848\(rdram,\s*ctx\);\s*$")
# Walk-back: find nearest preceding "// 0xPC: jal 0x80167848" comment.
jal_re  = re.compile(r"^\s*//\s*0x([0-9A-Fa-f]+):\s*jal\s+0x80167848\b")

patched_total = 0
sites = []  # (file, pc)
for fp in FILES:
    text = fp.read_text(encoding="utf-8", errors="replace")
    lines = text.split("\n")
    out = []
    i = 0
    file_patched = 0
    while i < len(lines):
        line = lines[i]
        m = call_re.match(line)
        if m:
            indent = m.group(1)
            # Walk back up to 20 lines to find the jal comment
            pc = None
            for j in range(i - 1, max(-1, i - 20), -1):
                jm = jal_re.match(lines[j])
                if jm:
                    pc = int(jm.group(1), 16)
                    break
            # Don't double-patch
            already = (i > 0 and "g_last_167848_site" in lines[i-1])
            if pc is not None and not already:
                out.append(f"{indent}g_last_167848_site = 0x{pc:08X};")
                sites.append((fp.name, pc))
                file_patched += 1
        out.append(line)
        i += 1
    if file_patched:
        fp.write_text("\n".join(out), encoding="utf-8")
        patched_total += file_patched
        print(f"  patched {file_patched} sites in {fp.name}")

print(f"\nTotal patched: {patched_total}")
print(f"Distinct PCs: {len(set(s[1] for s in sites))}")

# Emit a header snippet to add to the override (manually) with the global declaration
print("\n--- Append to seattle_overrides.c (or include) ---")
print("uint32_t g_last_167848_site = 0;\n")
