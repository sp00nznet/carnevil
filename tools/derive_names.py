#!/usr/bin/env python3
"""
Derive meaningful function names from the Ghidra export.

Sources, in priority order (later overrides earlier):
  1. String-derived self-names: a debug string that begins with an identifier
     followed by ':' or '()' (e.g. "main_init_sound:", "coin_volume_proc():").
     Only used when that string is referenced by exactly ONE function (so it is
     the function naming itself, not a shared logger format).
  2. String-derived descriptive slug: for a still-unnamed function that uniquely
     references a descriptive string, slugify it. Lower confidence; addr-suffixed.
  3. Curated RE seed list (hand-verified this session) — highest priority.

Output:
  ghidra_export/name_map.json   {"0x800CC1E4": "entity_register", ...}
  ghidra_export/name_report.txt  human-readable, grouped by source
Names are sanitized to valid unique C identifiers; the 8-hex address is appended
to every name to guarantee uniqueness and keep round-trip traceability.
"""
import json, re, collections
from pathlib import Path

EXPORT = Path("D:/recomp/arcade/carnevil/ghidra_export")
fns = json.load(open(EXPORT / "functions.json"))

# addr(int) -> function record
by_addr = {f["addr"]: f for f in fns}

# ---- Curated RE seed list (verified this session). addr -> base name ----
SEED = {
    0x80167848: "voodoo_dma_cmd_build",   # DMA command builder -> Voodoo
    0x800CC1E4: "entity_register",         # builds {id,handler,...}, calls alloc
    0x800CC218: "entity_register_entry",   # split entry, falls into alloc
    0x800CC220: "entity_alloc_slot",       # slot alloc in 0x801E3880, bumps gate
    0x800CC47C: "entity_dispatch_update",  # per-entry dispatch of hook at +0x20
    0x800CB5AC: "render_handler_cb5ac",    # registered attract render handler
    0x800CB42C: "render_handler_cb42c",    # registered attract render handler
    0x800CB86C: "render_handler_cb86c",    # registered via func_80148138
    0x800D4C24: "display_channel_setup",
    0x800D5B10: "channel_init",
    0x800D5484: "render_submit_dispatch",
    0x800D5D04: "scene_graph_render",
    0x800D7600: "scene_node_create",
    0x8014A488: "state_machine_dispatch",
    0x80151B74: "mode_register",
    0x80148AFC: "mode_register_entry",
    0x80151618: "fiber_yield",
    0x800C47E0: "task_body",
    0x8010FE90: "zone_parser",
    0x801143CC: "geometry_build",
}

# ---- Pass 0: index string -> referring functions ----
str_referrers = collections.defaultdict(list)
for f in fns:
    for s in f["string_refs"]:
        str_referrers[s].append(f["addr"])

def sanitize(ident):
    ident = re.sub(r"[^0-9A-Za-z_]", "_", ident)
    ident = re.sub(r"_+", "_", ident).strip("_")
    if ident and ident[0].isdigit():
        ident = "_" + ident
    return ident[:48]

# Identifier-at-start patterns that indicate a function self-naming in a log line
SELF_RE = [
    re.compile(r"^\s*([A-Za-z_]\w{2,46})\s*\(\s*\)"),   # name()
    re.compile(r"^\s*([A-Za-z_]\w{2,46})\s*:"),          # name:
]
# Reject obvious non-names (all-caps words, common tokens)
BAD_NAME = re.compile(r"^(ERROR|FATAL|WARNING|INFO|OK|DONE|NULL|TRUE|FALSE)$", re.I)

def self_name_from_string(s):
    for rx in SELF_RE:
        m = rx.match(s)
        if not m:
            continue
        ident = m.group(1)
        # require it to look like a function name: lowercase letter present,
        # length >= 4, not a generic keyword/all-caps.
        if len(ident) < 4: continue
        if BAD_NAME.match(ident): continue
        if not any(c.islower() for c in ident): continue
        return ident
    return None

names = {}        # addr -> base name
source = {}       # addr -> source tag

# ---- Pass 1: self-names (single referrer) ----
for s, refs in str_referrers.items():
    nm = self_name_from_string(s)
    if nm is None:
        continue
    if len(refs) != 1:
        continue  # shared logger format -> ambiguous
    addr = refs[0]
    if addr in names:
        continue
    names[addr] = sanitize(nm)
    source[addr] = "self-string"

# ---- Pass 2: descriptive slug from a uniquely-referenced string ----
FMT = re.compile(r"%[-+ 0#]*\d*\.?\d*[hlL]*[diouxXeEfgGcsp%]")
def clean_str(s):
    s = FMT.sub(" ", s)                 # drop printf format specifiers
    s = s.replace("\\n", " ").replace("\\r", " ").replace("\\t", " ")
    s = re.sub(r"[^0-9A-Za-z _]", " ", s)
    s = re.sub(r"\s+", " ", s).strip()
    return s

def good_descriptive(s):
    s = clean_str(s)
    if len(s) < 4 or len(s) > 60: return False
    letters = sum(c.isalpha() for c in s)
    return letters >= 4

def slug_rank(s):
    # Prefer single-token labels (e.g. "diags", "select1") over long sentences:
    # rank by (number of words, then length).
    c = clean_str(s)
    return (len(c.split()), len(c))

for f in fns:
    addr = f["addr"]
    if addr in names:
        continue
    cands = [s for s in f["string_refs"]
             if len(str_referrers[s]) == 1 and good_descriptive(s)]
    if not cands:
        continue
    cands.sort(key=slug_rank)            # shortest/most label-like first
    base = sanitize(clean_str(cands[0]).lower())
    if not base:
        continue
    names[addr] = base
    source[addr] = "slug-string"

# ---- Pass 3: curated seed (override) ----
for addr, nm in SEED.items():
    names[addr] = sanitize(nm)
    source[addr] = "seed"

# ---- Finalize: append addr suffix for uniqueness + traceability ----
final = {}
for addr, base in names.items():
    final["0x%08X" % addr] = "%s_%08X" % (base, addr)

json.dump(final, open(EXPORT / "name_map.json", "w"), indent=1, sort_keys=True)

# ---- Report ----
by_src = collections.defaultdict(list)
for addr in names:
    by_src[source[addr]].append(addr)
with open(EXPORT / "name_report.txt", "w", encoding="utf-8") as r:
    r.write("Derived %d names of %d functions\n\n" % (len(final), len(fns)))
    for src in ("seed", "self-string", "slug-string"):
        addrs = sorted(by_src[src])
        r.write("== %s (%d) ==\n" % (src, len(addrs)))
        for a in addrs:
            r.write("  0x%08X  %s\n" % (a, final["0x%08X" % a]))
        r.write("\n")

print("Derived %d names (%d seed, %d self-string, %d slug-string)" % (
    len(final), len(by_src["seed"]), len(by_src["self-string"]), len(by_src["slug-string"])))
print("Wrote name_map.json + name_report.txt")
