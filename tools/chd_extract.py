#!/usr/bin/env python3
"""
CHD (MAME Compressed Hunks of Data) v5 extractor.
Extracts raw disk image data from CHD files.

Usage:
    python chd_extract.py <input.chd> [output.img]
"""

import struct
import sys
import os
import zlib
import lzma
import time
import io

try:
    import soundfile as sf
    import numpy as np
    HAS_SOUNDFILE = True
except ImportError:
    HAS_SOUNDFILE = False


# ============================================================================
# CHD v5 compression type constants (used in the map)
# ============================================================================
COMPRESSION_TYPE_0 = 0
COMPRESSION_TYPE_1 = 1
COMPRESSION_TYPE_2 = 2
COMPRESSION_TYPE_3 = 3
COMPRESSION_NONE = 4
COMPRESSION_SELF = 5
COMPRESSION_PARENT = 6
COMPRESSION_RLE_SMALL = 7
COMPRESSION_RLE_LARGE = 8
COMPRESSION_SELF_0 = 9
COMPRESSION_SELF_1 = 10
COMPRESSION_PARENT_SELF = 11
COMPRESSION_PARENT_0 = 12
COMPRESSION_PARENT_1 = 13

CHD_MAGIC = b"MComprHD"

COMP_NAMES = {
    0: "codec0", 1: "codec1", 2: "codec2", 3: "codec3",
    4: "none", 5: "self", 6: "parent", 7: "rle_small",
    8: "rle_large", 9: "self_0", 10: "self_1",
    11: "parent_self", 12: "parent_0", 13: "parent_1"
}


# ============================================================================
# Bitstream reader (MSB-first, matching MAME's bitstream_in)
# ============================================================================
class BitReader:
    def __init__(self, data):
        self.data = data
        self.pos = 0  # bit position

    def read(self, n):
        """Read n bits MSB-first and return as unsigned integer."""
        val = 0
        for _ in range(n):
            byte_idx = self.pos >> 3
            bit_idx = 7 - (self.pos & 7)
            if byte_idx < len(self.data):
                val = (val << 1) | ((self.data[byte_idx] >> bit_idx) & 1)
            else:
                val = val << 1
            self.pos += 1
        return val

    def peek(self, n):
        """Peek at n bits without advancing."""
        saved = self.pos
        val = self.read(n)
        self.pos = saved
        return val

    def remove(self, n):
        """Skip n bits."""
        self.pos += n


