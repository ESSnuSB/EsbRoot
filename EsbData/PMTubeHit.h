/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

/// \file PMTubeHit.h
/// \brief Definition PMT Digit
/// \author Peter Christiansen
#ifndef ESBROOT_ESBDATA_PMTUBEHIT_H
#define ESBROOT_ESBDATA_PMTUBEHIT_H

#include <FairHit.h>

namespace esbroot {
namespace data {

/// \class PMTubeHit
/// This is the definition of the PMT Digit object. For not it contains the
/// center of the PMT, the radius, the energy signal and the time. There are
/// thus many of the parameters in the class FairHit that are not defined. In
/// the future one could even imagine to have the time structure of the
/// signal.
class PMTubeHit : public FairHit
{
 public:
  
  /** Default constructor **/
  PMTubeHit();
  
  /** Constructor with arguments
   *@param x       center of PMT hit
   *@param y       center of PMT hit
   *@param z       center of PMT hit
   *@param r       radius of PMT
   *@param charge  charge of PMT signal (e.g. n photons)
   *@param time    time of PMT signal
   **/
  PMTubeHit(Double_t x, Double_t y, Double_t z, Double_t r, 
	    Double_t charge, Double_t time);
  
  /** Destructor **/
  virtual ~PMTubeHit();
  
  /** Accessors **/
  Double_t GetRadius() const { return fRadius; }
  Double_t GetCharge() const { return fCharge; }
  Double_t GetTime()   const { return fTime; }  

  /** Modifiers **/
  void SetRadius(Double_t radius) { fRadius = radius; }
  void SetCharge(Double_t charge) { fCharge = charge; }
  void SetTime(Double_t time) { fTime = time; }
   
  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;
  
 protected:
  
  Double_t fRadius; ///< Radius of PMT [cm]
  Double_t fCharge; ///< Charge of PMT signal [a.u.]
  Double_t fTime;   ///< Time of PMT signal [ns]

  ClassDef(PMTubeHit, 2);
};
 
}
}

#endif // ESBROOT_ESBDATA_PMTUBEHIT_H
