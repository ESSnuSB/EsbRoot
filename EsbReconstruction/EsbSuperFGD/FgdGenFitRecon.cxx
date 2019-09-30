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
  , fHitArray(nullptr)
  , isDefinedMaterials(false)
  , fDebuglvl_genfit(0)
  , fmediaFile("")
  , fTracksArray(nullptr)
  , fdisplay(nullptr)
  , isGenFitVisualization(false)
  , fGenFitVisOption("")
  , fminGenFitInterations(2)
  , fmaxGenFitIterations(4)
  , fminHits(25)
  , ffinder(TrackFinder::HOUGH_PATHFINDER_CURL)
{ 
}
// -------------------------------------------------------------------------

// -----   Constructor   -------------------------------------------
FgdGenFitRecon::FgdGenFitRecon(const char* name
                          , const char* geoConfigFile
                          , const char* mediaFile
                          , Int_t verbose
                          , double debugLlv
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
  , fdisplay(nullptr)
  , isGenFitVisualization(visualize)
  , fGenFitVisOption(visOption)
  , fminGenFitInterations(2)
  , fmaxGenFitIterations(4)
  , fminHits(25)
  , ffinder(TrackFinder::HOUGH_PATHFINDER_CURL)
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
    std::vector<std::vector<TVector3>> foundTracks;
    std::vector<Int_t> trackPdgs;
    std::vector<TVector3> points;

    bool rc = GetHits(allhits);
    if(rc)
    { 
      LOG(debug) <<" Hits to retrieve tracks from " << allhits.size();
      switch(ffinder)
      {
        case TrackFinder::HOUGH_PATHFINDER_LINE:
                                          ConvertHitToVec(points, allhits);
                                          rc = FindUsingHough(points, allhits, foundTracks, trackPdgs, FindTrackType::STRAIGHT_LINE);
                                          break;
        case TrackFinder::HOUGH_PATHFINDER_HELIX:
                                          ConvertHitToVec(points, allhits);
                                          rc = FindUsingHough(points, allhits, foundTracks, trackPdgs, FindTrackType::HELIX);
                                          break;
        case TrackFinder::HOUGH_PATHFINDER_CURL:
                                          ConvertHitToVec(points, allhits);
                                          rc = FindUsingHough(points, allhits, foundTracks, trackPdgs, FindTrackType::CURL);
                                          break;
        case TrackFinder::GRAPH:
                                          rc = FindUsingGraph(allhits, foundTracks, trackPdgs);
                                          break;

        case TrackFinder::GRAPH_HOUGH_PATHFINDER:
                                          rc = FindUsingGraphHough(allhits, foundTracks, trackPdgs);
                                          break;
        default: 
                rc = false;
                break;
      }
    }

    if(rc)
    {
      LOG(debug) <<" Tracks found " << foundTracks.size();
      FitTracks(foundTracks, trackPdgs);
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

  LOG(debug) << "allHits.size()" << allHits.size();

  return (allHits.size() > 0);
}

Bool_t FgdGenFitRecon::FindUsingHough(std::vector<TVector3>& points
                                , std::vector<ReconHit>& hits
                                , std::vector<std::vector<TVector3>>& foundTracks
                                , std::vector<Int_t>& trackPdgs
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
  for(size_t i=0; i< points.size(); ++i)
  {
    allHits.emplace_back(pathfinder::basicHit(  points[i].X()
                                                  , points[i].Y()
                                                  , points[i].Z()
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
    std::vector<pathfinder::TrackFinderTrack> pfTracks = newTrackFinder.getTracks();
    for(Int_t i =0; i <  pfTracks.size() ; i++)
    {
      pathfinder::TrackFinderTrack& trFinder = pfTracks[i];
      const std::vector<pathfinder::basicHit>& hitsOnTrack = trFinder.getHitsOnTrack();
      std::vector<TVector3> track;
      for(size_t j =0; j< hitsOnTrack.size(); ++j)
      {
          track.emplace_back(hitsOnTrack[j].getX(),hitsOnTrack[j].getY(),hitsOnTrack[j].getZ());
      }
      foundTracks.push_back(track);
    }

    std::vector<std::vector<ReconHit*>> forPdgTracks;
    // Get the pdg codes
    for(size_t i = 0; i <  foundTracks.size() ; i++)
    {
      LOG(debug2) << "=============================== ";
      LOG(debug2) << "Track " << i;

      std::vector<TVector3>& tr = foundTracks[i];
      std::vector<ReconHit*> trPdg;
      for(size_t j = 0; j <  tr.size() ; j++)
      {
        ReconHit temp;
        temp.fHitPos.SetXYZ(tr[j].X(), tr[j].Y(),tr[j].Z());

        std::vector<ReconHit>::iterator iter = find(hits.begin(), hits.end(), temp);
        if(iter!=hits.end())
        {
          ReconHit* hit = &(*iter);
          trPdg.push_back(hit);
          LOG(debug2) << " \tX " << iter->fmppcLoc.X() << " \tY " << iter->fmppcLoc.Y() << " \tZ " << iter->fmppcLoc.Z();
        }
      }
      LOG(debug2) << "=============================== ";
      forPdgTracks.push_back(trPdg);
    }
    GetPdgCode(forPdgTracks, trackPdgs);
  }

  return found;
}

Bool_t FgdGenFitRecon::FindUsingGraph(std::vector<ReconHit>& hits
                  , std::vector<std::vector<TVector3>>& foundTracks
                  , std::vector<Int_t>& trackPdgs)
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
      ReconHit* localHit = hits[i].fAllHits[j];
      TVector3 diff = center->fmppcLoc - localHit->fmppcLoc;
      LOG(debug) << " Local Id "<< hits[i].fAllHits[j]->fLocalId << " \t X " << diff.X() << " \t Y " << diff.Y() << " \t Z " << diff.Z();
    }
    LOG(debug) << "X " << hits[i].fmppcLoc.X() << " Y " << hits[i].fmppcLoc.Y()<< " Z " << hits[i].fmppcLoc.Z();
    LOG(debug) << "Photons "<< " X " << hits[i].fphotons.X() << " Y " << hits[i].fphotons.Y()<< " Z " << hits[i].fphotons.Z();
    LOG(debug) << "=====";
  }


  FgdReconTemplate reconTemplates;

  std::vector<std::vector<ReconHit*>> tracks;

  ReconHit* currentHit = nullptr;
  ReconHit* nextHit = nullptr;
  ReconHit* previousHit = nullptr;
  for(size_t i=0; i<hits.size(); ++i)
  {
    if(hits[i].fIsVisited)
    {
      continue;
    }

    if(reconTemplates.IsLeaf(&hits[i]))
    {
      std::vector<ReconHit*> track;
      track.push_back(&hits[i]);

      hits[i].fIsLeaf = true;
      currentHit = &hits[i];

      while(reconTemplates.GetNextHit(previousHit, currentHit, nextHit)) 
      {
        if(nextHit->fIsLeaf || nextHit->fIsVisited)
        {
          break;
        }
        track.push_back(nextHit);
        currentHit->fIsVisited = true;
        previousHit = currentHit;
        currentHit = nextHit;
      }

      tracks.push_back(track);
    }
  }

  LOG(debug) <<"Leaves found " << tracks.size(); // Initially leaves are equal to the number of tracks

  CalculateGrad(tracks);

  std::vector<std::vector<ReconHit*>> splitTracks;

  SplitTrack(tracks, splitTracks);

  Int_t totalHitsInTracks(0);
  for(size_t i = 0; i<splitTracks.size(); ++i)
  {
    std::vector<ReconHit*>& track = splitTracks[i];
    std::vector<TVector3> currentTrack;
    LOG(debug2) << "=============================== ";
    LOG(debug2) << "Track " << i;
    
    totalHitsInTracks+=track.size();

    for(size_t j = 0; j<track.size(); ++j)
    {
      ReconHit* trackHit = track[j];

      currentTrack.emplace_back(trackHit->fHitPos.X()
                              , trackHit->fHitPos.Y()
                              , trackHit->fHitPos.Z());   

      LOG(debug2) << " \tX " << trackHit->fmppcLoc.X() << " \tY " << trackHit->fmppcLoc.Y() << " \tZ " << trackHit->fmppcLoc.Z();    
    }
    LOG(debug2) << "=============================== ";

    foundTracks.push_back(currentTrack);
  }

  GetPdgCode(splitTracks, trackPdgs);

  LOG(debug) << "Total hits in tracks " << totalHitsInTracks;

  return !foundTracks.empty();
}

