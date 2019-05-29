/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "EsbGeometry/FgdDetector.h"
#include "EsbData/FgdDetectorPoint.h" 


#include "FairVolume.h"
#include "FairRootManager.h"
#include "FairGenericStack.h"
#include "FairStack.h"
#include "FairGeoLoader.h"
#include "FairGeoInterface.h"
#include "FairGeoBuilder.h"
#include "FairGeoMedia.h"

#include "TClonesArray.h"
#include "TVirtualMC.h"
#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGraph.h"

#include "Geant4GM/volumes/Factory.h"
#include "RootGM/volumes/Factory.h"
#include "TGeoManager.h"

#include <iostream>
using std::cout;
using std::endl;

namespace esbroot {

namespace geometry {
// PC: work around
static const Int_t kFgdDetector = 2;

//___________________________________________________________________
FgdDetector::FgdDetector(std::string geoConfigFile)
  : FairDetector("FgdDetector", kTRUE, kFgdDetector),
    fgdConstructor(geoConfigFile),
    fTrackID(-1),
    fVolumeID(-1),
    fPos(),
    fMom(),
    fTime(-1.),
    fLength(-1.),
    fELoss(-1),
    fFgdDetectorPointCollection(new TClonesArray(data::FgdDetectorPoint::Class()))
{
}

//___________________________________________________________________
  FgdDetector::FgdDetector(const char* name, std::string geoConfigFile, Bool_t active)
  : FairDetector(name, active, kFgdDetector),
    fgdConstructor(geoConfigFile),
    fTrackID(-1),
    fVolumeID(-1),
    fPos(),
    fMom(),
    fTime(-1.),
    fLength(-1.),
    fELoss(-1),
    fFgdDetectorPointCollection(new TClonesArray(data::FgdDetectorPoint::Class())) 
{
}

//___________________________________________________________________
FgdDetector::~FgdDetector()
{
  if (fFgdDetectorPointCollection) {
    fFgdDetectorPointCollection->Delete();
    delete fFgdDetectorPointCollection;
  }
}

//___________________________________________________________________
void FgdDetector::Initialize()
{
  FairDetector::Initialize();
}

//___________________________________________________________________
Bool_t  FgdDetector::ProcessHits(FairVolume* vol)
{
	cout << __PRETTY_FUNCTION__ << endl;
  /** This method is called from the MC stepping */

  if ( TVirtualMC::GetMC()->IsTrackEntering() ) {
    fELoss  = 0.;
    fTime   = TVirtualMC::GetMC()->TrackTime() * 1.0e09;
    fLength = TVirtualMC::GetMC()->TrackLength();
    TVirtualMC::GetMC()->TrackPosition(fPos);
    TVirtualMC::GetMC()->TrackMomentum(fMom);
  }

  // Sum energy loss for all steps in the active volume
  fELoss += TVirtualMC::GetMC()->Edep();

  // Create FairTutorialDet1Point at exit of active volume
  if ( TVirtualMC::GetMC()->IsTrackExiting()    ||
       TVirtualMC::GetMC()->IsTrackStop()       ||
       TVirtualMC::GetMC()->IsTrackDisappeared()   ) {

    fTrackID  = TVirtualMC::GetMC()->GetStack()->GetCurrentTrackNumber();
    fVolumeID = vol->getMCid();
    //~ if (fELoss == 0. ) { return kFALSE; }
    AddHit(fTrackID, fVolumeID, TVector3(fPos.X(),  fPos.Y(),  fPos.Z()),
           TVector3(fMom.Px(), fMom.Py(), fMom.Pz()), fTime);
  }

  return kTRUE;
}

void FgdDetector::EndOfEvent()
{
  fFgdDetectorPointCollection->Clear();
}



void FgdDetector::Register()
{

  /** This will create a branch in the output tree called
      EsbFgdDetectorPoint, setting the last parameter to kFALSE means:
      this collection will not be written to the file, it will exist
      only during the simulation.
  */

  FairRootManager::Instance()->Register("EsbFgdDetectorPoint", "FgdDetector", 
                                        fFgdDetectorPointCollection, kTRUE);

}


TClonesArray* FgdDetector::GetCollection(Int_t iColl) const
{
  if (iColl == 0) { return fFgdDetectorPointCollection; }
  else { return NULL; }
}

void FgdDetector::Reset()
{
  fFgdDetectorPointCollection->Clear();
}

void FgdDetector::ConstructGeometry()
{
	//Create the real Fgd geometry
	
  G4VPhysicalVolume* superFgdG4Vol = fgdConstructor.Construct();

  // Import Geant4 geometry to VGM
  Geant4GM::Factory g4Factory;
  g4Factory.Import(superFgdG4Vol);

  // Export VGM geometry to Root
  RootGM::Factory rtFactory;
  g4Factory.Export(&rtFactory);
  //gGeoManager->CloseGeometry();
}

//___________________________________________________________________
data::FgdDetectorPoint* FgdDetector::AddHit(Int_t trackID, Int_t detID, 
					  TVector3 pos, TVector3 mom,
					  Double_t time)
{
  TClonesArray& clref = *fFgdDetectorPointCollection;
  Int_t size = clref.GetEntriesFast();

  return new(clref[size]) data::FgdDetectorPoint(trackID, detID, pos, mom, 
					     time);
}

}
}

