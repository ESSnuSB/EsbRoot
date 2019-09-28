#include "EsbReconstruction/EsbSuperFGD/FgdGenFitRecon.h"
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
#include <TFile.h>


// Genfit headers
#include "AbsBField.h"
#include "AbsMeasurement.h"
#include "ConstField.h"
#include <Exception.h>
#include <EventDisplay.h>
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
#include <TGeoMaterialInterface.h>
#include <Track.h>
#include <TrackCand.h>
#include <TrackPoint.h>
#include <TRandom.h>
#include <TVector3.h>


// PathFinder headers
#include "FinderParameter.h"
#include "HoughTrafoTrackFinder.h"
#include "TrackParameterFull.h"


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
FgdGenFitRecon::FgdGenFitRecon() :
  FairTask(), fsuperFgdVol(nullptr)
  , fgdConstructor("")
  , fstartPos(TVector3(0,0,0))
  , fstartMom(TVector3(0,0,0))
  , fHitArray(nullptr)
  , isDefinedMaterials(false)
  , fDebuglvl_genfit(0)
  , fmediaFile("")
  , freconFile("")
  , fTracksArray(nullptr)
  , fdisplay(nullptr)
  , isGenFitVisualization(false)
  , fGenFitVisOption("")
  , fminGenFitInterations(2)
  , fmaxGenFitIterations(4)
  , fminHits(25)
  , ffinder(TrackFinder::HOUGH_PATHFINDER_ALL)
{ 
}
// -------------------------------------------------------------------------

// -----   Constructor   -------------------------------------------
FgdGenFitRecon::FgdGenFitRecon(const char* name
                          , const char* geoConfigFile
                          , const char* mediaFile
                          , const char* reconFile
                          , TVector3 startPos
                          , TVector3 startMom
                          , Int_t verbose
                          , double debugLlv
                          , bool visualize
                          , std::string visOption) :
  FairTask(name, verbose)
  , fsuperFgdVol(nullptr)
  , fgdConstructor(geoConfigFile)
  , fstartPos(startPos)
  , fstartMom(startMom)
  , fHitArray(nullptr)
  , isDefinedMaterials(false)
  , fDebuglvl_genfit(debugLlv)
  , fmediaFile(mediaFile)
  , freconFile(reconFile)
  , fTracksArray(nullptr)
  , fdisplay(nullptr)
  , isGenFitVisualization(visualize)
  , fGenFitVisOption(visOption)
  , fminGenFitInterations(2)
  , fmaxGenFitIterations(4)
  , fminHits(25)
  , ffinder(TrackFinder::HOUGH_PATHFINDER_ALL)
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

  if(fTracksArray) {
    fTracksArray->Delete();
    delete fTracksArray;
  }
}
// -------------------------------------------------------------------------



// -----   Public method Init   --------------------------------------------
InitStatus FgdGenFitRecon::Init() 
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
    LOG(fatal) << "-E- FgdGenFitRecon::Init: " << "FairRootManager not instantised!";
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
void FgdGenFitRecon::FinishEvent()
{
  
}

void FgdGenFitRecon::FinishTask()
{
  if(isGenFitVisualization)
  {
    fdisplay->open();
  }
}

void FgdGenFitRecon::Exec(Option_t* opt) 
{  
  try
  {
    std::vector<ReconHit> allhits;
    std::vector<pathfinder::TrackFinderTrack> foundTracks;

    bool rc = GetHits(allhits);

    if(rc)
    { 
      switch(ffinder)
      {
        case TrackFinder::HOUGH_PATHFINDER_ALL:
                                          rc = FindAllTracks(allhits, foundTracks, FindTrackType::CURL);
                                          break;
        case TrackFinder::HOUGH_PATHFINDER_ABOVE_BELOW:
                                          rc = FindAboveBelowTracks(allhits, foundTracks, FindTrackType::CURL);
                                          break;
        case TrackFinder::HOUGH_PATHFINDER_TIME_INTERVALS:
                                          rc = FindByIntervalsTracks(allhits, foundTracks, FindTrackType::CURL);
                                          break;
        case TrackFinder::USE_GRAPH_WITH_ALL_TEMPLATES:
                                          rc = FindUsingGraph(allhits, foundTracks);
                                          break;
        case TrackFinder::USE_GRAPH_ONLY_LEAF_TEMPLATES:
                                          rc = FindUsingGraph(allhits, foundTracks, true);
                                          break;
        default: 
                rc = false;
                break;
      }
    }

    if(rc)
    {
      LOG(debug) <<" Tracks found " << foundTracks.size();
      LOG(debug) <<" Hits to fit " << allhits.size();
      FitTracks(foundTracks);
    }
    else
    {
      LOG(error) << " Could not find clean hits or tracks! ";
    }
  }
  catch(genfit::Exception& e)
  {
      LOG(fatal) << "Exception, when tryng to fit track";
      LOG(fatal) << e.what();
  }
}
// -------------------------------------------------------------------------


// -----   Private methods   --------------------------------------------

