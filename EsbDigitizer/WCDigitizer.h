/// \file WCDigitizer.h
/// \brief Definition of PMT Digitizer class
/// \author Peter Christiansen
#ifndef ESBROOT_ESBDIGITIZER_WCDIGITIZER_H
#define ESBROOT_ESBDIGITIZER_WCDIGITIZER_H


#include <FairTask.h>
#include <TClonesArray.h>
#include <TSpline.h>

namespace esbroot {
namespace digitizer {

/// \class PMTubeHit
/// This is the definition of the PMT Digit object. For not it contains the
/// center of the PMT, the radius, the energy signal and the time. There are
/// thus many of the parameters in the class FairHit that are not defined. In
/// the future one could even imagine to have the time structure of the
/// signal.
class WCDigitizer : public FairTask
{

 public:

  /** Default constructor **/  
  WCDigitizer();

  /** Constructor with argument
   *@param name        Name of task
   *@param pmt_array  Array where the location and size of PMTs are defined
   *@param iVerbose    Verbosity level
  **/  
  WCDigitizer(const char* name, TClonesArray* pmt_array, Int_t i_verbose = 1);

  /** Destructor **/
  ~WCDigitizer();


  /// Setter for Quantum Efficiency spline
  void SetQEffSpline(TSpline* spline) { fQEffSpline = spline; }

  /// Getter for Quantum Efficiency spline
  TSpline* GetQEffSpline() { return fQEffSpline; }

  /** Virtual method Init **/
  virtual InitStatus Init();


  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);


 private:
  /** Input array of EsbWCdetectorPoint(s)**/
  TClonesArray* fPhotonArray;     //! 

  /** PMT array of EsbPMT(s) **/
  TClonesArray* fPMTubeArray;        //!

  /** Output array with EsbPMThit(s) **/
  TClonesArray* fHitArray;        //!

  TSpline* fQEffSpline;           //! Quantum Efficiency Spline
  /*
    Is this needed?
    WCDigitizer(const WCDigitizer&);
    WCDigitizer operator=(const WCDigitizer&);
  */

  ClassDef(WCDigitizer, 2);

};

}
}

#endif // ESBROOT_ESBDIGITIZER_WCDIGITIZER_H