# ============================================================================
# Huffman decoder (matching MAME's huffman_decoder<16, 8>)
# ============================================================================
class HuffmanDecoder:
    def __init__(self, numcodes, maxbits):
        self.numcodes = numcodes
        self.maxbits = maxbits
        self.numbits = [0] * numcodes  # code lengths per symbol
        self.bits = [0] * numcodes     # canonical codes per symbol
        self.lookup = [0] * (1 << maxbits)  # lookup table

    def import_tree_rle(self, bitbuf):
        """Import Huffman tree from RLE-encoded bitstream (MAME format)."""
        # Determine bits per entry based on maxbits
        if self.maxbits >= 16:
            numbits = 5
        elif self.maxbits >= 8:
            numbits = 4
        else:
            numbits = 3

        curnode = 0
        while curnode < self.numcodes:
            nodebits = bitbuf.read(numbits)
            if nodebits != 1:
                # Raw code length
                self.numbits[curnode] = nodebits
                curnode += 1
            else:
                # Escape: read next value
                nodebits = bitbuf.read(numbits)
                if nodebits == 1:
                    # Double 1 = literal 1
                    self.numbits[curnode] = 1
                    curnode += 1
                else:
                    # RLE: next value is repeat count + 3
                    repcount = bitbuf.read(numbits) + 3
                    for _ in range(repcount):
                        if curnode < self.numcodes:
                            self.numbits[curnode] = nodebits
                            curnode += 1

        self._assign_canonical_codes()
        self._build_lookup_table()

    def _assign_canonical_codes(self):
        """Assign canonical Huffman codes based on code lengths."""
        # Build histogram of bit lengths
        bithisto = [0] * 33
        for i in range(self.numcodes):
            if self.numbits[i] <= 32:
                bithisto[self.numbits[i]] += 1

        # Calculate starting code for each length (working from longest to shortest)
        curstart = 0
        for codelen in range(32, 0, -1):
            nextstart = (curstart + bithisto[codelen]) >> 1
            bithisto[codelen] = curstart
            curstart = nextstart

        # Assign codes
        for i in range(self.numcodes):
            if self.numbits[i] > 0:
                self.bits[i] = bithisto[self.numbits[i]]
                bithisto[self.numbits[i]] += 1

    def _build_lookup_table(self):
        """Build fast lookup table for decoding."""
        for i in range(len(self.lookup)):
            self.lookup[i] = 0

        for i in range(self.numcodes):
            if self.numbits[i] > 0:
                code = self.bits[i]
                codelen = self.numbits[i]
                # Fill all table entries that match this prefix
                shift = self.maxbits - codelen
                for j in range(1 << shift):
                    idx = (code << shift) | j
                    # Pack: symbol in upper bits, length in lower 5 bits
                    self.lookup[idx] = (i << 5) | codelen

    def import_tree_huffman(self, bitbuf):
        """Import Huffman tree encoded with a secondary Huffman tree (MAME format).
        Used by huffman_8bit_decoder::decode() for hunk data compression."""
        # Parse the small tree (24 codes, max 6 bits)
        smallhuff = HuffmanDecoder(24, 6)
        smallhuff.numbits[0] = bitbuf.read(3)
        start = bitbuf.read(3) + 1
        count = 0
        for index in range(1, 24):
            if index < start or count == 7:
                smallhuff.numbits[index] = 0
            else:
                count = bitbuf.read(3)
                smallhuff.numbits[index] = 0 if count == 7 else count

        smallhuff._assign_canonical_codes()
        smallhuff._build_lookup_table()

        # Determine max RLE count bits
        temp = self.numcodes - 9
        rlefullbits = 0
        while temp != 0:
            temp >>= 1
            rlefullbits += 1

        # Decode the main tree using the small tree
        last = 0
        curcode = 0
        while curcode < self.numcodes:
            value = smallhuff.decode_one(bitbuf)
            if value != 0:
                last = value - 1
                self.numbits[curcode] = last
                curcode += 1
            else:
                rle_count = bitbuf.read(3) + 2
                if rle_count == 7 + 2:  # 9
                    rle_count += bitbuf.read(rlefullbits)
                for _ in range(rle_count):
                    if curcode < self.numcodes:
                        self.numbits[curcode] = last
                        curcode += 1

        self._assign_canonical_codes()
        self._build_lookup_table()

    def decode_one(self, bitbuf):
        """Decode one symbol from the bitstream."""
        bits = bitbuf.peek(self.maxbits)
        entry = self.lookup[bits]
        length = entry & 0x1f
        symbol = entry >> 5
        bitbuf.remove(length)
        return symbol


# ============================================================================
# 8-bit Huffman decoder for hunk data (huffman_8bit_decoder in MAME)
# ============================================================================
class Huffman8BitDecoder:
    """Decoder for huffman-compressed hunk data (256 symbols, up to 16 bits)."""

    def __init__(self):
        self.decoder = HuffmanDecoder(256, 16)

    def decompress(self, src, destlen):
        """Decompress huffman-encoded data."""
        bitbuf = BitReader(src)
        self.decoder.import_tree_huffman(bitbuf)

        result = bytearray(destlen)
        for i in range(destlen):
            result[i] = self.decoder.decode_one(bitbuf)
        return bytes(result)