Bool_t FgdGenFitRecon::GetHits(std::vector<ReconHit>& allHits)
{
  Double_t errPhotoLimit = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::FGD_ERR_PHOTO_LIMIT);

  LOG(debug) << "fHitArray->GetEntries() " << fHitArray->GetEntries();

  std::map<Long_t, Bool_t> visited;

  Int_t numVis(0);
 
  for(Int_t i =0; i <  fHitArray->GetEntries() ; i++)
  {
    data::superfgd::FgdHit* hit = (data::superfgd::FgdHit*)fHitArray->At(i);
    TVector3  photoE = hit->GetPhotoE();    
    TVector3  mppcLoc = hit->GetMppcLoc();  

    Int_t&& x = mppcLoc.X();
    Int_t&& y = mppcLoc.Y();
    Int_t&& z = mppcLoc.Z();
    
    Int_t ind = ArrInd(x,y,z);
    if(visited[ind])
    {
      // If already exists, add the photons
      ReconHit toFind;
      toFind.fmppcLoc = mppcLoc;
      std::vector<ReconHit>::iterator recHit = find(allHits.begin(), allHits.end(), toFind);
      ReconHit& foundHit = *recHit;
      foundHit.fphotons = foundHit.fphotons + photoE;
      continue;
    }
    visited[ind] = true;

    Double_t totalPhotons = photoE.X() + photoE.Y() + photoE.Z();
    if(totalPhotons >= errPhotoLimit)
    {
      TVectorD hitPos(3);
      hitPos(0) = -f_total_X/2 + f_step_X*mppcLoc.X()  +f_step_X/2;
      hitPos(1) = -f_total_Y/2 + f_step_Y*mppcLoc.Y()  +f_step_Y/2;
      hitPos(2) = -f_total_Z/2 + f_step_Z*mppcLoc.Z()  +f_step_Z/2;

      allHits.emplace_back(ReconHit(
                                mppcLoc
                              , TVector3(hitPos(0),hitPos(1),hitPos(2))
                              , photoE
                              , hit->GetTime()
                              , hit->GetPgd()
                              , hit->GetTrackId()
                              , hit->GetPhotoDist1()
                              , hit->GetMppcDist1()
                              , hit->GetPhotoDist2()
                              , hit->GetMppcDist2()
                            )); 
    }
  }

  // std::cout << "allHits.size()" << allHits.size() << std::endl;

  LOG(debug) << "allHits.size()" << allHits.size();

  return (allHits.size() > 0);
}

Bool_t FgdGenFitRecon::FindAllTracks(std::vector<ReconHit>& hits
                                , std::vector<pathfinder::TrackFinderTrack>& foundTracks
                                , FindTrackType trackType)
{
  LOG(debug) << "hits " << hits.size();

  unsigned int use_vertex = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_USE_VERTEX);
  double vertexX = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_VERTEXX);
  double vertexY = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_VERTEXY);
  double maxdistxy = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_MAXDISTXY);
  double maxdistsz = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_MAXDISTSZ);
  double maxdistxyfit = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_MAXDISTXYFIT);
  double maxdistszfit = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_MAXDISTSZFIT);
  unsigned int minhitnumber = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_MINHITNUMBER);
  unsigned int xythetabins = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_XYTHETABINS);
  unsigned int xyd0bins = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_XYD0BINS);
  unsigned int xyomegabins = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_XYOMEGABINS);
  unsigned int szthetabins = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_SZTHETABINS);
  unsigned int szd0bins = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_SZD0BINS);
  double maxdxy = f_total_X + f_total_Y;
  double maxdsz = f_total_Z;
  unsigned int searchneighborhood = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_SEACHINTHENEIGHBORHOOD);

  pathfinder::FinderParameter* newFinderParameter = nullptr;
  switch(trackType)
  {
    case FindTrackType::HELIX:
        newFinderParameter= new pathfinder::FinderParameter(false, true); 
        newFinderParameter -> setFindCurler(false);
        break;
    case FindTrackType::CURL:
        newFinderParameter= new pathfinder::FinderParameter(false, true); 
        newFinderParameter -> setFindCurler(true);
        break;
    case FindTrackType::STRAIGHT_LINE:
    default:
        newFinderParameter= new pathfinder::FinderParameter(true, false); 
        newFinderParameter -> setFindCurler(false);
        break;
  }


  //  if(use_vertex == 0) newFinderParameter -> setUseVertex(false);
  //  if(use_vertex == 1) newFinderParameter->setUseVertex(true);

  if(use_vertex == 1) 
  {  
    std::pair<double, double> vertex(vertexX, vertexY);
    newFinderParameter -> setVertex(vertex);
  }

  newFinderParameter -> setMaxXYDistance(maxdistxy);
  newFinderParameter -> setMaxSZDistance(maxdistsz);
  newFinderParameter -> setMaxXYDistanceFit(maxdistxyfit);
  newFinderParameter -> setMaxSZDistanceFit(maxdistszfit);
  newFinderParameter -> setMinimumHitNumber(minhitnumber);
  newFinderParameter -> setNumberXYThetaBins(xythetabins);
  newFinderParameter -> setNumberXYDzeroBins(xyd0bins);
  newFinderParameter -> setNumberXYOmegaBins(xyomegabins);
  newFinderParameter -> setNumberSZThetaBins(szthetabins);
  newFinderParameter -> setNumberSZDzeroBins(szd0bins);
  newFinderParameter -> setMaxDxy(maxdxy);
  newFinderParameter -> setMaxDsz(maxdsz);
  
  if(searchneighborhood == 0)
  {
    newFinderParameter -> setSearchNeighborhood(false);
  }
  else
  {
    newFinderParameter -> setSearchNeighborhood(true);
  }
  newFinderParameter -> setSaveRootFile(false);

  std::vector<pathfinder::basicHit> allHits;
  for(Int_t i=0; i< hits.size(); ++i)
  {
    allHits.emplace_back(pathfinder::basicHit(  hits[i].fHitPos.X()
                                                  , hits[i].fHitPos.Y()
                                                  , hits[i].fHitPos.Z()
                                                  )
                            );
  }
  
  pathfinder::HoughTrafoTrackFinder newTrackFinder;

  //setting steering parameter
  newTrackFinder.setFinderParameter(*newFinderParameter);

  // If there is not time interval include all hits
  newTrackFinder.setInitialHits(allHits);

  //do the actual track finding
  Bool_t found = newTrackFinder.find();
  if(found)
  {
    foundTracks = newTrackFinder.getTracks();
  }

  return found;
}

