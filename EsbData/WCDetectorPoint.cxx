/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "EsbData/WCDetectorPoint.h"

#include <iostream>
using std::cout;
using std::endl;

namespace esbroot {

namespace data {

// -----   Default constructor   -------------------------------------------
WCDetectorPoint::WCDetectorPoint()
  : FairMCPoint()
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
WCDetectorPoint::WCDetectorPoint(Int_t trackID, Int_t detID,
                                   TVector3 pos, TVector3 mom,
                                   Double_t tof)
  : FairMCPoint(trackID, detID, pos, mom, tof, 0, 0)
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
WCDetectorPoint::~WCDetectorPoint() { }
// -------------------------------------------------------------------------

Int_t WCDetectorPoint::Compare(const TObject *obj) const {
  
  cout << "In compare!" << endl;

  WCDetectorPoint* photonObj = (WCDetectorPoint*)obj;
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
void WCDetectorPoint::Print(const Option_t* /*opt*/) const
{
  cout << "-I- WCDetectorPoint: WCDetector point for track " << fTrackID
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