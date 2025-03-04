// This macro takes as input the coordinates (x, y, z) measured by each FOOT
// in each event as well as the real z-vertex. It will calculate analytically
// the reconstructed vertex by calculating the nearest point of the lines
// defined by 2 initial point with the 2 final points.

// As a result, a the program creates a file with the reconstructed vertex and
// the real vertex.

Double_t cutPoint(std::vector<Double_t> Pos)
{

    Double_t xi = Pos[0];
    Double_t yi = Pos[1];
    Double_t zi = Pos[2];

    Double_t xf = Pos[3];
    Double_t yf = Pos[4];
    Double_t zf = Pos[5];

    Double_t Xi = Pos[6];
    Double_t Yi = Pos[7];
    Double_t Zi = Pos[8];

    Double_t Xf = Pos[9];
    Double_t Yf = Pos[10];
    Double_t Zf = Pos[11];

    Double_t dx = xf - xi;
    Double_t dy = yf - yi;
    Double_t dz = zf - zi;

    Double_t DX = Xf - Xi;
    Double_t DY = Yf - Yi;
    Double_t DZ = Zf - Zi;

    Double_t A = dx * dx + dy * dy + dz * dz;
    Double_t C = DX * DX + DY * DY + DZ * DZ;
    Double_t B = dx * DX + dy * DY + dz * DZ;

    Double_t wx = xi - Xi;
    Double_t wy = yi - Yi;
    Double_t wz = zi - Zi;

    Double_t E = wx * dx + wy * dy + wz * dz;
    Double_t F = wx * DX + wy * DY + wz * DZ;

    Double_t denom = A * C - B * B;

    Double_t num = (zf - zi) * (B * F - C * E) + (Zf - Zi) * (A * F - B * E);

    Double_t zMean = 0.5 * (zi + Zi + num / denom);

    return zMean;
}

void anareco(TString inFileName, TString outFileName)
{
    std::ifstream inFile(inFileName);
    std::ofstream outFile(outFileName);
    std::string line;

    Double_t zVertex;
    Double_t zReal;

    while (std::getline(inFile, line))
    {
        std::istringstream iss(line);
        std::vector<Double_t> values;
        std::vector<Double_t> pos;
        Double_t value;

        while (iss >> value)
        {
            values.push_back(value);
        }

        pos = {values.begin(), values.end() - 1};

        // Calculate the z-coordinate of the reconstructed vertex
        zVertex = cutPoint(pos);
        zReal = values.back();

        outFile << zVertex << " " << zReal << "\n";
    }
}