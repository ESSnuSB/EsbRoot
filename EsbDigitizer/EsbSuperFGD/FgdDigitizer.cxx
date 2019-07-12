#include "EsbDigitizer/EsbSuperFGD/FgdDigitizer.h"
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
FgdDigitizer::FgdDigitizer() :
  FairTask(), fX(0), fY(0), fZ(0)
{ 
}
// -------------------------------------------------------------------------

// -----   Constructor   -------------------------------------------
FgdDigitizer::FgdDigitizer(const char* name
                          ,const char* geoConfigFile
                          ,double x, double y, double z
                          , Int_t verbose) :
  FairTask(name, verbose), fX(x), fY(y), fZ(z)
{ 
  fParams.LoadPartParams(geoConfigFile);
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
FgdDigitizer::~FgdDigitizer() 
{
}
// -------------------------------------------------------------------------



// -----   Public method Init   --------------------------------------------
InitStatus FgdDigitizer::Init() 
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

  fdPoints = (TClonesArray*) manager->GetObject(geometry::superfgd::DP::FGD_BRANCH.c_str());
  if (!fdPoints) 
  {
      LOG(fatal) << "Exec", "No fgd points array";
      return kFATAL;
  }

  return kSUCCESS;
}


// -------------------------------------------------------------------------



// -----   Public method Exec   --------------------------------------------
void FgdDigitizer::Exec(Option_t* opt) 
{
  const Int_t points = fdPoints->GetEntries();
  for(Int_t i =0; i < points; i++)
  {
    data::superfgd::FgdDetectorPoint* point = (data::superfgd::FgdDetectorPoint*)fdPoints->At(i);
    point->Print(nullptr);
  }
  
}
// -------------------------------------------------------------------------
  


// -----   Private methods   --------------------------------------------
const double FgdDigitizer::CBIRKS = 0.0208 * CLHEP::cm/CLHEP::MeV; // used in SciBooNE MC
const double FgdDigitizer::EdepToPhotConv_FGD = 70.8 / CLHEP::MeV; // contains both collection in fiber and edep->gamma conversion 
const double FgdDigitizer::EdepToPhotConv_SuperFGD = EdepToPhotConv_FGD * 1.3;

double FgdDigitizer::ApplyScintiResponse(double edep, double trackLength, double charge)
{
    // Calculated as in 
    //  geant4/examples/advanced/amsEcal/src/SteppingAction.cc
    //  G4double SteppingAction::BirksAttenuation(const G4Step* aStep)
    //  light attenuation (Birks' formula)
    double dedx = edep/trackLength;
    double edep_birk = edep/(1. + CBIRKS*dedx);
    // Calculate edep to photons
    return edep_birk * EdepToPhotConv_SuperFGD;  
}

void FgdDigitizer::ApplyFiberResponse(double& numPhotons, double& time, double distance)
{
    double temp_np = numPhotons;
    numPhotons = ApplyFiberAttenuation(temp_np, distance);

    double temp_time = time;
    time = ApplyFiberTime(temp_time, distance);
}

const double FgdDigitizer::DistMPPCscint_FGD = 41*CLHEP::mm;
const double FgdDigitizer::LongCompFrac_FGD = 0.816;
const double FgdDigitizer::LongAtt_FGD = 11926.*CLHEP::mm;
const double FgdDigitizer::ShortAtt_FGD = 312.*CLHEP::mm;
const double FgdDigitizer::DecayLength_FGD = 0.0858 / CLHEP::mm;
const double FgdDigitizer::Lbar_FGD = 1864.3 * CLHEP::mm;

double FgdDigitizer::ApplyFiberAttenuation(double Nphot0,double x)
{
    // Apply the light attenuation to the collected photons
    // Used for official FGD reconstruction: Eq. (4) in TN-103
  
    double a=0.;        // long attenuation component fraction
    double d=0.;        // distance MPPC-scint outside the bar
    double LongAtt=0.;  // long attenuation length
    double ShortAtt=0.; // short attenuation length
    double Ldecay=0.;   // decay length
    double Lbar=0.;     // bar length

    a = LongCompFrac_FGD;
    d = DistMPPCscint_FGD; 
    LongAtt = LongAtt_FGD;
    ShortAtt = ShortAtt_FGD;  
    Ldecay= DecayLength_FGD;
    Lbar = Lbar_FGD;
  
    double Nphot = Nphot0;
    Nphot *= ( a*exp((-x-d)/LongAtt) + (1-a)*exp((-x-d)/ShortAtt) );

    return Nphot;
}

const double FgdDigitizer::TransTimeInFiber = 1./28.;  //  1cm/2.8e10[cm/s] * 1e9 [ns]

double FgdDigitizer::ApplyFiberTime(double& time, double x)
{
  double fiberTime = time + TransTimeInFiber*x;
  return fiberTime;
}

const double FgdDigitizer::MPPCEff_SuperFGD = 0.38;

void FgdDigitizer::ApplyMPPCResponse(double& npe)
{
    double rndunif =0.;
    double npe_passed = 0.;
    int npe_integer = (int) (npe+0.5);

    for (int i=0;i<npe_integer;i++)
    {
        rndunif = rand.Rndm();
        if (rndunif < MPPCEff_SuperFGD)
        {
            npe_passed++;
        } 
    }

    npe =  npe_passed;
    return;
}
// -------------------------------------------------------------------------

} //superfgd
} //digitizer
} //esbroot
