#include "EsbReconstruction/EsbSuperFGD/FgdMuonRecon.h"

// Root headers
#include <TClonesArray.h>
#include <TFile.h>

// Fair headers
#include "FairLogger.h"

// Genie headers
#include "Framework/ParticleData/PDGCodes.h"

// STL headers
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>

namespace esbroot {
namespace reconstruction {
namespace superfgd {

// -----   Default constructor   -------------------------------------------
FgdMuonRecon::FgdMuonRecon() : FgdMCGenFitRecon()
    , foutputFile("")
    , fnuEFile("")
    , fEventCount(0)
    , fTfile(nullptr)
    , fTtree(nullptr)
    , fx(0.), fy(0.), fz(0.), fp(0.)
    , fx_fit(0.), fy_fit(0.), fz_fit(0.), fp_fit(0.)
    , fpMC_min_pFit(0.), fE_mu(0.), fxy(0.)
    , fzMC_min_zFit(0.)
    , fshouldCOnvergeToExport(false)
    , fcosTheta(0)
{ 
}
// -------------------------------------------------------------------------

// -----   Constructor   -------------------------------------------
FgdMuonRecon::FgdMuonRecon(const char* name
              , const char* geoConfigFile
              , const char* mediaFile
              , const char* outputFile
              , const char* nuEFile
              , Int_t verbose 
              , double debugLlv 
              , bool visualize 
              , std::string visOption)
    : FgdMCGenFitRecon(name, geoConfigFile, mediaFile, verbose, debugLlv, visualize, visOption)
    , foutputFile(outputFile)
    , fnuEFile(nuEFile)
    , fEventCount(0)
    , fTfile(nullptr)
    , fTtree(nullptr)
    , fx(0.), fy(0.), fz(0.), fp(0.)
    , fx_fit(0.), fy_fit(0.), fz_fit(0.), fp_fit(0.)
    , fpMC_min_pFit(0.), fE_mu(0.), fxy(0.)
    , fzMC_min_zFit(0.)
    , fshouldCOnvergeToExport(false)
    , fcosTheta(0)
{ 
  
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
FgdMuonRecon::~FgdMuonRecon() 
{
  if(fTfile)
  {
    delete fTfile;
  }
  
  if(fTtree)
  {
    delete fTtree;
  }
}
// -------------------------------------------------------------------------



// -----   Public method Init   --------------------------------------------
InitStatus FgdMuonRecon::Init() 
{   
    FgdMCGenFitRecon::Init();
  
    if(!foutputFile.empty())
    {
        fTfile = new TFile(foutputFile.c_str(), "RECREATE", "Fgd Muon only fit momentums");
        fTtree = new TTree("momentums", "Fitted momentums");

        // Set Monte carlo momentum
        fTtree->Branch("x_MC_momentum", &fx);
        fTtree->Branch("y_MC_momentum", &fy);
        fTtree->Branch("z_MC_momentum", &fz);
        fTtree->Branch("total_MC_momentum", &fp);

        // Set Genfit momentum
        fTtree->Branch("x_Fit_momentum", &fx_fit);
        fTtree->Branch("y_Fit_momentum", &fy_fit);
        fTtree->Branch("z_Fit_momentum", &fz_fit);
        fTtree->Branch("total_Fit_momentum", &fp_fit);

        fTtree->Branch("XY_MC_minus_XY_FIt", &fxy);
        fTtree->Branch("Z_MC_minus_Z_FIt", &fzMC_min_zFit);

        // Set statistics
        fTtree->Branch("P_monte_carlo_minus_P_genfit", &fpMC_min_pFit);
        fTtree->Branch("E_fit_subtract_nu_momentum", &fE_mu);
        
        // Set cos (Thetha)
        fTtree->Branch("cos_theta", &fcosTheta);
    }

    ReadEnuFile();

    return kSUCCESS;
}


// -------------------------------------------------------------------------




// -----   Public methods   --------------------------------------------

void FgdMuonRecon::FinishEvent() 
{
    FgdMCGenFitRecon::FinishEvent();
    LOG(debug) << "Neutrino energy for event[" << fEnu[fEventCount]<<" GeV]";
    ++fEventCount;
}

void FgdMuonRecon::FinishTask()
{
    FgdMCGenFitRecon::FinishTask();
    if(!foutputFile.empty())
    {
        fTfile->WriteTObject(fTtree);
        fTfile->Close();
    }
}
// -------------------------------------------------------------------------

void FgdMuonRecon::WriteOutput( Int_t pdg
                            ,   const TVector3& fitMom
                            ,   const TVector3& mcMom
                            ,   const genfit::Track& fitTrack
                            ,   genfit::FitStatus*& fiStatuStatus)
{
    Bool_t isMUon = (pdg==genie::kPdgMuon) || (pdg == genie::kPdgAntiMuon);
    static Double_t radToDeg = 180./TMath::Pi();

    if(fiStatuStatus->isFitConverged() && isMUon && fTtree!=nullptr && fEventCount< fEnu.size())
    {
        // Set Monte Carlo momentum
        fx = mcMom.X();
        fy = mcMom.Y();
        fz = mcMom.Z();
        fp = mcMom.Mag();

        // Set Genfit momentum
        fx_fit = fitMom.X();
        fy_fit = fitMom.Y();
        fz_fit = fitMom.Z();
        fp_fit = fitMom.Mag();

        double tempXY_mc = sqrt(fx*fx + fy*fy);
        double tempXY_fit = sqrt(fx_fit*fx_fit + fy_fit*fy_fit);
        fxy = tempXY_mc - tempXY_fit;

        fzMC_min_zFit = fz - fz_fit;

        fpMC_min_pFit = mcMom.Mag() - fitMom.Mag();
        fE_mu = sqrt(
                        0.106*0.106       /* mass of muon */ 
                        + fp_fit*fp_fit   /* fitted momentum of muon */
                    )
                - fEnu[fEventCount];          /* Initial energy of neutrino */

        TVector3 muMom(0,0,1);
        fcosTheta = radToDeg * muMom.Angle(muMom);

        fTtree->Fill();
    }
}

// -----   Private methods   --------------------------------------------
void FgdMuonRecon::ReadEnuFile()
{
    std::string nuEfile(fnuEFile);

    if(!nuEfile.empty())
    {
        const char spaceChar(' ');
        std::ifstream nuFileStream;

        try
        {
            nuFileStream.open(nuEfile.c_str(), std::ios::in);

            if(nuFileStream.is_open())
            {
                std::string line;
                while(std::getline(nuFileStream,line))
                {
                    std::istringstream ss(line);
                    std::string token;
                    Int_t lineCount = 1;
                    while(std::getline(ss, token, spaceChar))
                    {
                        // Write only the neutrino energy
                        if(lineCount==2 && !token.empty())
                        {
                            Double_t&& neutrinoEnergy = std::stod(token);
                            fEnu.push_back(neutrinoEnergy);
                        }
                        ++lineCount;
                    }
                }
            }
        }
        catch(const std::exception& e)
        {
            LOG(fatal) << e.what();
        }

        if(nuFileStream.is_open())
        {
            nuFileStream.close();
        }
    }
}


// -------------------------------------------------------------------------

}// namespace superfgd
}// namespace reconstruction
}// namespace esbroot
