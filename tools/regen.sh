#!/bin/sh
# Regenerate recomp_out/ from carnevil_recomp.toml and rtos_recomp.toml.
# Needs a MidwayRecomp build next to this repo (../MidwayRecomp) or $MIDWAYRECOMP.
set -e
cd "$(dirname "$0")/.."
R="${MIDWAYRECOMP:-../MidwayRecomp/build/Release/MidwayRecomp.exe}"
mkdir -p recomp_out/funcs recomp_out/rtos_funcs
for c in carnevil rtos; do "$R" "${c}_recomp.toml" 2>&1 | grep -E "Error|Unhandled|No function" || true; done
