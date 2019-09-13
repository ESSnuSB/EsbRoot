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
FgdHit::FgdHit(Double_t x, Double_t y, Double_t z, TVector3 mppcLoc, TVector3 photoE,TVector3 dpos)
  : FairHit(),fmppcLoc(mppcLoc),fphotoE(photoE), fdpos(dpos)
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
