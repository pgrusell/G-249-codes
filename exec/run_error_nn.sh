#!/bin/bash

if [ ! -d "../results" ]; then
    mkdir -p "../results"
fi

RESULTSPATH=$(realpath "../results")

cd ../geo
root -b -q create_target_area_2025_15cm_geo.C

cd ../sim
root -b -q sim_foot_only.C

cd ../ana/sim2ana
root -b -q -l "sim2ana.C(\"../../sim/sim.root\", \"$RESULTSPATH/mapped.txt\")"

cd ../reco
root -b -q -l "anareco.C(\"$RESULTSPATH/mapped.txt\", \"$RESULTSPATH/reconstructed_ana.txt\")"
python nnreco.py "$RESULTSPATH/mapped.txt" "$RESULTSPATH/reconstructed_nn.txt" "model.h5"

cd ../analysis
root -b -q -l "error.C(\"$RESULTSPATH/reconstructed_nn.txt\", \"$RESULTSPATH/reconstructed_nn.root\")"
root -b -q -l "error.C(\"$RESULTSPATH/reconstructed_ana.txt\", \"$RESULTSPATH/reconstructed_ana.root\")"

cd ../../exec