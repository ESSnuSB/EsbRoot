#include "EsbDigitizer/EsbSuperFGD/FgdMppcDisplay.h"
#include "EsbData/EsbSuperFGD/FgdDetectorPoint.h"

#include <TClonesArray.h>
#include <TSpline.h>
#include <TRandom.h>

#include <FairRootManager.h>
#include "FairLogger.h"

#include "EsbData/WCDetectorPoint.h"
#include "EsbGeometry/PMTube.h"
#include "EsbData/PMTubeHit.h"

#include <iostream>
using std::cout;
using std::endl;

namespace esbroot {
namespace digitizer {
namespace superfgd {

// -----   Default constructor   -------------------------------------------
FgdMppcDisplay::FgdMppcDisplay() :
  FairTask(), fX(0), fY(0), fZ(0)
{ 
}
// -------------------------------------------------------------------------

// -----   Constructor   -------------------------------------------
FgdMppcDisplay::FgdMppcDisplay(const char* name
                          ,const char* geoConfigFile
                          ,double x, double y, double z
                          , Int_t verbose) :
  FairTask(name, verbose), fX(x), fY(y), fZ(z)
{ 
  fParams.LoadPartParams(geoConfigFile);
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
FgdMppcDisplay::~FgdMppcDisplay() 
{
}
// -------------------------------------------------------------------------



// -----   Public method Init   --------------------------------------------
InitStatus FgdMppcDisplay::Init() 
{   
  flunit = fParams.GetLenghtUnit(); // [cm]

  f_step_X  = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::length_X) * flunit;
  f_step_Y  = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::length_Y) * flunit;
  f_step_Z  = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::length_Z) * flunit;

  f_bin_X = fParams.ParamAsInt(esbroot::geometry::superfgd::DP::number_cubes_X);
  f_bin_Y = fParams.ParamAsInt(esbroot::geometry::superfgd::DP::number_cubes_Y);
  f_bin_Z = fParams.ParamAsInt(esbroot::geometry::superfgd::DP::number_cubes_Z);

  f_total_X = f_step_X * f_bin_X;
  f_total_Y = f_step_Y * f_bin_Y;
  f_total_Z = f_step_Z * f_bin_Z;

  // Get RootManager
  FairRootManager* manager = FairRootManager::Instance();
  if ( !manager ) {
    LOG(error) << "-E- FgdDigitizer::Init: " << "FairRootManager not instantised!";
    return kFATAL;
  }

  fHitArray = (TClonesArray*) manager->GetObject(geometry::superfgd::DP::FGD_HIT.c_str());
  if (!fHitArray) 
  {
      LOG(fatal) << "Exec", "No fgd hits array";
      return kFATAL;
  }

  return kSUCCESS;
}


// -------------------------------------------------------------------------



// -----   Public method Exec   --------------------------------------------
void FgdMppcDisplay::Exec(Option_t* opt) 
{
  const Int_t hits = fHitArray->GetEntries();
  for(Int_t i =0; i < hits; i++)
  {
      data::superfgd::FgdHit* hit = (data::superfgd::FgdHit*)fHitArray->At(i);
      hit->Print(nullptr);
  }
}
// -------------------------------------------------------------------------
  
}// namespace superfgd
}// namespace digitizer
}// namespace esbroot
