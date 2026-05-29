#!/usr/bin/env python3
"""
Fix the systemic MidwayRecomp split-entry bug: alternate entry points that
should FALL THROUGH into the next sequential function were emitted as standalone
functions that just return after their few instructions.

High-confidence rule: a function F (entry addr A, N instruction-comments, last
instr at A+(N-1)*4) is a fall-through stub iff:
  - its body contains NO terminating control flow (no 'return', no 'goto L_',
    no 'LOOKUP_FUNC', no inner 'func_/static_0_(' call), AND
  - A + N*4 == entry address of another known function G.
For each such F we append `<G>(rdram, ctx);` before its closing brace so control
continues into G exactly as the original fall-through did.

Idempotent: skips a function already ending in a call.
"""
import re, glob

FUNCS = sorted(glob.glob("recomp_out/funcs/funcs_*.c"))

defn = re.compile(r'RECOMP_FUNC void (func_[0-9A-Fa-f]{8}|static_0_[0-9A-Fa-f]+|recomp_entrypoint)\(uint8_t\* rdram, recomp_context\* ctx\) \{')
addr_of_name = re.compile(r'_(?:0_)?([0-9A-Fa-f]{8})$')
instr = re.compile(r'//\s*0x([0-9A-Fa-f]+):')

# Pass 1: collect every function: name, entry addr, list of instr addrs, body span
funcs = []           # (name, entry_addr, file, start_line, end_line, instr_addrs, body_text)
entry_by_addr = {}   # addr -> name

def parse_file(fp):
    lines = open(fp, encoding="utf-8", errors="replace").split("\n") if False else open(fp, encoding="utf-8", errors="replace").read().split("\n")
    res = []
    i = 0
    while i < len(lines):
        m = defn.search(lines[i])
        if m:
            name = m.group(1)
            am = addr_of_name.search(name)
            entry = int(am.group(1), 16) if am else None
            j = i + 1
            iaddrs = []
            while j < len(lines) and not re.match(r'^\s*;?\}\s*$', lines[j]):
                im = instr.search(lines[j])
                if im:
                    iaddrs.append(int(im.group(1), 16))
                j += 1
            res.append([name, entry, i, j, iaddrs])  # j = closing-brace line index
            i = j
        else:
            i += 1
    return lines, res

file_data = {}
for fp in FUNCS:
    lines, res = parse_file(fp)
    file_data[fp] = (lines, res)
    for name, entry, si, ei, iaddrs in res:
        if entry is not None:
            entry_by_addr.setdefault(entry, name)

# Pass 2: identify fall-through stubs and patch
patched = []
for fp in FUNCS:
    lines, res = file_data[fp]
    # Patch from bottom to top so line indices stay valid
    changed = False
    for name, entry, si, ei, iaddrs in sorted(res, key=lambda r: -r[2]):
        if entry is None or not iaddrs:
            continue
        body = "\n".join(lines[si+1:ei])
        has_ctrl = ('return' in body) or ('goto ' in body) or ('LOOKUP_FUNC' in body) \
                   or re.search(r'\b(func_[0-9A-Fa-f]{8}|static_0_[0-9A-Fa-f]+|recomp_entrypoint)\(', body)
        if has_ctrl:
            continue
        last = max(iaddrs)
        nxt = last + 4
        target = entry_by_addr.get(nxt)
        if target is None or target == name:
            continue
        # Insert call before the closing brace line (ei)
        indent = "    "
        lines.insert(ei, f"{indent}{target}(rdram, ctx); /* split-entry fall-through */")
        patched.append((name, "0x%08X" % entry, target, "0x%08X" % nxt, fp.split('\\')[-1]))
        changed = True
    if changed:
        open(fp, "w", encoding="utf-8").write("\n".join(lines))

print(f"Patched {len(patched)} fall-through stubs")
for name, ea, tgt, na, f in patched[:60]:
    print(f"  {name} @{ea} -> {tgt} @{na}  ({f})")
