#!/usr/bin/env python3
"""Recover per-model .ZM vertex/normal/face counts from the game's own code.

The counts are NOT stored in the .ZM files. The game passes them to its model
loader func_800CA724, whose allocation arithmetic is:

    a1*12  +  [sp+0x8C]*12  +  a3*40   ->  malloc
    verts      normals          faces

so each call site carries the counts for one model as literal immediates. This
scans every call site in the recompiled sources and, given the extracted files,
matches each .ZM to its call site by the exact size identity

    verts*12 + normals*12 + faces*40 == content size

Usage:
    python tools/zm_counts.py [recomp_out/funcs/funcs_*.c] [extracted/files]
"""
import glob
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
DEF_FUNCS = os.path.join(HERE, '..', 'recomp_out', 'funcs', 'funcs_*.c')
DEF_FILES = os.path.join(HERE, '..', 'extracted', 'files')

RE_ADDR = re.compile(r'//\s*0x([0-9A-F]{8}):\s+(\S+)\s*(.*)')


def call_sites(pattern):
    """-> [(addr, vcount, ncount, fcount)] for every func_800CA724 call."""
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
            # include the delay slot at i+1, which executes with the call
            for _, op2, ar2 in dis[max(0, i - 40):i + 2]:
                m = re.match(r'\$(\w+), \$zero, (-?0x[0-9A-Fa-f]+|-?\d+)', ar2)
                if op2 == 'addiu' and m:
                    reg[m.group(1)] = int(m.group(2), 0)
                    continue
                m = re.match(r'\$(\w+), (-?0x[0-9A-Fa-f]+|-?\d+)\(\$sp\)', ar2)
                if op2 == 'sw' and m:
                    stack[int(m.group(2), 0)] = reg.get(m.group(1))
            out.append((addr, reg.get('a1'), stack.get(0x14), reg.get('a3')))
    return out


def main():
    pattern = sys.argv[1] if len(sys.argv) > 1 else DEF_FUNCS
    files = sys.argv[2] if len(sys.argv) > 2 else DEF_FILES

    sites = call_sites(pattern)
    print('%d loader call sites' % len(sites))
    if not os.path.isdir(files):
        for addr, v, n, f in sites:
            print('  0x%08X  verts=%-5s normals=%-5s faces=%s' % (addr, v, n, f))
        return

    print('%-14s %8s  %-10s %6s %8s %6s' %
          ('file', 'size', 'call site', 'verts', 'normals', 'faces'))
    matched = total = 0
    for name in sorted(os.listdir(files)):
        if not name.upper().endswith('.ZM'):
            continue
        total += 1
        size = os.path.getsize(os.path.join(files, name))
        # extracted files are thumbnail(512) + content(size)
        content = size - 512 if size > 512 else size
        hit = [s for s in sites
               if None not in s[1:] and 12 * s[1] + 12 * s[2] + 40 * s[3] == content]
        if hit:
            matched += 1
            a, v, n, f = hit[0]
            print('%-14s %8d  0x%08X %6d %8d %6d' % (name, size, a, v, n, f))
        else:
            print('%-14s %8d  %-10s %s' % (name, size, '-', 'no matching call site'))
    print('\nmatched %d / %d .ZM files' % (matched, total))


if __name__ == '__main__':
    main()
