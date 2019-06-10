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

CubeScintConstructor::CubeScintConstructor(std::string n) 
: fCube(nullptr)
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

  // The Box shape from which the holes for the fiber will be subtracted
  TGeoBBox* cubeWithCoating = new TGeoBBox("CubeCoating",GetBase()/2, GetHeight()/2, GetLength()/2);

  // The Box shape from which the holes for the fiber will be subtracted
  double coatingThickness = GetCoatingThickness(); // coating thickness in cm (root default length dimention)
  TGeoBBox* cube = new TGeoBBox("Cube",
                                GetBase()/2 - coatingThickness,
                                GetHeight()/2 - coatingThickness,
                                GetLength()/2 - coatingThickness);

  TGeoCompositeShape* coating = new TGeoCompositeShape("coating","CubeCoating - Cube");

  TGeoVolume* cubeWithCoatingVolume = new TGeoVolume("CubeCoatingVolume",coating, gGeoManager->GetMedium(materials::scintillatorCoating));

  //=======================================================================================
  // Create the fiber hole along X
  //-----------------------------------
  TGeoCone* fiberHoleX = new TGeoCone("FX", GetBase()/2, 0, GetHoleRadius(),0 ,GetHoleRadius());

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

  // Create the fiber shape - the fiber shape is the contained within the fiber coating
  TGeoCone* fiberShapeX = new TGeoCone("fiberShapeX", GetBase()/2, 0, GetFiberRadius(),0 ,GetFiberRadius());

  // Create the fiber coating-> this is the hole minus the fiber itself
  TGeoCompositeShape* fiberXShapeCoat = new TGeoCompositeShape("fiberXShapeCoat","FX - fiberShapeX");

  // Create fiber coating
  TGeoVolume* fiberXCoatVolume = new TGeoVolume("fiberXCoatVolume",fiberXShapeCoat, gGeoManager->GetMedium(materials::fiberCladding));

  // Create fiber core volume
  TGeoVolume* fiberXVolume = new TGeoVolume("fiberXVolume",fiberShapeX, gGeoManager->GetMedium(materials::fiberCore));

  // Place the fiber core inside its coating
  fiberXCoatVolume->AddNode(fiberXVolume, 1 /* One copy*/);

  //-----------------------------------
  //=======================================================================================

  //=======================================================================================
	// Create the fiber hole along Y
  //-----------------------------------
  TGeoCone* fiberHoleY = new TGeoCone("FY", GetHeight()/2, 0, GetHoleRadius(),0 ,GetHoleRadius());

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

  // Create the fiber shape - the fiber shape is the contained within the fiber coating
  TGeoCone* fiberShapeY = new TGeoCone("fiberShapeY", GetHeight()/2, 0, GetFiberRadius(),0 ,GetFiberRadius());

  // Create the fiber coating-> this is the hole minus the fiber itself
  TGeoCompositeShape* fiberYShapeCoat = new TGeoCompositeShape("fiberYShapeCoat","FY - fiberShapeY");

  // Create fiber coating
  TGeoVolume* fiberYCoatVolume = new TGeoVolume("fiberYCoatVolume",fiberYShapeCoat, gGeoManager->GetMedium(materials::fiberCladding));

  // Create fiber core volume
  TGeoVolume* fiberYVolume = new TGeoVolume("fiberYVolume",fiberShapeY, gGeoManager->GetMedium(materials::fiberCore));

  // Place the fiber core inside its coating
  fiberYCoatVolume->AddNode(fiberYVolume, 1 /* One copy*/);

  //-----------------------------------
  //=======================================================================================

  //=======================================================================================
  // Create the fiber hole along Z
  //-----------------------------------
  TGeoCone* fiberHoleZ = new TGeoCone("FZ", GetLength()/2, 0, GetHoleRadius(),0 ,GetHoleRadius());

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


  // Create the fiber shape - the fiber shape is the contained within the fiber coating
  TGeoCone* fiberShapeZ = new TGeoCone("fiberShapeZ", GetLength()/2, 0, GetFiberRadius(),0 ,GetFiberRadius());

  // Create the fiber coating-> this is the hole minus the fiber itself
  TGeoCompositeShape* fiberZShapeCoat = new TGeoCompositeShape("fiberZShapeCoat","FZ - fiberShapeZ");

  // Create fiber coating
  TGeoVolume* fiberZCoatVolume = new TGeoVolume("fiberZCoatVolume",fiberZShapeCoat, gGeoManager->GetMedium(materials::fiberCladding));

  // Create fiber core volume
  TGeoVolume* fiberZVolume = new TGeoVolume("fiberZVolume",fiberShapeZ, gGeoManager->GetMedium(materials::fiberCore));

  // Place the fiber core inside its coating
  fiberZCoatVolume->AddNode(fiberZVolume, 1 /* One copy*/);

  //-----------------------------------
  //=======================================================================================

  // Create scintilator cube shape
  TGeoCompositeShape* cubeComp = new TGeoCompositeShape("cubeComp","Cube - FX:locationX - FY:locationY - FY:locationZ");

  // Create Volume scintilator cube
  TGeoVolume* cubeScntVol = new TGeoVolume(GetName().c_str(),cubeComp, gGeoManager->GetMedium(materials::scintillator));

  // Place the scintilator cube into the cube coating
  cubeWithCoatingVolume->AddNode(cubeScntVol, 1 /* One Element*/ /*, Identity matrix is by default used for location*/);

  // Place the fiber coatings with fiber core
  cubeWithCoatingVolume->AddNode(fiberXCoatVolume, 1 /* One Element*/, locationX);
  cubeWithCoatingVolume->AddNode(fiberYCoatVolume, 1 /* One Element*/, locationY);
  cubeWithCoatingVolume->AddNode(fiberZCoatVolume, 1 /* One Element*/, locationZ);
  
  // TODO2 : Add Sensitive volume
  //AddSensitiveVolume(volume); //From FairModule

  fCube = cubeWithCoatingVolume;;
}


}   //superfgd
}   //geometry
}   //esbroot