Bool_t FgdGenFitRecon::FindAboveBelowTracks(std::vector<ReconHit>& hits
                                , std::vector<pathfinder::TrackFinderTrack>& foundTracks
                                , FindTrackType trackType)
{
  LOG(debug) << "hits " << hits.size();

  unsigned int use_vertex = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_USE_VERTEX);
  double vertexX = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_VERTEXX);
  double vertexY = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_VERTEXY);
  double maxdistxy = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_MAXDISTXY);
  double maxdistsz = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_MAXDISTSZ);
  double maxdistxyfit = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_MAXDISTXYFIT);
  double maxdistszfit = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_MAXDISTSZFIT);
  unsigned int minhitnumber = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_MINHITNUMBER);
  unsigned int xythetabins = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_XYTHETABINS);
  unsigned int xyd0bins = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_XYD0BINS);
  unsigned int xyomegabins = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_XYOMEGABINS);
  unsigned int szthetabins = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_SZTHETABINS);
  unsigned int szd0bins = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_SZD0BINS);
  double maxdxy = f_total_X + f_total_Y;
  double maxdsz = f_total_Z;
  unsigned int searchneighborhood = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_SEACHINTHENEIGHBORHOOD);

  pathfinder::FinderParameter* newFinderParameter = nullptr;
  switch(trackType)
  {
    case FindTrackType::HELIX:
        newFinderParameter= new pathfinder::FinderParameter(false, true); 
        newFinderParameter -> setFindCurler(false);
        break;
    case FindTrackType::CURL:
        newFinderParameter= new pathfinder::FinderParameter(false, true); 
        newFinderParameter -> setFindCurler(true);
        break;
    case FindTrackType::STRAIGHT_LINE:
    default:
        newFinderParameter= new pathfinder::FinderParameter(true, false); 
        newFinderParameter -> setFindCurler(false);
        break;
  }


  //  if(use_vertex == 0) newFinderParameter -> setUseVertex(false);
  //  if(use_vertex == 1) newFinderParameter->setUseVertex(true);

  if(use_vertex == 1) 
  {  
    std::pair<double, double> vertex(vertexX, vertexY);
    newFinderParameter -> setVertex(vertex);
  }

  newFinderParameter -> setMaxXYDistance(maxdistxy);
  newFinderParameter -> setMaxSZDistance(maxdistsz);
  newFinderParameter -> setMaxXYDistanceFit(maxdistxyfit);
  newFinderParameter -> setMaxSZDistanceFit(maxdistszfit);
  newFinderParameter -> setMinimumHitNumber(minhitnumber);
  newFinderParameter -> setNumberXYThetaBins(xythetabins);
  newFinderParameter -> setNumberXYDzeroBins(xyd0bins);
  newFinderParameter -> setNumberXYOmegaBins(xyomegabins);
  newFinderParameter -> setNumberSZThetaBins(szthetabins);
  newFinderParameter -> setNumberSZDzeroBins(szd0bins);
  newFinderParameter -> setMaxDxy(maxdxy);
  newFinderParameter -> setMaxDsz(maxdsz);
  
  if(searchneighborhood == 0)
  {
    newFinderParameter -> setSearchNeighborhood(false);
  }
  else
  {
    newFinderParameter -> setSearchNeighborhood(true);
  }
  newFinderParameter -> setSaveRootFile(false);

  //set the vector of basic hits in which tracks should be found
  //here: use all hits deliverd by the track generator
  const Double_t timeInterval = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::FGD_TIME_INTERVAL_HITS);
  const Bool_t hasTimeInterval = fParams.ParamAsBool(esbroot::geometry::superfgd::DP::FGD_TIME_INTERVAL_HITS);

  bool exCludeBelow = fParams.ParamAsBool(esbroot::geometry::superfgd::DP::FGD_EXCLUDE_HITS_BELOW_TIME_INTERVAL);
  bool exCludeAbove = fParams.ParamAsBool(esbroot::geometry::superfgd::DP::FGD_EXCLUDE_HITS_ABOVE_TIME_INTERVAL);

  std::vector<pathfinder::basicHit> belowLimit;
  std::vector<pathfinder::basicHit> aboveLimit;

  for(Int_t i=0; i< hits.size(); ++i)
  {
    if(hasTimeInterval && !exCludeBelow && hits[i].ftime<=timeInterval)
    {
      belowLimit.emplace_back(pathfinder::basicHit(   hits[i].fHitPos.X()
                                                    , hits[i].fHitPos.Y()
                                                    , hits[i].fHitPos.Z()
                                                  )
                              );
    }
    
    if(hasTimeInterval && !exCludeAbove && hits[i].ftime>timeInterval)
    {
      aboveLimit.emplace_back(pathfinder::basicHit(   hits[i].fHitPos.X()
                                                    , hits[i].fHitPos.Y()
                                                    , hits[i].fHitPos.Z()
                                                  )
                              );
    }
  }
  
  Bool_t found(false);


  if(hasTimeInterval)
  {
    Bool_t belowfound(false);
    Bool_t abovefound(false);
    
    if(!exCludeBelow)
    {
      pathfinder::HoughTrafoTrackFinder belowTrackFinder;

      //setting steering parameter
      belowTrackFinder.setFinderParameter(*newFinderParameter);

      // 1. Find below limit
      belowTrackFinder.setInitialHits(belowLimit);

      //do the actual track finding
      belowfound = belowTrackFinder.find();
      if(belowfound)
      {
        foundTracks = belowTrackFinder.getTracks();
      }
      // std::cout << "Tracks below limit ["<< timeInterval <<"] " << foundTracks.size() << std::endl;
      LOG(debug) << "Tracks below limit ["<< timeInterval <<"] " << foundTracks.size();
    }
    else
    {
      // std::cout << "Hits below limit ["<< timeInterval <<"] are excluded by settings." << std::endl;
      LOG(debug) << "Hits below limit ["<< timeInterval <<"] are excluded by settings.";
    }
    

    if(!exCludeAbove)
    {
      pathfinder::HoughTrafoTrackFinder aboveTrackFinder;

      //setting steering parameter
      aboveTrackFinder.setFinderParameter(*newFinderParameter);

      // 2. Find above limit
      aboveTrackFinder.setInitialHits(aboveLimit);

      //do the actual track finding
      abovefound = aboveTrackFinder.find();

      if(abovefound)
      {
        const std::vector<pathfinder::TrackFinderTrack>& aboveTracks = aboveTrackFinder.getTracks();
        // std::cout << "Tracks above limit ["<< timeInterval <<"] " << aboveTracks.size() << std::endl;
        LOG(debug)  << "Tracks above limit ["<< timeInterval <<"] " << aboveTracks.size();
        for(Int_t tr = 0; tr < aboveTracks.size(); ++tr)
        {
          foundTracks.push_back(aboveTracks[tr]);
        }
      }
    }
    else
    {
      // std::cout << "Hits above limit ["<< timeInterval <<"] are excluded by settings." << std::endl;
      LOG(debug)  << "Hits above limit ["<< timeInterval <<"] are excluded by settings.";
    }

    found = belowfound || abovefound;
  }

  return found;
}

