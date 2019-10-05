#include "EsbGenerators/EsbSuperFGD/FgdFluxDriver.h"

#include <iostream>
#include <random>

using namespace std;

namespace esbroot {
namespace generators {
namespace superfgd {

FgdFluxDriver::FgdFluxDriver(const char* geoConfigFile, unsigned int seed)
    : fseed(seed)
{ 
    fdetectorParams.LoadPartParams(geoConfigFile);

    Double_t lunit = fdetectorParams.GetLenghtUnit(); // [cm]

    Double_t step_X  = fdetectorParams.ParamAsDouble(esbroot::geometry::superfgd::DP::length_X) * lunit;
    Double_t step_Y  = fdetectorParams.ParamAsDouble(esbroot::geometry::superfgd::DP::length_Y) * lunit;
    Double_t step_Z  = fdetectorParams.ParamAsDouble(esbroot::geometry::superfgd::DP::length_Z) * lunit;

    Double_t bin_X = fdetectorParams.ParamAsInt(esbroot::geometry::superfgd::DP::number_cubes_X);
    Double_t bin_Y = fdetectorParams.ParamAsInt(esbroot::geometry::superfgd::DP::number_cubes_Y);
    Double_t bin_Z = fdetectorParams.ParamAsInt(esbroot::geometry::superfgd::DP::number_cubes_Z);

    f_total_X = step_X * bin_X;
    f_total_Y = step_Y * bin_Y;
    f_total_Z = step_Z * bin_Z;
}

bool FgdFluxDriver::GenerateNext(void)
{
    calculateNext4position();
    calculateNext4Momentum();
    fcurrentEvent++;
    return true;
}

//-------------------------------------------------------
//                  Private methods
//-------------------------------------------------------
void FgdFluxDriver::calculateNext4position(void)
{
    std::mt19937 gen(fseed); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<Double_t> dist(0.0, 1.0);

    // Set the Position of the event
    Double_t rndm_X = (f_total_X * dist(gen) - f_total_X/2);//*m_lunits; // X range from -X/2 to X/2
    Double_t rndm_Y = (f_total_Y * dist(gen) - f_total_Y/2);//*m_lunits; // Y range from -Y/2 to Y/2
    Double_t rndm_Z = -f_total_Z/2; // Particle should start at beginning of the detector (z direction)

    f4position.SetX(rndm_X);
    f4position.SetY(rndm_Y);
    f4position.SetZ(rndm_Z);
    f4position.SetT(0);
}

void FgdFluxDriver::calculateNext4Momentum(void)
{
    double energyOfNeutrino = fMaxEv;

    // calculate only pZ since beam is parallel to Z axis
    // neutrino is massless so p=E/c
    double pZ = energyOfNeutrino;///CLHEP::c_light;
    double pX = 0;
    double pY = 0;

    f4momentum.SetPxPyPzE(pX,pY,pZ,energyOfNeutrino);
}

void FgdFluxDriver::calculateNextPdgCode(void){
    if(ffPdgCList.size()>0)
    {
        fpdgCode = ffPdgCList[0]; // assuming the first element is a muon neutrino
    }
}

} //namespace superfgd 
} //namespace generators
} //namespace esbroot
