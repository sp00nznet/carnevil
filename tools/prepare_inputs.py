"""Build the recompiler inputs from your own CarnEvil boot ROM and hard disk.

usage: prepare_inputs.py carnevil1_9.u32 carnevi1.img

Writes extracted/RTOS_padded.bin and extracted/GAME_padded.bin, the images
rtos_recomp.toml and carnevil_recomp.toml point at (each has 0x1000 bytes of
padding in front, where the symbol files put the code):
  - RTOS: the BIOS image the boot ROM copies to 0x80000000 on a warm reset,
    ROM offset 0xF20..0x16B60.
  - GAME: GAME.EXE from the disk's Phoenix filesystem: load address at
    +0x204, code from +0x208.
The symbol files describe v1.0.1 (MAME's carnevil1, disk carnevi1). Other
revisions produce different images and the script refuses them.
"""
import hashlib, os, struct, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import seattle_fs

EXPECTED = {
    "RTOS_padded.bin": "90f280d2de3a4db0434701ef7ad83333747ce53d",
    "GAME_padded.bin": "63ac9a5584662168bac1a7094b6fbada8b60793d",
}
GAME_CODE_SIZE = 0xDF504   # the .text size in carnevil_syms.toml
PAD = b"\0" * 0x1000


def main():
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    rom_path, disk_path = sys.argv[1:]
    rom = open(rom_path, "rb").read()
    if len(rom) != 0x80000:
        sys.exit(f"{rom_path}: expected a 512 KB boot ROM, got {len(rom)} bytes")
    rtos = rom[0xF20:0x16B60]

    with open(disk_path, "rb") as f:
        game = [e for e in seattle_fs.read_directory(f) if e["name"] == "GAME.EXE"]
        if not game:
            sys.exit(f"{disk_path}: no GAME.EXE in the filesystem directory")
        f.seek(game[0]["disk_offset"])
        exe = f.read(0x208 + GAME_CODE_SIZE)
    load = struct.unpack_from("<I", exe, 0x204)[0]
    if load != 0x800C4000:
        sys.exit(f"GAME.EXE loads at 0x{load:08X}, expected 0x800C4000")

    os.makedirs("extracted", exist_ok=True)
    ok = True
    for name, data in (("RTOS_padded.bin", PAD + rtos), ("GAME_padded.bin", PAD + exe[0x208:])):
        digest = hashlib.sha1(data).hexdigest()
        if digest != EXPECTED[name]:
            print(f"{name}: SHA-1 {digest} does not match v1.0.1 ({EXPECTED[name]})")
            ok = False
            continue
        open(os.path.join("extracted", name), "wb").write(data)
        print(f"wrote extracted/{name}")
    if not ok:
        sys.exit("these do not look like CarnEvil v1.0.1 dumps; nothing written for the mismatches")


if __name__ == "__main__":
    main()