Bool_t FgdGenFitRecon::FindByIntervalsTracks(std::vector<ReconHit>& hits
                                , std::vector<pathfinder::TrackFinderTrack>& foundTracks
                                , FindTrackType trackType)
{
  LOG(debug) << "hits " << hits.size();
  cout << "hits " << hits.size();

  unsigned int use_vertex = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_USE_VERTEX);
  double vertexX = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_VERTEXX);
  double vertexY = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_VERTEXY);
  double maxdistxy = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_MAXDISTXY);
  double maxdistsz = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_MAXDISTSZ);
  double maxdistxyfit = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_MAXDISTXYFIT);
  double maxdistszfit = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_MAXDISTSZFIT);
  unsigned int minhitnumber = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_MINHITNUMBER);
  unsigned int xythetabins = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_XYTHETABINS);
  unsigned int xyd0bins = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_XYD0BINS);
  unsigned int xyomegabins = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_XYOMEGABINS);
  unsigned int szthetabins = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_SZTHETABINS);
  unsigned int szd0bins = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_SZD0BINS);
  double maxdxy = f_total_X + f_total_Y;
  double maxdsz = f_total_Z;
  unsigned int searchneighborhood = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_SEACHINTHENEIGHBORHOOD);

  pathfinder::FinderParameter* newFinderParameter = nullptr;
  switch(trackType)
  {
    case FindTrackType::HELIX:
        newFinderParameter= new pathfinder::FinderParameter(false, true); 
        newFinderParameter -> setFindCurler(false);
        break;
    case FindTrackType::CURL:
        newFinderParameter= new pathfinder::FinderParameter(false, true); 
        newFinderParameter -> setFindCurler(true);
        break;
    case FindTrackType::STRAIGHT_LINE:
    default:
        newFinderParameter= new pathfinder::FinderParameter(true, false); 
        newFinderParameter -> setFindCurler(false);
        break;
  }


  //  if(use_vertex == 0) newFinderParameter -> setUseVertex(false);
  //  if(use_vertex == 1) newFinderParameter->setUseVertex(true);

  if(use_vertex == 1) 
  {  
    std::pair<double, double> vertex(vertexX, vertexY);
    newFinderParameter -> setVertex(vertex);
  }

  newFinderParameter -> setMaxXYDistance(maxdistxy);
  newFinderParameter -> setMaxSZDistance(maxdistsz);
  newFinderParameter -> setMaxXYDistanceFit(maxdistxyfit);
  newFinderParameter -> setMaxSZDistanceFit(maxdistszfit);
  newFinderParameter -> setMinimumHitNumber(minhitnumber);
  newFinderParameter -> setNumberXYThetaBins(xythetabins);
  newFinderParameter -> setNumberXYDzeroBins(xyd0bins);
  newFinderParameter -> setNumberXYOmegaBins(xyomegabins);
  newFinderParameter -> setNumberSZThetaBins(szthetabins);
  newFinderParameter -> setNumberSZDzeroBins(szd0bins);
  newFinderParameter -> setMaxDxy(maxdxy);
  newFinderParameter -> setMaxDsz(maxdsz);
  
  if(searchneighborhood == 0)
  {
    newFinderParameter -> setSearchNeighborhood(false);
  }
  else
  {
    newFinderParameter -> setSearchNeighborhood(true);
  }
  newFinderParameter -> setSaveRootFile(false);

  //set the vector of basic hits in which tracks should be found
  //here: use all hits deliverd by the track generator
  const Double_t timeInterval = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::FGD_TIME_INTERVAL_HITS);
  const Bool_t hasTimeInterval = fParams.ParamAsBool(esbroot::geometry::superfgd::DP::FGD_TIME_INTERVAL_HITS);

  if(!hasTimeInterval)
  {
    std::string errMsg = "This type of track findnig requires time interval, but time interval was 0!";
    cout << errMsg << endl;
    LOG(fatal) << errMsg;
    throw errMsg;
  }

  std::sort(hits.begin(), hits.end(), [](ReconHit& rh1, ReconHit& rh2)->bool{ return rh1.ftime < rh2.ftime;});

  Bool_t found(false);

  std::vector<pathfinder::basicHit> intervalHits;
  pathfinder::HoughTrafoTrackFinder newTrackFinder;

  //setting steering parameter
  newTrackFinder.setFinderParameter(*newFinderParameter);

  Int_t hitsProcessed=0;
  Int_t interval = 1;
  while(hitsProcessed<hits.size())
  {
    while(hitsProcessed<hits.size() && hits[hitsProcessed].ftime < (interval*timeInterval))
    {
      intervalHits.emplace_back(pathfinder::basicHit(  hits[hitsProcessed].fHitPos.X()
                                              , hits[hitsProcessed].fHitPos.Y()
                                              , hits[hitsProcessed].fHitPos.Z()
                                            ));
      ++hitsProcessed;
    }
    // 2. Find tracks in interval
    newTrackFinder.setInitialHits(intervalHits);

    Bool_t foundInInterval = newTrackFinder.find();
    found = (found || foundInInterval);

    if(foundInInterval)
    {
      const std::vector<pathfinder::TrackFinderTrack>& tracks = newTrackFinder.getTracks();
      LOG(debug)  << "Tracks in time interval from ["<< (interval-1)*timeInterval <<"] " <<  " to ["<< interval*timeInterval <<"] " <<tracks.size();
      for(Int_t tr = 0; tr < tracks.size(); ++tr)
      {
        foundTracks.push_back(tracks[tr]);
      }
    }
    intervalHits.clear();
    ++interval;
  }


  return found;
}

