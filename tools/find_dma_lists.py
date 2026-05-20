"""Scan the MAME RAM dump for Galileo DMA command-list records.
Each record (per seattle_io_write32's DMA handler) is 16 bytes:
  +0: byte_count
  +4: src_phys   (RAM source address)
  +8: dst_pci    (PCI dest: Voodoo regs 0x08xxxxxx or LFB 0x008xxxxx)
  +C: next_ptr   (chain to next record, or 0)
"""
import struct
with open("D:/recomp/mame/attract_sdram.bin","rb") as f: ram = f.read()
def u32(o): return struct.unpack_from("<I", ram, o)[0] if 0 <= o < len(ram)-3 else 0

# A DMA record has a plausible byte_count, a src in RAM, a dst in
# Voodoo/LFB space, and a next that's either 0 or another RAM address.
records = []
for off in range(0, len(ram) - 16, 4):
    bc   = u32(off)
    src  = u32(off + 4)
    dst  = u32(off + 8)
    nxt  = u32(off + 12)
    src_p = src & 0x1FFFFFFF
    dst_voodoo = (0x08000000 <= dst < 0x09000000)
    dst_lfb    = (0x00800000 <= dst < 0x01800000)
    if not (dst_voodoo or dst_lfb): continue
    if bc == 0 or bc > 0x40000: continue
    if src_p == 0 or src_p >= 0x800000: continue
    nxt_p = nxt & 0x1FFFFFFF
    if nxt != 0 and (nxt_p >= 0x800000 or (nxt_p & 3)): continue
    records.append((off, bc, src, dst, nxt))

print(f"Found {len(records)} candidate DMA records")
# Group into chains by following next pointers
starts = []
record_offs = set(r[0] for r in records)
pointed_to = set()
for off, bc, src, dst, nxt in records:
    if nxt:
        pointed_to.add(nxt & 0x1FFFFFFF)
for off, bc, src, dst, nxt in records[:40]:
    tag = "CHAIN-HEAD" if off not in pointed_to else ""
    kind = "VOODOO" if 0x08000000 <= dst < 0x09000000 else "LFB"
    print(f"  @0x{off:06X}: count={bc:7d} src=0x{src:08X} dst=0x{dst:08X} ({kind}) next=0x{nxt:08X} {tag}")
if len(records) > 40:
    print(f"  ... +{len(records)-40} more")
