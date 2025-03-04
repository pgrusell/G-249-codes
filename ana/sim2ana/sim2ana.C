// This macro transforms the output of an R3BSimulation to something that can
// be analyzed. This is the 3 coordinates of point of every pair of FOOTs as
// well as the z-coordinate of the real vertex.

// We will select every event in which a non-25F heavy particle is measured
// in the last pair of FOOTs.

// If the configuration is "ideal" we will take the information given by a
// single FOOT, otherwise, more complex calculations will be done.

void sim2ana(TString fileNameIn, TString fileNameOut, TString conf = "ideal")
{

    auto *f = new TFile(fileNameIn, "READ");
    auto *tr = (TTree *)f->Get("evt");

    auto *brClone = new TClonesArray("R3BTraPoint");
    tr->SetBranchAddress("TraPoint", &brClone);

    auto *brClone1 = new TClonesArray("R3BMCTrack");
    tr->SetBranchAddress("MCTrack", &brClone1);

    Int_t nEvents;
    Int_t nHits;

    std::vector<Int_t> footID;
    std::vector<Int_t> pID;
    std::vector<Double_t> pos;

    std::vector<Int_t> goodEvent = {1, 2, 3, 4, 5, 6, 7, 8};
    Double_t zRec;

    Int_t nTracks;
    Int_t pIDTrack;

    Double_t zFinal;

    std::ofstream file(fileNameOut);

    auto *zHist = new TH1F("h1", "z-coordinate of the real vertex", 100, -30, 30);
    auto *zHist2d = new TH2F("h2", "z-coordinate of the real vertex", 100, -30, 30, 100, -30, 30);

    nEvents = tr->GetEntries();

    for (Int_t iEvent = 0; iEvent < nEvents; iEvent++)
    {
        tr->GetEntry(iEvent);
        nHits = brClone->GetEntries();

        footID.clear();
        pID.clear();
        pos.clear();

        // Store event information in std::vector
        for (Int_t iHit = 0; iHit < nHits; iHit++)
        {
            auto *hit = (R3BTraPoint *)brClone->At(iHit);

            pID.push_back(hit->GetPid());
            footID.push_back(hit->GetDetCopyID());

            if (conf == "ideal")
            {
                if (footID.back() % 2 == 0)
                {
                    // pos.push_back(gRandom->Gaus(hit->GetYIn(), .2 * std::pow(10, -2)));
                    // pos.push_back(gRandom->Gaus(hit->GetXIn(), .2 * std::pow(10, -2)));
                    // pos.push_back(hit->GetZIn());

                    pos.push_back(hit->GetXIn());
                    pos.push_back(hit->GetYIn());
                    pos.push_back(hit->GetZIn());
                }
            }
        }

        // Only events in which we have signal in every foot pair
        if (footID != goodEvent)
        {
            continue;
        }

        // 25F in the for first detectors
        if (pID[0] != 1000090250)
        {
            continue;
        }

        if (pID[1] != 1000090250)
        {
            continue;
        }

        if (pID[2] != 1000090250)
        {
            continue;
        }

        if (pID[3] != 1000090250)
        {
            continue;
        }

        // In the lats pair we must see something different
        if ((pID[6] == 1000090250) || (pID[6] == 1000090240) || (pID[6] < 995 * std::pow(10, 6)))
        {
            continue;
        }

        if ((pID[7] == 1000090250) || (pID[7] == 1000090240) || (pID[7] < 995 * std::pow(10, 6)))
        {
            continue;
        }

        // Get the real vertex
        zFinal = -9999;
        pIDTrack = pID[7];
        nTracks = brClone1->GetEntries();

        for (Int_t iTrack = 0; iTrack < nTracks; iTrack++)
        {
            auto *track = (R3BMCTrack *)brClone1->At(iTrack);

            if ((track->GetPdgCode() == pIDTrack) && (std::abs(track->GetStartZ()) < 20))
                if (track->GetPdgCode() == pIDTrack)
                {
                    zFinal = track->GetStartZ();
                }
        }

        if (zFinal == -9999)
        {
            continue;
        }

        // Save the positions measured on the FOOTs
        for (Double_t posValue : pos)
        {
            file << posValue << " ";
        }

        // Save the z-vertex coordinate
        file << zFinal << "\n";
    }
}