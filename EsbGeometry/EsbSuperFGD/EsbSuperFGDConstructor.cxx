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

SuperFGDConstructor::SuperFGDConstructor(std::string name)
{
    DefineMaterials();
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

  // Number of cubes along each axis 
  //fCubeNumX = 1;
  //fCubeNumY = 1;
  //fCubeNumZ = 1;

  SetWidth(fCubeNumX * fEdge);
  SetHeight(fCubeNumY * fEdge);
  SetLength(fCubeNumZ * fEdge);

  // Position of the center of the SuperFGD detector
  fPosX = 0.;
  fPosY = 0.;
  fPosZ = 0.;

  //
  // Note:
  // GetName() is the SuperFGD name given in DetectorConstruction  
  // i.e. /t2k/OA/.../SuperFGD
  //

  std::string nameRepXYZ = "RepY"; // replica of single cube along Z
  std::string nameRepXZ  = nameRepXYZ+"/RepX"; // replica of single row  along X
  std::string nameRepZ   = nameRepXZ +"/RepZ"; // replica of single layer along Y
  std::string nameCube   = nameRepZ+"/CubeScint";
  
  SetNameRepXYZ(nameRepXYZ);
  SetNameRepXZ(nameRepXZ);
  SetNameRepZ(nameRepZ);
  SetNameCube(nameCube);
}


void SuperFGDConstructor::GetPiece(void) 
{
  // TODO create detector volume
}


void SuperFGDConstructor::DefineMaterials() 
{
  FairGeoLoader *geoLoad = FairGeoLoader::Instance();
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


}   //superfgd
}   //geometry
}   //esbroot