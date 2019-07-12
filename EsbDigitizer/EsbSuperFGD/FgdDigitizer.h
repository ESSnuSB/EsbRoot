#ifndef ESBROOT_ESBDIGITIZER_FGD_DIGITIZER_H
#define ESBROOT_ESBDIGITIZER_FGD_DIGITIZER_H

#include <FairTask.h>
#include <TClonesArray.h>
#include "TRandom3.h"

#include "EsbGeometry/EsbSuperFGD/EsbFgdDetectorParameters.h"

namespace esbroot {
namespace digitizer {
namespace superfgd {


class FgdDigitizer : public FairTask
{

 public:

  /** Default constructor **/  
  FgdDigitizer();

  /** Constructor with argument
   *@param name       Name of task
   *@param geoConfigFile  Configuration file detector
   *@param inputRootFile  File containing the simulation results
   *@param x - X coordinate of the detector
   *@param y - Y coordinate of the detector
   *@param z - Z coordinate of the detector
   *@param verbose   Verbosity level
  **/  
  FgdDigitizer(const char* name
              ,const char* geoConfigFile
              ,const char* inputRootFile
              ,double x, double y, double z
              ,Int_t verbose = 1);

  /** Destructor **/
  ~FgdDigitizer();


  /** Virtual method Init **/
  virtual InitStatus Init();


  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt) override;


private:

  /** Constants used when digitalizing the energy loss in the detector to photons **/
  static const double CBIRKS;
  static const double EdepToPhotConv_FGD;
  static const double MPPCEff_SuperFGD;
  static const double EdepToPhotConv_SuperFGD;
  static const double DistMPPCscint_FGD;
  static const double LongCompFrac_FGD;
  static const double LongAtt_FGD;
  static const double ShortAtt_FGD;
  static const double DecayLength_FGD;
  static const double Lbar_FGD;
  static const double TransTimeInFiber;

  /** Class to hold the Detector parameters read from external file **/
  esbroot::geometry::superfgd::FgdDetectorParameters fParams;

  /** Coordinates of the detector **/
  double fX;
  double fY;
  double fZ;

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

  TRandom3 rand; // random varible generator


  /** Methods to calculate the response from detector physical characteristics **/
  double ApplyScintiResponse(double edep, double trackLength, double charge);
  void ApplyFiberResponse(double &numPhotons, double &time, double distance);
  double ApplyFiberAttenuation(double Nphot0,double x);
  double ApplyFiberTime(double &time, double x);
  void ApplyMPPCResponse(double &npe);

  /** Input array of FgdDetectorPoint(s)**/
  TClonesArray* fdPoints;     //! 
  	   
  ClassDef(FgdDigitizer, 2);

};

} //superfgd
} //digitizer
} //esbroot

#endif // ESBROOT_ESBDIGITIZER_FGD_DIGITIZER_H