# ============================================================================
# LZMA properties helper
# ============================================================================
def get_lzma_props(hunkbytes):
    """
    Reconstruct LZMA decoder properties matching MAME's configure_properties.
    MAME uses: level=8, reduceSize=hunkbytes, then LzmaEncProps_Normalize.
    Default lc=3, lp=0, pb=2. DictSize is normalized based on reduceSize.
    """
    lc, lp, pb = 3, 0, 2

    # LzmaEncProps_Normalize dictSize logic for level 8:
    # Base dictSize for level 8 = 1<<25 = 33554432
    # But capped by reduceSize via the normalization loop
    base_dict = 1 << 25

    if hunkbytes < base_dict:
        # Find smallest dict >= reduceSize from the series: 2<<i, 3<<i for i=11..30
        dictsize = base_dict
        for i in range(11, 31):
            if hunkbytes <= (2 << i):
                dictsize = (2 << i)
                break
            if hunkbytes <= (3 << i):
                dictsize = (3 << i)
                break
        # Don't exceed the level-based dict
        dictsize = min(dictsize, base_dict)
    else:
        dictsize = base_dict

    # Encode as 5-byte LZMA properties header
    prop_byte = lc + lp * 9 + pb * 9 * 5
    props = bytes([prop_byte]) + struct.pack('<I', dictsize)
    return props


def decompress_lzma_hunk(data, hunkbytes):
    """Decompress an LZMA-compressed hunk (no embedded props header)."""
    props = get_lzma_props(hunkbytes)
    # Build a proper LZMA alone stream:
    # 5 bytes props + 8 bytes uncompressed size + compressed data
    size_bytes = struct.pack('<Q', hunkbytes)
    lzma_stream = props + size_bytes + data
    return lzma.decompress(lzma_stream, format=lzma.FORMAT_ALONE)


def decompress_zlib_hunk(data, hunkbytes):
    """Decompress a raw-deflate compressed hunk."""
    return zlib.decompress(data, -15)


def decompress_flac_hunk(data, hunkbytes):
    """
    Decompress a FLAC-compressed hunk.
    Format: 1-byte endianness marker ('L' or 'B') + FLAC frame data.
    The disk data is treated as interleaved 16-bit stereo PCM at 44100 Hz.
    """
    if not HAS_SOUNDFILE:
        raise RuntimeError(
            "FLAC decompression requires 'soundfile' and 'numpy' packages. "
            "Install with: pip install soundfile numpy")

    endian_marker = chr(data[0])
    flac_frame = data[1:]

    num_samples = hunkbytes // 4  # stereo 16-bit = 4 bytes per sample pair

    # Compute blocksize the same way MAME does
    blocksize = num_samples
    while blocksize > 2048:
        blocksize //= 2

    # Build a minimal FLAC stream: fLaC + STREAMINFO + frame data
    si = bytearray(34)
    struct.pack_into('>HH', si, 0, blocksize, blocksize)
    si[4:10] = b'\x00' * 6  # min/max frame size = unknown
    # Sample rate (20) | channels-1 (3) | bps-1 (5) | total samples (36)
    val = (44100 << 44) | (1 << 41) | (15 << 36) | num_samples
    si[10:18] = val.to_bytes(8, 'big')
    si[18:34] = b'\x00' * 16  # MD5

    meta_header = struct.pack('>I', (1 << 31) | 34)  # last block, type=0, len=34
    flac_stream = b'fLaC' + meta_header + si + flac_frame

    samples, sr = sf.read(io.BytesIO(flac_stream), dtype='int16')

    # soundfile returns native-endian int16. Convert based on endianness marker.
    # 'L' = original data was little-endian, 'B' = big-endian
    result = samples.astype('<i2')  # ensure little-endian
    if endian_marker == 'B':
        result = result.byteswap()

    return result.tobytes()