Bool_t FgdGenFitRecon::FindUsingGraphHough(std::vector<ReconHit>& hits
                  , std::vector<std::vector<TVector3>>& foundTracks
                  , std::vector<Int_t>& trackPdgs)
{
  
  Bool_t rc(false);

  std::vector<std::vector<TVector3>> graphTracks;
  std::vector<Int_t> graphPdgs;
  rc = FindUsingGraph(hits, graphTracks, graphPdgs);

  if(rc)
  {
    // Traverse through the found graph tracks and use hough transform for each track
    // add the found hough track with the biggest number of found hits
    for(size_t i = 0; i < graphTracks.size(); ++i)
    {
      std::vector<Int_t> dummyPdgs;
      size_t biggestSize(0);
      std::vector<TVector3>* trackToAdd = nullptr;

      // 1. Line hough ========================
      std::vector<std::vector<TVector3>> hough_Line;
      if(FindUsingHough(graphTracks[i], hits, hough_Line, dummyPdgs, FindTrackType::STRAIGHT_LINE)
          && !hough_Line.empty())
      {
        size_t localBiggest(0);
        size_t ind(0);
        for(size_t j = 0; j < hough_Line.size(); ++j)
        {
          if(localBiggest < hough_Line[j].size())
          {
            localBiggest = hough_Line[j].size();
            ind = j;
          }
        }

        if(localBiggest > biggestSize)
        {
          biggestSize = hough_Line[ind].size();
          trackToAdd = &hough_Line[ind];
          LOG(debug) << "Hough transform from graph [line] size of track is: " << hough_Line[ind].size();
        }
      }
      LOG(debug)<< "Hough transform [line] " << hough_Line.size();
      dummyPdgs.clear();
      // ======================================

      // 2. Helix hough ========================
      std::vector<std::vector<TVector3>> hough_Helix;
      if(FindUsingHough(graphTracks[i], hits, hough_Helix, dummyPdgs, FindTrackType::HELIX)
        && !hough_Line.empty()) 
      {
        size_t localBiggest(0);
        size_t ind(0);
        for(size_t j = 0; j < hough_Helix.size(); ++j)
        {
          if(localBiggest < hough_Helix[j].size())
          {
            localBiggest = hough_Helix[j].size();
            ind = j;
          }
        }

        if(localBiggest > biggestSize)
        {
          biggestSize = hough_Helix[ind].size();
          trackToAdd = &hough_Helix[ind];
          LOG(debug) << "Hough transform from graph [helix] size of track is: " << hough_Helix[ind].size();
        }
      }
      LOG(debug)<< "Hough transform [helix] " << hough_Helix.size();
      dummyPdgs.clear();
      // ======================================

      // 3. Curl hough ========================
      std::vector<std::vector<TVector3>> hough_Curl;
      if(FindUsingHough(graphTracks[i], hits, hough_Curl, dummyPdgs, FindTrackType::CURL)
        && !hough_Line.empty()) 
      {
        size_t localBiggest(0);
        size_t ind(0);
        for(size_t j = 0; j < hough_Curl.size(); ++j)
        {
          if(localBiggest < hough_Curl[j].size())
          {
            localBiggest = hough_Curl[j].size();
            ind = j;
          }
        }

        if(localBiggest > biggestSize)
        {
          biggestSize = hough_Curl[ind].size();
          trackToAdd = &hough_Curl[ind];
          LOG(debug) << "Hough transform from graph [helix] size of track is: " << hough_Curl[ind].size();
        }
      }
      LOG(debug)<< "Hough transform [curl] " << hough_Curl.size();
      dummyPdgs.clear();
      // ======================================

      if(trackToAdd!=nullptr)
      {
        foundTracks.emplace_back(*trackToAdd);
        trackPdgs.push_back(graphPdgs[i]);
      }
      else
      {
        foundTracks.push_back(graphTracks[i]);
        trackPdgs.push_back(graphPdgs[i]);
        LOG(debug)<< "Hough transform could not find track, adding initial graph track. ";
      }
      
    }
  }

  return rc;
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
      

      hits[i].fAllHits.clear(); // Clear previous index positions
      hits[i].fLocalId = i;
    }


    auto checkNext = [&](Int_t x_pos, Int_t y_pos, Int_t z_pos, Int_t ind){
                                                                  Long_t&& key = ArrInd(x_pos,y_pos,z_pos);
                                                                  if(positionToId.find(key)!=positionToId.end())
                                                                  {
                                                                    ReconHit* toAdd = &hits[positionToId[key]];
                                                                    hits[ind].fAllHits.push_back(toAdd);
                                                                  }
                                                                };

    for(Int_t i=0; i<hits.size(); ++i)
    {
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
      checkNext(x+1,y+1,z, i);
      checkNext(x+1,y-1,z, i);
      checkNext(x-1,y+1,z, i);
      checkNext(x-1,y-1,z, i);

      // Check in X,Z corners
      checkNext(x+1,y,z+1, i);
      checkNext(x+1,y,z-1, i);
      checkNext(x-1,y,z+1, i);
      checkNext(x-1,y,z-1, i);

      // Check in Y,Z corners
      checkNext(x,y+1,z+1, i);
      checkNext(x,y+1,z-1, i);
      checkNext(x,y-1,z+1, i);
      checkNext(x,y-1,z-1, i);

      // Check in X,Y,Z corners
      checkNext(x+1,y+1,z+1, i);
      checkNext(x+1,y+1,z-1, i);
      checkNext(x+1,y-1,z+1, i);
      checkNext(x+1,y-1,z-1, i);

      checkNext(x-1,y+1,z+1, i);
      checkNext(x-1,y+1,z-1, i);
      checkNext(x-1,y-1,z+1, i);
      checkNext(x-1,y-1,z-1, i);
    }
}

