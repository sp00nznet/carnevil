#!/usr/bin/env python3
"""
Small wrapper to run the external m2c decompiler (matt-kempster/m2c, GPL-3.0)
on a region of the game binary for type/struct recovery during reverse
engineering. m2c is invoked as an external tool only (no code is copied in).

Pipeline: spimdisasm disassembles the requested vram range (little-endian) into
m2c-compatible GAS .s, then m2c decompiles the named functions with its
cross-function type + struct-field inference. Useful for recovering the layout
of in-memory structures (e.g. the entity table at 0x801E3880) that are hard to
read off raw recompiled code.

Example (entity-dispatch table):
  python tools/m2c_decompile.py --vram-start 0x800CC000 --vram-end 0x800CE000 \
      -f func_800CC218 -f func_800CC47C

Requires: pip install spimdisasm ; git clone matt-kempster/m2c (path via --m2c).
"""
import argparse, subprocess, sys, os, glob
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ap = argparse.ArgumentParser()
ap.add_argument("--bin", default=str(ROOT / "extracted/GAME.bin"))
ap.add_argument("--load-vram", type=lambda x: int(x, 0), default=0x800C4000,
                help="vram address of byte 0 of --bin")
ap.add_argument("--vram-start", type=lambda x: int(x, 0), required=True)
ap.add_argument("--vram-end", type=lambda x: int(x, 0), required=True)
ap.add_argument("-f", "--func", action="append", default=[], dest="funcs",
                help="function name(s) to decompile (repeatable)")
ap.add_argument("--m2c", default="C:/tools/m2c/m2c.py")
ap.add_argument("--target", default=None, help="m2c -t target (optional)")
ap.add_argument("--asm-out", default=str(ROOT / "m2c_asm"))
ap.add_argument("--out", default=None, help="write C here (default: stdout)")
a = ap.parse_args()

fstart = a.vram_start - a.load_vram
fend = a.vram_end - a.load_vram
asm_dir = Path(a.asm_out)
if asm_dir.exists():
    for p in asm_dir.rglob("*"):
        if p.is_file():
            p.unlink()
asm_dir.mkdir(parents=True, exist_ok=True)

# 1) spimdisasm -> little-endian GAS .s
dis = [sys.executable, "-m", "spimdisasm.singleFileDisasm", a.bin,
       str(asm_dir / "region"),
       "--start", hex(fstart), "--end", hex(fend),
       "--vram", hex(a.vram_start), "--instr-category", "cpu",
       "--endian", "little"]
r = subprocess.run(dis, capture_output=True, text=True)
if r.returncode != 0:
    sys.stderr.write(r.stdout + r.stderr); sys.exit("spimdisasm failed")
asm_files = glob.glob(str(asm_dir / "**" / "*.s"), recursive=True)
if not asm_files:
    sys.exit("no .s produced")

# 2) m2c
cmd = [sys.executable, a.m2c, "--globals", "used"]
if a.target:
    cmd += ["-t", a.target]
cmd += asm_files
for fn in a.funcs:
    cmd += ["-f", fn]
r = subprocess.run(cmd, capture_output=True, text=True)
out = r.stdout
if a.out:
    Path(a.out).write_text(out, encoding="utf-8")
    print("wrote %s (%d bytes)" % (a.out, len(out)))
    if r.stderr.strip():
        sys.stderr.write(r.stderr)
else:
    sys.stdout.write(out)
    if r.stderr.strip():
        sys.stderr.write("\n--- m2c stderr ---\n" + r.stderr)
