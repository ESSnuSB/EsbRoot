#include "EsbSimulators/EsbSuperFGD/EsbSuperFGDDetectorConstruction.h"
#include "EsbSimulators/EsbSuperFGD/EsbSuperFGDConstructor.h"

#include "G4Box.hh"
#include "G4PVPlacement.hh"

#include "G4FieldManager.hh"
#include "G4UniformMagField.hh"
#include "G4TransportationManager.hh"

namespace esbroot {
namespace simulators {
namespace superfgd {

SuperFGDDetectorConstruction::SuperFGDDetectorConstruction()
  :FgdDetectorConstruction()
{
}

SuperFGDDetectorConstruction::SuperFGDDetectorConstruction(std::string detectorFile)
  : FgdDetectorConstruction(detectorFile)
{
}

SuperFGDDetectorConstruction::~SuperFGDDetectorConstruction()
{
}

G4VPhysicalVolume* SuperFGDDetectorConstruction::Construct()
{
    G4String cNameLogicSuperFGD1 = "Esb/SuperFGD";
    
    shared_ptr<ND280SuperFGDConstructor> fSuperFGDConstructor1 = make_shared<ND280SuperFGDConstructor>(cNameLogicSuperFGD1);
    fSuperFGDConstructor1->SetVisibility(fdetector.ParamAsBool(data::superfgd::detector::DP::visdetail));

    G4String nameSuperFGD1 = fSuperFGDConstructor1->GetName();

    Double_t lunit = fdetector.GetLenghtUnit();
    Double_t cube_X = fdetector.ParamAsDouble(data::superfgd::detector::DP::length_X) * lunit;

    Double_t cube_X_N = fdetector.ParamAsDouble(data::superfgd::detector::DP::number_cubes_X);
    Double_t cube_Y_N = fdetector.ParamAsDouble(data::superfgd::detector::DP::number_cubes_Y);
    Double_t cube_Z_N = fdetector.ParamAsDouble(data::superfgd::detector::DP::number_cubes_Z);

    double edge = cube_X;
    int cubenumX = cube_X_N;
    int cubenumY = cube_Y_N;
    int cubenumZ = cube_Z_N;

    G4double x = fdetector.ParamAsDouble(data::superfgd::detector::DP::positon_X);
    G4double y = fdetector.ParamAsDouble(data::superfgd::detector::DP::positon_Y);
    G4double z = fdetector.ParamAsDouble(data::superfgd::detector::DP::positon_Z);

    fSuperFGDConstructor1->SetEdge(edge*CLHEP::mm);
    fSuperFGDConstructor1->SetCubeNumX(cubenumX);
    fSuperFGDConstructor1->SetCubeNumY(cubenumY);
    fSuperFGDConstructor1->SetCubeNumZ(cubenumZ);

    G4LogicalVolume* logicSuperFGD1 = fSuperFGDConstructor1->GetPiece(); // SuperFGD logical volume (define the real size here!!!)

    // Target mother volume --> use it in TrackerSD to calculate the distance from the hit to the MPPC plane
    
    double width  = fSuperFGDConstructor1->GetWidth(); // dimensions set inside ND280SuperFGDConstructor based on the # of cubes and its size
    double length = fSuperFGDConstructor1->GetLength();
    double height = fSuperFGDConstructor1->GetHeight();  

    G4Box* solidTarget1 = new G4Box(nameSuperFGD1,width/2,height/2,length/2);
    G4LogicalVolume* logicTarget1 = new G4LogicalVolume(solidTarget1,ND280SuperFGDConstructor::FindMaterial("Air"),nameSuperFGD1,0,0,0); 
    
  //-------------------------------------------------------------------------
  // Magnetic field
  //-------------------------------------------------------------------------
  if (fdetector.existsParam(data::superfgd::detector::DP::magField)) 
  {
        cout << "InMagneticField" << endl;
        G4double magField_X = fdetector.ParamAsDouble(data::superfgd::detector::DP::magField_X) * CLHEP::tesla;
        G4double magField_Y = fdetector.ParamAsDouble(data::superfgd::detector::DP::magField_Y) * CLHEP::tesla;
        G4double magField_Z = fdetector.ParamAsDouble(data::superfgd::detector::DP::magField_Z) * CLHEP::tesla;

        G4UniformMagField* magField = new G4UniformMagField(G4ThreeVector(magField_X,magField_Y,magField_Z));
        G4FieldManager* localFieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();
        localFieldManager->SetDetectorField(magField);
        localFieldManager->CreateChordFinder(magField);
        G4bool allLocal = true; // Assign field only to those daughter volumes which do not have a field manager already.
        logicSuperFGD1->SetFieldManager(localFieldManager, allLocal);
  } 
      
    G4VPhysicalVolume* physiSuperFGD1 = new G4PVPlacement(
				       0, // no rotation
				       G4ThreeVector(0,0,0),  // same origin as Target1
				       logicSuperFGD1,        // its logical volume    
				       nameSuperFGD1,         // its name
				       logicTarget1,          // its mother
				       false,                 // no boolean operations
				       0);  
       
    G4VPhysicalVolume* world = new G4PVPlacement(
				       0, // no rotation
				       G4ThreeVector(0,0,0),  // same origin as Target1
				       logicTarget1,          // its logical volume    
				       "world",               // its name
				       0,
				       false,                 // no boolean operations
               0);

    std::cout << "logicTarget1 Mass -> " << logicTarget1->GetMass()/CLHEP::gram << std::endl;

    return world;
}

}   //superfgd
}   //simulators
}   //esbroot