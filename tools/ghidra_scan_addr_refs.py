# -*- coding: utf-8 -*-
# Look for any data-word occurrences and lui+addiu pairs that construct
# the address 0x80167848 itself (not its callers). Also list all reference
# types Ghidra knows about for this address. This catches whether func_80167848
# is reached via a function-pointer table, jr-through-register, etc.
#
# @category CarnEvil
# @runtime Jython

TARGETS = [0x80167848]
TARGET_SET = set(TARGETS)
OUTPATH = "D:/recomp/arcade/carnevil/ghidra_scan_167848_refs.log"

fm = currentProgram.getFunctionManager()
af = currentProgram.getAddressFactory()
mem = currentProgram.getMemory()
rm = currentProgram.getReferenceManager()
listing = currentProgram.getListing()

fout = open(OUTPATH, "w")
def w(s):
    println(s)
    fout.write(s + "\n")

# 1) All Ghidra references TO 0x80167848 with their types
w("=" * 78)
w("Ghidra references TO 0x80167848 — every reference type")
w("=" * 78)
addr = af.getAddress("80167848")
refs = rm.getReferencesTo(addr)
rt_counts = {}
for r in refs:
    t = str(r.getReferenceType())
    rt_counts[t] = rt_counts.get(t, 0) + 1
for t, c in sorted(rt_counts.items(), key=lambda x: -x[1]):
    w("  %4d  %s" % (c, t))

# 2) Find every .text block scan: pair lui+addiu that constructs 0x80167848
w("")
w("=" * 78)
w("lui+addiu/ori pairs constructing 0x80167848")
w("=" * 78)
text_block = None
for b in mem.getBlocks():
    if b.getName() == ".text":
        text_block = b
        break
if text_block is None:
    w("ERROR no .text")
    fout.close()
    raise SystemExit()

def sign_ext_16(v):
    v = v & 0xFFFF
    return v if v < 0x8000 else v - 0x10000

start_off = text_block.getStart().getOffset()
end_off   = text_block.getEnd().getOffset()
last_lui = {}
lui_addiu_hits = []
addr = text_block.getStart()
while addr.getOffset() <= end_off - 3:
    try:
        ww = mem.getInt(addr) & 0xFFFFFFFF
    except:
        addr = addr.add(4); continue
    opcode = (ww >> 26) & 0x3F
    rs = (ww >> 21) & 0x1F
    rt = (ww >> 16) & 0x1F
    imm = ww & 0xFFFF
    if opcode == 0x0F and rs == 0:
        last_lui[rt] = (addr.getOffset(), imm)
    elif opcode == 0x09 or opcode == 0x0D:
        if rs in last_lui:
            lui_addr, high_imm = last_lui[rs]
            low = sign_ext_16(imm) if opcode == 0x09 else imm
            target = ((high_imm << 16) + low) & 0xFFFFFFFF
            if target in TARGET_SET:
                lui_addiu_hits.append((lui_addr, addr.getOffset(), target))
    addr = addr.add(4)
w("Found %d lui+addiu/ori pairs constructing 0x80167848" % len(lui_addiu_hits))
for (la, aa, t) in lui_addiu_hits:
    f = fm.getFunctionContaining(af.getAddress("%x" % la))
    fn = f.getName() if f else "?"
    fe = f.getEntryPoint().getOffset() if f else 0
    w("  lui@0x%08X addiu@0x%08X -> 0x%08X  in %s @ 0x%08X" % (la, aa, t, fn, fe))

# 3) Scan binary for the word 0x80167848 as 4-byte data
w("")
w("=" * 78)
w("0x80167848 as a 4-byte word in any block")
w("=" * 78)
word_hits = []
for block in mem.getBlocks():
    if not block.isInitialized():
        continue
    a = block.getStart()
    end = block.getEnd().getOffset()
    while a.getOffset() <= end - 3:
        try:
            v = mem.getInt(a) & 0xFFFFFFFF
            if v in TARGET_SET:
                word_hits.append((a.getOffset(), v, block.getName()))
        except:
            pass
        a = a.add(4)
w("Found %d word-of-address occurrences" % len(word_hits))
for (a, v, b) in word_hits:
    w("  0x%08X = 0x%08X (block %s)" % (a, v, b))

fout.close()
println("Wrote " + OUTPATH)
