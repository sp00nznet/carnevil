#!/usr/bin/env python3
"""
Midway Seattle Filesystem Extractor (TRAP format)
Extracts files from CarnEvil and other Midway Seattle arcade disk images.

The disk uses the TRAP filesystem format:
- Sector 0: TRAP header with partition table
  - Magic "TRAP" + entry count + entries (start_sector, size_in_sectors, type)
- Entry 1 (type=1): Filesystem metadata partition containing:
  - Superblock at byte 0x600
  - FAT table at byte 0x8800
  - LFN32 directory entries (variable location, 0x126 bytes each)
- Entries 2-16 (type=0): Data partitions (contiguous, no gaps)

Each TRAP entry occupies a contiguous range of sectors.
Entry N+1 starts at Entry N start + Entry N size.
"""

import struct
import os
import sys
from pathlib import Path


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


def find_lfn32_entries(f, max_search=2 * 1024 * 1024):
    """Find all LFN32 directory entries in the image."""
    # LFN32 entries are within the first few MB (entry 1 metadata area)
    f.seek(0)
    data = f.read(max_search)

    entries = []
    idx = 0
    while True:
        idx = data.find(b'LFN32\x00', idx)
        if idx == -1:
            break

        if idx + 0x126 > len(data):
            break

        entry_data = data[idx:idx + 0x126]

        # Parse entry fields
        fmt_version = struct.unpack_from('<H', entry_data, 0x06)[0]
        uid = struct.unpack_from('<I', entry_data, 0x16)[0]
        f1a = struct.unpack_from('<H', entry_data, 0x1A)[0]
        f1c = struct.unpack_from('<H', entry_data, 0x1C)[0]

        # Filename is 14 bytes at offset 0x1E, null-padded
        fname_raw = entry_data[0x1E:0x2C]
        fname = fname_raw.split(b'\x00')[0].decode('ascii', errors='replace')

        checksum = struct.unpack_from('<I', entry_data, 0x122)[0]

        if fname:  # skip empty names
            entries.append({
                'name': fname,
                'fmt_version': fmt_version,
                'uid': uid,
                'f1a': f1a,
                'f1c': f1c,
                'checksum': checksum,
                'dir_offset': idx,
            })

        idx += 1  # advance past this match

    return entries


def identify_trap_entry_content(f, entry):
    """Try to identify what a TRAP entry contains."""
    f.seek(entry['byte_offset'])
    header = f.read(min(256, entry['byte_size']))

    if not header:
        return "empty"

    if header[:2] == b'#=':
        return "djgpp_config"

    # Check for text content
    text_chars = sum(1 for b in header[:64] if 32 <= b < 127 or b in [10, 13, 9])
    if text_chars > 48:
        return "text_config"

    if all(b == 0 for b in header[:32]):
        return "empty_or_uninitialized"

    return "binary_data"


def compute_file_sizes(dir_entries):
    """Compute file sizes from consecutive f1a offsets within each f1c group."""
    # Group entries by (f1a, f1c) - entries sharing these are aliases
    unique_blocks = {}
    for entry in dir_entries:
        key = (entry['f1c'], entry['f1a'])
        if key not in unique_blocks:
            unique_blocks[key] = []
        unique_blocks[key].append(entry['name'])

    # For each f1c value, sort unique f1a values and compute sizes from gaps
    by_f1c = {}
    for (f1c, f1a), names in unique_blocks.items():
        if f1c not in by_f1c:
            by_f1c[f1c] = []
        by_f1c[f1c].append((f1a, names))

    file_sizes = {}  # (f1c, f1a) -> size_in_sectors
    for f1c, items in by_f1c.items():
        sorted_items = sorted(items, key=lambda x: x[0])
        for i in range(len(sorted_items)):
            f1a = sorted_items[i][0]
            names = sorted_items[i][1]
            if i + 1 < len(sorted_items):
                next_f1a = sorted_items[i + 1][0]
                size_sectors = next_f1a - f1a
            else:
                # Last file in this group - use a reasonable default
                # Based on observed patterns, use 256 sectors (128KB)
                size_sectors = 256

            file_sizes[(f1c, f1a)] = size_sectors

    return file_sizes


