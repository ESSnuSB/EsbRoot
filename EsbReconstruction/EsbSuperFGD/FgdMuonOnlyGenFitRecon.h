#ifndef ESBROOT_ESBDRECONSTRUCTION_FGD_ONLY_MUON_GENFIT_H
#define ESBROOT_ESBDRECONSTRUCTION_FGD_ONLY_MUON_GENFIT_H

#include <FairTask.h>
#include "EsbGeometry/EsbSuperFGD/EsbFgdDetectorParameters.h"
#include "EsbGeometry/EsbSuperFGD/EsbSuperFGDDetectorConstruction.h"
#include "EsbData/EsbSuperFGD/FgdHit.h"

#include <include/EventDisplay.h>

namespace esbroot {
namespace reconstruction {
namespace superfgd {


class FgdMuonOnlyGenFitRecon : public FairTask
{

 public:

  /** Default constructor **/  
  FgdMuonOnlyGenFitRecon();

  /** Constructor with argument
   *@param name       Name of task
   *@param geoConfigFile - Configuration file detector
   *@param mediaFile - Configuration file for the used mediums
   *@param verbose - Verbosity level
   *@param debugLlv - debug level for genfit
   *@param outFile - output comparison
   *@param inFile - in file with initial muon momentum and position
   *@param visualize - to visualize the event using genfit::EventDisplay
   *@param visOption - option to be passed to genfit::EventDisplay
  **/  
  FgdMuonOnlyGenFitRecon(const char* name
              , const char* geoConfigFile
              , const char* mediaFile
              , Int_t verbose = 1
              , double debugLlv = 0
              , const char* outFile = ""
              , const char* inFile = ""
              , bool visualize = false
              , std::string visOption ="D");

  /** Destructor **/
  ~FgdMuonOnlyGenFitRecon();


  /** Virtual method Init **/
  virtual InitStatus Init() override;
  virtual void FinishEvent() override;
  virtual void FinishTask() override;


  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt) override;

private:

  /** Define materials used in the reconstruction phase **/
  void DefineMaterials();

  /** Define materials used in the reconstruction phase **/
  void GetInitialMomPos();

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
  double fDebuglvl_genfit;//!<!

  /** Path to the used media.geo file - containing definitions of materials **/
  std::string fmediaFile;//!<!

  /** Path to file where to write Monte Carlo and genfit momentum for comparisons **/
  std::string fOutputFile;//!<!

  /** Path to file with initial momentum and positions of muons **/
  std::string finputFile;//!<!
  std::vector<TVector3> fInitialMomentum;//!<!
  std::vector<TVector3> fInitialPosition;//!<!
  Int_t fMuonInd;//!<!

  /** Are materials already defined **/
  bool isDefinedMaterials;

  /** local Members for genfit visualization  **/
  genfit::EventDisplay* fdisplay;//!<!
  bool isGenFitVisualization;//!<!
  std::string fGenFitVisOption;//!<!
  	   
  ClassDef(FgdMuonOnlyGenFitRecon, 2);

};

} //superfgd
} //reconstruction
} //esbroot

#endif // ESBROOT_ESBDIGITIZER_FGD_MPPC_DISPLAY_H
