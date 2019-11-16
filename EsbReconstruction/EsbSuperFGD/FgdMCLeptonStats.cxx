#include "EsbReconstruction/EsbSuperFGD/FgdMCLeptonStats.h"
#include "EsbReconstruction/EsbSuperFGD/FgdReconTemplate.h"
//#include "EsbData/EsbSuperFGD/FgdDetectorPoint.h"

// FairRoot headers
#include "FairGeoBuilder.h"
#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoMedia.h"
#include "FairLogger.h"
#include <FairRootManager.h>
#include "FairVolume.h"


// Root headers
#include <TClonesArray.h>
#include <TEveManager.h>
#include <TGeoElement.h>
#include <TGeoManager.h>
#include <TFile.h>
#include <TTree.h>

// Genie headers
#include "Framework/ParticleData/PDGCodes.h"
#include "Framework/ParticleData/PDGUtils.h"



// STL headers
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include <math.h>
#include <bits/stdc++.h>

namespace esbroot {
namespace reconstruction {
namespace superfgd {

// -----   Default constructor   -------------------------------------------
FgdMCLeptonStats::FgdMCLeptonStats() : FgdMCGenFitRecon()
{
}
// -------------------------------------------------------------------------

// -----   Constructor   -------------------------------------------
FgdMCLeptonStats::FgdMCLeptonStats(const char* name
                          , const char* geoConfigFile
                          , const char* mediaFile
                          , const char* eventData
                          , const char* outputRootFile
                          , Int_t verbose
                          , double debugLlv
                          , bool visualize
                          , std::string visOption) :
  FgdMCGenFitRecon(name, geoConfigFile, mediaFile, verbose, 
                    debugLlv, visualize, visOption)
    , feventData(eventData), foutputRootFile(outputRootFile)
{ 
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
FgdMCLeptonStats::~FgdMCLeptonStats() 
{
    if(fTracksArray) 
    {
        fTracksArray->Delete();
        delete fTracksArray;
    }
}
// -------------------------------------------------------------------------



// -----   Public method Init   --------------------------------------------
InitStatus FgdMCLeptonStats::Init() 
{   
    FgdMCGenFitRecon::Init();

    std::ifstream eventFileStream;
    try
    {        
        eventFileStream.open(feventData.c_str(), std::ios::in);

        if(eventFileStream.is_open())
        {
            std::string line;
            while(std::getline(eventFileStream,line))
            {
                feventRecords.emplace_back(FgdMCEventRecord(line));
            }
        }
    }
    catch(const std::exception& e)
    {
        LOG(fatal) << e.what();
    }

    if(eventFileStream.is_open())
    {
        eventFileStream.close();
    }

    // for(size_t i =0 ; i < feventRecords.size(); i++)
    // {
    //     LOG(info) << " ===========  event " << i <<  " =========== ";
    //     LOG(info) << feventRecords[i].GetNuPdg() 
    //                 << " " << feventRecords[i].GetNuE()
    //                 << " " << feventRecords[i].IsWeakCC()
    //                 << " " << feventRecords[i].IsWeakNC()
    //                 << " " << feventRecords[i].IsQuasiElastic();
        
    //     auto par = feventRecords[i].GetPrimaryParticles();
    //     for(size_t j = 0; j < par.size(); ++j)
    //     {
    //         LOG(info) << par[j].first << " " << par[j].second.X() << " " << par[j].second.Y()<< " " << par[j].second.Z(); 
    //     }
    //     LOG(info) << " ====================== ";
    // }
    

    return kSUCCESS;
}

void FgdMCLeptonStats::OutputFileInit(FairRootManager* manager)
{
    FgdMCGenFitRecon::OutputFileInit(manager);
    // Create and register output array
    // fEventsArray = new TClonesArray(esbroot::reconstruction::superfgd::FgdMCEventRecord::Class(), 1000);
    // manager->Register(esbroot::geometry::superfgd::DP::FGD_MC_LEPTON_RECONSTRUCTION_ROOT_FILE.c_str()
    //                     , esbroot::geometry::superfgd::DP::FGD_MC_LEPTON_RECONSTRUCTION_BRANCH.c_str()
    //                     , fEventsArray, kTRUE);
}

// -------------------------------------------------------------------------



// -----   Public methods   --------------------------------------------

void FgdMCLeptonStats::Exec(Option_t* opt) 
{  
  try
  {
    std::vector<ReconHit> allhits;

    bool rc = FgdMCGenFitRecon::GetHits(allhits);

    if(rc)
    { 
      
    }
  }
  catch(genfit::Exception& e)
  {
      LOG(fatal) << "Exception, when tryng to fit track";
      LOG(fatal) << e.what();
  }
}

void FgdMCLeptonStats::FinishTask()
{
    TFile * outFile = new TFile(foutputRootFile.c_str(), "RECREATE", "Reconstruction data from Fgd Detector");
    outFile->SetCompressionLevel(9);

    FgdMCEventRecord* data = nullptr;
    TTree * outTree = new TTree(esbroot::geometry::superfgd::DP::FGD_MC_LEPTON_RECONSTRUCTION_TTREE.c_str()
                                ,esbroot::geometry::superfgd::DP::FGD_MC_LEPTON_RECONSTRUCTION_ROOT_FILE.c_str());
    outTree->Branch(esbroot::geometry::superfgd::DP::FGD_MC_LEPTON_RECONSTRUCTION_BRANCH.c_str(), &data);


    // TClonesArray& clref = *fEventsArray;

    for(size_t ind = 0 ; ind < feventRecords.size(); ind++)
    {
        // LOG(info) << "writing to output file " << ind;
        // auto test = new(clref[clref.GetEntries()]) esbroot::reconstruction::superfgd::FgdMCEventRecord(feventRecords[ind]);
        // LOG(info) << "Is null " << (test == nullptr);

        data = &feventRecords[ind];
        outTree->Fill();
    }

    outFile->WriteTObject(outTree);
    outFile->Close();
    
    delete outFile;

    FgdMCGenFitRecon::FinishTask();
}
// -------------------------------------------------------------------------


// -----   Protected methods   --------------------------------------------

// -------------------------------------------------------------------------

}// namespace superfgd
}// namespace reconstruction
}// namespace esbroot