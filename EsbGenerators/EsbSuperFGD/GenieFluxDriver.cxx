#include "EsbGenerators/EsbSuperFGD/GenieFluxDriver.h"

#include "Framework/ParticleData/PDGCodes.h"
#include <Framework/Conventions/Units.h>

#include "FairLogger.h"

#include <iostream>

using namespace std;

namespace esbroot {
namespace generators {
namespace superfgd {

GenieFluxDriver::GenieFluxDriver(const char* geoConfigFile
                            , const char* nuFluxFile
                            , unsigned int seed
                            , TVector3 detPos
                            , Int_t maxEvents
                            , Double_t maxEnergy)
    :   fnuFluXFile(nuFluxFile)
        , fdetPos(detPos)
        , frndGen(seed)
        , fdis(0.0, 1.0)
        , fpdgCode(0)
        , fMaxEv(maxEnergy)
        , fcurrentEvent(0)
        , fmaxEvents(maxEvents)
{ 
    InitDetectorParams(geoConfigFile);
    InitPDGList();
    Init4Momentum();
    Init4Position();

    ReadNuFluxFile(fnuFluXFile.c_str());
    CalculateProbability();
}

GenieFluxDriver::GenieFluxDriver(const GenieFluxDriver& gf)
{
    this->fnuFluXFile = gf.fnuFluXFile;
    this->fdetPos = gf.fdetPos;
    this->frndGen = gf.frndGen;
    this->fdis = gf.fdis;
    
    this->fMaxEv = gf.fMaxEv;
    this->fpdgCode = gf.fpdgCode;
    this->fcurrentEvent = gf.fcurrentEvent;

    this->fdetectorParams = const_cast<esbroot::geometry::superfgd::FgdDetectorParameters&>(gf.fdetectorParams);
    this->fPdgCList = gf.fPdgCList;
    this->f4momentum = gf.f4momentum;
    this->f4position = gf.f4position;
    this->f4AbsPos = gf.f4AbsPos;

    this->f_total_X = gf.f_total_X;
    this->f_total_Y = gf.f_total_Y;
    this->f_total_Z = gf.f_total_Z;
    this->fnuFluXFile = gf.fnuFluXFile;
    this->fFlux = gf.fFlux;
    this->fmaxEvents = fmaxEvents;
}

GenieFluxDriver& GenieFluxDriver::operator=(const GenieFluxDriver& gf)
{
    this->fnuFluXFile = gf.fnuFluXFile;
    this->fdetPos = gf.fdetPos;
    this->frndGen = gf.frndGen;
    this->fdis = gf.fdis;

    this->fMaxEv = gf.fMaxEv;
    this->fpdgCode = gf.fpdgCode;
    this->fcurrentEvent = gf.fcurrentEvent;

    this->fdetectorParams = const_cast<esbroot::geometry::superfgd::FgdDetectorParameters&>(gf.fdetectorParams);
    this->fPdgCList = gf.fPdgCList;
    this->f4momentum = gf.f4momentum;
    this->f4position = gf.f4position;
    this->f4AbsPos = gf.f4AbsPos;

    this->f_total_X = gf.f_total_X;
    this->f_total_Y = gf.f_total_Y;
    this->f_total_Z = gf.f_total_Z;
    this->fnuFluXFile = gf.fnuFluXFile;
    this->fFlux = gf.fFlux;
    this->fmaxEvents = fmaxEvents;
}

bool GenieFluxDriver::GenerateNext(void)
{
    int nuPdg(0);
    Double_t nuEnergy(0.);
    int ret = 0;
    Double_t nextNu = fdis(frndGen);

    for(size_t i = 0; i < fFlux.size(); ++i)
    {
        GenieFluxDriver::FLuxNeutrino& neutrino = fFlux[i];
        
        if(neutrino.GetNeutrino(nextNu, nuPdg, nuEnergy))
        {
            CalculateNext4position();
            CalculateNext4Momentum(nuEnergy);
            fpdgCode = nuPdg;
            
            fcurrentEvent++;

            return true;
        }        
    }

    return false;
}

//-------------------------------------------------------
//                  Private methods
//-------------------------------------------------------
void GenieFluxDriver::CalculateNext4position()
{
    Double_t x_det = (f_total_X * fdis(frndGen) - f_total_X/2);
    Double_t y_det = (f_total_Y * fdis(frndGen) - f_total_Y/2);
    Double_t z_det = (f_total_Z * fdis(frndGen) - f_total_Z/2);

    // Set the Position of the event
    Double_t rndm_X = fdetPos.X() + x_det;
    Double_t rndm_Y = fdetPos.Y() + y_det;
    Double_t rndm_Z = fdetPos.Z() + z_det;

    f4AbsPos.SetX(rndm_X);
    f4AbsPos.SetY(rndm_Y);
    f4AbsPos.SetZ(rndm_Z);
    f4AbsPos.SetT(0);

    /* For the moment the 4position is all zeros, till we know how to convert from genie units to ours? */
    f4position.SetX(0.);
    f4position.SetY(0.);
    f4position.SetZ(0.);
    f4position.SetT(0.);
}

void GenieFluxDriver::CalculateNext4Momentum(Double_t energyOfNeutrino)
{
    // calculate only pZ since beam is parallel to Z axis
    // neutrino is massless so p=E/c
    Double_t pZ = energyOfNeutrino;///CLHEP::c_light;
    Double_t pX = 0;
    Double_t pY = 0;

    f4momentum.SetPxPyPzE(pX,pY,pZ,energyOfNeutrino);
}

void GenieFluxDriver::InitPDGList(void)
{
    /* The neutrinos are added in the list in the order in which they are expected to appear in the flux file*/

    // Matter neutrinos
    fPdgCList.push_back(genie::kPdgNuE);        // Electron neutrino pdgCode = 12
    fPdgCList.push_back(genie::kPdgNuMu);       // Muon neutrino pdgCode = 14
    fPdgCList.push_back(genie::kPdgNuTau);      // Electron tau pdgCode = 16


    // Antimatter neutrinos
    fPdgCList.push_back(genie::kPdgAntiNuE);    // Electron antineutrino pdgCode = -12
    fPdgCList.push_back(genie::kPdgAntiNuMu);   // Muon antineutrino pdgCode = -14
    fPdgCList.push_back(genie::kPdgAntiNuTau);  // Tau antineutrino pdgCode = -16
}

void GenieFluxDriver::InitDetectorParams(const char* configFile)
{
    fdetectorParams.LoadPartParams(configFile);

    Double_t lunit = fdetectorParams.GetLenghtUnit(); // [cm]

    Double_t step_X  = fdetectorParams.ParamAsDouble(esbroot::geometry::superfgd::DP::length_X) * lunit;
    Double_t step_Y  = fdetectorParams.ParamAsDouble(esbroot::geometry::superfgd::DP::length_Y) * lunit;
    Double_t step_Z  = fdetectorParams.ParamAsDouble(esbroot::geometry::superfgd::DP::length_Z) * lunit;

    Int_t bin_X = fdetectorParams.ParamAsInt(esbroot::geometry::superfgd::DP::number_cubes_X);
    Int_t bin_Y = fdetectorParams.ParamAsInt(esbroot::geometry::superfgd::DP::number_cubes_Y);
    Int_t bin_Z = fdetectorParams.ParamAsInt(esbroot::geometry::superfgd::DP::number_cubes_Z);

    f_total_X = step_X * bin_X;
    f_total_Y = step_Y * bin_Y;
    f_total_Z = step_Z * bin_Z;
}

void GenieFluxDriver::Init4Momentum(void)
{
    f4momentum.SetX(0.);
    f4momentum.SetY(0.);
    f4momentum.SetZ(0.);
    f4momentum.SetT(0);
}

void GenieFluxDriver::Init4Position(void)
{
    f4position.SetX(0.);
    f4position.SetY(0.);
    f4position.SetZ(0.);
    f4position.SetT(0);

    f4AbsPos.SetX(0.);
    f4AbsPos.SetY(0.);
    f4AbsPos.SetZ(0.);
    f4AbsPos.SetT(0);
}

void GenieFluxDriver::ReadNuFluxFile(const char* fluxFile)
{
    std::string nufluxFile(fluxFile);

    if(!nufluxFile.empty())
    {
        const char spaceChar(' ');
        std::ifstream fluxFileStream;

        try
        {
            fluxFileStream.open(nufluxFile.c_str(), std::ios::in);

            if(fluxFileStream.is_open())
            {
                std::string line;
                while(std::getline(fluxFileStream,line))
                {
                    Double_t arr[] = {0,0,0,0,0,0,0};
                    int arrInd(0);

                    std::istringstream ss(line);
                    std::string token;
                    while( (arrInd<7) && std::getline(ss, token, spaceChar))
                    {
                        if(!token.empty())
                        {
                            Double_t parsedVal = std::stod(token);
                            arr[arrInd++] = parsedVal;
                        }
                    }

                    /* The first value is the energy value for all neutrinos on the line */
                    Double_t energy = arr[0];

                    /* The neutrinos are added in the list in the order in which they are expected to appear in the flux file*/
                    for(size_t i = 1; i < 7; ++i)
                    {
                        if(arr[i] != 0)
                        {
                            fFlux.emplace_back(fPdgCList[i-1], energy, arr[i]);
                        }
                    }
                }
            }
        }
        catch(const std::exception& e)
        {
            LOG(fatal) << e.what();
        }

        if(fluxFileStream.is_open())
        {
            fluxFileStream.close();
        }
    }
}

void GenieFluxDriver::CalculateProbability()
{
    Double_t totalFlux(0.);
    for(size_t i = 0; i < fFlux.size(); ++i)
    {
        GenieFluxDriver::FLuxNeutrino* neutrino = &fFlux[i];
        totalFlux+= neutrino->GetFlux();
    }

    Double_t currentLowerFluxVal(0.);
    for(size_t i = 0; i < fFlux.size(); ++i)
    {
        GenieFluxDriver::FLuxNeutrino& neutrino = fFlux[i];
        neutrino.SetLowerVal(currentLowerFluxVal);
        Double_t uppVal = currentLowerFluxVal + neutrino.GetFlux()/totalFlux;
        neutrino.SetUpperVal(uppVal);
        currentLowerFluxVal = uppVal;
    }
}


GenieFluxDriver::FLuxNeutrino::FLuxNeutrino(int pdg , Double_t energy, Double_t flux)
    : fpdg(pdg), fEnergy(energy), fFluxValue(flux), fLowerVal(0.) , fUpperVal(0.)
{
}

GenieFluxDriver::FLuxNeutrino::~FLuxNeutrino()
{
}

GenieFluxDriver::FLuxNeutrino::FLuxNeutrino(const FLuxNeutrino& fn)
{
    fpdg = fn.fpdg;
    fEnergy = fn.fEnergy;
    fFluxValue = fn.fFluxValue;

    fLowerVal = fn.fLowerVal;
    fUpperVal = fn.fUpperVal;
}

GenieFluxDriver::FLuxNeutrino& GenieFluxDriver::FLuxNeutrino::operator=(const FLuxNeutrino& fn)
{
    fpdg = fn.fpdg;
    fEnergy = fn.fEnergy;
    fFluxValue = fn.fFluxValue;

    fLowerVal = fn.fLowerVal;
    fUpperVal = fn.fUpperVal;

    return *this;
}

Bool_t GenieFluxDriver::FLuxNeutrino::GetNeutrino(Double_t val, int& pdg, Double_t& energy)
{
    if(val>=fLowerVal && val <fUpperVal)
    {
        pdg = fpdg;
        energy = fEnergy;
        return true;
    }
    return false;
}

} //namespace superfgd 
} //namespace generators
} //namespace esbroot
