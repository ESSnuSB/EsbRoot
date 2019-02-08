/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "EsbData/PMTubeHit.h"

#include <cmath>

#include <iostream>
using std::cout;
using std::endl;

namespace esbroot {
namespace data {

// -----   Default constructor   -------------------------------------------
PMTubeHit::PMTubeHit()
  : FairHit(),
    fRadius(std::nan("NI")),
    fCharge(std::nan("NI")),
    fTime(std::nan("NI"))
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
PMTubeHit::PMTubeHit(Double_t x, Double_t y, Double_t z, Double_t r, 
		     Double_t charge, Double_t time)
  
  : FairHit(),
    fRadius(r),
    fCharge(charge),
    fTime(time)    
{
  SetX(x);
  SetY(y);
  SetZ(z);
  SetDx(r);
  SetDy(r);
  SetDz(r);
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
PMTubeHit::~PMTubeHit() { }
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void PMTubeHit::Print(const Option_t* /*opt*/) const
{
  cout << "-I- PMTubeHit: Radius: " << fRadius << " cm, Position (" << fX << ", " << fY << ", " << fZ
       << ") cm" << endl;
  cout << " Charge: " << fCharge << " [a.u.], Time: " << fTime << " ns" << endl;
}
// -------------------------------------------------------------------------

}
}
