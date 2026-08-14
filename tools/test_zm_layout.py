#!/usr/bin/env python3
"""Self-check for the .ZM model layout that CARNEVIL_MESH3D depends on.

Layout was verified against the recompiled mesh walker func_800E1EF0, which
strides its face pointer (r18) by 0x28 and reads six halfwords at +4..+0xF:

    0x000  512B thumbnail header (same preview header .EXE files carry)
    0x200  vcount verts   x 12B float xyz
    ...    ccount normals x 12B float xyz
    ...    fcount faces   x 40B: u32 surfaceId
                                 u16 vert   idx @ +4/+6/+8
                                 u16 normal idx @ +0xA/+0xC/+0xE
                                 6x float S/T   @ +0x10

Reading from offset 0 instead of 0x200 puts every index 512B out of phase,
which is what produced the old "46/1706 faces valid -> assets must be a
different build" conclusion. Run: python tools/test_zm_layout.py
"""
import os
import struct
import sys

ZM = os.path.join(os.path.dirname(__file__), '..', 'extracted', 'files', 'BABY.ZM')
HDR, VCOUNT, CCOUNT, FCOUNT = 512, 942, 946, 1693


def check(path=ZM):
    data = open(path, 'rb').read()

    vp = HDR
    cp = vp + VCOUNT * 12
    fp = cp + CCOUNT * 12
    end = fp + FCOUNT * 40

    slack = len(data) - end
    assert 0 <= slack < 40, f"layout overruns file: {end} vs {len(data)} (slack {slack})"

    # Every vertex must be a finite, sanely-scaled coordinate.
    bad_v = [i for i in range(VCOUNT)
             if not all(abs(c) < 1e4 for c in struct.unpack_from('<3f', data, vp + i * 12))]
    assert not bad_v, f"{len(bad_v)} non-finite verts, first at {bad_v[:5]}"

    # Every face must index inside the vert and normal arrays. This is the
    # assertion that fails loudly if the 512B header offset is ever dropped.
    bad_f = []
    for i in range(FCOUNT):
        idx = struct.unpack_from('<6H', data, fp + i * 40 + 4)
        if not (all(v < VCOUNT for v in idx[:3]) and all(n < CCOUNT for n in idx[3:])):
            bad_f.append(i)
    assert not bad_f, f"{len(bad_f)}/{FCOUNT} faces have out-of-range indices, first {bad_f[:5]}"

    print(f"ok: {VCOUNT} verts, {CCOUNT} normals, {FCOUNT} faces all valid "
          f"({len(data)} bytes, {slack}B slack)")


if __name__ == '__main__':
    check(sys.argv[1] if len(sys.argv) > 1 else ZM)