void FgdGenFitRecon::CalculateGrad(std::vector<std::vector<ReconHit*>>& tracks)
{
  const Int_t distToCalc = fParams.ParamAsInt(esbroot::geometry::superfgd::DP::FGD_GRAD_DIST);
  const Int_t intervalToCal = fParams.ParamAsInt(esbroot::geometry::superfgd::DP::FGD_GRAD_INTERVAL_DIST);
  const Double_t radToDeg = 180/TMath::Pi();
  TVector3 zAxisVec(0,0,1);

  for(Int_t i=0; i<tracks.size(); ++i)
  {
    std::vector<ReconHit*>& track = tracks[i];
    LOG(debug2) << "=================================== ";
    LOG(debug2) << "Track " << i;

    for(Int_t j=0; j<track.size(); ++j)
    {

      ReconHit* currentHit = track[j];

      // 1. Calculate direction from the previous hit
      TVector3 diffVec(0,0,0);
      if(j>=1)
      {
        ReconHit* previous = track[j-1];
        diffVec = currentHit->fmppcLoc - previous->fmppcLoc;
      }

      // 2.Calculate cosine change beween 2 consecutive vectors - gradient
      Double_t diffAngle(0);    // Difference angle between two vectors - the vectors are the difference in position (grad (position))
                                // between two hits distToCalc cubes apart. The angles measures the angle change of the track
                                // from hit to hit. distToCalc distance is selected to smooth out the change in angle.

      Double_t zAxisAngle(0);   // Vector angle between the z axis and the vector difference between the current hit and the hit distToCalc 
                                // poisitions back.

      TVector3 diffVec1(0,0,0);
      TVector3 diffVec2(0,0,0);

      Int_t indOne = j - distToCalc + 1;
      Int_t indTwo = j; // current hit

      if(j>= (distToCalc -1) )
      {
        ReconHit* one = track[indOne];
        ReconHit* two = track[indTwo]; 
        diffVec1 = two->fmppcLoc - one->fmppcLoc;
        zAxisAngle = radToDeg * zAxisVec.Angle(diffVec1);

        // The z angle is calculated for the current hit
        currentHit->fZaxisAngle = zAxisAngle;
      }

      Int_t indOneP = indOne - intervalToCal;
      Int_t indTwoP = indTwo - intervalToCal;
      if(j>= (distToCalc + intervalToCal -1) )
      {
        ReconHit* oneP = track[indOneP];
        ReconHit* twoP = track[indTwoP];
        diffVec2 = twoP->fmppcLoc - oneP->fmppcLoc;

        diffAngle = radToDeg * diffVec1.Angle(diffVec2);

        // The angle is calculated for the current hit
        currentHit->fChangeAngle = diffAngle;
      }

      LOG(debug2) << "Id " << currentHit->fLocalId
                  << " \tX " << currentHit->fmppcLoc.X()
                  << " \tY " << currentHit->fmppcLoc.Y()
                  << " \tZ " << currentHit->fmppcLoc.Z() 
                  << " \tPhotons  " << "(" << currentHit->fphotons.X() << "," << currentHit->fphotons.Y() << "," << currentHit->fphotons.Z() << ")"
                  << " \tChange  " << "(" << diffVec.X() << "," << diffVec.Y() << "," << diffVec.Z() << ")"
                  // << " \t(" << diffVec1.X() << "," << diffVec1.Y() << "," << diffVec1.Z() << ") "
                  // << " \t(" << diffVec2.X() << "," << diffVec2.Y() << "," << diffVec2.Z() << ") "
                  << " \tAngle (dist = " << distToCalc << ", interval =" << intervalToCal << ") " << diffAngle
                  << " \tZ axis Angle (dist = " << distToCalc << ", interval =" << intervalToCal << ") " << zAxisAngle;
    }

    LOG(debug2) << "=================================== ";
  }
}

