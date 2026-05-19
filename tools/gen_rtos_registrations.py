"""Append RTOS registrations for the new mid-function entries to
src/runtime/rtos_registration.c. Reads new function names from
rtos_resume_additions.toml."""
import re

with open("rtos_resume_additions.toml") as f:
    txt = f.read()

new_funcs = []
for m in re.finditer(r'name = "(rtos_[0-9A-F]+)"\s*\n\s*vram = (0x[0-9A-Fa-f]+)', txt):
    new_funcs.append((m.group(1), int(m.group(2), 0)))

with open("src/runtime/rtos_registration.c") as f:
    existing = f.read()

# Find which are already in the file
already = set()
for m in re.finditer(r'\b(rtos_[0-9A-Fa-f]+)\b', existing):
    already.add(m.group(1).lower())

# Find which functions ACTUALLY got defined in the recompiler output —
# the recompiler may have chosen not to emit some (e.g. tail-called
# functions get inlined into their callers).
import glob
defined = set()
for path in glob.glob("recomp_out/rtos_funcs/*.c"):
    with open(path) as f: s = f.read()
    for m in re.finditer(r'^RECOMP_FUNC void (rtos_[0-9A-Fa-f]+)\(', s, re.MULTILINE):
        defined.add(m.group(1).lower())

new_funcs = [(n, a) for n, a in new_funcs
             if n.lower() not in already and n.lower() in defined]
print(f"Adding {len(new_funcs)} new RTOS registrations (filtered to existing defs)")

# Find injection points: just before the closing brace of rtos_register_all
closing = existing.rfind("}")
if closing < 0:
    print("Could not find closing brace of rtos_register_all")
    raise SystemExit(1)

externs = "\n".join(f"extern void {n}(uint8_t* rdram, recomp_context* ctx);" for n, a in new_funcs)
regs = "\n".join(f"    seattle_register_func(0x{a:08X}, {n});" for n, a in new_funcs)

# Insert externs just before "void rtos_register_all(void) {"
fn_start = existing.find("void rtos_register_all(void) {")
if fn_start < 0:
    print("Could not find rtos_register_all definition")
    raise SystemExit(1)

# Insert regs just before the closing brace
new_text = (
    existing[:fn_start] +
    "/* Auto-added mid-function entries from MAME dump analysis */\n" +
    externs + "\n\n" +
    existing[fn_start:closing] +
    "\n    /* Auto-added mid-function entries from MAME dump analysis */\n" +
    regs + "\n" +
    existing[closing:]
)

with open("src/runtime/rtos_registration.c", "w") as f:
    f.write(new_text)
print(f"Updated src/runtime/rtos_registration.c")