Bool_t FgdGenFitRecon::FindUsingGraph(std::vector<ReconHit>& hits
                  , std::vector<pathfinder::TrackFinderTrack>& foundTracks
                  , Bool_t useOnlyLeaves)
{
  if(hits.empty())
  {
    return false;
  }

  BuildGraph(hits);

  // Print out the build graph
  for(Int_t i=0; i<hits.size(); ++i)
  {
    LOG(debug) << "i " << i;
    ReconHit* center = &hits[i];
    for(Int_t j=0; j<hits[i].fAllHits.size(); ++j)
    {
      Int_t ind = hits[i].fAllHits[j];
      ReconHit* localHit = &hits[ind];
      TVector3 diff = center->fmppcLoc - localHit->fmppcLoc;
      LOG(debug) << " Local Id "<< hits[i].fAllHits[j] << " \t X " << diff.X() << " \t Y " << diff.Y() << " \t Z " << diff.Z();
    }
    LOG(debug) << "X " << hits[i].fmppcLoc.X() << " Y " << hits[i].fmppcLoc.Y()<< " Z " << hits[i].fmppcLoc.Z();
    LOG(debug) << "Photons "<< " X " << hits[i].fphotons.X() << " Y " << hits[i].fphotons.Y()<< " Z " << hits[i].fphotons.Z();
    LOG(debug) << "=====";
  }


  FgdReconTemplate reconTemplates(freconFile.c_str());
  reconTemplates.SetUseOnlyLeaves(useOnlyLeaves);
  reconTemplates.LoadTemplates();

  std::vector<std::vector<Int_t>> tracks;

  ReconHit* currentHit = nullptr;
  ReconHit* nextHit = nullptr;
  ReconHit* previousHit = nullptr;
  for(size_t i=0; i<hits.size(); ++i)
  {
    if(hits[i].fIsVisited)
    {
      continue;
    }

    if(reconTemplates.IsLeaf(&hits[i], hits))
    {
      std::vector<int> track;
      track.push_back(i);

      hits[i].fIsLeaf = true;
      currentHit = &hits[i];

      while(reconTemplates.GetNextHit2(previousHit, currentHit, nextHit, hits)) // TODO2
      {
        if(nextHit->fIsLeaf || nextHit->fIsVisited)
        {
          break;
        }
        track.push_back(nextHit->fLocalId);
        currentHit->fIsVisited = true;
        previousHit = currentHit;
        currentHit = nextHit;
      }

      tracks.push_back(track);
    }
  }

  LOG(debug) <<"Leaves found " << tracks.size();
  Int_t totalHitsInTracks(0);

  for(Int_t i=0; i<tracks.size(); ++i)
  {
    std::vector<Int_t>& track = tracks[i];
    std::vector<pathfinder::basicHit> currentTrack;
    LOG(debug) << "Track " << i;
    Int_t z_mppc=-1;
    totalHitsInTracks+=track.size();
    for(Int_t j=0; j<track.size(); ++j)
    {
      Int_t ind = track[j];
      currentTrack.emplace_back(hits[ind].fHitPos.X()
                              , hits[ind].fHitPos.Y()
                              , hits[ind].fHitPos.Z());


      
      // Calculate cosine change beween 2 consecutive vectors - gradient
      TVector3 diffVec(0,0,0);
      if(j>=1)
      {
        ReconHit* one = &hits[track[j-1]];
        ReconHit* two = &hits[track[j]];
        diffVec = two->fmppcLoc - one->fmppcLoc;
      }


      Double_t radToDeg = 180/TMath::Pi();
      Double_t angle(0);
      Double_t zAxisAngle(0);

      TVector3 zAxisVec(0,0,1);

      TVector3 diffVec1(0,0,0);
      TVector3 diffVec2(0,0,0);

      Int_t distToCalc = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::FGD_GRAD_DIST);
      Int_t intervalToCal = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::FGD_GRAD_INTERVAL_DIST);

      Int_t indOne = j - distToCalc + 1;
      Int_t indTwo = j;

      Int_t indOneP = indOne - intervalToCal;
      Int_t indTwoP = indTwo - intervalToCal;

      if(j>= (distToCalc -1) )
      {
        ReconHit* one = &hits[track[indOne]];
        ReconHit* two = &hits[track[indTwo]];
        diffVec1 = two->fmppcLoc - one->fmppcLoc;
        zAxisAngle = radToDeg * zAxisVec.Angle(diffVec1);
      }

      if(j>= (distToCalc + intervalToCal -1) )
      {
        ReconHit* oneP = &hits[track[indOneP]];
        ReconHit* twoP = &hits[track[indTwoP]];
        diffVec2 = twoP->fmppcLoc - oneP->fmppcLoc;

        
        angle = radToDeg * diffVec1.Angle(diffVec2);
      }

      LOG(debug2) << "Id " << ind 
                  << " \tX " << hits[ind].fmppcLoc.X()
                  << " \tY " << hits[ind].fmppcLoc.Y()
                  << " \tZ " << hits[ind].fmppcLoc.Z() 
                  << " \tPhotons  " << "(" << hits[ind].fphotons.X() << "," << hits[ind].fphotons.Y() << "," << hits[ind].fphotons.Z() << ")"
                  << " \tChange  " << "(" << diffVec.X() << "," << diffVec.Y() << "," << diffVec.Z() << ")"
                  // << " \t(" << diffVec1.X() << "," << diffVec1.Y() << "," << diffVec1.Z() << ") "
                  // << " \t(" << diffVec2.X() << "," << diffVec2.Y() << "," << diffVec2.Z() << ") "
                  << " \tAngle (dist = " << distToCalc << ", interval =" << intervalToCal << ") " << angle
                  << " \tZ axis Angle (dist = " << distToCalc << ", interval =" << intervalToCal << ") " << zAxisAngle;
                  
    }
    pathfinder::TrackFinderTrack tr(pathfinder::TrackParameterFull(0.,0.,0.,0.,0.)
                                    , std::move(currentTrack));
    foundTracks.emplace_back(tr);
  }

  LOG(debug) << "Total hits in tracks " << totalHitsInTracks;

  return !foundTracks.empty();
}

