"""Add (or resize) functions in a MidwayRecomp symbol file.

usage: syms_add.py SYMS.toml NAME:VRAM:SIZE [...]
Overlapping entries are fine: a mid-function entry point is recompiled as its
own copy running to the end of its host function.
"""
import re, sys

path, specs = sys.argv[1], sys.argv[2:]
text = open(path).read()
for spec in specs:
    name, vram, size = spec.split(":")
    vram, size = int(vram, 16), int(size, 16)
    entry = re.compile(r'\n  \[\[section\.functions\]\]\n  name = "[^"]+"\n  vram = 0x%08X\n  size = 0x[0-9A-Fa-f]+\n' % vram, re.I)
    new = f'\n  [[section.functions]]\n  name = "{name}"\n  vram = 0x{vram:08X}\n  size = 0x{size:X}\n'
    text, n = entry.subn(new, text)
    if n == 0:
        text = text.rstrip("\n") + "\n" + new
open(path, "w").write(text)
