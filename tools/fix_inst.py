import os
files = ['funcs_15.c','funcs_16.c','funcs_17.c','funcs_18.c','funcs_24.c','funcs_27.c','funcs_9.c']
# Also fix any test writes from earlier debugging:
BAD_DBG  = b'#%dXX"'
GOOD_DBG = b'#%d\\n"'   # backslash + n + " -> what C source needs
BAD_NL   = b'#%d\n"'    # literal newline + " (broken)
GOOD_NL  = b'#%d\\n"'   # literal backslash-n + " (fixed)
for f in files:
    p = f'D:/recomp/arcade/carnevil/recomp_out/funcs/{f}'
    with open(p, 'rb') as fh: s = fh.read()
    s = s.replace(BAD_DBG, GOOD_DBG)
    s = s.replace(BAD_NL,  GOOD_NL)
    with open(p, 'wb') as fh:
        fh.write(s); fh.flush(); os.fsync(fh.fileno())
    # verify
    with open(p, 'rb') as fh: v = fh.read()
    n_bad = v.count(BAD_NL)
    n_good = v.count(GOOD_NL)
    print(f'{f}: remaining_bad={n_bad} good={n_good}')
