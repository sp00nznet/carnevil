"""Remove the bulk parent-function instrumentation injected during Phase C."""
import re, os
files = ['funcs_15.c','funcs_16.c','funcs_17.c','funcs_18.c','funcs_24.c','funcs_27.c','funcs_9.c']
pat = re.compile(rb'    \{ static int cc = 0; cc\+\+; if \(cc <= 3\) fprintf\(stderr, "\[parent\] func_[0-9A-F]+ #%d\\n", cc\); \}\r?\n')
for f in files:
    p = f'D:/recomp/arcade/carnevil/recomp_out/funcs/{f}'
    with open(p,'rb') as fh: s = fh.read()
    s2 = pat.sub(b'', s)
    if s2 != s:
        with open(p,'wb') as fh:
            fh.write(s2); fh.flush(); os.fsync(fh.fileno())
        print(f'stripped {f}: {(len(s)-len(s2))} bytes')
    else:
        print(f'no match {f}')