def extract_trap_entries(img_path, output_dir):
    """Extract all TRAP partition entries as raw files."""
    with open(img_path, 'rb') as f:
        trap_entries = read_trap_header(f)

        trap_dir = os.path.join(output_dir, 'trap_entries')
        os.makedirs(trap_dir, exist_ok=True)

        print(f"\n{'='*70}")
        print(f"TRAP Partition Table ({len(trap_entries)} entries)")
        print(f"{'='*70}")
        print(f"{'Idx':>4} {'Start Sector':>12} {'Size (sectors)':>14} {'Size (bytes)':>14} {'Type':>5} {'Content'}")
        print(f"{'-'*4} {'-'*12} {'-'*14} {'-'*14} {'-'*5} {'-'*20}")

        for entry in trap_entries:
            content_type = identify_trap_entry_content(f, entry) if entry['index'] > 0 else "whole_disk"
            size_mb = entry['byte_size'] / 1024 / 1024

            print(f"{entry['index']:4d} {entry['start_sector']:12d} {entry['size_sectors']:14d} {entry['byte_size']:14d} {entry['type']:5d} {content_type} ({size_mb:.1f}MB)")

            # Extract entries 2-16 (skip entry 0=whole disk, entry 1=metadata)
            if entry['index'] >= 2:
                out_name = f"trap_entry_{entry['index']:02d}.bin"
                out_path = os.path.join(trap_dir, out_name)

                f.seek(entry['byte_offset'])
                data = f.read(entry['byte_size'])
                with open(out_path, 'wb') as out:
                    out.write(data)

        return trap_entries


def extract_files(img_path, output_dir, trap_entries):
    """Extract individual game files from the directory."""
    with open(img_path, 'rb') as f:
        dir_entries = find_lfn32_entries(f)

        print(f"\n{'='*70}")
        print(f"LFN32 Directory ({len(dir_entries)} entries)")
        print(f"{'='*70}")

        # Compute file sizes
        file_sizes = compute_file_sizes(dir_entries)

        # Group by file extension
        by_ext = {}
        for entry in dir_entries:
            name = entry['name']
            ext = name.rsplit('.', 1)[-1] if '.' in name else 'NOEXT'
            if ext not in by_ext:
                by_ext[ext] = []
            by_ext[ext].append(entry)

        print(f"\nFile types found:")
        for ext in sorted(by_ext.keys()):
            print(f"  .{ext}: {len(by_ext[ext])} files")

        # Build map of TRAP entry data regions
        # Each TRAP entry (2-16) is a contiguous data region
        data_regions = []
        for te in trap_entries[2:]:  # skip entry 0 (whole disk) and entry 1 (metadata)
            data_regions.append({
                'start_byte': te['byte_offset'],
                'size_bytes': te['byte_size'],
                'start_sector': te['start_sector'],
                'size_sectors': te['size_sectors'],
                'entry_index': te['index'],
            })

        # Print directory listing
        print(f"\n{'Name':<16} {'f1c':>5} {'f1a':>8} {'Size (est)':>12} {'Format':>8}")
        print(f"{'-'*16} {'-'*5} {'-'*8} {'-'*12} {'-'*8}")

        # Track unique data blocks to avoid extracting duplicates
        extracted_blocks = set()
        files_dir = os.path.join(output_dir, 'files')
        os.makedirs(files_dir, exist_ok=True)

        extracted_count = 0
        skipped_count = 0

        for entry in sorted(dir_entries, key=lambda e: (e['f1c'], e['f1a'], e['name'])):
            key = (entry['f1c'], entry['f1a'])
            size_sectors = file_sizes.get(key, 128)
            size_bytes = size_sectors * 512

            print(f"{entry['name']:<16} {entry['f1c']:5d} {entry['f1a']:8d} {size_bytes:12d} {entry['fmt_version']:>#8x}")

            # Try to extract the file data
            # The f1c field selects a TRAP data partition, f1a is sector offset within it
            # We try mapping f1c -> TRAP entry index

            if key in extracted_blocks:
                # Create a 0-byte marker file for aliases
                alias_path = os.path.join(files_dir, entry['name'])
                if not os.path.exists(alias_path):
                    # Find the primary file this aliases to
                    primary_names = [e['name'] for e in dir_entries
                                    if e['f1c'] == entry['f1c'] and e['f1a'] == entry['f1a']]
                    if primary_names:
                        # Create the file pointing to same data
                        try:
                            _extract_file_data(f, entry, trap_entries, file_sizes, files_dir)
                            extracted_count += 1
                        except Exception:
                            skipped_count += 1
                continue

            extracted_blocks.add(key)

            try:
                _extract_file_data(f, entry, trap_entries, file_sizes, files_dir)
                extracted_count += 1
            except Exception as e:
                skipped_count += 1

        print(f"\n{extracted_count} files extracted, {skipped_count} skipped")
        return dir_entries


