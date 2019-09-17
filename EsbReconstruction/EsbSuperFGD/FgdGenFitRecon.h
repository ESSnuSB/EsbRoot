#ifndef ESBROOT_ESBDRECONSTRUCTION_FGD_GENFIT_H
#define ESBROOT_ESBDRECONSTRUCTION_FGD_GENFIT_H

// EsbRoot headers
#include "EsbData/EsbSuperFGD/FgdHit.h"
#include "EsbGeometry/EsbSuperFGD/EsbFgdDetectorParameters.h"
#include "EsbGeometry/EsbSuperFGD/EsbSuperFGDDetectorConstruction.h"

// FairRoot headers
#include <FairTask.h>

// ROOT headers
#include <include/EventDisplay.h>

// Pathfinder headers
#include "basicHit.h"
#include "TrackFinderTrack.h"

namespace esbroot {
namespace reconstruction {
namespace superfgd {

class FgdGenFitRecon : public FairTask
{

 public:

  enum TrackFinder{
    HOUGH_PATHFINDER,
    TIME_OF_HITS
  };

  /** Default constructor **/  
  FgdGenFitRecon();

  /** Constructor with argument
   *@param name       Name of task
   *@param geoConfigFile  - Configuration file detector
   *@param mediaFile  - Configuration file for the used mediums
   *@param startPos - start position
   *@param startMOm - start momentum
   *@param verbose  - Verbosity level
   *@param debugLlv - debug level for genfit
   *@param visualize -  to visualize the event using genfit::EventDisplay
   *@param visOption -  option to be passed to genfit::EventDisplay
  **/  
  FgdGenFitRecon(const char* name
              , const char* geoConfigFile
              , const char* mediaFile
              , TVector3 startPos
              , TVector3 startMOm
              , Int_t verbose = 1
              , double debugLlv = 0
              , bool visualize = false
              , std::string visOption ="D");

  /** Destructor **/
  ~FgdGenFitRecon();

  void SetMinInterations(Int_t minIterations) {fminGenFitInterations = minIterations;}
  void SetMaxInterations(Int_t maxIterations) {fmaxGenFitIterations = maxIterations;}
  void SetMinHits(Int_t minHits) {fminHits = minHits;}
  void SetUseTracker(FgdGenFitRecon::TrackFinder finder){ffinder=finder;}

  /** Virtual method Init **/
  virtual InitStatus Init() override;
  virtual void FinishEvent() override;
  virtual void FinishTask() override;


  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt) override;

private:

  enum FindTrackType{
    STRAIGHT_LINE,
    HELIX,
    CURL
  };

  class ReconHit{

  public:
    ReconHit(TVector3 mppcLoc
            , TVector3 hitPosition
            , TVector3 photons
            , Double_t time
            , Int_t pdg
            , Int_t trackId
            , TVector3 ph1
            , TVector3 mppc1
            , TVector3 ph2
            , TVector3 mppc2)
      : fmppcLoc(mppcLoc)
        , fHitPos(hitPosition)
        , fphotons(photons)
        , ftime(time)
        , fpdg(pdg)
        , ftrackId(trackId)
        , fph1(ph1)
        , fmppc1(mppc1)
        , fph2(ph2)
        , fmppc2(mppc2)
    {};

    ReconHit(const ReconHit& c)
      : fmppcLoc(c.fmppcLoc)
        , fHitPos(c.fHitPos)
        , fphotons(c.fphotons)
        , fph1(c.fph1)
        , fmppc1(c.fmppc1)
        , fph2(c.fph2)
        , fmppc2(c.fmppc2)
        , ftime(c.ftime)
        , fpdg(c.fpdg)
        , ftrackId(c.ftrackId)
    {};

    ReconHit& operator=(const ReconHit& c)
    {
      fmppcLoc = c.fmppcLoc;
      fHitPos = c.fHitPos;
      fphotons = c.fphotons;
      fph1 = c.fph1;
      fmppc1 = c.fmppc1;
      fph2 = c.fph2;
      fmppc2 = c.fmppc2;
      ftime = c.ftime;
      fpdg = c.fpdg;
      ftrackId = c.ftrackId;
      return *this;
    }

    ~ReconHit(){}

    TVector3 fmppcLoc;
    TVector3 fHitPos;
    TVector3 fphotons;
    TVector3 fph1;
    TVector3 fmppc1;
    TVector3 fph2;
    TVector3 fmppc2;
    Double_t ftime;
    Int_t fpdg;
    Int_t ftrackId;
  };

  /** Get all hits **/
  bool GetHits(std::vector<ReconHit>& allHits);

  /** Extrack tracks from the hit using Hough Transform **/
  bool FindTracks(std::vector<ReconHit>& hits
                  , std::vector<pathfinder::TrackFinderTrack>& foundTracks
                  , FindTrackType trackType);

  /** Extrack tracks from the hit using time of occurrence**/
  bool FindTracksByTime(std::vector<ReconHit>& hits
                  , std::vector<pathfinder::TrackFinderTrack>& foundTracks);

  /** Fit the found tracks using genfit **/
  void FitTracks(std::vector<pathfinder::TrackFinderTrack>& foundTracks);

  /** Define materials used in the reconstruction phase **/
  void DefineMaterials();

  /** Print information for fitted grack **/
  void PrintFitTrack(genfit::Track& track);

  /** Check if the passed hit does not contain nearby hits - if it does not, it is noise **/
  bool IsNoiseHit(pathfinder::basicHit& hit, int range, bool *visited);

  /** Class to hold the Detector parameters read from external file **/
  esbroot::geometry::superfgd::FgdDetectorParameters fParams;

  /** Class containing the TGeometry for reconstruction of the tracks **/
  esbroot::geometry::superfgd::SuperFGDDetectorConstruction    fgdConstructor;	   //! SuperFgd Detector Constructor

  /** Detector geometry pointer**/
  TGeoVolume* fsuperFgdVol;

  /** Input array of FgdDetectorPoint(s)**/
  TClonesArray* fHitArray;     //! 

  /** Output array with genfit::Track(s) **/
  TClonesArray* fTracksArray;        //!

   /** Detector dimentions **/
  Double_t flunit;
  double f_step_X;
  double f_step_Y;
  double f_step_Z;
  int f_bin_X;
  int f_bin_Y;
  int f_bin_Z;
  double f_total_X;
  double f_total_Y;
  double f_total_Z;

  /** Start position and momentum **/
  //  0 - no additional info
  //  1 - debug info
  //  2- detail info
  double fDebuglvl_genfit;

  FgdGenFitRecon::TrackFinder ffinder;

  /** Path to the used media.geo file - containing definitions of materials **/
  std::string fmediaFile;

  /** Start position and momentum **/
  TVector3 fstartPos;
  TVector3 fstartMom;

  Int_t fminGenFitInterations;
  Int_t fmaxGenFitIterations;
  Int_t fminHits;

  /** Are materials already defined **/
  bool isDefinedMaterials;

  /** local Members for genfit visualization  **/
  genfit::EventDisplay* fdisplay;//!<!
  bool isGenFitVisualization;//!<!
  std::string fGenFitVisOption;//!<!
  	   
  ClassDef(FgdGenFitRecon, 2);

};

} //superfgd
} //reconstruction
} //esbroot

#endif // ESBROOT_ESBDIGITIZER_FGD_MPPC_DISPLAY_H
