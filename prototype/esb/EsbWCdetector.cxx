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
  cout << __PRETTY_FUNCTION__ << endl;

	FairGeoLoader *geoLoad = FairGeoLoader::Instance();
	FairGeoInterface *geoFace = geoLoad->getGeoInterface();
	
	FairGeoMedia *geoMedia = geoFace->getMedia();
	FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();
	
	FairGeoMedium* mWC = geoMedia->getMedium("H2O_ESSnuSB");
	geoBuild->createMedium(mWC);
  TGeoMedium *WC_med = gGeoManager->GetMedium("H2O_ESSnuSB");
  
  WC_med->Print();
	
	//~ exit(0);

  // Create water material and medium 
  // density should eventually be controlled via settable parameters
  //~ TGeoMaterial *water_mat = new TGeoMaterial("WcWater", 18, 10, 1.0);
  //~ TGeoMedium   *water_med = new TGeoMedium("WcWater", 100, water_mat);
  
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

  // Probably need to rotate it
  TGeoVolume *top = gGeoManager->GetTopVolume();
  //~ TGeoRotation r1;
  //~ r1.SetAngles(0,0,0);
  //~ TGeoTranslation t1(0, 0, 0);
  //~ TGeoCombiTrans c1(t1, r1);
  //~ TGeoHMatrix *h1 = new TGeoHMatrix(c1);
  top->AddNode(wc, 1);
  top->AddNode(wall, 1); 
  top->AddNode(endwall, 1, new TGeoTranslation(0.0, 0.0, 500.05));
  top->AddNode(endwall, 2, new TGeoTranslation(0.0, 0.0, -500.05));

  wc->SetLineColor(kRed);

  //~ // Set Cherenkov parameters
  //~ // Just some I found online for seawater for now
  //~ // TGeo units are GeV and cm
  //~ const Double_t eV = 1.0e-9;
  //~ const Double_t m  = 100.;
  //~ const Double_t nm = m / 1.0e9 ;
  
  //~ // (sea) water
  //~ static const Int_t nEnergies = 32;
  //~ static Double_t photEnergies[nEnergies] = {
    //~ 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
    //~ 2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
    //~ 2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
    //~ 2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
    //~ 2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
    //~ 3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
    //~ 3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
    //~ 3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV
  //~ };
  //~ static Double_t refractIdxSeaWater[nEnergies] = {
    //~ 1.3429, 1.3432, 1.3435, 1.3438, 1.3441,
    //~ 1.3445, 1.3448, 1.3452, 1.3456, 1.3461,
    //~ 1.3465, 1.3470, 1.3475, 1.3481, 1.3487,
    //~ 1.3493, 1.3500, 1.3507, 1.3514, 1.3522,
    //~ 1.3531, 1.3540, 1.3551, 1.3561, 1.3573,
    //~ 1.3586, 1.3600, 1.3615, 1.3632, 1.3650,
    //~ 1.3669, 1.3691
  //~ };
  //~ static Double_t absSeaWater[nEnergies] = {
    //~ 3.448*m,  4.082*m,  6.329*m,  9.174*m, 12.346*m, 13.889*m,
    //~ 15.152*m, 17.241*m, 18.868*m, 20.000*m, 26.316*m, 35.714*m,
    //~ 45.455*m, 47.619*m, 52.632*m, 52.632*m, 55.556*m, 52.632*m,
    //~ 52.632*m, 47.619*m, 45.455*m, 41.667*m, 37.037*m, 33.333*m,
    //~ 30.000*m, 28.500*m, 27.000*m, 24.500*m, 22.000*m, 19.500*m,
    //~ 17.500*m, 14.500*m
  //~ };
  
  //~ // Sea water index of refraction vs photon energy [GeV]
  //~ TGraph seaWaterRef(nEnergies, photEnergies, refractIdxSeaWater);

  //~ // Sea water absorbtion length [cm] vs photon energy [GeV]
  //~ TGraph seaWaterAbs(nEnergies, photEnergies, absSeaWater);
  
  //~ // efficiency: not sure what this is...
  //~ static Double_t efficiency[nEnergies];
  //~ for(Int_t i = 0; i < nEnergies; i++) {
    
    //~ //efficiency[i] = 0;
    //~ efficiency[i] = 1;
  //~ }
  
  //~ const Int_t waterId  = gMC->MediumId("WcWater");
  //~ gMC->SetCerenkov(101, nEnergies,
		   //~ photEnergies, absSeaWater,
		   //~ efficiency, refractIdxSeaWater);
  //gMC->SetCerenkov(waterId, seaWaterRef.GetN(),
		//~ //   seaWaterRef.GetX(), seaWaterAbs.GetY(),
		  //~ // efficiency, seaWaterRef.GetY());
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