def _extract_file_data(f, entry, trap_entries, file_sizes, output_dir):
    """Extract a single file's data from the image."""
    key = (entry['f1c'], entry['f1a'])
    size_sectors = file_sizes.get(key, 128)
    size_bytes = size_sectors * 512

    # Strategy: try multiple offset calculations
    # The data lives within entry 1 (the large metadata+data partition)
    entry1 = trap_entries[1]
    entry1_start = entry1['byte_offset']  # byte offset of entry 1

    # Try: entry 1 data area offset
    # Based on analysis, data_start appears to be around 0x2F400
    # (sector 0x17A, which is entry 1's size when treated as bytes / 512)
    data_start_candidates = [
        0x2F400,  # observed working offset for f1c=0 files
    ]

    best_data = None
    for data_start in data_start_candidates:
        offset = data_start + entry['f1a'] * 512
        file_size = f.seek(0, 2)  # get file size
        if offset + size_bytes <= file_size:
            f.seek(offset)
            data = f.read(size_bytes)
            # Check if data is non-zero (valid)
            if any(b != 0 for b in data[:min(64, len(data))]):
                best_data = data
                break

    if best_data is None:
        # Fallback: try direct sector addressing within each TRAP data entry
        for te_idx in range(2, len(trap_entries)):
            te = trap_entries[te_idx]
            offset = te['byte_offset'] + entry['f1a'] * 512
            file_size = f.seek(0, 2)
            if offset + size_bytes <= file_size and offset >= te['byte_offset']:
                f.seek(offset)
                data = f.read(size_bytes)
                if any(b != 0 for b in data[:min(64, len(data))]):
                    best_data = data
                    break

    if best_data is None:
        # Last resort: use the computed offset even if data looks empty
        offset = 0x2F400 + entry['f1a'] * 512
        f.seek(0, 2)
        total = f.tell()
        if offset < total:
            f.seek(offset)
            best_data = f.read(size_bytes)
        else:
            best_data = b'\x00' * size_bytes

    out_path = os.path.join(output_dir, entry['name'])
    with open(out_path, 'wb') as out:
        out.write(best_data)


def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <image_file> [output_dir]")
        print(f"  Extracts files from a Midway Seattle TRAP filesystem disk image")
        sys.exit(1)

    img_path = sys.argv[1]
    output_dir = sys.argv[2] if len(sys.argv) > 2 else os.path.join(os.path.dirname(img_path), '..', 'extracted')
    output_dir = os.path.abspath(output_dir)

    if not os.path.exists(img_path):
        print(f"Error: Image file not found: {img_path}")
        sys.exit(1)

    os.makedirs(output_dir, exist_ok=True)

    print(f"Midway Seattle TRAP Filesystem Extractor")
    print(f"Image: {img_path}")
    print(f"Output: {output_dir}")

    file_size = os.path.getsize(img_path)
    print(f"Image size: {file_size:,} bytes ({file_size/1024/1024/1024:.2f} GB)")

    # Step 1: Extract TRAP partition entries
    trap_entries = extract_trap_entries(img_path, output_dir)

    # Step 2: Extract individual files from directory
    dir_entries = extract_files(img_path, output_dir, trap_entries)

    # Step 3: Write file listing
    listing_path = os.path.join(output_dir, 'file_listing.txt')
    with open(listing_path, 'w') as lst:
        lst.write(f"Midway Seattle TRAP Filesystem - File Listing\n")
        lst.write(f"Image: {img_path}\n")
        lst.write(f"{'='*70}\n\n")

        lst.write(f"TRAP Partition Table:\n")
        for te in trap_entries:
            lst.write(f"  Entry {te['index']:2d}: sector {te['start_sector']:8d}, "
                      f"size {te['size_sectors']:10d} sectors ({te['byte_size']:,} bytes)\n")

        lst.write(f"\nDirectory Entries ({len(dir_entries)} files):\n")
        for entry in sorted(dir_entries, key=lambda e: e['name']):
            lst.write(f"  {entry['name']:<16s} f1c={entry['f1c']:3d} f1a={entry['f1a']:6d} "
                      f"fmt=0x{entry['fmt_version']:04x} uid=0x{entry['uid']:08x}\n")

    print(f"\nFile listing written to: {listing_path}")
    print(f"Done!")


if __name__ == '__main__':
    main()
