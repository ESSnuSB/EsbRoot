#include "EsbReconstruction/EsbSuperFGD/FgdMuonOnlyGenFitRecon.h"
#include "EsbData/EsbSuperFGD/FgdDetectorPoint.h"

#include <FairRootManager.h>
#include "FairLogger.h"

#include "FairGeoLoader.h"
#include "FairGeoInterface.h"
#include "FairGeoBuilder.h"
#include "FairGeoMedia.h"
#include "FairVolume.h"

#include <TClonesArray.h>
#include <TGeoElement.h>
#include <TFile.h>

#include <iostream>
#include <sstream>
#include <memory>
#include <math.h>

#include "AbsBField.h"
#include "AbsMeasurement.h"
#include "ConstField.h"
#include <Exception.h>
//#include <EventDisplay.h>
#include <FieldManager.h>
#include "FitStatus.h"
#include <KalmanFitterRefTrack.h>
#include "MaterialEffects.h"
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
#include <fstream>


namespace esbroot {
namespace reconstruction {
namespace superfgd {

// -----   Default constructor   -------------------------------------------
FgdMuonOnlyGenFitRecon::FgdMuonOnlyGenFitRecon() :
  FairTask(), fsuperFgdVol(nullptr)
  , fgdConstructor("")
  , fHitArray(nullptr)
  , isDefinedMaterials(false)
  , fDebuglvl_genfit(0)
  , fmediaFile("")
  , fTracksArray(nullptr)
  , fOutputFile("")
  , finputFile("")
  , fdisplay(nullptr)
  , isGenFitVisualization(false)
  , fGenFitVisOption("")
  , fMuonInd(0)
  , fposX(0.)
  , fposY(0.)
  , fposZ(0.)
{ 
  fInitialMomentum.clear();
  fInitialPosition.clear();
}
// -------------------------------------------------------------------------

// -----   Constructor   -------------------------------------------
FgdMuonOnlyGenFitRecon::FgdMuonOnlyGenFitRecon(const char* name
                          , const char* geoConfigFile
                          , const char* mediaFile
                          , double posX
                          , double posY
                          , double posZ
                          , Int_t verbose
                          , double debugLlv
                          , const char* outFile
                          , const char* inFile
                          , bool visualize
                          , std::string visOption) :
  FairTask(name, verbose)
  , fsuperFgdVol(nullptr)
  , fgdConstructor(geoConfigFile)
  , fHitArray(nullptr)
  , isDefinedMaterials(false)
  , fDebuglvl_genfit(debugLlv)
  , fmediaFile(mediaFile)
  , fTracksArray(nullptr)
  , fOutputFile(outFile)
  , finputFile(inFile)
  , fdisplay(nullptr)
  , isGenFitVisualization(visualize)
  , fGenFitVisOption(visOption)
  , fMuonInd(0)
  , fposX(posX)
  , fposY(posY)
  , fposZ(posZ)
{ 
  fParams.LoadPartParams(geoConfigFile);
  fInitialMomentum.clear();
  fInitialPosition.clear();
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
FgdMuonOnlyGenFitRecon::~FgdMuonOnlyGenFitRecon() 
{
  if(fHitArray) {
    fHitArray->Delete();
    delete fHitArray;
  }

  if(fTracksArray) {
    fTracksArray->Delete();
    delete fTracksArray;
  }
}
// -------------------------------------------------------------------------



// -----   Public method Init   --------------------------------------------
InitStatus FgdMuonOnlyGenFitRecon::Init() 
{   
  // Create the real Fgd geometry
  DefineMaterials();
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
    LOG(fatal) << "-E- FgdMuonOnlyGenFitRecon::Init: " << "FairRootManager not instantised!";
    return kFATAL;
  }

  fHitArray = (TClonesArray*) manager->GetObject(geometry::superfgd::DP::FGD_HIT.c_str());
  if (!fHitArray) 
  {
      LOG(fatal) << "Exec", "No fgd hits array";
      return kFATAL;
  }

  // Create and register output array
  fTracksArray = new TClonesArray(genfit::Track::Class(), 1000);
  manager->Register(geometry::superfgd::DP::FGD_FIT_TRACK.c_str()
                    , geometry::superfgd::DP::FGD_BRANCH_FIT.c_str()
                    , fTracksArray, kTRUE);

  GetInitialMomPos();  

  // Clear content of previous file
  std::ofstream outmuonFile;
  outmuonFile.open(fOutputFile, std::ios::trunc);
  outmuonFile.close();

  if(isGenFitVisualization)
  {
    fdisplay = genfit::EventDisplay::getInstance();
  }
  
  if(fdisplay!=nullptr && !fGenFitVisOption.empty())
  {
    fdisplay->setOptions(fGenFitVisOption);
  }
  

  return kSUCCESS;
}


// -------------------------------------------------------------------------



// -----   Public methods   --------------------------------------------
void FgdMuonOnlyGenFitRecon::FinishEvent()
{
  if(isGenFitVisualization)
  {
    fdisplay->open();
  }
}

void FgdMuonOnlyGenFitRecon::FinishTask()
{
}




// 1. Hit points have to be sorted
// 2. The materials have to be created beforehand e.g. in FgdDetector
void FgdMuonOnlyGenFitRecon::Exec(Option_t* opt) 
{  
  try
  {

    fTracksArray->Delete();
    const Int_t hits = fHitArray->GetEntries();
    unsigned int nMeasurements = hits;
    const int pdg = 13;               // particle pdg code
    int nextTrack(0);

    // init geometry and mag. field
    TVector3 magField = fgdConstructor.GetMagneticField(); // values are in kGauss
    genfit::FieldManager::getInstance()->init(new genfit::ConstField(magField.X(),magField.Y(), magField.Z())); 
    genfit::MaterialEffects::getInstance()->init(new genfit::TGeoMaterialInterface());
    genfit::MaterialEffects::getInstance()->setDebugLvl(fDebuglvl_genfit);


    // init fitter
    std::shared_ptr<genfit::AbsKalmanFitter> fitter = make_shared<genfit::KalmanFitterRefTrack>();
    fitter->setMinIterations(2);
    fitter->setMaxIterations(5);

    if(fMuonInd>=fInitialMomentum.size())
    {
      std::string errMsg("No more muons starting values. Simulations exceed generated muons starting conditions");
      LOG(fatal)<< errMsg;
      throw errMsg;
    }

    TVector3 posM(fInitialPosition[fMuonInd]);
    TVector3 momM(fInitialMomentum[fMuonInd]);
    fMuonInd++;

    // approximate covariance
    TMatrixDSym covM(6);
    double resolution = 0.1;
    for (int i = 0; i < 3; ++i)
        covM(i,i) = resolution*resolution;
    for (int i = 3; i < 6; ++i)
        covM(i,i) = covM(i,i) = pow(  ((resolution / nMeasurements) / sqrt(3)), 2); 

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

    // TODO2 
    //  1. Sort in Z diretion the mppc
    //  2. Fix to have only one mppc with given coordinates
    double max_z = -10000;
    bool visited[f_bin_X][f_bin_Y][f_bin_Z];
    for(int i=0; i< f_bin_X; i++)
      for(int j=0; j< f_bin_Y; j++)
        for(int k=0; k< f_bin_Z; k++)
          visited[i][j][k]=false;

    // TODO2 - extract initial pos, mom and tracks
    //  fit every track seperately
    int points(0);
    for(Int_t i =0; i <  hits ; i++)
    {
      data::superfgd::FgdHit* hit = (data::superfgd::FgdHit*)fHitArray->At(i);
      TVector3  photoE = std::move(hit->GetPhotoE());    
      TVector3  mppcLoc = std::move(hit->GetMppcLoc());  

      if(visited[(int)mppcLoc.X()][(int)mppcLoc.Y()][(int)mppcLoc.Z()])
      {
          continue;
      }
      visited[(int)mppcLoc.X()][(int)mppcLoc.Y()][(int)mppcLoc.Z()] = true;

      if(max_z<mppcLoc.Z())
      {
        max_z=mppcLoc.Z();
      }
      else
      {
        continue;
      }

      if(photoE.X() !=0 || photoE.Y()!=0 || photoE.Z()!=0)
      {
        // TODO2 - use mppc location
        TVectorD hitPos(3);
        // hitPos(0) = hit->GetX();
        // hitPos(1) = hit->GetY();
        // hitPos(2) = hit->GetZ();

        hitPos(0) = -f_total_X/2 + f_step_X*mppcLoc.X()  +f_step_X/2;
        hitPos(1) = -f_total_Y/2 + f_step_Y*mppcLoc.Y()  +f_step_Y/2;
        hitPos(2) = -f_total_Z/2 + f_step_Z*mppcLoc.Z()  +f_step_Z/2;

        genfit::AbsMeasurement* measurement = new genfit::SpacepointMeasurement(hitPos, hitCov, detId, points, nullptr);
        std::vector<genfit::AbsMeasurement*> measurements{measurement};

        fitTrack.insertPoint(new genfit::TrackPoint(measurements, &fitTrack));
        points++;
      }
    }
  
    // //check
    fitTrack.checkConsistency();

    fitter->setDebugLvl(fDebuglvl_genfit);
    // // do the fit
    fitter->processTrack(&fitTrack);

    // //check
    fitTrack.checkConsistency();

    if(isGenFitVisualization)
    {
      fdisplay->addEvent(&fitTrack);
    }

    // write result to output file
    new((*fTracksArray)[nextTrack++]) genfit::Track(fitTrack);
    

    const genfit::MeasuredStateOnPlane& me = fitTrack.getFittedState();
    LOG(debug)<< "Momentum  " << (me.getMom()).Mag();
    LOG(debug)<< " X  " << (me.getMom()).X()<< " Y " << (me.getMom()).Y()<< " Z  " << (me.getMom()).Z();

    std::cout << "Momentum  " << (me.getMom()).Mag() << std::endl;
    std::cout << " X  " << (me.getMom()).X()<< " Y " << (me.getMom()).Y()<< " Z  " << (me.getMom()).Z() << std::endl;

    genfit::FitStatus* fiStatuStatus = fitTrack.getFitStatus();
    fiStatuStatus->Print();

    LOG(debug)<< "fiStatuStatus->isFitted()  " << fiStatuStatus->isFitted();
    LOG(debug)<< "fiStatuStatus->isFitConverged()  " << fiStatuStatus->isFitConverged();
    LOG(debug)<< "fiStatuStatus->isFitConvergedFully()  " << fiStatuStatus->isFitConvergedFully();
    LOG(debug)<< "fiStatuStatus->isFitConvergedPartially()  " << fiStatuStatus->isFitConvergedPartially();
    LOG(debug)<< "Total measurement points  " << points;

    if(fiStatuStatus->isFitted() && fiStatuStatus->isFitConverged())
    {
      std::cout << "fiStatuStatus->isFitted()  " << fiStatuStatus->isFitted() << std::endl;
      std::cout << "fiStatuStatus->isFitConverged()  " << fiStatuStatus->isFitConverged() << std::endl;
      std::cout << "fiStatuStatus->isFitConvergedFully()  " << fiStatuStatus->isFitConvergedFully() << std::endl;
      std::cout << "fiStatuStatus->isFitConvergedPartially()  " << fiStatuStatus->isFitConvergedPartially() << std::endl;
      std::cout << "Total measurement points  " << points << std::endl;
      std::cout << "getCharge  " << fiStatuStatus->getCharge() << std::endl;

      std::ofstream outmuonFile;
      outmuonFile.open(fOutputFile, std::ios::app);

      if(outmuonFile.is_open())
      {
        outmuonFile << "Event  " << fMuonInd << std::endl;
        outmuonFile << "MOnte Carlo Momentum " << momM.Mag() << std::endl;
        outmuonFile << " X  " << momM.X()  << " Y " << momM.Y() << " Z  " << momM.Z() << std::endl;
        outmuonFile << "Genfit Momentum  " << (me.getMom()).Mag() << std::endl;
        outmuonFile << " X  " << (me.getMom()).X() << " Y " << (me.getMom()).Y()<< " Z  " << (me.getMom()).Z() << std::endl;
        outmuonFile << "Difference (MC - Genfit)  " << (momM.Mag() - (me.getMom()).Mag()) << std::endl;
        outmuonFile << "Difference % (MC - Genfit)  " << 100.*((momM.Mag() - (me.getMom()).Mag())/momM.Mag()) << std::endl;
        outmuonFile << " X  " << (momM.X() - (me.getMom()).X()) << " Y " << (momM.Y() - (me.getMom()).Y()) << " Z  " << (momM.Z() - (me.getMom()).Z()) << std::endl;
        outmuonFile << " X% " << 100.*((momM.X() - (me.getMom()).X())/momM.X()) 
                    << " Y% " << 100.*((momM.Y() - (me.getMom()).Y())/momM.Y())
                    << " Z% " << 100.*((momM.Z() - (me.getMom()).Z())/momM.Z()) << std::endl;
        outmuonFile << "fiStatuStatus->isFitted()  " << fiStatuStatus->isFitted()<< std::endl;
        outmuonFile << "fiStatuStatus->isFitConverged()  " << fiStatuStatus->isFitConverged() << std::endl;
        outmuonFile << "fiStatuStatus->isFitConvergedFully()  " << fiStatuStatus->isFitConvergedFully() << std::endl;
        outmuonFile << "fiStatuStatus->isFitConvergedPartially()  " << fiStatuStatus->isFitConvergedPartially() << std::endl;
        outmuonFile << "Total measurement points  " << points << std::endl;

        outmuonFile << "#x " << std::fabs(momM.X()) << " " << std::fabs(100.*((momM.X() - (me.getMom()).X())/momM.X())) << std::endl;
        outmuonFile << "#y " << std::fabs(momM.Y()) << " " << std::fabs(100.*((momM.Y() - (me.getMom()).Y())/momM.Y())) << std::endl;
        outmuonFile << "#z " << std::fabs(momM.Z()) << " " << std::fabs(100.*((momM.Z() - (me.getMom()).Z())/momM.Z())) << std::endl;
        outmuonFile << "=================================" << std::endl;
      }
      outmuonFile.close();
    }
  }
  catch(genfit::Exception& e)
  {
      std::cerr<<"Exception, when tryng to fit track"<<std::endl;
      std::cerr << e.what();
  }
}
// -------------------------------------------------------------------------


// -----   Private methods   --------------------------------------------
void FgdMuonOnlyGenFitRecon::DefineMaterials() 
{
  if(isDefinedMaterials) return; // Define materials only once

  isDefinedMaterials = true;

  new FairGeoLoader("TGeo","Geo Loader");
  FairGeoLoader *geoLoad = FairGeoLoader::Instance();
  if(geoLoad==nullptr)
  {
    LOG(error)<< "geoLoad is null";
    std::cout << "geoLoad is null" << endl;
    throw;
  }

	FairGeoInterface *geoFace = geoLoad->getGeoInterface();

  geoFace->setMediaFile(fmediaFile.c_str());
  geoFace->readMedia();

	FairGeoMedia *geoMedia = geoFace->getMedia();
	FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  // FairGeoMedium* brass = geoMedia->getMedium(esbroot::geometry::superfgd::materials::brass);
	// geoBuild->createMedium(brass);

  // FairGeoMedium* bronze = geoMedia->getMedium(esbroot::geometry::superfgd::materials::bronze);
	// geoBuild->createMedium(bronze);

  // FairGeoMedium* stainlessSteel = geoMedia->getMedium(esbroot::geometry::superfgd::materials::stainlessSteel);
	// geoBuild->createMedium(stainlessSteel);

  // FairGeoMedium* methane = geoMedia->getMedium(esbroot::geometry::superfgd::materials::methane);
	// geoBuild->createMedium(methane);

  // FairGeoMedium* carbonDioxide = geoMedia->getMedium(esbroot::geometry::superfgd::materials::carbonDioxide);
	// geoBuild->createMedium(carbonDioxide);

  // FairGeoMedium* carbontetraFloride = geoMedia->getMedium(esbroot::geometry::superfgd::materials::carbontetraFloride);
	// geoBuild->createMedium(carbontetraFloride);

  // FairGeoMedium* titaniumDioxide = geoMedia->getMedium(esbroot::geometry::superfgd::materials::titaniumDioxide);
	// geoBuild->createMedium(titaniumDioxide);

  // FairGeoMedium* polystyrene = geoMedia->getMedium(esbroot::geometry::superfgd::materials::polystyrene);
	// geoBuild->createMedium(polystyrene);

  FairGeoMedium* scintillator = geoMedia->getMedium(esbroot::geometry::superfgd::materials::scintillator);
  scintillator->setMediumIndex(esbroot::geometry::superfgd::materials::GetNextIndex());
	geoBuild->createMedium(scintillator);
  scintillator->Print();

  FairGeoMedium* paraterphnyl = geoMedia->getMedium(esbroot::geometry::superfgd::materials::paraterphnyl);
	geoBuild->createMedium(paraterphnyl);

  // FairGeoMedium* podscintillator = geoMedia->getMedium(esbroot::geometry::superfgd::materials::podscintillator);
	// geoBuild->createMedium(podscintillator);

  // FairGeoMedium* polyethylene = geoMedia->getMedium(esbroot::geometry::superfgd::materials::polyethylene);
	// geoBuild->createMedium(polyethylene);

  // FairGeoMedium* poduleEpoxy = geoMedia->getMedium(esbroot::geometry::superfgd::materials::poduleEpoxy);
	// geoBuild->createMedium(poduleEpoxy);

  // FairGeoMedium* polycarbonate = geoMedia->getMedium(esbroot::geometry::superfgd::materials::polycarbonate);
	// geoBuild->createMedium(polycarbonate);

  // FairGeoMedium* carbonFiber = geoMedia->getMedium(esbroot::geometry::superfgd::materials::carbonFiber);
	// geoBuild->createMedium(carbonFiber);

  FairGeoMedium* fiberCore = geoMedia->getMedium(esbroot::geometry::superfgd::materials::fiberCore);
	geoBuild->createMedium(fiberCore);

  FairGeoMedium* fiberCladding = geoMedia->getMedium(esbroot::geometry::superfgd::materials::fiberCladding);
	geoBuild->createMedium(fiberCladding);

  FairGeoMedium* fairTiO2 = geoMedia->getMedium(esbroot::geometry::superfgd::materials::titaniumDioxide);
  geoBuild->createMedium(fairTiO2);

  FairGeoMedium* fairPolystyrene = geoMedia->getMedium(esbroot::geometry::superfgd::materials::polystyrene);
  geoBuild->createMedium(fairPolystyrene);

  FairGeoMedium* fairAir = geoMedia->getMedium(esbroot::geometry::superfgd::materials::air);
  geoBuild->createMedium(fairAir);

  FairGeoMedium* vacuum = geoMedia->getMedium(esbroot::geometry::superfgd::materials::vacuum);
  geoBuild->createMedium(vacuum);
}

void FgdMuonOnlyGenFitRecon::GetInitialMomPos()
{
  if(!finputFile.empty())
  {
    std::ifstream muonFile;
    muonFile.open(finputFile.c_str(), std::ios::in);

    if(muonFile.is_open())
    {
      std::string line;
      while(std::getline(muonFile,line))
      {
          int b(0);
          int ind(0);
          double arr[6];
          int arrInd(0);

          while(ind < line.length())
          {
              if(line[ind]==' ' || (ind==line.length()-1))
              {
                  arr[arrInd++] = std::stod(line.substr(b,ind));
                  b=ind;
              }
              ++ind;
          }

          fInitialMomentum.emplace_back(TVector3(arr[0],arr[1],arr[2]));
          fInitialPosition.emplace_back(TVector3( (arr[3] - fposX) , (arr[4] - fposY) , (arr[5] - fposZ) ));
      }
    }
  }
}
// -------------------------------------------------------------------------

}// namespace superfgd
}// namespace reconstruction
}// namespace esbroot