void FgdGenFitRecon::BuildGraph(std::vector<ReconHit>& hits)
{
    // Create the position to which index in the vector it is pointing
    std::map<Long_t, Int_t> positionToId;
    for(Int_t i=0; i<hits.size(); ++i)
    {
      Int_t&& x = hits[i].fmppcLoc.X();
      Int_t&& y = hits[i].fmppcLoc.Y();
      Int_t&& z = hits[i].fmppcLoc.Z();

      Int_t&& ind = ArrInd(x,y,z);

      // GUARD agains double or more hits in the same cube
      if(positionToId.find(ind)==positionToId.end())
      {
        positionToId[ind] = i;
      }
      

      hits[i].fLocalHits.clear(); // Clear previous index positions
      hits[i].fLocalId = i;
    }

    auto checkNext = [&](Int_t x_pos, Int_t y_pos, Int_t z_pos, Int_t ind){
                                                                  Long_t&& key = ArrInd(x_pos,y_pos,z_pos);
                                                                  if(positionToId.find(key)!=positionToId.end())
                                                                  {
                                                                    hits[ind].fLocalHits.push_back(positionToId[key]);
                                                                    hits[ind].fAllHits.push_back(positionToId[key]);
                                                                  }
                                                                };

    auto checkNextEdge = [&](Int_t x_pos, Int_t y_pos, Int_t z_pos, Int_t ind){
                                                                  Long_t&& key = ArrInd(x_pos,y_pos,z_pos);
                                                                  if(positionToId.find(key)!=positionToId.end())
                                                                  {
                                                                    hits[ind].fLocalEdges.push_back(positionToId[key]);
                                                                    hits[ind].fAllHits.push_back(positionToId[key]);
                                                                  }
                                                                };

    auto checkNextCorner = [&](Int_t x_pos, Int_t y_pos, Int_t z_pos, Int_t ind){
                                                                  Long_t&& key = ArrInd(x_pos,y_pos,z_pos);
                                                                  if(positionToId.find(key)!=positionToId.end())
                                                                  {
                                                                    hits[ind].fLocalCorner.push_back(positionToId[key]);
                                                                    hits[ind].fAllHits.push_back(positionToId[key]);
                                                                  }
                                                                };

    for(Int_t i=0; i<hits.size(); ++i)
    {
      // Int_t&& x = hits[i].fHitPos.X();
      // Int_t&& y = hits[i].fHitPos.Y();
      // Int_t&& z = hits[i].fHitPos.Z();

      Int_t&& x = hits[i].fmppcLoc.X();
      Int_t&& y = hits[i].fmppcLoc.Y();
      Int_t&& z = hits[i].fmppcLoc.Z();

      // Check in X axis
      checkNext(x+1,y,z, i);
      checkNext(x-1,y,z, i);

      // Check in Y axis
      checkNext(x,y+1,z, i);
      checkNext(x,y-1,z, i);

      // Check in Z axis
      checkNext(x,y,z+1, i);
      checkNext(x,y,z-1, i);

      // Check in X,Y corners
      checkNextEdge(x+1,y+1,z, i);
      checkNextEdge(x+1,y-1,z, i);
      checkNextEdge(x-1,y+1,z, i);
      checkNextEdge(x-1,y-1,z, i);

      // Check in X,Z corners
      checkNextEdge(x+1,y,z+1, i);
      checkNextEdge(x+1,y,z-1, i);
      checkNextEdge(x-1,y,z+1, i);
      checkNextEdge(x-1,y,z-1, i);

      // Check in Y,Z corners
      checkNextEdge(x,y+1,z+1, i);
      checkNextEdge(x,y+1,z-1, i);
      checkNextEdge(x,y-1,z+1, i);
      checkNextEdge(x,y-1,z-1, i);

      // Check in X,Y,Z corners
      checkNextCorner(x+1,y+1,z+1, i);
      checkNextCorner(x+1,y+1,z-1, i);
      checkNextCorner(x+1,y-1,z+1, i);
      checkNextCorner(x+1,y-1,z-1, i);

      checkNextCorner(x-1,y+1,z+1, i);
      checkNextCorner(x-1,y+1,z-1, i);
      checkNextCorner(x-1,y-1,z+1, i);
      checkNextCorner(x-1,y-1,z-1, i);
    }
}

