/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "EsbGeometry/FgdDetector.h"
#include "EsbGeometry/EsbSuperFGD/Materials.h"
#include "EsbGeometry/EsbSuperFGD/Names.h"
#include "EsbData/FgdDetectorPoint.h" 

#include "FairLogger.h"
#include "FairGenericStack.h"
#include "FairGeoLoader.h"
#include "FairGeoInterface.h"
#include "FairGeoBuilder.h"
#include "FairGeoMedia.h"
#include "FairRootManager.h"
#include "FairStack.h"
#include "FairVolume.h"

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


#include "TGeoTube.h"

namespace esbroot {

namespace geometry {
// PC: work around
static const Int_t kFgdDetector = 1;

//___________________________________________________________________
FgdDetector::FgdDetector(const char* geoConfigFile, double posX, double posY, double posZ, Bool_t Active)
  : FairDetector("FgdDetector", kTRUE, kFgdDetector),
    fgdConstructor(geoConfigFile),
    fTrackID(-1),
    fVolumeID(-1),
    fPos(),
    fMom(),
    fTime(-1.),
    fLength(-1.),
    fELoss(-1),
    fposX(posX),
    fposY(posY),
    fposZ(posZ),
    isDefinedMaterials(false),
    fFgdDetectorPointCollection(new TClonesArray(data::FgdDetectorPoint::Class()))
{
}

//___________________________________________________________________
  FgdDetector::FgdDetector(const char* name, const char* geoConfigFile, double posX, double posY, double posZ, Bool_t active)
  : FairDetector(name, active, kFgdDetector),
    fgdConstructor(geoConfigFile),
    fTrackID(-1),
    fVolumeID(-1),
    fPos(),
    fMom(),
    fTime(-1.),
    fLength(-1.),
    fELoss(-1),
    fposX(posX),
    fposY(posY),
    fposZ(posZ),
    isDefinedMaterials(false),
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
  if(TVirtualMC::GetMC()->TrackPid() == 22 )
    return kTRUE;

	cout << __PRETTY_FUNCTION__ << endl;
  /** This method is called from the MC stepping */

  cout << "  TrackPid " << TVirtualMC::GetMC()->TrackPid() << endl;
  cout << "  TrackCharge " << TVirtualMC::GetMC()->TrackCharge() << endl;
  cout << "  Is track entering " << TVirtualMC::GetMC()->IsTrackEntering() << endl;
  cout << "  Is track exiting " << TVirtualMC::GetMC()->IsTrackExiting() << endl;
  cout << "vol->getCopyNo() " << vol->getCopyNo() << endl;
  cout << "vol->getVolumeId() " << vol->getVolumeId() << endl;
  TVirtualMC::GetMC()->TrackPosition(fPos);
  cout <<  "fPos.X() " << fPos.X() << endl;
  cout <<  "fPos.Y() " << fPos.Y() << endl;
  cout <<  "fPos.Z() " << fPos.Z() << endl;
  cout <<  "TrackLength " << TVirtualMC::GetMC()->TrackLength() << endl;
  cout <<  "GetCurrentTrackNumber " << TVirtualMC::GetMC()->GetStack()->GetCurrentTrackNumber() << endl;

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

