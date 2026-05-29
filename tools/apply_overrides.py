#!/usr/bin/env python3
"""
Resolve override conflicts after a regen, robustly.

For every symbol that is defined as RECOMP_FUNC in BOTH src/runtime/*.c (the
hand-written override) and recomp_out/funcs/*.c (the auto-recompiled body):
  - rename the recomp-side DEFINITION to NAME_original (so the override's plain
    NAME is the single definition that links and gets registered)
  - in recomp_out/funcs/funcs.h, ADD a `void NAME_original(...)` declaration
    while KEEPING the plain `void NAME(...)` declaration (the override needs the
    plain decl for func_registration.inc; the recomp body / override-calls-original
    need the _original decl).

This differs from the older rename_overrides.py, which *replaced* the plain
declaration with _original and thus left the override's plain name undeclared.
Idempotent.
"""
import re, glob
from pathlib import Path

ROOT = Path("D:/recomp/arcade/carnevil")
RECOMP = ROOT / "recomp_out/funcs"
HDR = RECOMP / "funcs.h"

# Symbols DEFINED as overrides in src/runtime. Must be a real definition, not an
# `extern RECOMP_FUNC void NAME(...);` reference (those name recomp functions the
# override CALLS and must stay plain).
override_names = set()
ovre = re.compile(r"^[ \t]*RECOMP_FUNC void ([A-Za-z_]\w*)\(", re.M)
for fp in glob.glob(str(ROOT / "src/runtime/*.c")):
    for line in Path(fp).read_text(encoding="utf-8", errors="replace").split("\n"):
        if "extern" in line:
            continue
        m = ovre.match(line)
        if m:
            override_names.add(m.group(1))

# Keep only those that ALSO have a recomp-side definition
recomp_defs = set()
for fp in RECOMP.glob("funcs_*.c"):
    for m in ovre.finditer(fp.read_text(encoding="utf-8", errors="replace")):
        recomp_defs.add(m.group(1))
conflict = sorted(override_names & recomp_defs)

# 1. Rename recomp-side DEFINITIONS to _original
renamed = 0
for fp in RECOMP.glob("funcs_*.c"):
    txt = fp.read_text(encoding="utf-8", errors="replace")
    changed = False
    for sym in conflict:
        pat = re.compile(r"(RECOMP_FUNC void )" + re.escape(sym) + r"(\()")
        # skip if already _original
        new, n = pat.subn(lambda m: m.group(1) + sym + "_original" + m.group(2), txt)
        if n:
            txt = new; changed = True; renamed += n
    if changed:
        fp.write_text(txt, encoding="utf-8")

# 2. In funcs.h: keep plain decl, ADD _original decl
hdr = HDR.read_text(encoding="utf-8", errors="replace")
added = 0
for sym in conflict:
    plain = "void %s(uint8_t* rdram, recomp_context* ctx);" % sym
    orig = "void %s_original(uint8_t* rdram, recomp_context* ctx);" % sym
    if orig in hdr:
        continue
    if plain in hdr:
        hdr = hdr.replace(plain, plain + "\n" + orig, 1)
        added += 1
    else:
        # plain decl missing (e.g. renamed func not in syms decl set) -> add both
        hdr = hdr.replace("#endif", plain + "\n" + orig + "\n#endif", 1) if "#endif" in hdr else hdr + "\n" + plain + "\n" + orig + "\n"
        added += 1
HDR.write_text(hdr, encoding="utf-8")

print("Conflicts: %d  | recomp defs renamed to _original: %d  | _original decls added: %d"
      % (len(conflict), renamed, added))
