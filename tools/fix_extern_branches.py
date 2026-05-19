"""Replace 'goto L_XXXX;' with a no-op return in C functions where L_XXXX
is not defined in the enclosing RECOMP_FUNC. These are 'branches outside
function' that the recompiler emitted but can't link."""
import re, os

def fix_file(path):
    with open(path, 'rb') as f: src = f.read().decode('latin-1')

    # Split into RECOMP_FUNC blocks
    matches = list(re.finditer(r'RECOMP_FUNC void (\w+)\(.*?\) \{', src))
    if not matches: return 0

    # Process each function body, collect REPLACEMENTS per global position
    repls = []  # (start, end, new_text)
    for i, m in enumerate(matches):
        bstart = m.start()
        bend = matches[i+1].start() if i+1 < len(matches) else len(src)
        body = src[bstart:bend]
        labels = set(re.findall(r'^(L_[0-9A-Fa-f]+):', body, re.MULTILINE))
        for g in re.finditer(r'goto (L_[0-9A-Fa-f]+);', body):
            if g.group(1) in labels: continue
            repls.append((bstart + g.start(), bstart + g.end(),
                          "/* extern branch %s -- no-op */ (void)0;" % g.group(1)))

    if not repls: return 0
    # Apply replacements right-to-left so earlier offsets remain valid
    repls.sort(reverse=True)
    out = src
    for s, e, new in repls:
        out = out[:s] + new + out[e:]
    with open(path, 'wb') as f: f.write(out.encode('latin-1'))
    return len(repls)

import glob
total = 0
for path in glob.glob("recomp_out/rtos_funcs/*.c") + glob.glob("recomp_out/funcs/*.c"):
    n = fix_file(path)
    if n:
        print(f"{path}: fixed {n} extern branches")
        total += n
print(f"TOTAL: {total}")
