/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "EsbData/EsbSuperFGD/FgdHit.h"

#include <cmath>

#include <iostream>
using std::cout;
using std::endl;

namespace esbroot {
namespace data {
namespace superfgd {

// -----   Default constructor   -------------------------------------------
FgdHit::FgdHit()
  : FairHit()
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
FgdHit::FgdHit(Double_t x, Double_t y, Double_t z, TVector3 mppcLoc
        , TVector3 photoE, TVector3 dpos, Double_t time, TVector3 mom, TVector3 momExit
        , Double_t trackLength, Double_t trackLengthOrigin , TVector3 photoE_direction1, TVector3 distance_to_mppcLoc1
        , TVector3 photoE_direction2, TVector3 distance_to_mppcLoc2
        , Int_t pdg, Int_t trackId, Double_t edep)
  : FairHit(),fmppcLoc(mppcLoc),fphotoE(photoE), fdpos(dpos), ftime(time), fmom(mom)
  , fphotoE_dist1(photoE_direction1), fmppcLoc_dist1(distance_to_mppcLoc1)
  , fphotoE_dist2(photoE_direction2), fmppcLoc_dist2(distance_to_mppcLoc2)
  , fpdg(pdg), ftrackId(trackId), fmomExit(momExit), ftrackLength(trackLength)
  , fedep(edep), ftrackLengthOrigin(trackLengthOrigin)
{
  SetX(x);
  SetY(y);
  SetZ(z);
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
FgdHit::~FgdHit() { }
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void FgdHit::Print(const Option_t* /*opt*/) const
{
  cout << "-I- FgdHit: "  << " [cm] Position (" << fX << ", " << fY << ", " << fZ
       << ") cm" << endl;
  cout << "Mppc: " << "x: " << fmppcLoc.X() << " y: " << fmppcLoc.Y() << " z: " << fmppcLoc.Z() << endl;
  cout << "Total photons in each direction: " << " x: " << fphotoE.X() << " y: " << fphotoE.Y() 
  << " z: " << fphotoE.Z() << endl;
}
// -------------------------------------------------------------------------

}
}
}