void FgdGenFitRecon::SplitTrack(std::vector<std::vector<ReconHit*>>& originalTracks, std::vector<std::vector<ReconHit*>>& splitTracks)
{
  const Double_t gradAllowedDiff = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::FGD_GRAD_ALLOWABLE_DIFF);
  LOG(debug) << "Initial tracks size " << originalTracks.size();

  for(Int_t i=0; i<originalTracks.size(); ++i)
  {
    std::vector<ReconHit*>& track = originalTracks[i];

    if(track.empty())
    {
      continue;
    }

    std::vector<ReconHit*> trackToAdd;
    trackToAdd.push_back(track[0]); // Add the first hit to start the track

    for(Int_t j=1; j<track.size(); ++j)
    {
      ReconHit* currentHit = track[j];
      ReconHit* previousHit = track[j-1];

      if(currentHit->fChangeAngle == 0)
      {
        // If there is no change add the track
        // Also '0' indicates that the track hit is too short to calculate the 
        // the change.
        // Only compare if both the current and previous angle is not zero.
        trackToAdd.push_back(currentHit);
      }
      else if(  (currentHit->fChangeAngle != 0) && (previousHit->fChangeAngle != 0)  )
      {
        Double_t differenceInAngle = currentHit->fChangeAngle - previousHit->fChangeAngle;
        Double_t absDiff = std::fabs(differenceInAngle);
        if(absDiff >= gradAllowedDiff)
        {
          // If the difference is not allowed add the current track hit points
          // and reset it to start a new track.
          splitTracks.emplace_back(trackToAdd);
          trackToAdd.clear();
        }
        trackToAdd.push_back(currentHit);
      }
    }

    splitTracks.emplace_back(trackToAdd);
  }

  LOG(debug) << "Split tracks size " << splitTracks.size();
}


