"""Find addresses in the MAME RAM dump that point to the scene-graph
node region. Whichever address holds a 0x803F7xxx value with the right
shape is likely the global head pointer."""
import struct
with open("D:/recomp/mame/attract_sdram.bin","rb") as f: ram = f.read()

# All 32-bit values pointing into the heap region where we found scene
# node candidates (0x803F7000 - 0x803F8000)
hits = []
for off in range(0, len(ram) - 3, 4):
    w = struct.unpack_from("<I", ram, off)[0]
    if 0x803F7000 <= w < 0x803F8000:
        hits.append((off, w))

print(f"Found {len(hits)} references into 0x803F7xxx region")
print()

# Group by destination
by_dst = {}
for off, w in hits:
    by_dst.setdefault(w, []).append(off)

# Show top destinations by reference count
for dst, refs in sorted(by_dst.items(), key=lambda x: -len(x[1]))[:15]:
    print(f"  -> 0x{dst:08X} from {len(refs)} locations")
    for r in refs[:5]:
        print(f"     ref @0x{r:06X}")

# Also dump all references to 0x803F75A0 (the very first SG candidate from earlier)
print()
print("References to first candidate 0x803F75A0:")
for off, w in hits:
    if w == 0x803F75A0:
        print(f"  ref @0x{off:06X}")
