/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "EsbWCdetectorPoint.h"

#include <iostream>
using std::cout;
using std::endl;


// -----   Default constructor   -------------------------------------------
EsbWCdetectorPoint::EsbWCdetectorPoint()
  : FairMCPoint()
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
EsbWCdetectorPoint::EsbWCdetectorPoint(Int_t trackID, Int_t detID,
                                   TVector3 pos, TVector3 mom,
                                   Double_t tof)
  : FairMCPoint(trackID, detID, pos, mom, tof, 0, 0)
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
EsbWCdetectorPoint::~EsbWCdetectorPoint() { }
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void EsbWCdetectorPoint::Print(const Option_t* /*opt*/) const
{
  cout << "-I- EsbWCdetectorPoint: EsbWCdetector point for track " << fTrackID
       << " in detector " << fDetectorID << endl;
  cout << "    Position (" << fX << ", " << fY << ", " << fZ
       << ") cm" << endl;
  cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz
       << ") GeV" << endl;
  cout << "    Time " << fTime << " ns" << endl;
}
// -------------------------------------------------------------------------

ClassImp(EsbWCdetectorPoint)