Bool_t FgdGenFitRecon::CalculateInitialMomentum(const std::vector<TVector3>& track,const TVector3& magField, TVector3& momentum , TVector3& momentumLoss)
{
  if(track.empty())
  {
      momentum.SetXYZ(0.,0.,0);
      momentumLoss.SetXYZ(0.,0.,0);
      return false;
  }

  Int_t segment = fParams.ParamAsInt(esbroot::geometry::superfgd::DP::FGD_TRACK_MOMENTUM_SEGMENT);
  Int_t avgInitialPoints = fParams.ParamAsInt(esbroot::geometry::superfgd::DP::FGD_INITIAL_TRACK_POINTS_MOMENTUM);

  const Int_t defaultSegment = 3;
  segment = (segment<=0) ? defaultSegment : segment;
  avgInitialPoints = (avgInitialPoints<=0) ? defaultSegment : avgInitialPoints;

  // If the track  lenght is less than the set segment, calculate the momentum from the arc of the whole track
  // using for the 3 points the begin, end and all the middle points between them
  Int_t lengthSize = (track.size() < segment ) ? (track.size() -1) : (segment -1) ;

  std::vector<TVector3> trackMomentums;
  std::vector<TVector3> tarckMomentumLosses;

  for(size_t i = lengthSize; i< track.size() ; ++i)
  {
    size_t p1_pos = i - lengthSize;
    TVector3 p1 = track[p1_pos];
    TVector3 p3 = track[i];

    size_t p2_pos = p1_pos + 1;
    std::vector<TVector3> segmentMomentums;

    while(p2_pos<i)
    {
      TVector3 segmentMom(0,0,0);
      TVector3 p2 = track[p2_pos];
      if(CalculateMomentum(p1, p2, p3, magField, segmentMom))
      {
        segmentMomentums.push_back(segmentMom);
      }
      ++p2_pos;
    }

    Double_t xMom(0);
    Double_t yMom(0);
    Double_t zMom(0);
    for(size_t j = 0; j< segmentMomentums.size() ; ++j)
    {
      xMom+=segmentMomentums[j].X();
      yMom+=segmentMomentums[j].Y();
      zMom+=segmentMomentums[j].Z();
    }

    xMom = xMom/segmentMomentums.size();
    yMom = yMom/segmentMomentums.size();
    zMom = zMom/segmentMomentums.size();

    trackMomentums.emplace_back(xMom, yMom, zMom);
  }

  for(size_t i = 1; i< trackMomentums.size() ; ++i)
  {
    TVector3 momLoss = trackMomentums[i] - trackMomentums[i-1];
    tarckMomentumLosses.push_back(momLoss);
  }

  Double_t xTrackMom(0);
  Double_t yTrackMom(0);
  Double_t zTrackMom(0);
  for(size_t i = 0; i < avgInitialPoints && i < trackMomentums.size() ; ++i)
  {
    xTrackMom+=trackMomentums[i].X();
    yTrackMom+=trackMomentums[i].Y();
    zTrackMom+=trackMomentums[i].Z();;
  }

  xTrackMom = xTrackMom/ avgInitialPoints;
  yTrackMom = yTrackMom/ avgInitialPoints;
  zTrackMom = zTrackMom/ avgInitialPoints;

  momentum.SetXYZ(xTrackMom, yTrackMom, zTrackMom);


  Double_t xTrackMomLoss(0);
  Double_t yTrackMomLoss(0);
  Double_t zTrackMomLoss(0);
  for(size_t i = 0; i < avgInitialPoints && i < tarckMomentumLosses.size() ; ++i)
  {
    xTrackMomLoss+=tarckMomentumLosses[i].X();
    yTrackMomLoss+=tarckMomentumLosses[i].Y();
    zTrackMomLoss+=tarckMomentumLosses[i].Z();;
  }

  xTrackMomLoss = xTrackMomLoss/ avgInitialPoints;
  yTrackMomLoss = yTrackMomLoss/ avgInitialPoints;
  zTrackMomLoss = zTrackMomLoss/ avgInitialPoints;

  momentumLoss.SetXYZ(xTrackMomLoss, yTrackMomLoss, zTrackMomLoss);


  LOG(debug3) << " =========================== Track Momentum =========================== ";
  for(size_t i = 0; i < trackMomentums.size() ; ++i)
  {
    LOG(debug3) << " Track Momentum " << "\tMag " <<  trackMomentums[i].Mag() 
                                      << "\tX " <<  trackMomentums[i].X()
                                      << "\tY " <<  trackMomentums[i].Y()
                                      << "\tZ " <<  trackMomentums[i].Z();
  }
  LOG(debug3) << " ====================================================== ";

  LOG(debug3) << " =========================== Track Momentum Loss =========================== ";
  for(size_t i = 0; i < tarckMomentumLosses.size() ; ++i)
  {
    LOG(debug3) << " Track Momentum " << "\tMag " <<  tarckMomentumLosses[i].Mag() 
                                      << "\tX " <<  tarckMomentumLosses[i].X()
                                      << "\tY " <<  tarckMomentumLosses[i].Y()
                                      << "\tZ " <<  tarckMomentumLosses[i].Z();
  }
  LOG(debug3) << " ====================================================== ";

  Double_t inf = std::numeric_limits<Double_t>::infinity();

  return (!std::isnan(momentum.Mag()) && momentum.Mag()!=inf && trackMomentums.size() > 0 );
}

