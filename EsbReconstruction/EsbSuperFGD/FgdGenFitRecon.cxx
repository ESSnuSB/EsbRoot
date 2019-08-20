#include "EsbReconstruction/EsbSuperFGD/FgdGenFitRecon.h"
#include "EsbData/EsbSuperFGD/FgdDetectorPoint.h"

#include <FairRootManager.h>
#include "FairLogger.h"

#include <TClonesArray.h>

#include <iostream>
#include <sstream>
#include <memory>

using std::cout;
using std::endl;

//TODO2
#include "AbsMeasurement.h"
#include "ConstField.h"
#include <EventDisplay.h>
#include <Exception.h>
#include <FieldManager.h>
#include <KalmanFitterRefTrack.h>
#include "MaterialEffects.h"
//#include "MeasuredStateOnPlane.h"
#include "MeasuredStateOnPlane.h"
#include <PlanarMeasurement.h>
#include <RKTrackRep.h>
#include "SpacepointMeasurement.h"
#include <StateOnPlane.h>
#include "TDatabasePDG.h"
#include <TEveManager.h>
#include <TGeoMaterialInterface.h>
#include <TGeoManager.h>
#include <Track.h>
#include <TrackCand.h>
#include <TrackPoint.h>
#include <TRandom.h>
#include <TVector3.h>
#include <vector>

//TODO2

namespace esbroot {
namespace reconstruction {
namespace superfgd {

// -----   Default constructor   -------------------------------------------
FgdGenFitRecon::FgdGenFitRecon() :
  FairTask(), fsuperFgdVol(nullptr), fgdConstructor("")
  , fposX(0), fposY(0), fposZ(0), fstartPos(TVector3(0,0,0)), fstartMom(TVector3(0,0,0))
  , fHitArray(nullptr)
{ 
}
// -------------------------------------------------------------------------

// -----   Constructor   -------------------------------------------
FgdGenFitRecon::FgdGenFitRecon(const char* name
                          ,const char* geoConfigFile, double posX, double posY, double posZ
                          ,TVector3 startPos, TVector3 startMom
                          ,Int_t verbose) :
  FairTask(name, verbose), fsuperFgdVol(nullptr), fgdConstructor(geoConfigFile)
  , fposX(posX), fposY(posY), fposZ(posZ), fstartPos(startPos), fstartMom(startMom)
  , fHitArray(nullptr)
{ 
  fParams.LoadPartParams(geoConfigFile);
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
FgdGenFitRecon::~FgdGenFitRecon() 
{
  if(fHitArray) {
    fHitArray->Delete();
    delete fHitArray;
  }
}
// -------------------------------------------------------------------------



// -----   Public method Init   --------------------------------------------
InitStatus FgdGenFitRecon::Init() 
{   
  // Create the real Fgd geometry
  fsuperFgdVol = fgdConstructor.Construct();
  gGeoManager->SetTopVolume(fsuperFgdVol);

  // Get dimentions from geometry file
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
    LOG(fatal) << "-E- FgdGenFitRecon::Init: " << "FairRootManager not instantised!";
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



// -----   Public methods   --------------------------------------------
void FgdGenFitRecon::FinishEvent()
{
  
}

void FgdGenFitRecon::FinishTask()
{
}


void FgdGenFitRecon::Exec(Option_t* opt) 
{  
  const Int_t hits = fHitArray->GetEntries();
  unsigned int nMeasurements = hits;
  const int pdg = 13;               // particle pdg code


  // init geometry and mag. field
  TVector3 magField = fgdConstructor.GetMagneticField();
  genfit::FieldManager::getInstance()->init(new genfit::ConstField(magField.X(),magField.Y(), magField.Z())); 
  genfit::MaterialEffects::getInstance()->init(new genfit::TGeoMaterialInterface());

  // init fitter
  std::shared_ptr<genfit::AbsKalmanFitter> fitter = make_shared<genfit::KalmanFitterRefTrack>();

  TVector3 pos(fstartPos);
  TVector3 mom(fstartMom);

  TVector3 posM(pos);
  TVector3 momM(mom);

  // approximate covariance
  TMatrixDSym covM(6);
  double resolution = 0.1;
  for (int i = 0; i < 3; ++i)
      covM(i,i) = resolution*resolution;
  for (int i = 3; i < 6; ++i)
      covM(i,i) = pow(resolution / nMeasurements / sqrt(3), 2);

  TMatrixDSym hitCov(3);
  hitCov(0,0) = resolution*resolution;
  hitCov(1,1) = resolution*resolution;
  hitCov(2,2) = resolution*resolution;

  // trackrep
  genfit::AbsTrackRep* rep = new genfit::RKTrackRep(pdg);

  // smeared start state
  genfit::MeasuredStateOnPlane stateSmeared(rep);
  stateSmeared.setPosMomCov(posM, momM, covM);

  // create track
  TVectorD seedState(6);
  TMatrixDSym seedCov(6);
  stateSmeared.get6DStateCov(seedState, seedCov);
  
  genfit::Track fitTrack(rep, seedState, seedCov);

  int detId(1); // Detector id, it is the same, we only have one detector
  
  
  for(Int_t i =0; i < 5 /* hits */; i++)
  {
    data::superfgd::FgdHit* hit = (data::superfgd::FgdHit*)fHitArray->At(i);
    TVector3  photoE = std::move(hit->GetPhotoE());    //returns a temp obj
    TVector3  mppcLoc = std::move(hit->GetMppcLoc());  //returns a temp obj

    // "simulate" the detector
    TVectorD hitPos(3);
    hitPos(0) = f_step_X*mppcLoc.X()  +f_step_X/2;
    hitPos(1) = f_step_Y*mppcLoc.Y()  +f_step_Y/2;
    hitPos(2) = f_step_Z*mppcLoc.Z()  +f_step_Z/2;

    genfit::AbsMeasurement* measurement = new genfit::SpacepointMeasurement(hitPos, hitCov, detId, i, nullptr);
    std::vector<genfit::AbsMeasurement*> measurements{measurement};

    fitTrack.insertPoint(new genfit::TrackPoint(measurements, &fitTrack));
  }

  // //check
  fitTrack.checkConsistency();

  // // do the fit
  fitter->processTrack(&fitTrack);

  // //check
  fitTrack.checkConsistency();

  cout << "Number of points " << fitTrack.getNumPoints() << endl;

  const genfit::MeasuredStateOnPlane& me = fitTrack.getFittedState();
  cout << "Momentum X " << (me.getMom()).X() << endl;
  cout << "Momentum Y " << (me.getMom()).Y() << endl;
  cout << "Momentum Z " << (me.getMom()).Z() << endl;

  // const std::vector< genfit::TrackPoint* >& points = fitTrack.getPoints();
  // auto iter = points.begin();
  // while(iter!=points.end())
  // {
  //   genfit::TrackPoint* point = (genfit::TrackPoint*)(*iter);
  //   point->Print();
  //   ++iter;
  // }
}
// -------------------------------------------------------------------------


}// namespace superfgd
}// namespace reconstruction
}// namespace esbroot
