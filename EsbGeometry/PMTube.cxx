/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "EsbGeometry/PMTube.h"

#include <cmath>

#include <iostream>
using std::cout;
using std::endl;

namespace esbroot {
namespace geometry {

// -----   Default constructor   -------------------------------------------
PMTube::PMTube()
  : TObject(),
    fX(std::nan("NI")),
    fY(std::nan("NI")),
    fZ(std::nan("NI")),
    fRadius(std::nan("NI"))
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
PMTube::PMTube(Double_t x, Double_t y, Double_t z, Double_t r)
  : TObject(),
    fX(x),
    fY(y),
    fZ(z),
    fRadius(r)
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
PMTube::~PMTube() { }
// -------------------------------------------------------------------------
  
Int_t PMTube::Compare(const TObject *obj) const {
  
  PMTube* pmt_obj = (PMTube*)obj;
  if(pmt_obj == 0)
    return 0;

  if(fZ > pmt_obj->GetZ())
    return 1;
  else if(fZ < pmt_obj->GetZ())
    return -1;
  else
    return 0;
}

// -----   Public method Print   -------------------------------------------
void PMTube::Print(const Option_t* /*opt*/) const
{
  cout << "-I- PMTube: Radius: " << fRadius << " cm, Position (" << fX << ", " << fY << ", " << fZ
       << ") cm" << endl;
}
// -------------------------------------------------------------------------


}
}
