"""Search MAME RAM dump for scene-graph-like patterns: linked lists where
the 'next' field at +0x44 points to another valid struct."""
import struct
with open("D:/recomp/mame/attract_sdram.bin","rb") as f: ram = f.read()

def u32(o):
    if 0 <= o < len(ram)-3:
        return struct.unpack_from("<I", ram, o)[0]
    return 0

# Find function-pointer-rich regions and look for chains
# A scene graph node would have:
#   [+0]: type id (small int or address)
#   [+44]: pointer to next node (in the same ram region)
#   [+4C]: geometry/handler pointer (often 0x80xxxxxx)

# Look for chains: any address X where:
#   u32(X+0x44) is a plausible "next" pointer (X+something, or a different chunk)
#   AND u32(X+0x4C) looks like an 0x80xxxxxx address
chains = []
for off in range(0x230000, 0x800000, 4):  # heap region
    nx_ptr = u32(off + 0x44)
    geom = u32(off + 0x4C)
    if nx_ptr == 0:
        continue
    nx_phys = nx_ptr & 0x1FFFFFFF
    if nx_phys < 0x230000 or nx_phys >= 0x800000:
        continue  # not pointing into heap
    if (nx_phys & 3) != 0:
        continue  # not aligned
    if (geom & 0xFF000000) != 0x80000000:
        continue  # geom doesn't look like a virtual addr
    chains.append((off, nx_ptr, geom))

print(f"Found {len(chains)} candidate scene-node patterns")
for off, nx, geom in chains[:30]:
    head_val = u32(off)
    flag48 = u32(off + 0x48)
    print(f"  @0x{off:06X}: [00]=0x{head_val:08X} [+44=next]=0x{nx:08X} [+48]=0x{flag48:08X} [+4C=geom]=0x{geom:08X}")

# Walk one of these chains
if chains:
    print()
    print("Walk chain starting from first candidate:")
    head_off = chains[0][0]
    cur = 0x80000000 | head_off
    seen = set()
    depth = 0
    while cur and depth < 50:
        phys = cur & 0x1FFFFFFF
        if phys in seen or phys + 0x60 >= len(ram):
            break
        seen.add(phys)
        type_id = u32(phys + 0)
        nx = u32(phys + 0x44)
        geom = u32(phys + 0x4C)
        print(f"  [{depth}] @0x{cur:08X}: type=0x{type_id:08X} next=0x{nx:08X} geom=0x{geom:08X}")
        cur = nx
        depth += 1
    print(f"  chain length: {depth}")
