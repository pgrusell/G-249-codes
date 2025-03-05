// Simulation of the G-249 experiment ONLY with the vacuum chamber created by the geometry
// macros as well as the FOOTs.

void sim_foot_only(Int_t nEvents = 2000000)
{

    TString transport = "TGeant4";
    TString outFile = "./sim.root";
    TString parFile = "./par.root";

    Int_t randomSeed = 335566; // 0 for time-dependent random numbers

    // ------------------------------------------------------------------------
    TString dir = gSystem->Getenv("VMCWORKDIR");
    TString pardir = dir + "/../params/";
    pardir.ReplaceAll("//", "/");

    TString r3b_geomdir = dir + "/geometry/";
    gSystem->Setenv("GEOMPATH", r3b_geomdir.Data());
    r3b_geomdir.ReplaceAll("//", "/");

    TString r3b_pardir = pardir + "/geometry/";
    r3b_pardir.ReplaceAll("//", "/");

    TString r3b_confdir = dir + "/gconfig/";
    gSystem->Setenv("CONFIG_DIR", r3b_confdir.Data());
    r3b_confdir.ReplaceAll("//", "/");

    // -----   Timer   --------------------------------------------------------
    TStopwatch timer;
    timer.Start();

    // -----   Create simulation run   ----------------------------------------
    FairRunSim *run = new FairRunSim();
    run->SetName(transport);            // Transport engine
    run->SetOutputFile(outFile.Data()); // Output file
    FairRuntimeDb *rtdb = run->GetRuntimeDb();

    // -----   Create media   -------------------------------------------------
    run->SetMaterials("media_r3b.geo"); // Materials

    // -----   Create R3B geometry --------------------------------------------
    // R3B Cave definition
    FairModule *cave = new R3BCave("CAVE");
    cave->SetGeometryFileName("r3b_cave.geo");
    run->AddModule(cave);

    //////////////////// DETECTORS //////////////////////////////

    // ---------------LH2 target + vacuum chamber + FOOT --------

    auto *foots = new R3BTra("target_area_v2025_15cm.geo.root");
    run->AddModule(foots);

    // -----   Create PrimaryGenerator   --------------------------------------
    FairPrimaryGenerator *primGen = new FairPrimaryGenerator();

    Double_t pz = 31.34 / 25.;
    auto ionGen = new FairIonGenerator(9, 25, 9, 1, 0., 0., pz, 0., 0., -300.);
    primGen->AddGenerator(ionGen);

    run->SetGenerator(primGen);
    run->SetStoreTraj(true);

    FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
    FairLogger::GetLogger()->SetLogScreenLevel("INFO"); // nolog for no output, INFO to show the n. of hits

    // -----   Initialize simulation run   ------------------------------------
    run->Init();
    TVirtualMC::GetMC()->SetRandom(new TRandom3(randomSeed));

    // ------  Increase nb of step for CALO
    Int_t nSteps = -15000;
    TVirtualMC::GetMC()->SetMaxNStep(nSteps);

    // -----   Runtime database   ---------------------------------------------
    Bool_t kParameterMerged = kTRUE;
    FairParRootFileIo *parOut = new FairParRootFileIo(kParameterMerged);
    parOut->open(parFile.Data());
    rtdb->setOutput(parOut);
    rtdb->saveOutput();
    rtdb->print();

    // -----   Start run   ----------------------------------------------------
    if (nEvents > 0)
    {
        run->Run(nEvents);
    }

    // -----   Finish   -------------------------------------------------------
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    cout << endl
         << endl;
    cout << "Macro finished succesfully." << endl;
    cout << "Output file is " << outFile << endl;
    cout << "Parameter file is " << parFile << endl;
    cout << "Real time " << rtime << " s, CPU time " << ctime << "s" << endl
         << endl;

    cout << " Test passed" << endl;
    cout << " All ok " << endl;
}
