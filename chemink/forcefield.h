#pragma once

#include <string>

class ForceField {
public:
    ForceField();

    ~ForceField();

    std::string getCoordinate(
            const char *inData,
            const char *inType = "smiles",
            const char *outType = "mol2",
            const char *forcefield = "uff"
    );

    double getEnergy(
            const char *inData,
            const char *inType = "mol2",
            const char *forcefield = "uff"
    );

    double getEnergy();

private:
    double energy;
    static bool pluginLoaded;
};
