/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "EsbEsb.h"

#include "EsbEsbPoint.h"
#include "EsbEsbGeo.h"
#include "EsbEsbGeoPar.h"

#include "FairVolume.h"
#include "FairGeoVolume.h"
#include "FairGeoNode.h"
#include "FairRootManager.h"
#include "FairGeoLoader.h"
#include "FairGeoInterface.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "EsbDetectorList.h"
#include "EsbStack.h"

#include "TClonesArray.h"
#include "TVirtualMC.h"
#include "TGeoManager.h"
#include "TGeoBBox.h"
#include "TGeoCompositeShape.h"
#include "TGeoTube.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"

#include <iostream>
using std::cout;
using std::endl;

EsbEsb::EsbEsb()
  : FairDetector("EsbEsb", kTRUE, kEsbEsb),
    fTrackID(-1),
    fVolumeID(-1),
    fPos(),
    fMom(),
    fTime(-1.),
    fLength(-1.),
    fELoss(-1),
    fEsbEsbPointCollection(new TClonesArray("EsbEsbPoint"))
{
}

EsbEsb::EsbEsb(const char* name, Bool_t active)
  : FairDetector(name, active, kEsbEsb),
    fTrackID(-1),
    fVolumeID(-1),
    fPos(),
    fMom(),
    fTime(-1.),
    fLength(-1.),
    fELoss(-1),
    fEsbEsbPointCollection(new TClonesArray("EsbEsbPoint"))
{
}

EsbEsb::~EsbEsb()
{
  if (fEsbEsbPointCollection) {
    fEsbEsbPointCollection->Delete();
    delete fEsbEsbPointCollection;
  }
}

void EsbEsb::Initialize()
{
  FairDetector::Initialize();
  FairRuntimeDb* rtdb= FairRun::Instance()->GetRuntimeDb();
  EsbEsbGeoPar* par=(EsbEsbGeoPar*)(rtdb->getContainer("EsbEsbGeoPar"));
}

Bool_t  EsbEsb::ProcessHits(FairVolume* vol)
{
  /** This method is called from the MC stepping */

  //Set parameters at entrance of volume. Reset ELoss.
  if ( gMC->IsTrackEntering() ) {
    fELoss  = 0.;
    fTime   = gMC->TrackTime() * 1.0e09;
    fLength = gMC->TrackLength();
    gMC->TrackPosition(fPos);
    gMC->TrackMomentum(fMom);
  }

  // Sum energy loss for all steps in the active volume
  fELoss += gMC->Edep();

  // Create EsbEsbPoint at exit of active volume
  if ( gMC->IsTrackExiting()    ||
       gMC->IsTrackStop()       ||
       gMC->IsTrackDisappeared()   ) {
    fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
    fVolumeID = vol->getMCid();
    if (fELoss == 0. ) { return kFALSE; }
    AddHit(fTrackID, fVolumeID, TVector3(fPos.X(),  fPos.Y(),  fPos.Z()),
           TVector3(fMom.Px(), fMom.Py(), fMom.Pz()), fTime, fLength,
           fELoss);

    // Increment number of EsbEsb det points in TParticle
    EsbStack* stack = (EsbStack*) gMC->GetStack();
    stack->AddPoint(kEsbEsb);
  }

  return kTRUE;
}

void EsbEsb::EndOfEvent()
{

  fEsbEsbPointCollection->Clear();

}



void EsbEsb::Register()
{

  /** This will create a branch in the output tree called
      EsbEsbPoint, setting the last parameter to kFALSE means:
      this collection will not be written to the file, it will exist
      only during the simulation.
  */

  FairRootManager::Instance()->Register("EsbEsbPoint", "EsbEsb",
                                        fEsbEsbPointCollection, kTRUE);

}


TClonesArray* EsbEsb::GetCollection(Int_t iColl) const
{
  if (iColl == 0) { return fEsbEsbPointCollection; }
  else { return NULL; }
}

void EsbEsb::Reset()
{
  fEsbEsbPointCollection->Clear();
}

void EsbEsb::ConstructGeometry()
{
    
    TGeoVolume *top=gGeoManager->GetTopVolume();
    TGeoMedium *Si =gGeoManager->GetMedium("Si");
    TGeoMedium *Carbon = gGeoManager->GetMedium("Carbon");
    
    if(Si==0){
        TGeoMaterial *matSi     = new TGeoMaterial("Si", 28.0855, 14, 2.33);
        Si     = new TGeoMedium("Si", 2, matSi);
    }
    if(Carbon==0){
        TGeoMaterial *matCarbon    = new TGeoMaterial("C", 12.011, 6.0, 2.265);
        Carbon     = new TGeoMedium("C", 3, matCarbon);
    }

    
    TGeoVolume *det1= gGeoManager->MakeTubs("Det1",Si,5,80,0.1,0,360);
    AddSensitiveVolume(det1);
    TGeoRotation r1;
    r1.SetAngles(0,0,0);
    TGeoTranslation t1(0, 0, 0);
    TGeoCombiTrans c1(t1, r1);
    TGeoHMatrix *h1 = new TGeoHMatrix(c1);
    top->AddNode(det1,1,h1);
    det1->SetLineColor(kGreen);
    
    TGeoVolume *passive1= gGeoManager->MakeTubs("Pass1",Si,5,120,10,0,360);
    TGeoRotation rp1;
    rp1.SetAngles(0,0,0);
    TGeoTranslation tp1(0, 0, 20);
    TGeoCombiTrans cp1(tp1, rp1);
    TGeoHMatrix *hp1 = new TGeoHMatrix(cp1);
    top->AddNode(passive1,1,hp1);
    passive1->SetLineColor(kRed);
    
    
    
    TGeoVolume *det2= gGeoManager->MakeTubs("Det2",Si,5,150,0.1,0,360);
    AddSensitiveVolume(det2);
    TGeoRotation r2;
    r2.SetAngles(0,0,0);
    TGeoTranslation t2(0, 0, 70);
    TGeoCombiTrans c2(t2, r2);
    TGeoHMatrix *h2 = new TGeoHMatrix(c2);
    top->AddNode(det2,1,h2);
    det2->SetLineColor(kGreen);
    
    TGeoVolume *det3= gGeoManager->MakeTubs("Det3",Si,5,150,0.1,0,360);
    AddSensitiveVolume(det3);
    TGeoRotation r3;
    r3.SetAngles(0,0,0);
    TGeoTranslation t3(0, 0, 150);
    TGeoCombiTrans c3(t3, r3);
    TGeoHMatrix *h3 = new TGeoHMatrix(c3);
    top->AddNode(det3,1,h3);
    det3->SetLineColor(kGreen);
    
    
    
    
    
    
}

EsbEsbPoint* EsbEsb::AddHit(Int_t trackID, Int_t detID,
                                      TVector3 pos, TVector3 mom,
                                      Double_t time, Double_t length,
                                      Double_t eLoss)
{
  TClonesArray& clref = *fEsbEsbPointCollection;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) EsbEsbPoint(trackID, detID, pos, mom,
         time, length, eLoss);
}

ClassImp(EsbEsb)
