// This macro will calculate the difference between the reconstructed vertex
// and the real vertex to obtain the resolving power.

void error(TString fileName, TString fileOut)
{

    std::ifstream file(fileName);
    std::string line;
    Double_t realVertex, reconstructedVertex;

    auto *h = new TH1F("h", "Error (z_{rec} - z_{sim}) (cm)", 100, -20, 20);

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        iss >> reconstructedVertex >> realVertex;
        h->Fill(reconstructedVertex - realVertex);
    }

    h->Draw();

    auto *f = new TFile(fileOut, "RECREATE");
    h->Write();
}