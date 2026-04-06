#!/usr/bin/env python3
"""
Midway Seattle Filesystem Extractor (TRAP + Phoenix format)
Extracts files from CarnEvil and other Midway Seattle arcade disk images.

Disk layout:
- Sector 0: TRAP header with partition table
  - Magic "TRAP" + entry count + entries (start_sector, size_in_sectors, type)
  - Entry 0 spans the entire disk
  - Entry 1 (type=1): Filesystem metadata
  - Entries 2-16 (type=0): Data partitions

Phoenix filesystem (within the whole disk, entry 0):
- Base offset: 0x11400 (70656 bytes from disk start)
- Block size: 4096 bytes
- Directory entries: 24 bytes each (12 byte-swapped name + 4 size + 4 UID + 4 block ptr)
- Directory entries are scattered across the metadata partition, found via pattern scan
- File data: BASE + block_ptr * BLOCK_SIZE, size = size_field * 4 bytes
- Files stored contiguously, each occupying ceil(size/4096) blocks

EXE file format (.EXE files like GAME.EXE, DIAG.EXE):
- 0x000-0x1FF: 512-byte boot thumbnail (RGB pixel data)
- 0x200-0x203: Unknown/padding
- 0x204-0x207: MIPS load address (LE uint32)
- 0x208+: MIPS-IV LE flat binary code

LFN32 directory entries (secondary, used by FILESYS.CHK for verification):
- Located within the metadata area (searched by "LFN32\\x00" magic)
- 0x126 bytes each, contain filename, UID, checksum
"""

import struct
import os
import sys


# Phoenix filesystem constants
BASE_OFFSET = 0x11400       # Data area starts here
BLOCK_SIZE = 4096            # Bytes per block
DIR_ENTRY_SIZE = 24          # Bytes per directory entry
EXE_HEADER_SIZE = 0x208      # Header size in .EXE files
EXE_LOADADDR_OFFSET = 0x204  # Load address location in .EXE header
METADATA_SCAN_SIZE = 60 * 1024 * 1024  # Scan first 60MB for directory entries


def read_trap_header(f):
    """Read and parse the TRAP partition table header."""
    f.seek(0)
    header = f.read(0x200)

    magic = header[0:4]
    if magic != b'TRAP':
        raise ValueError(f"Not a TRAP filesystem (magic: {magic})")

    num_entries = struct.unpack_from('<H', header, 4)[0]

    entries = []
    for i in range(num_entries):
        off = 8 + i * 12
        start_sector, size_sectors, entry_type = struct.unpack_from('<III', header, off)
        entries.append({
            'index': i,
            'start_sector': start_sector,
            'size_sectors': size_sectors,
            'type': entry_type,
            'byte_offset': start_sector * 512,
            'byte_size': size_sectors * 512,
        })

    return entries


def byteswap_filename(name_raw):
    """Byte-swap a 12-byte filename in 4-byte groups [3,2,1,0,7,6,5,4,11,10,9,8]."""
    swapped = bytearray(12)
    for group in range(3):
        for j in range(4):
            swapped[group * 4 + j] = name_raw[group * 4 + (3 - j)]
    return swapped.rstrip(b'\x00')


