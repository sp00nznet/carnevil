"""Check that every file NOTICE lists as ported from BSD-3-Clause code carries
the notice itself: the SPDX line, the copyright holders, the three
conditions and the disclaimer (what BSD-3-Clause asks redistributed source
to keep).

usage: python tools/check_notices.py      (from the repository root)
Exit code 1 if a listed file is missing its notice.
"""
import re, sys

table = open("NOTICE", encoding="utf-8").read().split("MAME as a whole")[0]   # the table only
rows = re.findall(r"^((?:src|tools)/\S+)", table, re.M)
files = []
for row in rows:                      # "src/runtime/adsp2100.h, .c" style rows
    files.append(row.rstrip(","))
    if row.endswith(","):
        files.append(row.rstrip(",").rsplit(".", 1)[0] + ".c")
need = ["SPDX-License-Identifier: BSD-3-Clause", "copyright-holders:",
        "Redistributions of source code must retain", "Neither the name of the copyright holder",
        "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS"]
bad = 0
for f in sorted(set(files)):
    head = re.sub(r"\s+", " ", open(f, encoding="utf-8").read(4000).replace("#", " ").replace("*", " "))
    missing = [n for n in need if n not in head]
    print(("ok      " if not missing else "MISSING ") + f + ("" if not missing else "  (" + "; ".join(missing) + ")"))
    bad += bool(missing)
sys.exit(1 if bad else 0)
