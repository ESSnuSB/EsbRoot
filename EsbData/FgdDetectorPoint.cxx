/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "EsbData/FgdDetectorPoint.h"

#include <iostream>
using std::cout;
using std::endl;

namespace esbroot {

namespace data {

// -----   Default constructor   -------------------------------------------
FgdDetectorPoint::FgdDetectorPoint()
  : FairMCPoint()
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
FgdDetectorPoint::FgdDetectorPoint(Int_t trackID, Int_t detID,
                                   TVector3 pos, TVector3 mom,
                                   Double_t tof)
  : FairMCPoint(trackID, detID, pos, mom, tof, 0, 0)
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
FgdDetectorPoint::~FgdDetectorPoint() { }
// -------------------------------------------------------------------------

Int_t FgdDetectorPoint::Compare(const TObject *obj) const {
  
  FgdDetectorPoint* photonObj = (FgdDetectorPoint*)obj;
  if(photonObj == 0)
    return 0;

  if(fZ > photonObj->GetZ())
    return 1;
  else if(fZ < photonObj->GetZ())
    return -1;
  else
    return 0;
}

// -----   Public method Print   -------------------------------------------
void FgdDetectorPoint::Print(const Option_t* /*opt*/) const
{
  cout << "-I- FgdDetectorPoint: FgdDetector point for track " << fTrackID
       << " in detector " << fDetectorID << endl;
  cout << "    Position (" << fX << ", " << fY << ", " << fZ
       << ") cm" << endl;
  cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz
       << ") GeV" << endl;
  cout << "    Time " << fTime << " ns" << endl;
}
// -------------------------------------------------------------------------

}//namespace data

}//namespace esbroot