def read_directory(f):
    """Read all directory entries by scanning the metadata area for valid entries.

    Directory entries are 24 bytes each with a byte-swapped 12-byte filename,
    but they are scattered across the metadata partition at varying alignments.
    We find them by searching for the '.' character (0x2E) at expected byte
    positions within the byte-swapped filename field.
    """
    f.seek(0)
    meta = f.read(METADATA_SCAN_SIZE)

    # Compute all possible raw byte positions for '.' in the filename
    # Original position P in the filename maps to raw position:
    # group = P // 4, offset = 3 - (P % 4), raw = group*4 + offset
    dot_raw_positions = set()
    for p in range(1, 12):
        group = p // 4
        offset = 3 - (p % 4)
        dot_raw_positions.add(group * 4 + offset)

    entries = {}  # name -> best entry (largest size wins)

    for raw_dot_pos in sorted(dot_raw_positions):
        for i in range(0, len(meta) - DIR_ENTRY_SIZE):
            if meta[i + raw_dot_pos] != 0x2E:
                continue

            entry_bytes = meta[i:i + DIR_ENTRY_SIZE]
            name = byteswap_filename(entry_bytes[:12])

            if len(name) < 4 or not all(32 <= b < 127 for b in name):
                continue
            if name.count(b'.') != 1:
                continue

            name_s = name.decode('ascii')
            parts = name_s.split('.')
            if len(parts) != 2:
                continue
            if not all(c.isalnum() or c == '_' for c in parts[0]):
                continue
            if not all(c.isalnum() for c in parts[1]) or not (2 <= len(parts[1]) <= 4):
                continue

            size_raw = struct.unpack_from('<I', entry_bytes, 12)[0]
            uid = struct.unpack_from('<I', entry_bytes, 16)[0]
            block_ptr = struct.unpack_from('<I', entry_bytes, 20)[0]

            if size_raw == 0 or size_raw > 0x1000000 or block_ptr > 0x100000:
                continue

            actual_size = size_raw * 4

            if name_s not in entries or entries[name_s]['size'] < actual_size:
                entries[name_s] = {
                    'name': name_s,
                    'size': actual_size,
                    'size_raw': size_raw,
                    'uid': uid,
                    'block_ptr': block_ptr,
                    'disk_offset': BASE_OFFSET + block_ptr * BLOCK_SIZE,
                }

    return list(entries.values())


