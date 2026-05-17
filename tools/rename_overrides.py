"""Rename recompiled-function definitions to *_original to let the manual
overrides in seattle_overrides.c / rtos_trampolines.c link cleanly.

Targets every function that's defined in BOTH src/runtime/*.c (override) and
recomp_out/funcs/*.c (auto-recompiled). Detects conflicts by parsing linker
errors first, then rewrites the recomp-side definition.
"""
import os, re, sys, glob

# List of symbols to rename (from current linker errors)
SYMBOLS = """
func_800D4C24
func_800F25E0
func_80138954
func_80142850
func_80142C58
func_80142F10
func_80143A40
func_80143E74
func_80144E70
func_80144EB8
func_80145020
func_80145CE4
func_80145DE0
func_80145F98
func_801515C8
func_80151618
func_80151718
func_80161140
func_80167848
func_801A2A3C
static_0_800C40A4
static_0_800C40B4
static_0_800C40C4
static_0_800C4114
static_0_800C411C
static_0_800C414C
static_0_800C4154
static_0_800C4164
static_0_800C416C
static_0_800C4174
static_0_800C417C
static_0_800C4184
static_0_800C4194
static_0_800C419C
static_0_800C41A4
static_0_800C41B4
static_0_800C41E4
static_0_800C421C
static_0_800C4224
static_0_800C4244
static_0_800C424C
static_0_800C4264
static_0_800C426C
static_0_800C4274
static_0_800C427C
static_0_800C4284
static_0_800C428C
static_0_80142DF8
static_0_801451F0
""".split()

ROOT = 'D:/recomp/arcade/carnevil/recomp_out/funcs'

# Pattern: definition is exactly  "RECOMP_FUNC void NAME("
def_pats = {s: re.compile(rb'(\bRECOMP_FUNC void )(' + s.encode() + rb')(\()') for s in SYMBOLS}
# Pattern: header declaration "void NAME(" at start of line
decl_pats = {s: re.compile(rb'^(void )(' + s.encode() + rb')(\(uint8_t)', re.MULTILINE) for s in SYMBOLS}

renamed_defs = {s: 0 for s in SYMBOLS}
renamed_decls = {s: 0 for s in SYMBOLS}

for path in glob.glob(os.path.join(ROOT, '*.c')) + glob.glob(os.path.join(ROOT, '*.h')):
    with open(path, 'rb') as f:
        s = f.read()
    orig = s
    for sym in SYMBOLS:
        s2, n = def_pats[sym].subn(lambda m: m.group(1) + m.group(2) + b'_original' + m.group(3), s)
        if n > 0:
            renamed_defs[sym] += n
            s = s2
        s2, n = decl_pats[sym].subn(lambda m: m.group(1) + m.group(2) + b'_original' + m.group(3), s)
        if n > 0:
            renamed_decls[sym] += n
            s = s2
    if s != orig:
        with open(path, 'wb') as f:
            f.write(s); f.flush(); os.fsync(f.fileno())

print('renamed defs:')
for sym, n in sorted(renamed_defs.items()):
    if n: print(f'  {sym}: {n}')
print('renamed decls:')
for sym, n in sorted(renamed_decls.items()):
    if n: print(f'  {sym}: {n}')

# Sanity: list any symbol that got 0 def renames
missing_defs = [s for s in SYMBOLS if renamed_defs[s] == 0]
if missing_defs:
    print('NOT RENAMED (def):', missing_defs)