Bool_t FgdGenFitRecon::CalculateMomentum(const TVector3& p1, const TVector3& p2, const TVector3& p3 , const TVector3& magField, TVector3& momentum)
{
  //
  //  p [Gev/c] = e [1.6 x 10^-19 coulumb] * B [T] * R [m]
  //

  Bool_t rc(false);

  // For calculation charge is taken as 1 unit of 'e'
  const Double_t charge = 1.;

  Double_t inf = std::numeric_limits<Double_t>::infinity();

  TVector3 x_axis(1,0,0);
  TVector3 y_axis(0,1,0);
  TVector3 z_axis(0,0,1);

  TVector3 length = p3 - p1;
  Double_t x_angle = x_axis.Angle(length);
  Double_t y_angle = y_axis.Angle(length);
  Double_t z_angle = z_axis.Angle(length);

  // For each magnetic field plane calculate it for the perpendicular projections
  if(magField.X()!=0)
  {
    TVector3 mag_point1(0,p1.Y(), p1.Z());
    TVector3 mag_point2(0,p2.Y(), p2.Z());
    TVector3 mag_point3(0,p3.Y(), p3.Z());
    Double_t radius = GetRadius(mag_point1,mag_point2,mag_point3); // radius is returned in [cm]

    if(!std::isnan(radius) && radius!=inf)
    {
      Double_t R = radius/100.; // convert in meters
      Double_t magField_T = magField.X() / 10.; // convert from kGauss to Tesla units
      Double_t mom = charge * R * magField_T;

      Double_t mom_x = std::cos(x_angle) * mom;
      Double_t mom_y = std::cos(y_angle) * mom;
      Double_t mom_z = std::cos(z_angle) * mom;

      momentum.SetX(mom_x);
      momentum.SetY(mom_y);
      momentum.SetZ(mom_z);
      rc = true;
    }
  }

  if(magField.Y()!=0)
  {
    TVector3 mag_point1(p1.X(), 0. , p1.Z());
    TVector3 mag_point2(p2.X(), 0. , p2.Z());
    TVector3 mag_point3(p3.X(), 0. , p3.Z());
    Double_t radius = GetRadius(mag_point1,mag_point2,mag_point3); // radius is returned in [cm]

    if(!std::isnan(radius) && radius!=inf)
    {
      Double_t R = radius/100.; // convert in meters
      Double_t magField_T = magField.Y() / 10.; // convert from kGauss to Tesla units
      Double_t mom = charge * R * magField_T;

      Double_t mom_x = std::cos(x_angle) * mom + momentum.X();
      Double_t mom_y = std::cos(y_angle) * mom + momentum.Y();
      Double_t mom_z = std::cos(z_angle) * mom + momentum.Z();

      momentum.SetX(mom_x);
      momentum.SetY(mom_y);
      momentum.SetZ(mom_z);
      rc = true;
    }
  }
  
  if(magField.Z()!=0)
  {
    TVector3 mag_point1(p1.X(), p1.Y() , 0.);
    TVector3 mag_point2(p2.X(), p2.Y() , 0.);
    TVector3 mag_point3(p3.X(), p3.Y() , 0.);
    Double_t radius = GetRadius(mag_point1,mag_point2,mag_point3); // radius is returned in [cm]

    if(!std::isnan(radius) && radius!=inf)
    {
      Double_t R = radius/100.; // convert in meters
      Double_t magField_T = magField.Z() / 10.; // convert from kGauss to Tesla units
      Double_t mom = charge * R * magField_T;

      Double_t mom_x = std::cos(x_angle) * mom + momentum.X();
      Double_t mom_y = std::cos(y_angle) * mom + momentum.Y();
      Double_t mom_z = std::cos(z_angle) * mom + momentum.Z();

      momentum.SetX(mom_x);
      momentum.SetY(mom_y);
      momentum.SetZ(mom_z);
      rc = true;
    }
  }

  return rc;
}