def extract_files(img_path, output_dir):
    """Extract all files from the disk image."""
    with open(img_path, 'rb') as f:
        # Step 1: Read TRAP header
        trap_entries = read_trap_header(f)

        print(f"{'=' * 70}")
        print(f"TRAP Partition Table ({len(trap_entries)} entries)")
        print(f"{'=' * 70}")
        print(f"{'Idx':>4} {'Start Sector':>12} {'Size (sectors)':>14} {'Size':>10} {'Type':>5}")
        print(f"{'-' * 4} {'-' * 12} {'-' * 14} {'-' * 10} {'-' * 5}")

        for entry in trap_entries:
            size_mb = entry['byte_size'] / 1024 / 1024
            print(f"{entry['index']:4d} {entry['start_sector']:12d} "
                  f"{entry['size_sectors']:14d} {size_mb:>8.1f}MB {entry['type']:5d}")

        # Step 2: Read directory
        print(f"\nScanning metadata area for directory entries...")
        dir_entries = read_directory(f)

        print(f"\n{'=' * 70}")
        print(f"Phoenix Directory ({len(dir_entries)} files)")
        print(f"{'=' * 70}")

        # Group by extension
        by_ext = {}
        for entry in dir_entries:
            ext = entry['name'].rsplit('.', 1)[-1] if '.' in entry['name'] else 'NOEXT'
            by_ext.setdefault(ext, []).append(entry)

        print(f"\nFile types:")
        for ext in sorted(by_ext.keys()):
            total_size = sum(e['size'] for e in by_ext[ext])
            print(f"  .{ext:<6s}: {len(by_ext[ext]):>5} files  ({total_size / 1024 / 1024:>8.1f} MB)")

        total_size = sum(e['size'] for e in dir_entries)
        print(f"  {'Total':<8s}: {len(dir_entries):>5} files  ({total_size / 1024 / 1024:>8.1f} MB)")

        # Step 3: Extract files
        files_dir = os.path.join(output_dir, 'files')
        os.makedirs(files_dir, exist_ok=True)

        f.seek(0, 2)
        file_size = f.tell()

        extracted = 0
        skipped = 0

        for entry in sorted(dir_entries, key=lambda e: e['block_ptr']):
            out_path = os.path.join(files_dir, entry['name'])

            if entry['disk_offset'] + entry['size'] > file_size:
                read_size = max(0, file_size - entry['disk_offset'])
            else:
                read_size = entry['size']

            if read_size > 0:
                f.seek(entry['disk_offset'])
                data = f.read(read_size)

                with open(out_path, 'wb') as out:
                    out.write(data)
                extracted += 1
            else:
                skipped += 1

        print(f"\n{extracted} files extracted, {skipped} skipped")
        print(f"Output: {files_dir}")

        # Step 4: Extract MIPS binaries from EXE files
        exe_entries = [e for e in dir_entries if e['name'].upper().endswith('.EXE')]
        if exe_entries:
            print(f"\n{'=' * 70}")
            print(f"MIPS Binary Extraction")
            print(f"{'=' * 70}")

            for entry in exe_entries:
                f.seek(entry['disk_offset'])
                exe_data = f.read(entry['size'])

                if len(exe_data) > EXE_HEADER_SIZE:
                    load_addr = struct.unpack_from('<I', exe_data, EXE_LOADADDR_OFFSET)[0]
                    mips_binary = exe_data[EXE_HEADER_SIZE:]
                    va_end = load_addr + len(mips_binary)

                    bin_name = entry['name'].rsplit('.', 1)[0] + '.bin'
                    bin_path = os.path.join(output_dir, bin_name)

                    with open(bin_path, 'wb') as out:
                        out.write(mips_binary)

                    w0 = struct.unpack_from('<I', mips_binary, 0)[0]
                    has_init = w0 == 0x3c1d8080

                    func_count = 0
                    for i in range(0, len(mips_binary) - 4, 4):
                        w = struct.unpack_from('<I', mips_binary, i)[0]
                        if (w >> 16) == 0x27BD:
                            imm = struct.unpack('<h', struct.pack('<H', w & 0xFFFF))[0]
                            if imm < 0:
                                func_count += 1

                    print(f"\n  {entry['name']} -> {bin_name}")
                    print(f"    Load address:  {load_addr:#010x}")
                    print(f"    VA range:      {load_addr:#010x} - {va_end:#010x}")
                    print(f"    Binary size:   {len(mips_binary):,} bytes ({len(mips_binary) / 1024:.1f} KB)")
                    print(f"    Init code:     {'YES' if has_init else 'NO'}")
                    print(f"    Functions:     ~{func_count}")
                    print(f"    Saved to:      {bin_path}")

        # Step 5: Write file listing
        listing_path = os.path.join(output_dir, 'file_listing.txt')
        with open(listing_path, 'w') as lst:
            lst.write(f"Midway Seattle Phoenix Filesystem - File Listing\n")
            lst.write(f"Image: {img_path}\n")
            lst.write(f"{'=' * 70}\n\n")

            lst.write(f"TRAP Partition Table:\n")
            for te in trap_entries:
                lst.write(f"  Entry {te['index']:2d}: sector {te['start_sector']:8d}, "
                          f"size {te['size_sectors']:10d} sectors ({te['byte_size']:,} bytes)\n")

            lst.write(f"\nPhoenix Directory ({len(dir_entries)} files):\n")
            lst.write(f"  {'Name':<16s} {'Size':>10s} {'Block':>8s} {'UID':>10s}\n")
            lst.write(f"  {'-' * 16} {'-' * 10} {'-' * 8} {'-' * 10}\n")
            for entry in sorted(dir_entries, key=lambda e: e['name']):
                lst.write(f"  {entry['name']:<16s} {entry['size']:>10,} "
                          f"{entry['block_ptr']:>8} {entry['uid']:#010x}\n")

            for entry in exe_entries:
                f.seek(entry['disk_offset'] + EXE_LOADADDR_OFFSET)
                load_addr = struct.unpack('<I', f.read(4))[0]
                mips_size = entry['size'] - EXE_HEADER_SIZE
                lst.write(f"\n{entry['name']} MIPS Binary:\n")
                lst.write(f"  Load address: {load_addr:#010x}\n")
                lst.write(f"  Code size:    {mips_size:,} bytes\n")
                lst.write(f"  VA range:     {load_addr:#010x} - {load_addr + mips_size:#010x}\n")

        print(f"\nFile listing: {listing_path}")
        print(f"Done!")

        return dir_entries


def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <image_file> [output_dir]")
        print(f"  Extracts files from a Midway Seattle TRAP/Phoenix filesystem disk image")
        sys.exit(1)

    img_path = sys.argv[1]
    output_dir = sys.argv[2] if len(sys.argv) > 2 else os.path.join(os.path.dirname(img_path), '..', 'extracted')
    output_dir = os.path.abspath(output_dir)

    if not os.path.exists(img_path):
        print(f"Error: Image file not found: {img_path}")
        sys.exit(1)

    os.makedirs(output_dir, exist_ok=True)

    print(f"Midway Seattle Phoenix Filesystem Extractor")
    print(f"Image: {img_path}")
    print(f"Output: {output_dir}")

    file_size = os.path.getsize(img_path)
    print(f"Image size: {file_size:,} bytes ({file_size / 1024 / 1024 / 1024:.2f} GB)\n")

    extract_files(img_path, output_dir)


if __name__ == '__main__':
    main()
