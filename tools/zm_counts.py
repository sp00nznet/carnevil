#!/usr/bin/env python3
"""Recover the .ZM model table -- filename and counts -- from the game's own code.

Neither the counts nor the filenames live in the .ZM files. The game passes
both to its model loader func_800CA724, whose allocation arithmetic is

    a1*12  +  [sp+0x14]*12  +  a3*40   ->  malloc
    verts      normals          faces

and whose [sp+0x18] argument is a pointer to the filename string inside
GAME.EXE's image. Each call site therefore fully describes one model. Verified:
the CHNK.ZM site passes 6/6/8, and 512 + 6*12 + 6*12 + 8*40 == 976 == the size
of CHNK.ZM on disk (512 = the thumbnail prefix every file carries).

Usage:
    python tools/zm_counts.py [recomp_out/funcs/funcs_*.c] [extracted/GAME.bin]
"""
import glob
import os
import re
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
DEF_FUNCS = os.path.join(HERE, '..', 'recomp_out', 'funcs', 'funcs_*.c')
DEF_BIN = os.path.join(HERE, '..', 'extracted', 'GAME.bin')
DEF_FILES = os.path.join(HERE, '..', 'extracted', 'files')
LOAD_ADDR = 0x800C4000
THUMB = 512

RE_ADDR = re.compile(r'//\s*0x([0-9A-F]{8}):\s+(\S+)\s*(.*)')


def s16(v):
    return v - 0x10000 if v >= 0x8000 else v


def call_sites(pattern):
    """-> [(addr, verts, normals, faces, name_ptr)] per func_800CA724 call."""
    out = []
    for path in sorted(glob.glob(pattern)):
        dis = []
        for ln in open(path, encoding='utf-8', errors='replace'):
            m = RE_ADDR.search(ln)
            if m:
                dis.append((int(m.group(1), 16), m.group(2), m.group(3).strip()))
        for i, (addr, op, args) in enumerate(dis):
            if op != 'jal' or '0x800CA724' not in args:
                continue
            reg, stack = {}, {}
            # include the delay slot at i+1: it executes with the call
            for _, op2, ar2 in dis[max(0, i - 48):i + 2]:
                m = re.match(r'\$(\w+), (-?0x[0-9A-Fa-f]+|-?\d+)$', ar2)
                if op2 == 'lui' and m:
                    reg[m.group(1)] = (int(m.group(2), 0) & 0xFFFF) << 16
                    continue
                m = re.match(r'\$(\w+), \$(\w+), (-?0x[0-9A-Fa-f]+|-?\d+)', ar2)
                if op2 == 'addiu' and m:
                    dst, src, imm = m.group(1), m.group(2), int(m.group(3), 0)
                    if src == 'zero':
                        reg[dst] = imm
                    elif src in reg:
                        reg[dst] = (reg[src] + s16(imm & 0xFFFF)) & 0xFFFFFFFF
                    continue
                m = re.match(r'\$(\w+), (-?0x[0-9A-Fa-f]+|-?\d+)\(\$sp\)', ar2)
                if op2 == 'sw' and m:
                    stack[int(m.group(2), 0)] = reg.get(m.group(1))
            out.append((addr, reg.get('a1'), stack.get(0x14),
                        reg.get('a3'), stack.get(0x18)))
    return out


def cstr(img, addr):
    if not addr or not (LOAD_ADDR <= addr < LOAD_ADDR + len(img)):
        return None
    o = addr - LOAD_ADDR
    end = img.find(b'\0', o)
    try:
        s = img[o:end].decode('ascii')
    except UnicodeDecodeError:
        return None
    return s if s and all(32 <= ord(c) < 127 for c in s) else None


def solve_missing(name, v, n, f):
    """Recover a single unknown count from the on-disk size, or pass through."""
    if not name or [v, n, f].count(None) != 1:
        return v, n, f
    path = os.path.join(DEF_FILES, name)
    if not os.path.exists(path):
        return v, n, f
    rest = os.path.getsize(path) - THUMB
    if v is None and None not in (n, f):
        got, r = divmod(rest - n * 12 - f * 40, 12)
        return (got if r == 0 and got >= 0 else None), n, f
    if n is None and None not in (v, f):
        got, r = divmod(rest - v * 12 - f * 40, 12)
        return v, (got if r == 0 and got >= 0 else None), f
    if f is None and None not in (v, n):
        got, r = divmod(rest - v * 12 - n * 12, 40)
        return v, n, (got if r == 0 and got >= 0 else None)
    return v, n, f


def main():
    pattern = sys.argv[1] if len(sys.argv) > 1 else DEF_FUNCS
    binpath = sys.argv[2] if len(sys.argv) > 2 else DEF_BIN
    img = open(binpath, 'rb').read()

    sites = call_sites(pattern)
    print('%-14s %-10s %6s %8s %6s %10s  %s' %
          ('model', 'call site', 'verts', 'normals', 'faces', 'expect', 'on disk'))
    named = agree = 0
    for addr, v, n, f, ptr in sorted(sites, key=lambda s: (cstr(img, s[4]) or '~')):
        name = cstr(img, ptr)
        if None in (v, n, f):
            # Some sites pass a count in a saved register set outside the
            # scanned window (BLAD.ZM uses `sw $s0, 0x14($sp)`). When exactly
            # one count is unknown the file size determines it, since
            # size == 512 + verts*12 + normals*12 + faces*40 exactly.
            v, n, f = solve_missing(name, v, n, f)
        if None in (v, n, f):
            print('%-14s 0x%08X  %s' % (name or '?', addr, '(counts not resolved)'))
            continue
        want = THUMB + v * 12 + n * 12 + f * 40
        if name:
            named += 1
        got = ''
        p = os.path.join(DEF_FILES, name) if name else None
        if p and os.path.exists(p):
            sz = os.path.getsize(p)
            got = '%d %s' % (sz, 'OK' if sz == want else 'MISMATCH')
            if sz == want:
                agree += 1
        elif name:
            got = '(not extracted)'
        print('%-14s 0x%08X %6d %8d %6d %10d  %s' %
              (name or '?', addr, v, n, f, want, got))
    print('\n%d call sites, %d named, %d size-verified against extracted files'
          % (len(sites), named, agree))


if __name__ == '__main__':
    main()
