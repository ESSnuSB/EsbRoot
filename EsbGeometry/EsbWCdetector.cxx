/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "EsbWCdetector.h"

#include "EsbWCdetectorPoint.h"
//#include "EsbWCdetectorGeo.h"
//#include "EsbWCdetectorGeoPar.h"


#include "FairVolume.h"
#include "FairRootManager.h"
//#include "FairRuntimeDb.h"
//#include "EnuDetectorList.h"
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

#include <iostream>
using std::cout;
using std::endl;

// PC: work around
static const Int_t kEsbWCdetector = 1;

//___________________________________________________________________
EsbWCdetector::EsbWCdetector()
  : FairDetector("EsbWCdetector", kTRUE, kEsbWCdetector),
    fTrackID(-1),
    fVolumeID(-1),
    fPos(),
    fMom(),
    fTime(-1.),
    fLength(-1.),
    fELoss(-1),
    fEsbWCdetectorPointCollection(new TClonesArray("EsbWCdetectorPoint"))
{
}

//___________________________________________________________________
EsbWCdetector::EsbWCdetector(const char* name, Bool_t active)
  : FairDetector(name, active, kEsbWCdetector),
    fTrackID(-1),
    fVolumeID(-1),
    fPos(),
    fMom(),
    fTime(-1.),
    fLength(-1.),
    fELoss(-1),
    fEsbWCdetectorPointCollection(new TClonesArray("EsbWCdetectorPoint"))
{
}

//___________________________________________________________________
EsbWCdetector::~EsbWCdetector()
{
  if (fEsbWCdetectorPointCollection) {
    fEsbWCdetectorPointCollection->Delete();
    delete fEsbWCdetectorPointCollection;
  }
}

//___________________________________________________________________
void EsbWCdetector::Initialize()
{
  FairDetector::Initialize();
  //  FairRuntimeDb* rtdb= FairRun::Instance()->GetRuntimeDb();
  //  PC: This is where we will load the detector parameters 
  //  EsbWCdetectorGeoPar* par=(EsbWCdetectorGeoPar*)(rtdb->getContainer("EsbWCdetectorGeoPar"));
}

//___________________________________________________________________
Bool_t  EsbWCdetector::ProcessHits(FairVolume* vol)
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

    // Increment number of tutorial det points in TParticle
    //~ FairStack* stack = static_cast<FairStack*>(TVirtualMC::GetMC()->GetStack());
    //~ stack->AddPoint(kEsbWCdetector);
  }

  return kTRUE;

  //~ // We could define the emission point for the photons. In that way we would
  //~ // have the "track" we want to reconstruct.

  //~ // Create EsbWCdetectorPoint at exit of active volume
  //~ if ( gMC->IsTrackExiting() ) {

    //~ fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
    //~ fVolumeID = vol->getMCid();
    //~ AddHit(fTrackID, fVolumeID, TVector3(fPos.X(),  fPos.Y(),  fPos.Z()),
           //~ TVector3(fMom.Px(), fMom.Py(), fMom.Pz()), fTime);

    //~ // PC: work around
    //~ // // Increment number of EsbWCdetector det points in TParticle
    //~ // FairGenericStack* stack = (FairGenericStack*) gMC->GetStack();
    //~ // stack->AddPoint(kEsbWCdetector);
  //~ }
  
  //~ return kTRUE;
}

void EsbWCdetector::EndOfEvent()
{

  fEsbWCdetectorPointCollection->Clear();

}



void EsbWCdetector::Register()
{

  /** This will create a branch in the output tree called
      EsbWCdetectorPoint, setting the last parameter to kFALSE means:
      this collection will not be written to the file, it will exist
      only during the simulation.
  */

  FairRootManager::Instance()->Register("EsbWCdetectorPoint", "EsbWCdetector",
                                        fEsbWCdetectorPointCollection, kTRUE);

}


TClonesArray* EsbWCdetector::GetCollection(Int_t iColl) const
{
  if (iColl == 0) { return fEsbWCdetectorPointCollection; }
  else { return NULL; }
}

void EsbWCdetector::Reset()
{
  fEsbWCdetectorPointCollection->Clear();
}

void EsbWCdetector::ConstructGeometry()
{
	//TODO: Make a function for this a into EsbTools directory
	FairGeoLoader *geoLoad = FairGeoLoader::Instance();
	FairGeoInterface *geoFace = geoLoad->getGeoInterface();
	
	FairGeoMedia *geoMedia = geoFace->getMedia();
	FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();
	
	FairGeoMedium* mWC = geoMedia->getMedium("H2O_ESSnuSB");
	geoBuild->createMedium(mWC);
  TGeoMedium *WC_med = gGeoManager->GetMedium("H2O_ESSnuSB");
  
	//TODO: Change this to use media.geo file
  TGeoMaterial *Al_mat = new TGeoMaterial("Al", 26.98, 13, 2.7);
  TGeoMedium *Al_med = new TGeoMedium("Al", 101, Al_mat);
  
  // Create water cylinder
  TGeoVolume *wc = gGeoManager->MakeTube("wc", WC_med, 0.0, 300.0, 500.0);
  
  // Create thin wall around the water cylinder, 1 cm thick, to act as sensitive volume
  TGeoVolume *wall = gGeoManager->MakeTube("wall", Al_med, 300.0, 300.1, 500.0);
  TGeoVolume *endwall = gGeoManager->MakeTube("endwall", Al_med, 0, 300, 0.05);

  TList* media = gGeoManager->GetListOfMedia();
  for(TObject *obj : *media) {
		obj->Print();
	}	

  AddSensitiveVolume(wall); //From FairModule
  AddSensitiveVolume(endwall); //From FairModule

  TGeoVolume *top = gGeoManager->GetTopVolume();
  top->AddNode(wc, 1);
  top->AddNode(wall, 1); 
  top->AddNode(endwall, 1, new TGeoTranslation(0.0, 0.0, 500.05));
  top->AddNode(endwall, 2, new TGeoTranslation(0.0, 0.0, -500.05));

  wc->SetLineColor(kRed);

}

//___________________________________________________________________
EsbWCdetectorPoint* EsbWCdetector::AddHit(Int_t trackID, Int_t detID,
					  TVector3 pos, TVector3 mom,
					  Double_t time)
{
  TClonesArray& clref = *fEsbWCdetectorPointCollection;
  Int_t size = clref.GetEntriesFast();
  //~ cout << __PRETTY_FUNCTION__ << ": Size=" << size << " " << trackID << " " << detID << endl;
  //~ exit(1);
  return new(clref[size]) EsbWCdetectorPoint(trackID, detID, pos, mom,
					     time);
}

ClassImp(EsbWCdetector)
