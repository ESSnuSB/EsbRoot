#include "EsbReconstruction/EsbSuperFGD/FgdMCLeptonStats.h"
#include "EsbReconstruction/EsbSuperFGD/FgdReconTemplate.h"
#include "EsbData/EsbSuperFGD/FgdDetectorPoint.h"

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
                          , Int_t verbose
                          , double debugLlv
                          , bool visualize
                          , std::string visOption) :
  FgdMCGenFitRecon(name, geoConfigFile, mediaFile, verbose, 
                    debugLlv, visualize, visOption)
    , feventData(eventData)
{ 
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
FgdMCLeptonStats::~FgdMCLeptonStats() 
{
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
// -------------------------------------------------------------------------


// -----   Protected methods   --------------------------------------------

// -------------------------------------------------------------------------

}// namespace superfgd
}// namespace reconstruction
}// namespace esbroot
