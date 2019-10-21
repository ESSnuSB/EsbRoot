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
  FairTask(), fX(0), fY(0), fZ(0), fdPoints(nullptr), fHitArray(nullptr)
{ 
}
// -------------------------------------------------------------------------

// -----   Constructor   -------------------------------------------
FgdDigitizer::FgdDigitizer(const char* name
                          ,const char* geoConfigFile
                          ,double x, double y, double z
                          , Int_t verbose) :
  FairTask(name, verbose), fX(x), fY(y), fZ(z),
  fdPoints(nullptr), fHitArray(nullptr)
{ 
  fParams.LoadPartParams(geoConfigFile);
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
FgdDigitizer::~FgdDigitizer() 
{
  if(fHitArray) {
    fHitArray->Delete();
    delete fHitArray;
  }
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

  // Create and register output array
  fHitArray = new TClonesArray(data::superfgd::FgdHit::Class(), 1000);
  manager->Register(geometry::superfgd::DP::FGD_HIT.c_str()
                    , geometry::superfgd::DP::FGD_DETECTOR_NAME.c_str()
                    , fHitArray, kTRUE);

  return kSUCCESS;
}


// -------------------------------------------------------------------------



// -----   Public method Exec   --------------------------------------------
void FgdDigitizer::Exec(Option_t* opt) 
{
  // Reset output array
  fHitArray->Delete();
  
  const Int_t points = fdPoints->GetEntries();
  int nextPoint(0);
  for(Int_t i =0; i < points; i++)
  {
    data::superfgd::FgdDetectorPoint* point = (data::superfgd::FgdDetectorPoint*)fdPoints->At(i);

    TVector3 dpos = point->GetDetectorpos(); // Get the detector position
    //=============================================
    //====          Position        ===============
    //=============================================
    double pos_x = point->GetX() - dpos.X();
    double pos_y = point->GetY() - dpos.Y();
    double pos_z = point->GetZ() - dpos.Z();

    // calculate the bin position 
    // pos_x,y,z -> relative from the center of the solid (half l,h,w)
    // to normalize add half l,h,w and divide by the step in each direction
    int bin_pos_x = (pos_x + f_total_X/2)/f_step_X;  
    int bin_pos_y = (pos_y + f_total_Y/2)/f_step_Y;  
    int bin_pos_z = (pos_z + f_total_Z/2)/f_step_Z;  

    //=============================================
    //====              MPPC        ===============
    //=============================================
    // Calculate the distance from the center of the cube to the MPPC
    double mppcX = bin_pos_x*f_step_X + f_step_X/2;
    double mppcX_2ndSide = f_total_X - mppcX; // along the opposite direction

    double mppcY = bin_pos_y*f_step_Y + f_step_Y/2;
    double mppcY_2ndSide = f_total_Y - mppcY; // along the opposite direction

    double mppcZ = bin_pos_z*f_step_Z + f_step_Z/2;
    double mppcZ_2ndSide = f_total_Z - mppcZ; // along the opposite direction

    //=============================================
    //====    Scintilation Response        ========
    //=============================================
    double pe = ApplyScintiResponse(point->GetEnergyLoss()*1e3 // EnergyLoss is returned in [GeV], we use [eV]
                                    ,point->GetTrackLenght() // TrackLength
                                    ,1.0                // Charge, for the moment it is not used
                                    );
    double time = point->GetTime();

    // Calculate the # of p.e. that reach the MPPC (X,Y,Z). For the moment it is just considered that 
    // the part of the photons that go to x,y,z are taken into
    // account in the coefficients when applying the scintillation response.
    // -> light collection + attenuation
    double peX1=pe/2.; 
    double peX2=pe/2.;
    double timepeX=time;
    ApplyFiberResponse(peX1,timepeX,mppcX); // along X fiber
    ApplyFiberResponse(peX2,timepeX,mppcX_2ndSide); // along -X fiber
    double peX = peX1 + peX2;       // pe along fiber X

    double peY1=pe/2.; 
    double peY2=pe/2.; 
    double timepeY=time;
    ApplyFiberResponse(peY1,timepeY,mppcY); // along Y fiber
    ApplyFiberResponse(peY2,timepeY,mppcY_2ndSide); // along -Y fiber
    double peY = peY1 + peY2;       // pe along fiber Y

    double peZ1=pe/2.; 
    double peZ2=pe/2.; 
    double timepeZ=time;
    ApplyFiberResponse(peZ1,timepeZ,mppcZ); // along Z fiber
    ApplyFiberResponse(peZ2,timepeZ,mppcZ_2ndSide); // along -Z fiber
    double peZ = peZ1 + peZ2;       // pe along fiber Z

    ApplyMPPCResponse(peX);
    ApplyMPPCResponse(peY);
    ApplyMPPCResponse(peZ);
    TVector3 photoElectrons(peX,peY,peZ);

    // Write the mppc location in terms of cube number position in x,y,z
    if(peX!=0 || peY!=0 || peZ!=0)
    {
      TVector3 mppcLocalPosition(bin_pos_x,bin_pos_y,bin_pos_z);
      TVector3 cubeCoordinatesRelToDetector(pos_x, pos_y ,pos_z);

      double center_coor_x = (bin_pos_x*f_step_X - f_total_X/2 + f_step_X/2) + dpos.X(); 
      double center_coor_y = (bin_pos_y*f_step_Y - f_total_Y/2 + f_step_Y/2) + dpos.Y(); 
      double center_coor_z = (bin_pos_z*f_step_Z - f_total_Z/2 + f_step_Z/2) + dpos.Z(); 

      TVector3 mom(point->GetPx(), point->GetPy(), point->GetPz());

      new((*fHitArray)[nextPoint++]) data::superfgd::FgdHit(center_coor_x, center_coor_y, center_coor_z
                                                            , mppcLocalPosition
                                                            , photoElectrons
                                                            , cubeCoordinatesRelToDetector
                                                            , point->GetTime()
                                                            , mom
                                                            , TVector3(peX1, peY1, peZ1), TVector3(mppcX, mppcY, mppcZ)
                                                            , TVector3(peX2, peY2, peZ2), TVector3(mppcX_2ndSide, mppcY_2ndSide, mppcZ_2ndSide)
                                                            , point->GetPdg(), point->GetTrackID());
    }
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