# ============================================================================
# CHD v5 file reader
# ============================================================================
class CHDFile:
    def __init__(self, filepath):
        self.filepath = filepath
        self.f = open(filepath, 'rb')
        self._parse_header()
        self._decompress_map()

    def _parse_header(self):
        """Parse the CHD v5 header."""
        self.f.seek(0)
        header = self.f.read(124)

        magic = header[0:8]
        if magic != CHD_MAGIC:
            raise ValueError(f"Not a CHD file (magic: {magic!r})")

        self.header_len = struct.unpack('>I', header[8:12])[0]
        self.version = struct.unpack('>I', header[12:16])[0]

        if self.version != 5:
            raise ValueError(f"Only CHD v5 is supported, got v{self.version}")

        # Four codec tags (4 bytes each, ASCII)
        self.compressors = []
        for i in range(4):
            tag = header[16 + i*4 : 20 + i*4]
            self.compressors.append(tag.decode('ascii', errors='replace').strip('\x00'))

        self.logical_bytes = struct.unpack('>Q', header[32:40])[0]
        self.map_offset = struct.unpack('>Q', header[40:48])[0]
        self.meta_offset = struct.unpack('>Q', header[48:56])[0]
        self.hunk_bytes = struct.unpack('>I', header[56:60])[0]
        self.unit_bytes = struct.unpack('>I', header[60:64])[0]
        self.raw_sha1 = header[64:84]
        self.sha1 = header[84:104]
        self.parent_sha1 = header[104:124]

        self.hunk_count = (self.logical_bytes + self.hunk_bytes - 1) // self.hunk_bytes

        print(f"CHD v{self.version}")
        print(f"  Compressors: {self.compressors}")
        print(f"  Logical size: {self.logical_bytes} bytes ({self.logical_bytes / (1024*1024*1024):.2f} GB)")
        print(f"  Hunk size: {self.hunk_bytes} bytes")
        print(f"  Unit size: {self.unit_bytes} bytes")
        print(f"  Total hunks: {self.hunk_count}")
        print(f"  Map offset: {self.map_offset}")

    def _decompress_map(self):
        """Decompress the v5 hunk map."""
        self.f.seek(self.map_offset)
        map_header = self.f.read(16)

        map_bytes = struct.unpack('>I', map_header[0:4])[0]
        first_offset = int.from_bytes(map_header[4:10], 'big')
        map_crc = struct.unpack('>H', map_header[10:12])[0]
        lengthbits = map_header[12]
        selfbits = map_header[13]
        parentbits = map_header[14]

        print(f"  Map compressed size: {map_bytes}")
        print(f"  First hunk offset: {first_offset}")
        print(f"  Length bits: {lengthbits}, Self bits: {selfbits}, Parent bits: {parentbits}")

        # Read compressed map data
        compressed_map = self.f.read(map_bytes)
        bitbuf = BitReader(compressed_map)

        # Phase 1: Import Huffman tree (16 symbols, max 8 bits)
        decoder = HuffmanDecoder(16, 8)
        decoder.import_tree_rle(bitbuf)

        # Phase 2: Decode compression types for all hunks
        # rawmap: hunk_count entries, each 12 bytes
        self.rawmap = bytearray(self.hunk_count * 12)

        lastcomp = 0
        repcount = 0
        for hunknum in range(self.hunk_count):
            offset = hunknum * 12
            if repcount > 0:
                self.rawmap[offset] = lastcomp
                repcount -= 1
            else:
                val = decoder.decode_one(bitbuf)
                if val == COMPRESSION_RLE_SMALL:
                    self.rawmap[offset] = lastcomp
                    repcount = 2 + decoder.decode_one(bitbuf)
                elif val == COMPRESSION_RLE_LARGE:
                    self.rawmap[offset] = lastcomp
                    repcount = 2 + 16 + (decoder.decode_one(bitbuf) << 4)
                    repcount += decoder.decode_one(bitbuf)
                else:
                    self.rawmap[offset] = val
                    lastcomp = val

        # Phase 3: Read per-entry data (lengths, CRCs, offsets, refs)
        curoffset = first_offset
        last_self = 0
        last_parent = 0

        for hunknum in range(self.hunk_count):
            base = hunknum * 12
            comp_type = self.rawmap[base]
            offset = curoffset
            length = 0
            crc = 0

            if comp_type in (COMPRESSION_TYPE_0, COMPRESSION_TYPE_1,
                             COMPRESSION_TYPE_2, COMPRESSION_TYPE_3):
                length = bitbuf.read(lengthbits)
                curoffset += length
                crc = bitbuf.read(16)

            elif comp_type == COMPRESSION_NONE:
                length = self.hunk_bytes
                curoffset += length
                crc = bitbuf.read(16)

            elif comp_type == COMPRESSION_SELF:
                offset = bitbuf.read(selfbits)
                last_self = offset

            elif comp_type == COMPRESSION_PARENT:
                offset = bitbuf.read(parentbits) if parentbits > 0 else 0
                last_parent = offset

            elif comp_type == COMPRESSION_SELF_0:
                self.rawmap[base] = COMPRESSION_SELF
                offset = last_self

            elif comp_type == COMPRESSION_SELF_1:
                last_self += 1
                self.rawmap[base] = COMPRESSION_SELF
                offset = last_self

            elif comp_type == COMPRESSION_PARENT_SELF:
                self.rawmap[base] = COMPRESSION_PARENT
                offset = (hunknum * self.hunk_bytes) // self.unit_bytes
                last_parent = offset

            elif comp_type == COMPRESSION_PARENT_0:
                self.rawmap[base] = COMPRESSION_PARENT
                offset = last_parent

            elif comp_type == COMPRESSION_PARENT_1:
                last_parent += self.hunk_bytes // self.unit_bytes
                self.rawmap[base] = COMPRESSION_PARENT
                offset = last_parent

            # Store length (3 bytes big-endian)
            self.rawmap[base + 1] = (length >> 16) & 0xFF
            self.rawmap[base + 2] = (length >> 8) & 0xFF
            self.rawmap[base + 3] = length & 0xFF

            # Store offset (6 bytes big-endian)
            off_bytes = offset.to_bytes(6, 'big')
            self.rawmap[base + 4 : base + 10] = off_bytes

            # Store CRC (2 bytes big-endian)
            self.rawmap[base + 10] = (crc >> 8) & 0xFF
            self.rawmap[base + 11] = crc & 0xFF

        # Verify map CRC
        computed_crc = crc16(self.rawmap[:self.hunk_count * 12])
        if computed_crc != map_crc:
            print(f"  WARNING: Map CRC mismatch! Expected {map_crc:#06x}, got {computed_crc:#06x}")
        else:
            print(f"  Map CRC verified OK ({map_crc:#06x})")

        # Print compression type statistics
        type_counts = {}
        for hunknum in range(self.hunk_count):
            ct = self.rawmap[hunknum * 12]
            type_counts[ct] = type_counts.get(ct, 0) + 1
        print("  Hunk type distribution:")
        for ct in sorted(type_counts.keys()):
            name = COMP_NAMES.get(ct, f"unknown({ct})")
            print(f"    {name}: {type_counts[ct]}")

    def get_map_entry(self, hunknum):
        """Get the map entry for a hunk: (comp_type, length, offset, crc)."""
        base = hunknum * 12
        comp_type = self.rawmap[base]
        length = (self.rawmap[base+1] << 16) | (self.rawmap[base+2] << 8) | self.rawmap[base+3]
        offset = int.from_bytes(self.rawmap[base+4:base+10], 'big')
        crc = (self.rawmap[base+10] << 8) | self.rawmap[base+11]
        return comp_type, length, offset, crc

    def read_hunk(self, hunknum, hunk_cache=None):
        """Read and decompress a single hunk."""
        comp_type, length, offset, crc = self.get_map_entry(hunknum)

        if comp_type in (COMPRESSION_TYPE_0, COMPRESSION_TYPE_1,
                         COMPRESSION_TYPE_2, COMPRESSION_TYPE_3):
            # Read compressed data from file
            self.f.seek(offset)
            compressed = self.f.read(length)

            codec_tag = self.compressors[comp_type]

            if codec_tag == 'zlib':
                return decompress_zlib_hunk(compressed, self.hunk_bytes)
            elif codec_tag == 'lzma':
                return decompress_lzma_hunk(compressed, self.hunk_bytes)
            elif codec_tag == 'huff':
                dec = Huffman8BitDecoder()
                return dec.decompress(compressed, self.hunk_bytes)
            elif codec_tag == 'flac':
                return decompress_flac_hunk(compressed, self.hunk_bytes)
            else:
                raise ValueError(f"Unknown codec: {codec_tag}")

        elif comp_type == COMPRESSION_NONE:
            # Read raw data from file
            self.f.seek(offset)
            return self.f.read(self.hunk_bytes)

        elif comp_type == COMPRESSION_SELF:
            # Reference to another hunk
            if hunk_cache is not None and offset in hunk_cache:
                return hunk_cache[offset]
            return self.read_hunk(offset, hunk_cache)

        elif comp_type == COMPRESSION_PARENT:
            raise NotImplementedError("Parent CHD references not supported")

        else:
            raise ValueError(f"Unknown compression type {comp_type} at hunk {hunknum}")

    def extract(self, output_path):
        """Extract the full raw disk image."""
        print(f"\nExtracting to: {output_path}")
        print(f"Output size will be: {self.logical_bytes} bytes "
              f"({self.logical_bytes / (1024*1024*1024):.2f} GB)")

        start_time = time.time()
        bytes_written = 0
        hunk_cache = {}  # Cache for self-referencing hunks

        with open(output_path, 'wb') as out:
            for hunknum in range(self.hunk_count):
                # Progress
                if hunknum % 10000 == 0 or hunknum == self.hunk_count - 1:
                    elapsed = time.time() - start_time
                    pct = (hunknum + 1) / self.hunk_count * 100
                    if elapsed > 0 and hunknum > 0:
                        rate = bytes_written / elapsed / (1024 * 1024)
                        eta = (self.hunk_count - hunknum) / (hunknum / elapsed)
                        print(f"\r  Hunk {hunknum+1}/{self.hunk_count} "
                              f"({pct:.1f}%) - {rate:.1f} MB/s - "
                              f"ETA: {eta:.0f}s", end="", flush=True)
                    else:
                        print(f"\r  Hunk {hunknum+1}/{self.hunk_count} "
                              f"({pct:.1f}%)", end="", flush=True)

                try:
                    data = self.read_hunk(hunknum, hunk_cache)
                except Exception as e:
                    print(f"\n  ERROR at hunk {hunknum}: {e}")
                    # Write zeros for failed hunks
                    data = b'\x00' * self.hunk_bytes

                # Cache this hunk for potential self-references
                # Only cache if this hunk is referenced by others (can't know ahead,
                # so we keep a bounded cache of recent hunks)
                hunk_cache[hunknum] = data
                # Keep cache bounded to save memory
                if len(hunk_cache) > 1000:
                    oldest = min(hunk_cache.keys())
                    del hunk_cache[oldest]

                # For the last hunk, might need to truncate
                if hunknum == self.hunk_count - 1:
                    remaining = self.logical_bytes - bytes_written
                    out.write(data[:remaining])
                    bytes_written += remaining
                else:
                    out.write(data)
                    bytes_written += len(data)

        elapsed = time.time() - start_time
        print(f"\n  Done! Wrote {bytes_written} bytes in {elapsed:.1f}s "
              f"({bytes_written / elapsed / (1024*1024):.1f} MB/s)")

    def close(self):
        self.f.close()


