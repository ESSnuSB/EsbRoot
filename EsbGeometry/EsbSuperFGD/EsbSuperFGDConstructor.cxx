#include "EsbGeometry/EsbSuperFGD/EsbSuperFGDConstructor.h"
#include "EsbGeometry/EsbSuperFGD/EsbCubeScintilatorConstructor.h"
#include "EsbGeometry/EsbSuperFGD/EsbFgdDetectorParameters.h"
#include "EsbGeometry/EsbSuperFGD/Materials.h"

#include "FairLogger.h"                 // for FairLogger, etc

// For  Medium creation
#include "FairGeoBuilder.h"             
#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoMedia.h"

#include "TGeoMaterial.h"

namespace esbroot {
namespace geometry {
namespace superfgd {

SuperFGDConstructor::SuperFGDConstructor(std::string name, FairGeoLoader* geoLoad)
:fVolume(nullptr), fgeoLoad(geoLoad)
{
    SetName(name);
    DefineMaterials();
    Init();
}
  
SuperFGDConstructor::~SuperFGDConstructor()
{
}

void SuperFGDConstructor::Init(void) 
{
  /// The edge of a cube of the SuperFGD detector 
  //fEdge = 1*CLHEP::cm;

  // Number of cubes along each axis (in cm)
  fCubeNumX = 1;
  fCubeNumY = 1;
  fCubeNumZ = 1;

  fEdge = 1.0; // in cm

  SetWidth(fCubeNumX * fEdge);
  SetHeight(fCubeNumY * fEdge);
  SetLength(fCubeNumZ * fEdge);

  // Position of the center of the SuperFGD detector
  fPosX = 0.;
  fPosY = 0.;
  fPosZ = 0.;

  std::string nameCube   = "CubeScint";
  SetNameCube(nameCube);
}


TGeoVolume* SuperFGDConstructor::GetPiece(void) 
{
  if(!fVolume){
      Construct();
  }

  return fVolume;
}


void SuperFGDConstructor::DefineMaterials() 
{
  FairGeoLoader *geoLoad = fgeoLoad;
	FairGeoInterface *geoFace = geoLoad->getGeoInterface();
	
	FairGeoMedia *geoMedia = geoFace->getMedia();
	FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* brass = geoMedia->getMedium(materials::brass);
	geoBuild->createMedium(brass);

  FairGeoMedium* bronze = geoMedia->getMedium(materials::bronze);
	geoBuild->createMedium(bronze);

  FairGeoMedium* stainlessSteel = geoMedia->getMedium(materials::stainlessSteel);
	geoBuild->createMedium(stainlessSteel);

  FairGeoMedium* methane = geoMedia->getMedium(materials::methane);
	geoBuild->createMedium(methane);

  FairGeoMedium* carbonDioxide = geoMedia->getMedium(materials::carbonDioxide);
	geoBuild->createMedium(carbonDioxide);

  FairGeoMedium* carbontetraFloride = geoMedia->getMedium(materials::carbontetraFloride);
	geoBuild->createMedium(carbontetraFloride);

  FairGeoMedium* titaniumDioxide = geoMedia->getMedium(materials::titaniumDioxide);
	geoBuild->createMedium(titaniumDioxide);

  FairGeoMedium* polystyrene = geoMedia->getMedium(materials::polystyrene);
	geoBuild->createMedium(polystyrene);

  FairGeoMedium* scintillator = geoMedia->getMedium(materials::scintillator);
	geoBuild->createMedium(scintillator);

  FairGeoMedium* podscintillator = geoMedia->getMedium(materials::podscintillator);
	geoBuild->createMedium(podscintillator);

  FairGeoMedium* polyethylene = geoMedia->getMedium(materials::polyethylene);
	geoBuild->createMedium(polyethylene);

  FairGeoMedium* poduleEpoxy = geoMedia->getMedium(materials::poduleEpoxy);
	geoBuild->createMedium(poduleEpoxy);

  FairGeoMedium* polycarbonate = geoMedia->getMedium(materials::polycarbonate);
	geoBuild->createMedium(polycarbonate);

  FairGeoMedium* carbonFiber = geoMedia->getMedium(materials::carbonFiber);
	geoBuild->createMedium(carbonFiber);

  FairGeoMedium* fiberCore = geoMedia->getMedium(materials::fiberCore);
	geoBuild->createMedium(fiberCore);

  FairGeoMedium* fiberCladding = geoMedia->getMedium(materials::fiberCladding);
	geoBuild->createMedium(fiberCladding);

  TGeoMedium *tiO2 = gGeoManager->GetMedium(materials::titaniumDioxide);
  TGeoMedium *c8H8 = gGeoManager->GetMedium(materials::polystyrene);

  TGeoMixture *scintillatorCoating = new TGeoMixture(materials::scintillatorCoating,2, 1.164);
  scintillatorCoating->AddElement(tiO2->GetMaterial(), 0.15);
  scintillatorCoating->AddElement(c8H8->GetMaterial(), 0.85);

}

void SuperFGDConstructor::Construct()
{
  // All dimentions are in cm (root default dimention length)

  fCubeNumX = 3;
  fCubeNumY = 4;
  fCubeNumZ = 5;

  double TotWidth  = fCubeNumX * fEdge; 
  double TotLength = fCubeNumY * fEdge;  
  double TotHeight = fCubeNumZ * fEdge;

  SetWidth(TotWidth);
  SetLength(TotLength);
  SetHeight(TotHeight);

  // Build the plastic scintillator cube
  CubeScintConstructor cube(GetNameCube());
  cube.SetVisibility(true);
  cube.SetName(GetName() + "/cube");
  
  cube.SetBase(fEdge);
  cube.SetLength(fEdge);
  cube.SetHeight(fEdge);  
  cube.SetHoleRadius(0.07);
  cube.SetFiberRadius(0.05);
  cube.SetCoatingThickness(0.025);
  cube.SetGap(0.0);
  
  double shift = 0.15; 
  ThreeVector HolePosAlongX = ThreeVector(-shift, shift, 0); // hole along X
  ThreeVector HolePosAlongY = ThreeVector(shift, 0, -shift); // hole along Y
  ThreeVector HolePosAlongZ = ThreeVector(0, -shift, shift); // hole along Z
  cube.SetHolePosition_X(HolePosAlongX); 
  cube.SetHolePosition_Y(HolePosAlongY); 
  cube.SetHolePosition_Z(HolePosAlongZ); 

  RotMatrix rotXX(0,0,0);
  RotMatrix rotYY(0,90,0);
  RotMatrix rotZZ(90,90,0);

  cube.SetHoleRot_X(rotXX);
  cube.SetHoleRot_Y(rotYY);
  cube.SetHoleRot_Z(rotZZ);

  //========================================
  // Repeat the volume in X
  TGeoBBox* rowX = new TGeoBBox("rowX", TotWidth, fEdge, fEdge);
  TGeoVolume* rowXVol = new TGeoVolume("rowXVol",rowX);

  TGeoVolume* cube_vol = cube.GetPiece();
  double startPosX = -TotWidth/2 + fEdge/2;

  for(int i=0; i < fCubeNumX; i++)
  {
    int copyNo = i+1;
    rowXVol->AddNode(cube_vol,copyNo,new TGeoTranslation((startPosX + i*fEdge),0,0));
  }
  //========================================

  //========================================
  // Repeat the volume in XY
  TGeoBBox* rowXY = new TGeoBBox("rowXY", TotWidth, TotLength, fEdge);
  TGeoVolume* rowXYVol = new TGeoVolume("rowXYVol",rowXY);

  double startPosXY = -TotLength/2 + fEdge/2;

  for(int i=0; i < fCubeNumY; i++)
  {
    int copyNo = i+1;
    rowXYVol->AddNode(rowXVol,copyNo,new TGeoTranslation(0,(startPosXY + i*fEdge),0));
  }
  //========================================

  //========================================
  // Repeat the volume in XYZ
  TGeoBBox* rowXYZ = new TGeoBBox("rowXYZ", TotWidth, TotLength, TotHeight);
  TGeoVolume* rowXYZVol = new TGeoVolume("rowXYZVol",rowXYZ);

  double startPosXYZ = -TotHeight/2 + fEdge/2;

  for(int i=0; i < fCubeNumZ; i++)
  {
    int copyNo = i+1;
    rowXYZVol->AddNode(rowXYVol,copyNo,new TGeoTranslation(0,0,(startPosXYZ + i*fEdge)));
  }
  //========================================

  fVolume = rowXYZVol;
}


}   //superfgd
}   //geometry
}   //esbroot