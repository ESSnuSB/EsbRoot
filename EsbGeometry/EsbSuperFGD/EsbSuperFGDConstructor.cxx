#include "EsbGeometry/EsbSuperFGD/EsbSuperFGDConstructor.h"
#include "EsbGeometry/EsbSuperFGD/EsbCubeScintilatorConstructor.h"
#include "EsbGeometry/EsbSuperFGD/EsbFgdDetectorParameters.h"
#include "EsbGeometry/EsbSuperFGD/Materials.h"

#include "FairLogger.h"                 // for FairLogger, etc

#include "TGeoMaterial.h"

namespace esbroot {
namespace geometry {
namespace superfgd {

SuperFGDConstructor::SuperFGDConstructor(std::string name)
:fVolume(nullptr)
{
    SetName(name);
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

void SuperFGDConstructor::Construct()
{
  // All dimentions are in cm (root default dimention length)

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