# ============================================================================
# CRC-16 (CCITT, matching MAME's crc16_creator)
# ============================================================================
def crc16(data):
    """Compute CRC-16/CCITT-FALSE (polynomial 0x1021, init 0xFFFF)."""
    crc = 0xFFFF
    for byte in data:
        crc ^= byte << 8
        for _ in range(8):
            if crc & 0x8000:
                crc = (crc << 1) ^ 0x1021
            else:
                crc = crc << 1
            crc &= 0xFFFF
    return crc


# ============================================================================
# Main
# ============================================================================
def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <input.chd> [output.img]")
        sys.exit(1)

    input_path = sys.argv[1]

    if len(sys.argv) >= 3:
        output_path = sys.argv[2]
    else:
        # Default: same name with .img extension, in same directory
        base = os.path.splitext(input_path)[0]
        output_path = base + '.img'

    print(f"Opening: {input_path}")
    chd = CHDFile(input_path)

    try:
        chd.extract(output_path)
    finally:
        chd.close()

    # Verify output
    actual_size = os.path.getsize(output_path)
    print(f"\nOutput file: {output_path}")
    print(f"Output size: {actual_size} bytes")
    if actual_size == chd.logical_bytes:
        print("Size matches expected logical size.")
    else:
        print(f"WARNING: Expected {chd.logical_bytes}, got {actual_size}")


if __name__ == '__main__':
    main()
