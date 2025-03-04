cd ../geo
root -b -q create_target_area_2025_15cm_geo.C
cd ../sim
root -b -q sim_foot_only.C
cd ../ana/sim2ana
root -b -q -l 'sim2ana.C("../../sim/sim.root", "mapped.txt")'
cd ../reco
root -b -q -l 'anareco.C("../sim2ana/mapped.txt", "reconstructed.txt")'
cd ../analysis
root -l 'error.C("../reco/reconstructed.txt")'
cd ../../exec
