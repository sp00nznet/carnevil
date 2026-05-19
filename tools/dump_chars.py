"""Characterize the MAME RAM dump: find regions with code-like or pointer-like
data, count distribution of function-pointer-looking values."""
import struct
DUMP = "D:/recomp/mame/attract_sdram.bin"
with open(DUMP, "rb") as f: ram = f.read()
print(f"Size: {len(ram)} bytes")
print()

# Count regions of all-zero vs non-zero
print("Non-zero distribution (by 64KB block):")
for blk in range(0, len(ram), 0x10000):
    nz = sum(1 for b in ram[blk:blk+0x10000] if b != 0)
    if nz > 0:
        pct = 100.0 * nz / 0x10000
        marker = "*" if pct > 5 else " "
        print(f"  {marker} 0x{blk:06X}: {nz:6d} non-zero ({pct:5.1f}%)")

# Count function-pointer-looking values (0x80xxxxxx) per region
print()
print("0x80xxxxxx 32-bit values (function pointer candidates):")
fnptr_count_by_blk = {}
for off in range(0, len(ram) - 3, 4):
    w = struct.unpack_from("<I", ram, off)[0]
    if (w & 0xFFF00000) == 0x80100000 or (w & 0xFFF00000) == 0x80000000 \
       or (w & 0xFFF00000) == 0x800C0000 or (w & 0xFFF00000) == 0x80140000:
        blk = off & ~0xFFFF
        fnptr_count_by_blk[blk] = fnptr_count_by_blk.get(blk, 0) + 1

for blk in sorted(fnptr_count_by_blk):
    cnt = fnptr_count_by_blk[blk]
    if cnt > 5:
        print(f"  0x{blk:06X}: {cnt} fnptr-like values")

# Check what's at 0x00179258 = 5 in our scan — that's display_init flag.
# Compare to a few well-known addresses we set in our runtime
print()
print("Key addresses (compare to our runtime values):")
known = {
    0x00179258: ("display_init", 1),
    0x0022A444: ("render_ptr", "non-zero"),
    0x001AA660: ("voodoo_base", 0x08100000),
    0x002122D4: ("display_mode", "0x40 or 2"),
    0x001DDDE0: ("DCS_flags", "with 0x4"),
    0x001DDD80: ("vbl_sync", 2),
    0x001E6504: ("pic_serial", 486),
    0x001A35CC: ("vblank_counter", "varies"),
}
for addr, (name, expected) in known.items():
    val = struct.unpack_from("<I", ram, addr)[0]
    print(f"  [0x{addr:06X}] {name:20s} = 0x{val:08X} (expected: {expected})")
