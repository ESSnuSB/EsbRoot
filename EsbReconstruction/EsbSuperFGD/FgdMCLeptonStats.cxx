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
        const char spaceChar(' ');
        
        eventFileStream.open(feventData.c_str(), std::ios::in);

        if(eventFileStream.is_open())
        {
            std::string line;
            while(std::getline(eventFileStream,line))
            {
                // 1st value is the pdg of the neutrino
                // 2nd value is the neutrino energy
                // 3th value is IsWeakCC - charge current
                // 4th value is IsWeakNC - neutral current
                int counter(0);
                FgdMCEventRecord eventRec;

                std::istringstream ss(line);
                std::string token;
                while( (counter<4) && std::getline(ss, token, spaceChar))
                {
                    // 1st value is the pdg of the neutrino
                    if(!token.empty() && counter == 0)
                    {
                        Int_t parsedVal = std::stoi(token);
                        eventRec.SetNuPdg(parsedVal);
                    }

                    // 2nd value is the neutrino energy
                    if(!token.empty() && counter == 1)
                    {
                        Double_t parsedVal = std::stod(token);
                        eventRec.SetNuE(parsedVal);
                    }

                    // 3th value is IsWeakCC - charge current
                    if(!token.empty() && counter == 2)
                    {
                        Int_t parsedVal = std::stoi(token);
                        eventRec.SetWeakCC(parsedVal);
                    }

                    // 4th value is IsWeakNC - neutral current
                    if(!token.empty() && counter == 3)
                    {
                        Int_t parsedVal = std::stoi(token);
                        eventRec.SetWeakNC(parsedVal);
                    }

                    counter++;
                }

                feventRecords.emplace_back(eventRec);

                LOG(debug) << eventRec.GetNuPdg() << " " << eventRec.GetNuE() << " " << eventRec.GetWeakCC() << " " << eventRec.GetWeakNC();
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
