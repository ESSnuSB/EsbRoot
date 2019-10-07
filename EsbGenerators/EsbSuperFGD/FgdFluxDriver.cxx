#include "EsbGenerators/EsbSuperFGD/FgdFluxDriver.h"

#include "Framework/ParticleData/PDGCodes.h"

#include "FairLogger.h"

using namespace std;

namespace esbroot {
namespace generators {
namespace superfgd {

FgdFluxDriver::FgdFluxDriver(const char* geoConfigFile
                            , const char* nuFluxFile
                            , unsigned int seed
                            , TVector3 detPos
                            , Double_t maxEnergy)
    :   fnuFluXFile(nuFluxFile)
        , fdetPos(detPos)
        , fseed(seed)
        , fdis(0.0, 1.0)
        , fpdgCode(0)
        , fMaxEv(maxEnergy)
        , fcurrentEvent(0)
{ 
    InitDetectorParams(geoConfigFile);
    InitPDGList();
    Init4Momentum();
    Init4Position();

    ReadNuFluxFile(fnuFluXFile.c_str());
    CalculateProbability();
}

bool FgdFluxDriver::GenerateNext(void)
{
    static int count = 0;
    count++;
    if(count>=3) throw "stop";

    Double_t rndVal = fdis(fseed);
    LOG(debug) << "rndVal " << rndVal;
    int nuPdg(0);
    Double_t nuEnergy(0.);

    for(size_t i = 0; i < fFlux.size(); ++i)
    {
        FgdFluxDriver::FLuxNeutrino& neutrino = fFlux[i];
        
        if(neutrino.GetNeutrino(rndVal, nuPdg, nuEnergy))
        {
            CalculateNext4position(rndVal);
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
void FgdFluxDriver::CalculateNext4position(Double_t rndVal)
{
    // Set the Position of the event
    Double_t rndm_X = fdetPos.X() + (f_total_X * rndVal - f_total_X/2);//*m_lunits; // X range from -X/2 to X/2
    Double_t rndm_Y = fdetPos.Y() + (f_total_Y * rndVal - f_total_Y/2);//*m_lunits; // Y range from -Y/2 to Y/2
    Double_t rndm_Z = fdetPos.Z() - f_total_Z/2; // Particle should start at beginning of the detector (z direction)

    f4position.SetX(rndm_X);
    f4position.SetY(rndm_Y);
    f4position.SetZ(rndm_Z);
    f4position.SetT(0);
}

void FgdFluxDriver::CalculateNext4Momentum(Double_t energyOfNeutrino)
{
    // calculate only pZ since beam is parallel to Z axis
    // neutrino is massless so p=E/c
    Double_t pZ = energyOfNeutrino;///CLHEP::c_light;
    Double_t pX = 0;
    Double_t pY = 0;

    f4momentum.SetPxPyPzE(pX,pY,pZ,energyOfNeutrino);
}

void FgdFluxDriver::InitPDGList(void)
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

void FgdFluxDriver::InitDetectorParams(const char* configFile)
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

void FgdFluxDriver::Init4Momentum(void)
{
    f4momentum.SetX(0.);
    f4momentum.SetY(0.);
    f4momentum.SetZ(0.);
    f4momentum.SetT(0);
}

void FgdFluxDriver::Init4Position(void)
{
    f4position.SetX(0.);
    f4position.SetY(0.);
    f4position.SetZ(0.);
    f4position.SetT(0);
}

void FgdFluxDriver::ReadNuFluxFile(const char* fluxFile)
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

void FgdFluxDriver::CalculateProbability()
{
    Double_t totalFlux(0.);
    for(size_t i = 0; i < fFlux.size(); ++i)
    {
        FgdFluxDriver::FLuxNeutrino* neutrino = &fFlux[i];
        totalFlux+= neutrino->GetFlux();
    }

    Double_t currentLowerFluxVal(0.);
    for(size_t i = 0; i < fFlux.size(); ++i)
    {
        FgdFluxDriver::FLuxNeutrino& neutrino = fFlux[i];
        neutrino.SetLowerVal(currentLowerFluxVal);
        Double_t uppVal = currentLowerFluxVal + neutrino.GetFlux()/totalFlux;
        neutrino.SetUpperVal(uppVal);
        currentLowerFluxVal = uppVal;
    }
}


FgdFluxDriver::FLuxNeutrino::FLuxNeutrino(int pdg , Double_t energy, Double_t flux)
    : fpdg(pdg), fEnergy(energy), fFluxValue(flux), fLowerVal(0.) , fUpperVal(0.)
{
}

FgdFluxDriver::FLuxNeutrino::~FLuxNeutrino()
{
}

FgdFluxDriver::FLuxNeutrino::FLuxNeutrino(const FLuxNeutrino& fn)
{
    fpdg = fn.fpdg;
    fEnergy = fn.fEnergy;
    fFluxValue = fn.fFluxValue;

    fLowerVal = fn.fLowerVal;
    fUpperVal = fn.fUpperVal;
}

FgdFluxDriver::FLuxNeutrino& FgdFluxDriver::FLuxNeutrino::operator=(const FLuxNeutrino& fn)
{
    fpdg = fn.fpdg;
    fEnergy = fn.fEnergy;
    fFluxValue = fn.fFluxValue;

    fLowerVal = fn.fLowerVal;
    fUpperVal = fn.fUpperVal;

    return *this;
}

Bool_t FgdFluxDriver::FLuxNeutrino::GetNeutrino(Double_t val, int& pdg, Double_t& energy)
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
