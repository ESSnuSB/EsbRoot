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
   *@param time      time of entering of the particle inside the cube
   *@param photoE_direction1      generated photons on +x,+y,+z direction
   *@param distance_to_mppcLoc1      distance to mppc to +x,+y,+z direction
   *@param photoE_direction2      generated photons on -x,-y, z direction
   *@param distance_to_mppcLoc1      distance to mppc to -x,-y,-z direction
   *@param pdg      pdg codes of particles 
   *@param trackId      trackId
   **/
  FgdHit(Double_t x, Double_t y, Double_t z, TVector3 mppcLoc
        , TVector3 photoE, TVector3 dpos, Double_t time
        , TVector3 photoE_direction1, TVector3 distance_to_mppcLoc1
        , TVector3 photoE_direction2, TVector3 distance_to_mppcLoc2
        , Int_t pdg, Int_t trackId);
  
  /** Destructor **/
  virtual ~FgdHit();
  
  /** Accessors **/ 
  TVector3 GetMppcLoc(){return fmppcLoc;}
  TVector3 GetPhotoE(){return fphotoE;}
  TVector3 GetDpos(){return fdpos;}
  Double_t GetTime(){return ftime;}

  TVector3 GetPhotoDist1(){return fphotoE_dist1;}
  TVector3 GetMppcDist1(){return fmppcLoc_dist1;}
  TVector3 GetPhotoDist2(){return fphotoE_dist2;}
  TVector3 GetMppcDist2(){return fmppcLoc_dist2;}
  Int_t GetPgd(){return fpdg;}
  Int_t GetTrackId(){return ftrackId;}

  /** Modifiers **/
  void SetMppcLoc(TVector3 mppcLoc){fmppcLoc = mppcLoc;}
  void SetPhotoE(TVector3 p){fphotoE = p;}
  void SetDpos(TVector3 dpos){fdpos = dpos;}
  void SetTime(Double_t time){ftime=time;}

  void SetPhotoDist1(TVector3 photo1){fphotoE_dist1 = photo1;}
  void SetMppcDist1(TVector3 mppc1){fmppcLoc_dist1 = mppc1;}
  void SetPhotoDist2(TVector3 photo2){fphotoE_dist2 = photo2;}
  void SetMppcDist2(TVector3 mppc2){fmppcLoc_dist2 = mppc2;}
  void SetPgd(Int_t pdg){fpdg = pdg;}
  void SetTrackId(Int_t trackId){ftrackId = trackId;}
   
  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;
  
 private:
  /** Private variables to hold the position and number of generated photons in the cube **/
  TVector3 fmppcLoc;
  TVector3 fphotoE;
  TVector3 fdpos;
  Double_t ftime;

  TVector3 fphotoE_dist1;
  TVector3 fmppcLoc_dist1;
  TVector3 fphotoE_dist2;
  TVector3 fmppcLoc_dist2;

  Int_t fpdg;
  Int_t ftrackId;

  ClassDef(FgdHit, 2);
};
 
}
}
}

#endif // ESBROOT_ESBDATA_SUPERFGD_FGD_HIT_H