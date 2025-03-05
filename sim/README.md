# sim 

Those macros simulates the interaction between the incoming beam with the simulation setup. 

1. **sim_ideal.C:** This macro simulates an ideal scenario where a beam reacts on a 15cm target. As result a file with the interaction point with every par of FOOTs will be created.

2. **sim_foot_only.C:** This is a R3B-based simulation that describes the interaction between the geometry created by the geo macros with a 25-F beam.

All R3B based simulations need to be "translated" with the sim2ana macro in order to make it analyzable with the different tools provided in these codes.