# -*- coding: utf-8 -*-
# Find function-pointer references to depth-1 callers of func_80167848.
# These are the addresses of render primitives. If their entry points appear
# as words in the data segment, that word is part of a function-pointer
# table (dispatch table). Whoever reads that table is the higher-level
# dispatcher that connects per-frame render code to these primitives.
#
# We also scan code memory for the same word values, which catches lui+addiu
# "address-of" loads even when they bypass the reference manager.
#
# @category CarnEvil
# @runtime Jython

import os

# 34 depth-1 callers of func_80167848 from the xref walk
DEPTH1 = [
    # game render primitives (0x800Dxxxx)
    0x800D1408, 0x800D16F0, 0x800D1A44, 0x800D1E2C,
    0x800D2074, 0x800D231C, 0x800D2654, 0x800D2A8C,
    0x800D2CA8, 0x800D2F24, 0x800D31E0,
    # RTOS-side (0x8015Exxx / 0x8015Fxxx / 0x80167xxx)
    0x8015E2F4, 0x8015E3E4, 0x8015E658, 0x8015E70C,
    0x8015EC70, 0x8015ED34, 0x8015EE64, 0x8015F178,
    0x8015F264, 0x8015F308, 0x8015F3C4, 0x8015F498,
    0x8015F558, 0x8015F7F8, 0x8015F8D4, 0x8015F9A8,
    0x8015FA3C, 0x80167D7C,
]

TARGET_SET = set(DEPTH1)
OUTPATH = "D:/recomp/arcade/carnevil/ghidra_fnptr_scan.log"

fm = currentProgram.getFunctionManager()
af = currentProgram.getAddressFactory()
mem = currentProgram.getMemory()
listing = currentProgram.getListing()

fout = open(OUTPATH, "w")
def w(s):
    println(s)
    fout.write(s + "\n")

w("=" * 78)
w("Scanning memory for occurrences of depth-1 caller addresses")
w("Targets: %d distinct addresses" % len(TARGET_SET))
w("=" * 78)

# Iterate every initialized memory block, scan 4-byte aligned words.
# Skip the block(s) containing the caller functions themselves to avoid noise.
SKIP_RANGES = []  # could blacklist code-only ranges if too noisy

hits = []  # (where, target, kind)
for block in mem.getBlocks():
    if not block.isInitialized():
        continue
    start = block.getStart().getOffset()
    end   = block.getEnd().getOffset()
    name  = block.getName()
    size  = end - start + 1
    w("")
    w("Block %s [0x%08X..0x%08X] %d bytes" % (name, start, end, size))
    a = block.getStart()
    # Iterate in 4-byte words
    addr = a
    count = 0
    while addr.getOffset() <= end - 3:
        try:
            v = mem.getInt(addr) & 0xFFFFFFFF
            if v in TARGET_SET:
                # Skip if this address is inside a known instruction's operand
                # (we want to catch BOTH data tables and code immediates here).
                # Classify by listing:
                code_unit = listing.getCodeUnitAt(addr)
                kind = "?"
                if code_unit is not None:
                    if code_unit.getMnemonicString() == "??" or len(code_unit.getMnemonicString()) == 0:
                        kind = "data"
                    elif code_unit.getMnemonicString() in ("ddw", "dw", "dword", "addr"):
                        kind = "data"
                    else:
                        kind = "instr(" + code_unit.getMnemonicString() + ")"
                else:
                    kind = "raw"
                hits.append((addr.getOffset(), v, kind))
                count += 1
        except Exception, e:
            pass
        addr = addr.add(4)
    w("  found %d target-word hits in this block" % count)

w("")
w("=" * 78)
w("All hits (%d total)" % len(hits))
w("=" * 78)
# Sort by target then by hit address — makes runs of consecutive table entries obvious
hits.sort(key=lambda h: (h[1], h[0]))

# Group by clusters of consecutive (same target across consecutive addresses)
prev_addr = None
prev_target = None
for (haddr, target, kind) in hits:
    sep = "" if (prev_target == target and prev_addr is not None and haddr - prev_addr <= 16) else "\n"
    w("%s  0x%08X -> 0x%08X  [%s]" % (sep, haddr, target, kind))
    prev_addr = haddr
    prev_target = target

# Also produce a TABLE-DENSITY summary: any 64-byte window containing 4+ target hits
w("")
w("=" * 78)
w("Dense-hit regions (4+ target words within 64 bytes) — likely dispatch tables")
w("=" * 78)
# Sort by hit address
by_addr = sorted([(h[0], h[1]) for h in hits])
i = 0
while i < len(by_addr):
    j = i
    base = by_addr[i][0]
    while j < len(by_addr) and by_addr[j][0] - base < 64:
        j += 1
    span = j - i
    if span >= 4:
        w("")
        w("  CLUSTER @ 0x%08X .. 0x%08X (%d entries):" % (
            by_addr[i][0], by_addr[j-1][0], span))
        for k in range(i, j):
            w("    0x%08X -> 0x%08X" % (by_addr[k][0], by_addr[k][1]))
        i = j
    else:
        i += 1

fout.close()
println("Wrote " + OUTPATH)
