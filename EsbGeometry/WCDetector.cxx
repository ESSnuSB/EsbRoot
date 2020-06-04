/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "EsbGeometry/WCDetector.h"

#include "EsbData/WCDetectorPoint.h"
//#include "EsbWCDetectorGeo.h"
//#include "EsbWCDetectorGeoPar.h"


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

namespace esbroot {

namespace geometry {
// PC: work around
static const Int_t kWCDetector = 1;

//___________________________________________________________________
WCDetector::WCDetector()
  : FairDetector("EsbWCDetector", kTRUE, kWCDetector),
    fTrackID(-1),
    fVolumeID(-1),
    fPos(),
    fMom(),
    fTime(-1.),
    fLength(-1.),
    fELoss(-1),
    fWCradius(-1),
    fWChalflength(-1),
    fWCDetectorPointCollection(new TClonesArray(data::WCDetectorPoint::Class()))
{
}

//___________________________________________________________________
  WCDetector::WCDetector(const char* name, Double_t wcRadius, 
			       Double_t wcHalflength, Bool_t active)
  : FairDetector(name, active, kWCDetector),
    fTrackID(-1),
    fVolumeID(-1),
    fPos(),
    fMom(),
    fTime(-1.),
    fLength(-1.),
    fELoss(-1),
    fWCradius(wcRadius),
    fWChalflength(wcHalflength),
    fWCDetectorPointCollection(new TClonesArray(data::WCDetectorPoint::Class()))
{
}

//___________________________________________________________________
WCDetector::~WCDetector()
{
  if (fWCDetectorPointCollection) {
    fWCDetectorPointCollection->Delete();
    delete fWCDetectorPointCollection;
  }
}

//___________________________________________________________________
void WCDetector::Initialize()
{
  FairDetector::Initialize();
  //  FairRuntimeDb* rtdb= FairRun::Instance()->GetRuntimeDb();
  //  PC: This is where we will load the detector parameters 
  //  EsbWCDetectorGeoPar* par=(EsbWCDetectorGeoPar*)(rtdb->getContainer("EsbWCDetectorGeoPar"));
}

//___________________________________________________________________
Bool_t  WCDetector::ProcessHits(FairVolume* vol)
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
    //~ stack->AddPoint(kEsbWCDetector);
  }

  return kTRUE;

  //~ // We could define the emission point for the photons. In that way we would
  //~ // have the "track" we want to reconstruct.

  //~ // Create EsbWCDetectorPoint at exit of active volume
  //~ if ( gMC->IsTrackExiting() ) {

    //~ fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
    //~ fVolumeID = vol->getMCid();
    //~ AddHit(fTrackID, fVolumeID, TVector3(fPos.X(),  fPos.Y(),  fPos.Z()),
           //~ TVector3(fMom.Px(), fMom.Py(), fMom.Pz()), fTime);

    //~ // PC: work around
    //~ // // Increment number of EsbWCDetector det points in TParticle
    //~ // FairGenericStack* stack = (FairGenericStack*) gMC->GetStack();
    //~ // stack->AddPoint(kEsbWCDetector);
  //~ }
  
  //~ return kTRUE;
}

void WCDetector::EndOfEvent()
{

  fWCDetectorPointCollection->Clear();

}



void WCDetector::Register()
{

  /** This will create a branch in the output tree called
      EsbWCDetectorPoint, setting the last parameter to kFALSE means:
      this collection will not be written to the file, it will exist
      only during the simulation.
  */

  FairRootManager::Instance()->Register("EsbWCDetectorPoint", "EsbWCDetector",
                                        fWCDetectorPointCollection, kTRUE);

}


TClonesArray* WCDetector::GetCollection(Int_t iColl) const
{
  if (iColl == 0) { return fWCDetectorPointCollection; }
  else { return NULL; }
}

void WCDetector::Reset()
{
  fWCDetectorPointCollection->Clear();
}

void WCDetector::ConstructGeometry()
{
	//TODO: Make a function for this a into EsbTools directory
	FairGeoLoader *geoLoad = FairGeoLoader::Instance();
	FairGeoInterface *geoFace = geoLoad->getGeoInterface();
	
	FairGeoMedia *geoMedia = geoFace->getMedia();
	FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();
	
	FairGeoMedium* mWC = geoMedia->getMedium("WCSim_Water");
	geoBuild->createMedium(mWC);
  TGeoMedium *WC_med = gGeoManager->GetMedium("WCSim_Water");
  
	//TODO: Change this to use media.geo file
  TGeoMaterial *Al_mat = new TGeoMaterial("Al", 26.98, 13, 2.7);
  TGeoMedium *Al_med = new TGeoMedium("Al", 101, Al_mat);
  
  // Create water cylinder
  TGeoVolume *wc = gGeoManager->MakeTube("wc", WC_med, 0.0, fWCradius, fWChalflength);
  
  // Create thin wall around the water cylinder, 1 cm thick, to act as sensitive volume
  TGeoVolume *wall = gGeoManager->MakeTube("wall", Al_med, fWCradius, fWCradius+0.1, fWChalflength);
  TGeoVolume *endwall = gGeoManager->MakeTube("endwall", Al_med, 0, fWCradius, 0.05);

  TList* media = gGeoManager->GetListOfMedia();
  for(TObject *obj : *media) {
		obj->Print();
	}	

  AddSensitiveVolume(wall); //From FairModule
  AddSensitiveVolume(endwall); //From FairModule

  //TODO: Top volume should be a parameter in the constructor
  TGeoVolume *top = gGeoManager->GetTopVolume();
  top->AddNode(wc, 1);
  top->AddNode(wall, 1); 
  top->AddNode(endwall, 1, new TGeoTranslation(0.0, 0.0, fWChalflength+0.05));
  top->AddNode(endwall, 2, new TGeoTranslation(0.0, 0.0, -fWChalflength-0.05));

  wc->SetLineColor(kRed);

}

//___________________________________________________________________
data::WCDetectorPoint* WCDetector::AddHit(Int_t trackID, Int_t detID,
					  TVector3 pos, TVector3 mom,
					  Double_t time)
{
  TClonesArray& clref = *fWCDetectorPointCollection;
  Int_t size = clref.GetEntriesFast();
  //~ cout << __PRETTY_FUNCTION__ << ": Size=" << size << " " << trackID << " " << detID << endl;
  //~ exit(1);
  return new(clref[size]) data::WCDetectorPoint(trackID, detID, pos, mom,
					     time);
}

}
}

//~ ClassImp(WCDetector)