// Calculate the radius from 3 points using the "Menger curvature" theorem
Double_t FgdGenFitRecon::GetRadius(const TVector3& p1, const TVector3& p2, const TVector3& p3)
{
  //
  //          p1
  //          /\\
  //     y   /  \\  x
  //        /    \\
  //       /______\\
  //      p2        p3
  //          z
  //
  //                    2 sin (Angle(xyz))
  //      c(x,y,z) = ------------------------
  //                        |x - z|
  //
  //    where Angle(xyz) is the angle from point p2 to point p1 and p3
  //    and |x - z| is the magnitude of the line connecting points p2 and p3

  TVector3 x = p3 - p1;
  TVector3 y = p2 - p1;
  TVector3 z = p2 - p3;

  Double_t angle = y.Angle(x);
  Double_t x_z_Mag = z.Mag();

  Double_t c = (2*std::sin(angle))/x_z_Mag;
  Double_t R = 1./c;

  LOG(debug3) << "Radius is -> " << R << " [cm]";

  return R;
}

void FgdGenFitRecon::GetPdgCode(std::vector<std::vector<ReconHit*>>& tracks
                  , std::vector<Int_t>& trackPdgs)
{
  const Int_t defaultPdg = 13; // Muon pdg code

  for(size_t i=0; i<tracks.size(); ++i)
  {
    std::vector<ReconHit*>& track = tracks[i];

    Double_t totalPhotons(0);

    for(size_t j=0; j<track.size(); ++j)
    {
      ReconHit* trackHit = track[j];  
      totalPhotons+= (trackHit->fphotons.X() + trackHit->fphotons.Y() + trackHit->fphotons.Z());
    }

    Double_t avgPhoto = totalPhotons / track.size();
    LOG(debug2) << "========== Track  ========= " << i << "  ========= ";
    LOG(debug2) << "Average photons \t" << avgPhoto;
    LOG(debug2) << "=============================== " << i;

    Int_t pdg(defaultPdg);
    for(size_t k=0; k<fpdgAvgPh.size(); ++k)
    {
      PdgFromPhotons& p = fpdgAvgPh[k];
      if(p.GetPdg(avgPhoto, pdg))
      {
        break;
      }
    }

    trackPdgs.push_back(pdg);
  }
}

