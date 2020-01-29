#ifndef ESBROOT_ESBDRECONSTRUCTION_FGD_MC_GENFIT_H
#define ESBROOT_ESBDRECONSTRUCTION_FGD_MC_GENFIT_H

// EsbRoot headers
#include "EsbData/EsbSuperFGD/FgdHit.h"
#include "EsbGeometry/EsbSuperFGD/EsbFgdDetectorParameters.h"
#include "EsbGeometry/EsbSuperFGD/EsbSuperFGDDetectorConstruction.h"
#include "EsbReconstruction/EsbSuperFGD/FgdReconHit.h"
#include "EsbReconstruction/EsbSuperFGD/PdgFromMomentumLoss.h"

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

class FgdMCGenFitRecon : public FairTask
{

 public:

  /** Default constructor **/  
  FgdMCGenFitRecon();

  /** Constructor with argument
   *@param name       Name of task
   *@param geoConfigFile  - Configuration file detector
   *@param mediaFile  - Configuration file for the used mediums
   *@param verbose  - Verbosity level
   *@param debugLlv - debug level for genfit
   *@param visualize -  to visualize the event using genfit::EventDisplay
   *@param visOption -  option to be passed to genfit::EventDisplay
  **/  
  FgdMCGenFitRecon(const char* name
              , const char* geoConfigFile
              , const char* mediaFile
              , Int_t verbose = 1
              , double debugLlv = 0
              , bool visualize = false
              , std::string visOption ="D");

  /** Destructor **/
  virtual ~FgdMCGenFitRecon();

  void SetMinInterations(Int_t minIterations) {fminGenFitInterations = minIterations;}
  void SetMaxInterations(Int_t maxIterations) {fmaxGenFitIterations = maxIterations;}
  void SetMinHits(Int_t minHits) {fminHits = minHits;}

  /** Virtual method Init **/
  virtual InitStatus Init() override;
  virtual void OutputFileInit(FairRootManager* manager); // Create virtual method for output file creation
  virtual void FinishEvent() override;
  virtual void FinishTask() override;


  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt) override;

  /** For decendents who want to write some data to output file */
  virtual void WriteOutput( Int_t pdg
                          , const TVector3& fitMom
                          , const TVector3& mcMom
                          , const genfit::Track& fitTrack
                          , genfit::FitStatus*& fiStatuStatus);

protected:

  /** Get all hits **/
  Bool_t GetHits(std::vector<ReconHit>& allHits);

  void SplitTrack(std::vector<ReconHit>& allHits, std::vector<std::vector<ReconHit>>& splitTracks);

  /** Fit the found tracks using genfit **/
  void FitTracks(std::vector<std::vector<ReconHit>>& foundTracks);

  /** Define materials used in the reconstruction phase **/
  void DefineMaterials();

  /** Print information for fitted grack **/
  void PrintFitTrack(genfit::Track& track);

  Long_t ArrInd(int i, int j, int k);

  Bool_t isParticleNeutral(Int_t pdg);

  /** Class to hold the Detector parameters read from external file **/
  esbroot::geometry::superfgd::FgdDetectorParameters fParams;//!<!

  /** Class containing the TGeometry for reconstruction of the tracks **/
  esbroot::geometry::superfgd::SuperFGDDetectorConstruction    fgdConstructor;//!<!	   //! SuperFgd Detector Constructor

  /** Detector geometry pointer**/
  TGeoVolume* fsuperFgdVol;//!<!

  /** Input array of FgdDetectorPoint(s)**/
  TClonesArray* fHitArray;     //! 

  /** Output array with genfit::Track(s) **/
  TClonesArray* fTracksArray;        //!

   /** Detector dimentions **/
  Double_t flunit;//!<!
  double f_step_X;//!<!
  double f_step_Y;//!<!
  double f_step_Z;//!<!
  int f_bin_X;//!<!
  int f_bin_Y;//!<!
  int f_bin_Z;//!<!
  double f_total_X;//!<!
  double f_total_Y;//!<!
  double f_total_Z;//!<!

  /** Start position and momentum **/
  //  0 - no additional info
  //  1 - debug info
  //  2- detail info
  double fDebuglvl_genfit;//!<!

  /** Path to the used media.geo file - containing definitions of materials **/
  std::string fmediaFile;//!<!

  /** Path to the events file containing Monte carlo simulation data - pdg codes, momentums etc. **/
  std::string feventFile;//!<!

  Int_t fminGenFitInterations;
  Int_t fmaxGenFitIterations;
  Int_t fminHits;

  /** Are materials already defined **/
  bool isDefinedMaterials;

  /** local Members for genfit visualization  **/
  genfit::EventDisplay* fdisplay;//!<!
  bool isGenFitVisualization;//!<!
  std::string fGenFitVisOption;//!<!
  std::vector<genfit::Track*> fgenTracks;//!<!

  	   
  ClassDef(FgdMCGenFitRecon, 2);

};

} //superfgd
} //reconstruction
} //esbroot

#endif // ESBROOT_ESBDRECONSTRUCTION_FGD_MC_GENFIT_H
