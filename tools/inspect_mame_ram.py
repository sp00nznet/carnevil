"""Read MAME's 8MB RAM dump (attract_sdram.bin) and inspect the live state
during 3D attract gameplay. Goal: see what the entity-dispatch structures
look like when the game is ACTUALLY rendering 3D scenes."""
import struct, sys

DUMP = "D:/recomp/mame/attract_sdram.bin"
with open(DUMP, "rb") as f:
    ram = f.read()
print(f"Loaded {len(ram)} bytes from {DUMP}")
print()

def u32(off):
    return struct.unpack_from("<I", ram, off)[0]

# Scene graph head: 0x0017B71C (after root struct at 0x0017B6D8 + 0x44)
print("=" * 70)
print("Scene graph at 0x0017B71C")
print("=" * 70)
head = u32(0x0017B71C)
print(f"  head = 0x{head:08X}")
cur = head
count = 0
while cur != 0 and count < 30:
    phys = cur & 0x1FFFFFFF
    if phys + 0x60 >= len(ram):
        print(f"  node {count} ptr 0x{cur:08X} out of range")
        break
    next_ptr = u32(phys + 0x44)
    type_id = u32(phys + 0x00)
    geom = u32(phys + 0x4C)
    flag = u32(phys + 0x48)
    print(f"  node {count} @0x{cur:08X}: [00]=0x{type_id:08X} [44]=0x{next_ptr:08X} "
          f"[48]=0x{flag:08X} [4C]=0x{geom:08X}")
    cur = next_ptr
    count += 1
print(f"  Total nodes: {count}")

# Display channel state
print()
print("=" * 70)
print("Display channel state")
print("=" * 70)
for ch in range(2):
    base = 0x001E0440 + ch * 0x118
    state = u32(base + 0x50)
    flag4 = u32(base + 0x04)
    flag5c = u32(base + 0x5C)
    print(f"  ch{ch} @0x{base:06X}: [+4]=0x{flag4:08X} [+50]=0x{state:08X} [+5C]=0x{flag5c:08X}")

# Render context
print()
print("=" * 70)
print("Render context @0x0022A444 etc")
print("=" * 70)
for addr in [0x22A444, 0x1AA660, 0x179258, 0x2122D4, 0x1DDDE0, 0x1DDD80]:
    v = u32(addr)
    print(f"  [0x{addr:06X}] = 0x{v:08X}")

# Look up function pointers — the 14 entity-draw functions. Scan the entire
# RAM for any 32-bit word matching them and report locations.
print()
print("=" * 70)
print("Scan RAM for entity-draw function pointer references")
print("=" * 70)
targets = {0x800ED458, 0x801014C8, 0x80102268, 0x80102A10,
           0x80103120, 0x80103848, 0x80103FD0, 0x80104780,
           0x801079F0, 0x801098AC, 0x8010D3D0, 0x8010D4FC,
           0x8012CFF8, 0x8013934C, 0x800D7600}
hits = []
for off in range(0, len(ram) - 3, 4):
    w = u32(off)
    if w in targets:
        hits.append((off, w))
print(f"Total hits: {len(hits)}")
for off, w in hits[:30]:
    # Print some context
    ctx_addrs = [off-4, off, off+4, off+8]
    ctx_vals = [u32(a) if 0 <= a < len(ram)-3 else 0 for a in ctx_addrs]
    print(f"  [0x{off:06X}] = 0x{w:08X}  (ctx: 0x{ctx_vals[0]:08X} >0x{ctx_vals[1]:08X}< 0x{ctx_vals[2]:08X} 0x{ctx_vals[3]:08X})")
if len(hits) > 30:
    print(f"  ... +{len(hits)-30} more")