void FgdGenFitRecon::ConvertHitToVec(std::vector<TVector3>& points, std::vector<ReconHit>& hits)
{
  for(size_t i=0; i< hits.size(); ++i)
  {
    points.emplace_back(hits[i].fHitPos.X()
                      , hits[i].fHitPos.Y() 
                      , hits[i].fHitPos.Z());
  }
}

void FgdGenFitRecon::FitTracks(std::vector<std::vector<TVector3>>& foundTracks, std::vector<Int_t>& trackPdgs)
{
    if(foundTracks.size()!=trackPdgs.size())
    {
      LOG(fatal) << "PdgCodes are not equal to the number of found tracks!";
      return;
    }

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

    // If the track could not be fitted because of Bette-bloch calculations
    // try to double the momentum
    Bool_t isMomentumLow(false);
    Int_t momCoef = 1;
    Int_t limitCoeff = 4;


    for(size_t i = 0; i <  foundTracks.size() ; ++i)
    {
      if(momCoef>=limitCoeff)
      {
        isMomentumLow = false;
        momCoef = 1;
      }

      if(isMomentumLow)
      {
        momCoef = momCoef * 2;
        --i;
        isMomentumLow = false;
        LOG(debug) << "Momentum too low, doubling momentum estimate";
      }
      std::vector<TVector3>& hitsOnTrack = foundTracks[i];

      // Set lower limit on track size
      if(hitsOnTrack.size()<fminHits)
      {
        LOG(debug) << "Track " << i << " below limit, continue with next track (" << hitsOnTrack.size() << " < " << fminHits << ")";
        continue;
      }
      
      int pdg = trackPdgs[i];

      TVector3 posM(hitsOnTrack[0].X(),hitsOnTrack[0].Y(),hitsOnTrack[0].Z());
      TVector3 momM(0,0,0);

      TVector3 momLoss(0,0,0);

      if(!CalculateInitialMomentum(hitsOnTrack, magField, momM, momLoss))
      {
        LOG(debug) << "Track " << i << " unable to extract momentum. Continue with next track";
        continue;
      }

      momM = momM * momCoef;

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

      LOG(debug) << "******************************************* ";
      LOG(debug) << "******    Track "<< i << "  ************************";
      LOG(debug) << "******************************************* ";
      LOG(debug) << " \tPdg code " << pdg;
      LOG(debug) << " \tHits in track "<< hitsOnTrack.size();
      LOG(debug) << " \tTrack Momentum [" << momM.Mag() << "]" << "(" << momM.X() << "," << momM.Y() << "," << momM.Z() << ")";
      LOG(debug) << " \tTrack Momentum Loss [" << momLoss.Mag() << "]" << "(" << momLoss.X() << "," << momLoss.Y() << "," << momLoss.Z() << ")";
      LOG(debug) << " \tMomentum / Momentum Loss [" << momM.Mag()/momLoss.Mag() << "]";
      
      for(Int_t bh = 0; bh < hitsOnTrack.size(); ++bh)
      {
        TVectorD hitPos(3);
        hitPos(0) = hitsOnTrack[bh].X();
        hitPos(1) = hitsOnTrack[bh].Y();
        hitPos(2) = hitsOnTrack[bh].Z();

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
        
        isMomentumLow = fiStatuStatus->isFitted() && !fiStatuStatus->isFitConverged();
        
      }
      catch(genfit::Exception& e)
      {
          LOG(error) <<"Exception, when tryng to fit track";
          LOG(error) << e.what();
          LOG(error) << e.getExcString();
          e.info();
          isMomentumLow = (e.getExcString().find("KalmanFitterInfo::getFittedState") != std::string::npos);
      }
    }
  
    if(isGenFitVisualization)
    {
      fdisplay->addEvent(genTracks);
    }

    // CalculateCurvature();
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