void FgdGenFitRecon::FitTracks(std::vector<pathfinder::TrackFinderTrack>& foundTracks)
{
    fTracksArray->Delete();
    
    // init geometry and mag. field
    TVector3 magField = fgdConstructor.GetMagneticField(); // values are in kGauss
    genfit::FieldManager::getInstance()->init(new genfit::ConstField(magField.X(),magField.Y(), magField.Z())); 
    genfit::MaterialEffects::getInstance()->init(new genfit::TGeoMaterialInterface());
    genfit::MaterialEffects::getInstance()->setDebugLvl(fDebuglvl_genfit);

    // init fitter
    std::shared_ptr<genfit::AbsKalmanFitter> fitter = make_shared<genfit::KalmanFitterRefTrack>();
    fitter->setMinIterations(fminGenFitInterations);
    fitter->setMaxIterations(fmaxGenFitIterations);
    fitter->setDebugLvl(fDebuglvl_genfit);

    std::vector<genfit::Track*> genTracks;
    int detId(1); // Detector id, it is the same, we only have one detector
    for(Int_t i =0; i <  foundTracks.size() ; i++)
    {
      std::vector<pathfinder::basicHit>& hitsOnTrack = const_cast<std::vector<pathfinder::basicHit>&>(foundTracks[i].getHitsOnTrack());
      // std::sort(hitsOnTrack.begin(), hitsOnTrack.end(), [](pathfinder::basicHit bh1, pathfinder::basicHit bh2){return bh1.getZ()<bh2.getZ();});

      // Set lower limit on track size
      if(hitsOnTrack.size()<fminHits)
      {
        LOG(debug) << "Track " << i << " below limit, continue with next track (" << hitsOnTrack.size() << " < " << fminHits << ")";
        continue;
      }
      
       // TODO2 extrack from finderTrack how to get initial guess for these values
      // =================================
      //const int pdg = 13;
      int pdg = 13;

      //TVector3 posM(fstartPos);
      TVector3 posM(hitsOnTrack[0].getX(),hitsOnTrack[0].getY(),hitsOnTrack[0].getZ());
      TVector3 momM(fstartMom);

      // First point should be the start of the track
      //TVector3 momM(hitsOnTrack[0].getX(),hitsOnTrack[0].getY(),hitsOnTrack[0].getZ());
      if(i==0)
      {
        pdg = 211;
        momM.SetXYZ(-0.187 ,   0.079 ,   0.150 );
      }

      if(i==1)
      {
        pdg = -11;
        momM.SetXYZ(-0.149, 0.013 , -0.074 );
      }

      // if(i==2)
      // {
      //   pdg = 13;
      //   momM.SetXYZ(-0.107, 0.211, -0.220 );
      // }

      // if(i==1)
      // {
      //   momM.SetXYZ(-0.070,-0.197,0.348);
      // }

      // if(i==1)
      // {
      //   pdg=2212;
      //   momM.SetX(0.343);
      //   momM.SetY(0.248);
      //   momM.SetZ(0.524);
      // }
      // =================================

      // approximate covariance
      double resolution = 0.1;
      TMatrixDSym hitCov(3);
      hitCov(0,0) = resolution*resolution;
      hitCov(1,1) = resolution*resolution;
      hitCov(2,2) = resolution*resolution;

      TMatrixDSym covM(6);
      for (int ci = 0; ci < 3; ++ci)
          covM(ci,ci) = resolution*resolution;
      for (int ci = 3; ci < 6; ++ci)
          covM(ci,ci) = covM(ci,ci) = pow(  ((resolution / hitsOnTrack.size()) / sqrt(3)), 2); 

      // trackrep
      genfit::AbsTrackRep* rep = new genfit::RKTrackRep(pdg);

      // smeared start state
      genfit::MeasuredStateOnPlane stateSmeared(rep);
      stateSmeared.setPosMomCov(posM, momM, covM);

      // create track
      TVectorD seedState(6);
      TMatrixDSym seedCov(6);
      stateSmeared.get6DStateCov(seedState, seedCov);
  
      genfit::Track* toFitTrack = new genfit::Track(rep, seedState, seedCov);

      LOG(debug) <<"******************************************* ";
      LOG(debug) <<"******    Track "<< i << "  ************************";
      LOG(debug) <<"******************************************* ";
      LOG(debug) <<"hitsOnTrack.size(); "<< hitsOnTrack.size();
      
      for(Int_t bh = 0; bh < hitsOnTrack.size(); ++bh)
      {
        TVectorD hitPos(3);
        hitPos(0) = hitsOnTrack[bh].getX();
        hitPos(1) = hitsOnTrack[bh].getY();
        hitPos(2) = hitsOnTrack[bh].getZ();

        genfit::AbsMeasurement* measurement = new genfit::SpacepointMeasurement(hitPos, hitCov, detId, 0, nullptr);
        std::vector<genfit::AbsMeasurement*> measurements{measurement};

        toFitTrack->insertPoint(new genfit::TrackPoint(measurements, toFitTrack));
      }

      try
      {
        //check
        toFitTrack->checkConsistency();

        // do the fit
        fitter->processTrack(toFitTrack, true);

        //check
        toFitTrack->checkConsistency();

        PrintFitTrack(*toFitTrack);

        LOG(debug) <<"******************************************* ";
        genfit::FitStatus* fiStatuStatus = toFitTrack->getFitStatus();

        if(fiStatuStatus->isFitted())
        {
          genTracks.push_back(toFitTrack);
        }
      }
      catch(genfit::Exception& e)
      {
          LOG(error) <<"Exception, when tryng to fit track";
          LOG(error) << e.what();
      }
    }
  
    if(isGenFitVisualization)
    {
      fdisplay->addEvent(genTracks);
    }
}

