/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/


#ifndef ESBROOT_ESBDATA_SUPERFGD_FGD_HIT_H
#define ESBROOT_ESBDATA_SUPERFGD_FGD_HIT_H

#include <FairHit.h>

namespace esbroot {
namespace data {
namespace superfgd {

/// \class FgdHit
/// This is the definition of the Fgd digit class.
/// The data is transformed from energy deposited inside a cube into
/// number of photons generated
class FgdHit : public FairHit
{
 public:
  
  /** Default constructor **/
  FgdHit();
  
  /** Constructor with arguments
   *@param x        X coordinates of cube in absolute coordinates
   *@param y        Y coordinates of cube in absolute coordinates
   *@param z        Z coordinates of cube in absolute coordinates
   *@param mppcLoc   bin locations for the mppcs
   *@param photoE    number of generated photons along each direction
   *@param dpos      coordinate of the hit relative to the detector
   **/
  FgdHit(Double_t x, Double_t y, Double_t z, TVector3 mppcLoc, TVector3 photoE, TVector3 dpos);
  
  /** Destructor **/
  virtual ~FgdHit();
  
  /** Accessors **/ 
  TVector3 GetMppcLoc(){return fmppcLoc;}
  TVector3 GetPhotoE(){return fphotoE;}
  TVector3 GetDpos(){return fdpos;}

  /** Modifiers **/
  void SetMppcLoc(TVector3 mppcLoc){fmppcLoc = mppcLoc;}
  void SetPhotoE(TVector3 p){fphotoE = p;}
  void SetDpos(TVector3 dpos){fdpos = dpos;}
   
  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;
  
 private:
  /** Private variables to hold the position and number of generated photons in the cube **/
  TVector3 fmppcLoc;
  TVector3 fphotoE;
  TVector3 fdpos;

  ClassDef(FgdHit, 2);
};
 
}
}
}

#endif // ESBROOT_ESBDATA_SUPERFGD_FGD_HIT_H
