#include "EsbDigitizer/EsbSuperFGD/FgdMppcDisplay.h"
#include "EsbData/EsbSuperFGD/FgdDetectorPoint.h"

#include <TClonesArray.h>
#include <TSpline.h>
#include <TRandom.h>
#include <TH2F.h>

#include <FairRootManager.h>
#include "FairLogger.h"

#include "EsbData/WCDetectorPoint.h"
#include "EsbGeometry/PMTube.h"
#include "EsbData/PMTubeHit.h"

#include <iostream>
#include <sstream>
using std::cout;
using std::endl;

namespace esbroot {
namespace digitizer {
namespace superfgd {

// -----   Default constructor   -------------------------------------------
FgdMppcDisplay::FgdMppcDisplay() :
  FairTask(), fX(0), fY(0), fZ(0),fevNum(0),
  f_xy_hist(nullptr), f_yz_hist(nullptr), f_xz_hist(nullptr), fHitArray(nullptr)
  , f_photo_ave(0.),f_photo_count(0)
{ 
}
// -------------------------------------------------------------------------

// -----   Constructor   -------------------------------------------
FgdMppcDisplay::FgdMppcDisplay(const char* name
                          ,const char* geoConfigFile
                          ,double x, double y, double z
                          , Int_t verbose) :
  FairTask(name, verbose), fX(x), fY(y), fZ(z),fevNum(0),
  f_xy_hist(nullptr), f_yz_hist(nullptr), f_xz_hist(nullptr), fHitArray(nullptr)
  , f_photo_ave(0.), f_photo_count(0)
{ 
  fParams.LoadPartParams(geoConfigFile);
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
FgdMppcDisplay::~FgdMppcDisplay() 
{
  if(fHitArray) {
    fHitArray->Delete();
    delete fHitArray;
  }
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

  fcanvas = new TCanvas();
  f_xy_hist = new TH2F("hist_xy","XY histogram",f_bin_X,0,f_bin_X,f_bin_Y,0,f_bin_Y);
  f_yz_hist = new TH2F("hist_yz","YZ histogram",f_bin_Y,0,f_bin_Y,f_bin_Z,0,f_bin_Z);
  f_xz_hist = new TH2F("hist_zx","XZ histogram",f_bin_X,0,f_bin_X,f_bin_Z,0,f_bin_Z);

  return kSUCCESS;
}


// -------------------------------------------------------------------------



// -----   Public methods   --------------------------------------------
void FgdMppcDisplay::FinishEvent()
{
  if(f_xy_hist)
  {
    f_xy_hist->Draw("colz");
    WriteCanvas("XY");
    f_xy_hist->Reset();
  } 

  if(f_yz_hist)
  {
    f_yz_hist->Draw("colz");
    WriteCanvas("YZ");
    f_yz_hist->Reset();
  } 

  if(f_xz_hist)
  {
    f_xz_hist->Draw("colz");
    WriteCanvas("XZ");
    f_xz_hist->Reset();
  } 

  if(fcanvas)
  {
    fcanvas->ResetDrawn();
  }
  fevNum++;
}

void FgdMppcDisplay::FinishTask()
{
  fevNum = 0;
  std::cout << " Average number of photons per cube " << (f_photo_ave/f_photo_count) << std::endl;
  std::cout << " Count of cubes " << f_photo_count << std::endl;
  
}


void FgdMppcDisplay::Exec(Option_t* opt) 
{
  cout << "Event " << fevNum << endl;
  const Int_t hits = fHitArray->GetEntries();
  for(Int_t i =0; i < hits; i++)
  {
      data::superfgd::FgdHit* hit = (data::superfgd::FgdHit*)fHitArray->At(i);
      TVector3  photoE = std::move(hit->GetPhotoE()); //returns a temp obj
      TVector3  mppcLoc = std::move(hit->GetMppcLoc()); //returns a temp obj

      
      Int_t&& x = mppcLoc.X();
      Int_t&& y = mppcLoc.Y();
      Int_t&& z = mppcLoc.Z();

      Int_t&& px = photoE.X();
      Int_t&& py = photoE.Y();
      Int_t&& pz = photoE.Z();
      cout  << "GetHits" << " x " << x << " y " << y << " z " << z << endl;
      cout  << "Photons" << " px " << px << " py " << py << " pz " << pz << endl;
      

      if(f_xy_hist) f_xy_hist->Fill(mppcLoc.X(), mppcLoc.Y(), photoE.Z());
      if(f_yz_hist) f_yz_hist->Fill(mppcLoc.Y(), mppcLoc.Z(), photoE.X());
      if(f_xz_hist) f_xz_hist->Fill(mppcLoc.X(), mppcLoc.Z(), photoE.Y());

      if(i<7)
      {
        f_photo_ave += photoE.Z() + photoE.X() + photoE.Y();
        ++f_photo_count;
      }
      
  }
  cout  << endl;
  cout  << endl;
}
// -------------------------------------------------------------------------
  

void FgdMppcDisplay::WriteCanvas(string hist)
{
    fcanvas->Update();
    fcanvas->Draw();
    fcanvas->Write();
    std::ostringstream strb;
    strb<< "hist_" << hist << fevNum << ".gif";
    fcanvas->SaveAs((strb.str()).c_str());
    fcanvas->ResetDrawn();
}

}// namespace superfgd
}// namespace digitizer
}// namespace esbroot
