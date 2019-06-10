#include "EsbGeometry/EsbSuperFGD/EsbCubeScintilatorConstructor.h"
#include "EsbGeometry/EsbSuperFGD/EsbSuperFGDConstructor.h"

#include "EsbGeometry/EsbSuperFGD/EsbFgdDetectorParameters.h"
#include "EsbGeometry/EsbSuperFGD/Materials.h"

#include "TGeoBBox.h"
#include "TGeoCone.h"
#include "TGeoCompositeShape.h"

namespace esbroot {
namespace geometry {
namespace superfgd {

CubeScintConstructor::CubeScintConstructor(std::string n) : fCube(nullptr)
{
}
  
CubeScintConstructor::~CubeScintConstructor()
{
}

TGeoVolume* CubeScintConstructor::GetPiece(void) 
{ 
  if(!fCube){
      Construct();
  }

  return fCube;
}

void CubeScintConstructor::Construct() 
{

  // Base is X direction
  // Height is Y direction
  // Lenght is Z direction

  //=============================
  //=============================
  //=============================
  SetBase(10.);
  SetHeight(10.);
  SetLength(10.);

  SetHoleRadius(0.5);

  double shift = 1.5*CLHEP::mm;

  ThreeVector locX(-shift, shift, 0);
  ThreeVector locY(shift, 0, -shift);
  ThreeVector locZ(0, -shift, shift);

  SetHolePosition_X(locX);
  SetHolePosition_Y(locY);
  SetHolePosition_Z(locZ);
  
  RotMatrix rotXX(0,0,0);
  RotMatrix rotYY(0,90,0);
  RotMatrix rotZZ(90,90,0);

  SetHoleRot_X(rotXX);
  SetHoleRot_Y(rotYY);
  SetHoleRot_Z(rotZZ);
  //=============================
  //=============================
  //=============================



  // The Box shape from which the holes for the fiber will be subtracted
  TGeoBBox* cube = new TGeoBBox("Cube",GetBase()/2, GetHeight()/2, GetLength()/2);

  // Create the fiber hole along X
  //-----------------------------------
  TGeoCone* fiberX = new TGeoCone("FX", GetBase()/2, 0, GetHoleRadius(),0 ,GetHoleRadius());

  TGeoRotation* rotX = new TGeoRotation("rotX", 
                            fHoleRotX.rotateX,
                            fHoleRotX.rotateY,
                            fHoleRotX.rotateZ);
  rotX->RegisterYourself();

  TGeoCombiTrans* locationX = new TGeoCombiTrans("locationX", 
                            fHolePositionX.X,
                            fHolePositionX.Y,
                            fHolePositionX.Z,
                            rotX);
  locationX->RegisterYourself();
  //-----------------------------------

	// Create the fiber hole along Y
  //-----------------------------------
  TGeoCone* fiberY = new TGeoCone("FY", GetHeight()/2, 0, GetHoleRadius(),0 ,GetHoleRadius());

  TGeoRotation* rotY = new TGeoRotation("rotY", 
                            fHoleRotY.rotateX,
                            fHoleRotY.rotateY,
                            fHoleRotY.rotateZ);
  rotY->RegisterYourself();

  TGeoCombiTrans* locationY = new TGeoCombiTrans("locationY", 
                            fHolePositionY.X,
                            fHolePositionY.Y,
                            fHolePositionY.Z,
                            rotY);
  locationY->RegisterYourself();

  //-----------------------------------

  // Create the fiber hole along Z
  //-----------------------------------
  TGeoCone* fiberZ = new TGeoCone("FZ", GetLength()/2, 0, GetHoleRadius(),0 ,GetHoleRadius());

  TGeoRotation* rotZ = new TGeoRotation("rotY", 
                            fHoleRotZ.rotateX,
                            fHoleRotZ.rotateY,
                            fHoleRotZ.rotateZ);
  rotZ->RegisterYourself();

  TGeoCombiTrans* locationZ = new TGeoCombiTrans("locationZ", 
                            fHolePositionZ.X,
                            fHolePositionZ.Y,
                            fHolePositionZ.Z,
                            rotZ);
  locationZ->RegisterYourself();

  //-----------------------------------

  // Create scintilator cube shape
  TGeoCompositeShape* comp = new TGeoCompositeShape("comp","Cube - FX:locationX - FY:locationY - FY:locationZ");

  // Create Volume scintilator cube
  fCube = new TGeoVolume(GetName().c_str(),comp, gGeoManager->GetMedium(materials::scintillator));
}


}   //superfgd
}   //geometry
}   //esbroot