void FgdGenFitRecon::DefineMaterials() 
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

void FgdGenFitRecon::PrintFitTrack(genfit::Track& fitTrack)
{
  const genfit::MeasuredStateOnPlane& me = fitTrack.getFittedState();
  LOG(debug)<< "Momentum  " << (me.getMom()).Mag();
  LOG(debug)<< " X  " << (me.getMom()).X()<< " Y " << (me.getMom()).Y()<< " Z  " << (me.getMom()).Z();

  genfit::FitStatus* fiStatuStatus = fitTrack.getFitStatus();
  fiStatuStatus->Print();

  LOG(debug)<< "fiStatuStatus->isFitted()  " << fiStatuStatus->isFitted();
  LOG(debug)<< "fiStatuStatus->isFitConverged()  " << fiStatuStatus->isFitConverged();
  LOG(debug)<< "fiStatuStatus->isFitConvergedFully()  " << fiStatuStatus->isFitConvergedFully();
  LOG(debug)<< "fiStatuStatus->isFitConvergedPartially()  " << fiStatuStatus->isFitConvergedPartially();
  LOG(debug)<< "fitTrack.getNumPoints() " << fitTrack.getNumPoints();
}

Long_t FgdGenFitRecon::ArrInd(int x, int y, int z)
{
  return (x*f_bin_Y*f_bin_Z + y*f_bin_Z+z);
}

// -------------------------------------------------------------------------

}// namespace superfgd
}// namespace reconstruction
}// namespace esbroot
