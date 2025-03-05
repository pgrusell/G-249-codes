#!/bin/bash

# Comprueba si el directorio results no existe y lo crea
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
root -b -q -l "anareco.C(\"$RESULTSPATH/mapped.txt\", \"$RESULTSPATH/reconstructed.txt\")"

cd ../analysis
root -l "error.C(\"$RESULTSPATH/reconstructed.txt\")"

cd ../../exec
