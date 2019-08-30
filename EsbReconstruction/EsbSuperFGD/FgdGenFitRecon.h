#ifndef ESBROOT_ESBDRECONSTRUCTION_FGD_GENFIT_H
#define ESBROOT_ESBDRECONSTRUCTION_FGD_GENFIT_H

#include <FairTask.h>
#include "EsbGeometry/EsbSuperFGD/EsbFgdDetectorParameters.h"
#include "EsbGeometry/EsbSuperFGD/EsbSuperFGDDetectorConstruction.h"
#include "EsbData/EsbSuperFGD/FgdHit.h"

#include <include/EventDisplay.h>

namespace esbroot {
namespace reconstruction {
namespace superfgd {


class FgdGenFitRecon : public FairTask
{

 public:

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


  /** Virtual method Init **/
  virtual InitStatus Init() override;
  virtual void FinishEvent() override;
  virtual void FinishTask() override;


  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt) override;

private:

  /** Define materials used in the reconstruction phase **/
  void DefineMaterials();

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

  /** Path to the used media.geo file - containing definitions of materials **/
  std::string fmediaFile;

  /** Start position and momentum **/
  TVector3 fstartPos;
  TVector3 fstartMom;

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
