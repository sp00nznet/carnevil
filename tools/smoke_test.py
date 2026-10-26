"""Boot smoke test: run the recompiled game headless and check it gets going.

usage: python tools/smoke_test.py [build/Release/carnevil.exe] [frames]
Run from the repository root (the exe loads the ROMs and disk from there).
Passes when the BIOS and the game print their banners, the game's process
dispatcher starts, and the run ends without a fatal stop.
"""
import os, subprocess, sys, tempfile

exe = os.path.abspath(sys.argv[1] if len(sys.argv) > 1 else os.path.join("build", "Release", "carnevil.exe"))
frames = sys.argv[2] if len(sys.argv) > 2 else "300"
nvram = os.path.join(tempfile.mkdtemp(), "nvram.bin")   # start from a blank CMOS, like a new board

run = subprocess.run([exe, "--headless", "--frames", frames, "--nvram", nvram],
                     capture_output=True, text=True, errors="replace", timeout=600)
log = run.stdout + run.stderr

milestones = [
    "Phoenix Seattle R5000 BIOS for Carnevil",   # BIOS running from the boot ROM image
    "Mother GOOSE - R5000",                       # GAME.EXE loaded from disk and started
    "Starting process dispatcher",                # the game's own process loop
]
missing = [m for m in milestones if m not in log]
fatal = [l for l in log.splitlines() if l.startswith("[fatal]")]

for m in milestones:
    print(("ok      " if m not in missing else "MISSING ") + m)
if fatal:
    print("\n".join(fatal))
if missing or fatal or run.returncode != 0:
    sys.exit(f"smoke test failed (exit code {run.returncode})")
print("smoke test passed")
