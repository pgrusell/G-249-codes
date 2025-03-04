// This macro simulates an incoming beam that passes through two
// ideal FOOTs (ideal in the sense that they can measure both x and y
// coordinates of the beam, as well as that they do not interact with it).
// The beam will impinge on a LH2 target of 15 cm length and react on a
// random point inside the target. The reaction will consist of a random angular
// deviation of the incoming fragment up to a thetaMax angel. Then two FOOTs,
// whose positions are z1 and z2, will measure the outcoming fragment.

// This macro generates a file (fileName.txt) with the (x, y, z) coordinates detected
// by the FOOTs, as well as the z-position of the real vertex. A total of nEvents will
// be generated.

void rPos(Double_t z, Double_t z0, Double_t theta, Double_t phi, Double_t &x, Double_t &y)
{

    Double_t r = (z - z0) / TMath::Cos(theta);

    x = r * TMath::Cos(phi) * TMath::Sin(theta);
    y = r * TMath::Sin(phi) * TMath::Sin(theta);
}

void sim_ideal(Int_t nEvents, Double_t thetaMax, Double_t z1, Double_t z2, TString fileName, Double_t inc)
{
    auto *gRandom = new TRandom(0);

    thetaMax *= TMath::DegToRad();

    Double_t x, y;
    Double_t r, theta, phi;
    Double_t z0;

    std::ofstream file(fileName);

    for (Int_t iEvent = 0; iEvent < nEvents; iEvent++)
    {
        std::vector<Double_t> pos = {0., 0., -85., 0., 0., -68.};

        // Generate theta (0, thetaMax) and phi (0, 2pi) randomly
        theta = gRandom->Uniform(thetaMax);
        phi = gRandom->Uniform(2 * TMath::Pi());

        // Generate the interaction point
        z0 = gRandom->Uniform(15.);

        // Calculate x and y for z = z1
        rPos(z1, z0, theta, phi, x, y);

        // Assert the fragment is seen by the FOOT
        if ((std::abs(x) > 2.5) || (std::abs(y) > 2.5))
        {
            continue;
        }

        pos.push_back(gRandom->Gaus(x, inc));
        pos.push_back(gRandom->Gaus(y, inc));
        pos.push_back(z1);

        // Calculate x and y for z = z2
        rPos(z2, z0, theta, phi, x, y);

        // Assert the fragment is seen by the FOOT
        if ((std::abs(x) > 2.5) || (std::abs(y) > 2.5))
        {
            continue;
        }

        pos.push_back(gRandom->Gaus(x, inc));
        pos.push_back(gRandom->Gaus(y, inc));
        pos.push_back(z2);
        pos.push_back(z0);

        for (Double_t xi : pos)
        {
            file << xi << " ";
        }

        file << "\n";
    }
}