  FairRootManager::Instance()->Register("EsbFgdDetectorPoint", "EsbFgdDetector", 
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
  using namespace geometry::superfgd;

  DefineMaterials();
  fgdConstructor.Construct();
	// Create the real Fgd geometry
	TGeoVolume* superFgdVol = gGeoManager->GetVolume(fgdnames::superFGDName);

  TGeoVolume *top = gGeoManager->GetTopVolume();
  top->AddNode(superFgdVol, 1, new TGeoTranslation(fposX, fposY, fposZ));
  
  // Extract the cube scintilator volume only and add it as the sensitive part
  AddToSensitiveVolumes(superFgdVol); 
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

void FgdDetector::DefineMaterials() 
{
  if(isDefinedMaterials) return; // Define materials only once

  isDefinedMaterials = true;

  FairGeoLoader *geoLoad = FairGeoLoader::Instance();
	FairGeoInterface *geoFace = geoLoad->getGeoInterface();
	
	FairGeoMedia *geoMedia = geoFace->getMedia();
	FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  // FairGeoMedium* brass = geoMedia->getMedium(esbroot::geometry::superfgd::materials::brass);
	// geoBuild->createMedium(brass);

  // FairGeoMedium* bronze = geoMedia->getMedium(esbroot::geometry::superfgd::materials::bronze);
	// geoBuild->createMedium(bronze);

  // FairGeoMedium* stainlessSteel = geoMedia->getMedium(esbroot::geometry::superfgd::materials::stainlessSteel);
	// geoBuild->createMedium(stainlessSteel);

  // FairGeoMedium* methane = geoMedia->getMedium(esbroot::geometry::superfgd::materials::methane);
	// geoBuild->createMedium(methane);

  // FairGeoMedium* carbonDioxide = geoMedia->getMedium(esbroot::geometry::superfgd::materials::carbonDioxide);
	// geoBuild->createMedium(carbonDioxide);

  // FairGeoMedium* carbontetraFloride = geoMedia->getMedium(esbroot::geometry::superfgd::materials::carbontetraFloride);
	// geoBuild->createMedium(carbontetraFloride);

  // FairGeoMedium* titaniumDioxide = geoMedia->getMedium(esbroot::geometry::superfgd::materials::titaniumDioxide);
	// geoBuild->createMedium(titaniumDioxide);

  // FairGeoMedium* polystyrene = geoMedia->getMedium(esbroot::geometry::superfgd::materials::polystyrene);
	// geoBuild->createMedium(polystyrene);

  FairGeoMedium* scintillator = geoMedia->getMedium(esbroot::geometry::superfgd::materials::scintillator);
	geoBuild->createMedium(scintillator);

  FairGeoMedium* paraterphnyl = geoMedia->getMedium(esbroot::geometry::superfgd::materials::paraterphnyl);
	geoBuild->createMedium(paraterphnyl);

  // FairGeoMedium* podscintillator = geoMedia->getMedium(esbroot::geometry::superfgd::materials::podscintillator);
	// geoBuild->createMedium(podscintillator);

  // FairGeoMedium* polyethylene = geoMedia->getMedium(esbroot::geometry::superfgd::materials::polyethylene);
	// geoBuild->createMedium(polyethylene);

  // FairGeoMedium* poduleEpoxy = geoMedia->getMedium(esbroot::geometry::superfgd::materials::poduleEpoxy);
	// geoBuild->createMedium(poduleEpoxy);

  // FairGeoMedium* polycarbonate = geoMedia->getMedium(esbroot::geometry::superfgd::materials::polycarbonate);
	// geoBuild->createMedium(polycarbonate);

  // FairGeoMedium* carbonFiber = geoMedia->getMedium(esbroot::geometry::superfgd::materials::carbonFiber);
	// geoBuild->createMedium(carbonFiber);

  FairGeoMedium* fiberCore = geoMedia->getMedium(esbroot::geometry::superfgd::materials::fiberCore);
	geoBuild->createMedium(fiberCore);

  FairGeoMedium* fiberCladding = geoMedia->getMedium(esbroot::geometry::superfgd::materials::fiberCladding);
	geoBuild->createMedium(fiberCladding);

  FairGeoMedium* fairTiO2 = geoMedia->getMedium(esbroot::geometry::superfgd::materials::titaniumDioxide);
  geoBuild->createMedium(fairTiO2);

  FairGeoMedium* fairPolystyrene = geoMedia->getMedium(esbroot::geometry::superfgd::materials::polystyrene);
  geoBuild->createMedium(fairPolystyrene);

  FairGeoMedium* fairAir = geoMedia->getMedium(esbroot::geometry::superfgd::materials::air);
  geoBuild->createMedium(fairAir);

  FairGeoMedium* vacuum = geoMedia->getMedium(esbroot::geometry::superfgd::materials::vacuum);
  geoBuild->createMedium(vacuum);
}

void FgdDetector::AddToSensitiveVolumes(TGeoVolume *vol)
{
  using namespace geometry::superfgd;
  TObjArray* arr = vol->GetNodes();

  for(Int_t i =0; arr && i < arr->GetEntries(); i++)
  {
    TGeoNode* node = (TGeoNode*)arr->At(i);
    TGeoVolume* nodeVol = node->GetVolume();

    // Compare if the volume name is the sensitive volume
    // 1. If it is, add it as a sensitive volume
    // 2. Else continue looping through the daughter volumes
    bool equalNames = nodeVol && (std::strcmp(nodeVol->GetName(),
                                     fgdnames::scintilatorVolume) == 0);

    if(nodeVol &&  equalNames)
    {
        AddSensitiveVolume(nodeVol);
        LOG(debug) << "Adding TGeoVolume " << nodeVol->GetName();
    }else if(nodeVol){
      AddToSensitiveVolumes(nodeVol);
    }
  }
}

}
}

