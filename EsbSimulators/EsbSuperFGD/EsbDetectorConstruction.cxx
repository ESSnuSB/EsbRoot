#include "EsbSimulators/EsbSuperFGD/EsbDetectorConstruction.h"

#include "EsbSimulators/EsbSuperFGD/EsbCubeSD.h"
#include "EsbSimulators/EsbSuperFGD/EsbCubeParametrisation.h"
#include "EsbSimulators/EsbSuperFGD/EsbFiberParametrisation.h"
#include "EsbSimulators/EsbSuperFGD/EsbFiberSD.h"
#include "EsbSimulators/EsbSuperFGD/EsbFgdStepAction.h"
#include "EsbSimulators/EsbSuperFGD/EsbSlabParametrization.h"

#include "EsbData/EsbSuperFGD/EsbDetector/EsbFgdDetectorParameters.h"

#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4FieldManager.hh"
#include "G4UniformMagField.hh"
#include "G4Box.hh"
#include "G4PVParameterised.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4TransportationManager.hh"
#include "G4SubtractionSolid.hh"

#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"

namespace esbroot {
namespace simulators {
namespace superfgd {

FgdDetectorConstruction::FgdDetectorConstruction()
  :G4VUserDetectorConstruction()
{

}

FgdDetectorConstruction::FgdDetectorConstruction(std::string detectorFile)
  : G4VUserDetectorConstruction()
{
  fdetector.LoadPartParams(detectorFile);
  fdetector.CreateRootGeometry();
}

FgdDetectorConstruction::~FgdDetectorConstruction()
{
}


G4VPhysicalVolume* FgdDetectorConstruction::Construct()
{
  // Envelope parameters
  Double_t lunit =fdetector.GetLenghtUnit();

  Double_t cube_X = fdetector.ParamAsDouble(data::superfgd::detector::DP::length_X) * lunit;
  Double_t cube_X_N = fdetector.ParamAsDouble(data::superfgd::detector::DP::number_cubes_X);
  G4double lengthX =  cube_X * cube_X_N;

  Double_t cube_Y = fdetector.ParamAsDouble(data::superfgd::detector::DP::length_Y) * lunit;
  Double_t cube_Y_N = fdetector.ParamAsDouble(data::superfgd::detector::DP::number_cubes_Y);
  G4double lengthY =  cube_Y * cube_Y_N;

  Double_t cube_Z = fdetector.ParamAsDouble(data::superfgd::detector::DP::length_Z) * lunit;
  Double_t cube_Z_N = fdetector.ParamAsDouble(data::superfgd::detector::DP::number_cubes_Z);
  G4double lengthZ =  cube_Z * cube_Z_N;
  

  G4NistManager* nist = G4NistManager::Instance();
  //G4Material* g4_air = nist->FindOrBuildMaterial("G4_AIR");
  G4double density     = universe_mean_density;    //from PhysicalConstants.h
  G4double pressure    = 3.e-18*pascal;
  G4double temperature = 2.73*kelvin;
  G4Material* vacuum =   
  new G4Material("Galactic", 1., 1.008*g/mole, density,
                             kStateGas,temperature,pressure);

  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  //-------------------------------------------------------------------------
  // Detector Surrounding - Start
  //-------------------------------------------------------------------------
  G4double detectorSurrounding_X = 2*lengthX;
  G4double detectorSurrounding_Y = 2*lengthY;
  G4double detectorSurrounding_Z  = 2*lengthZ;

  
  G4Box* detectorSurrounding =    
    new G4Box("Detector_Surrounding",                       //its name
       0.5*detectorSurrounding_X, 0.5*detectorSurrounding_Y, 0.5*detectorSurrounding_Z);     //its size
        
  G4LogicalVolume* logicDetectorSurrounding =                         
    new G4LogicalVolume(detectorSurrounding,       //its solid
                        vacuum,                    //its material
                        "Detector_Surrounding_LV");//its name

  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                        //no rotation
                      G4ThreeVector(),          //at (0,0,0)
                      logicDetectorSurrounding, //its logical volume
                      "Detector_Surrounding_PV",//its name
                      0,                        //its mother  volume, 0 indicating its the top most volume
                      false,                    //no boolean operation
                      0,                        //copy number
                      checkOverlaps);           //overlaps checking
  //-------------------------------------------------------------------------
  // Detector Surrounding - End
  //-------------------------------------------------------------------------

  //-------------------------------------------------------------------------
  // Detector Envelop, this holds all detector parts - Start
  //-------------------------------------------------------------------------
  G4Box* solidEnvDetector =    
    new G4Box("Envelope_Detector",              //its name
        0.5*lengthX, 0.5*lengthY, 0.5*lengthZ); //its size
      
  G4LogicalVolume* logicEnvDetector =                         
    new G4LogicalVolume(solidEnvDetector,       //its solid
                        vacuum,                 //its material
                        "Envelope_Detector_LV");//its name      

  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnvDetector,        //its logical volume
                    "Envelope_Detector_PV",  //its name
                    logicDetectorSurrounding,//its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking                      
  //-------------------------------------------------------------------------
  // Detector Envelop - End
  //-------------------------------------------------------------------------
  
  //-------------------------------------------------------------------------
  // Slab, contains detector cubes and fiber - Start
  //-------------------------------------------------------------------------               
  G4Box* slab =    
    new G4Box("Slab",                          //its name
        0.5*lengthX, 0.5*lengthY, 0.5*cube_Z); //its size
      
  G4LogicalVolume* logicSlab =                         
    new G4LogicalVolume(slab,            //its solid
                        vacuum,          //its material
                        "Slab_LV");      //its name

  SlabParametrisation* slabParam = new SlabParametrisation((-0.5*lengthZ + cube_Z/2) ,cube_Z);
    new G4PVParameterised("Slab_Placement",// their name
                        logicSlab,        // their logical volume
                        logicEnvDetector, // Mother logical volume
                        kZAxis,           // Are placed along this axis 
                        cube_Z_N,         // Number of chambers
                        slabParam,        // The parametrisation
                        checkOverlaps);   // checking overlaps 
  //-------------------------------------------------------------------------
  // Slab - End
  //------------------------------------------------------------------------

  //-------------------------------------------------------------------------
  // Cube with fiber - Start
  //-------------------------------------------------------------------------

  // Fiber             ------------------------------------------------------
  G4Material* fiberMaterial = nist->FindOrBuildMaterial(fdetector.ParamAsString(data::superfgd::detector::DP::fiber_g4material));
  Double_t fiber_radius = fdetector.ParamAsDouble(data::superfgd::detector::DP::fiber_radius) * lunit;

  G4VSolid* fiber_solid = 
      new G4Tubs("fiber_solid",
                                 0,
                      fiber_radius,
                       0.5*cube_Z,
                    0.* CLHEP::deg,
                  360.* CLHEP::deg);

  G4LogicalVolume* logic_fiber =                         
    new G4LogicalVolume(fiber_solid,    //its solid
                        fiberMaterial,  //its material
                        "fiber_LV");    //its name
  
  // Cube             ------------------------------------------------------
  G4Box* cube_solid =    
      new G4Box("Cube_Envelope_Detector",         //its name
       0.5*cube_X, 0.5*cube_Y, 0.5*cube_Z);       //its size

  G4RotationMatrix* rotmY = new G4RotationMatrix();
    rotmY->rotateX(90*deg);

  // Extruded Cube     -----------------------------------------------------
  G4VSolid* ext_cube_solid = new G4SubtractionSolid("cube_extruded_fiber",
					     cube_solid,
					     fiber_solid,
					     rotmY,
					     G4ThreeVector(0,0,0));

  //Initialize scintillator props
  G4Material* scintillator = nist->FindOrBuildMaterial(fdetector.ParamAsString(data::superfgd::detector::DP::scintillator));
  if(fdetector.ParamAsBool(data::superfgd::detector::DP::optPhotonOn))
  {
    AddScintillatonProperties(scintillator);
  };

  G4LogicalVolume* logic_Ext_Cube =                         
    new G4LogicalVolume(ext_cube_solid,      //its solid
                        scintillator,        //its material
                        "Cube_LV");          //its name

  // Place the fiber inside the extruded cube       
  new G4PVPlacement(rotmY,                   //rotation as extrusion
                    G4ThreeVector(),         //at (0,0,0)
                    logic_fiber,             //its logical volume
                    "Cube_Envelope_Detector_PV",  //its name
                    logic_Ext_Cube,          //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  CubeParametrisation* cubeParam = new CubeParametrisation(cube_X,cube_X_N,cube_Y,cube_Y_N);
    new G4PVParameterised("Cube_Placement",// their name
                        logic_Ext_Cube,   // their logical volume
                        logicSlab,        // Mother logical volume !!!
                        kZAxis,           // Are placed along this axis 
                        cube_X_N*cube_Y_N,// Number of chambers
                        cubeParam,        // The parametrisation
                        checkOverlaps);   // checking overlaps
  //-------------------------------------------------------------------------
  // Cube with fiber - End
  //-------------------------------------------------------------------------
  
  //------------------------------------------------------------------
  // Fiber Sensitive detectors - Start
  //------------------------------------------------------------------

  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  // attach sensitive detector to fibers
  G4String fiberSDname = "/mydet/fiberSD";
  FiberSD *fiberSD = new FiberSD(fiberSDname);
  SDman->AddNewDetector(fiberSD);
  logic_fiber->SetSensitiveDetector(fiberSD);

  //attach sensitive detector to cubes
  G4String cubeSDname = "/mydet/cubeSD";
  CubeSD *cubeSD = new CubeSD(cubeSDname , data::superfgd::detector::DP::NF_DETECTOR_CUBE_SD);
  SDman->AddNewDetector(cubeSD);
  logic_Ext_Cube->SetSensitiveDetector(cubeSD);

  //------------------------------------------------------------------
  // Fiber Sensitive detectors - End
  //------------------------------------------------------------------
  
  // Don`t know, took it from basic/B1 example :P
  FgdStepAction* steppingAction = FgdStepAction::Instance(); 
  if(steppingAction!=nullptr && steppingAction!=0)
  {
    steppingAction->SetVolume(logicEnvDetector);
  }

  //-------------------------------------------------------------------------
  // Magnetic field
  //-------------------------------------------------------------------------
  if (fdetector.ExistsParam(data::superfgd::detector::DP::magField)) 
  {
        cout << "InMagneticField" << endl;
        G4double magField_X = fdetector.ParamAsDouble(data::superfgd::detector::DP::magField_X) * CLHEP::tesla;
        G4double magField_Y = fdetector.ParamAsDouble(data::superfgd::detector::DP::magField_Y) * CLHEP::tesla;
        G4double magField_Z = fdetector.ParamAsDouble(data::superfgd::detector::DP::magField_Z) * CLHEP::tesla;

        G4UniformMagField* magField = new G4UniformMagField(G4ThreeVector(magField_X,magField_Y,magField_Z));
        G4FieldManager* localFieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();
        //G4FieldManager *localFieldManager = new G4FieldManager(magField);
        localFieldManager->SetDetectorField(magField);
        localFieldManager->CreateChordFinder(magField);
        G4bool allLocal = true; // Assign field only to those daughter volumes which do not have a field manager already.
        logicEnvDetector->SetFieldManager(localFieldManager, allLocal);
  }    

  //-------------------------------------------------------------------------
  // Visualization - Start
  //-------------------------------------------------------------------------
  bool isDetailed = (fdetector.ParamAsInt(data::superfgd::detector::DP::visdetail) > 0);

  if(!isDetailed)
  {
    logicSlab->SetVisAttributes(G4VisAttributes::GetInvisible());
    logicEnvDetector->SetVisAttributes(G4VisAttributes::GetInvisible());
    logic_fiber->SetVisAttributes(G4VisAttributes::GetInvisible());
    logic_Ext_Cube->SetVisAttributes(G4VisAttributes::GetInvisible());
  }

  //-------------------------------------------------------------------------
  // Visualization - End
  //-------------------------------------------------------------------------

  //
  //always return the physical World
  //
  return physWorld;

}


void FgdDetectorConstruction::AddScintillatonProperties(G4Material* scintillator)
{
  //
// ------------ Generate & Add Material Properties Table ------------
//
  const G4int nEntries = 32;

  G4double PhotonEnergy[nEntries] =
            { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
              2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
              2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
              2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
              2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
              3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
              3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
              3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV };
//
// Scintillator
//	      
  G4double RefractiveIndex1[nEntries] =
            { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
              1.346,  1.3465, 1.347,  1.3475, 1.348,
              1.3485, 1.3492, 1.35,   1.3505, 1.351,
              1.3518, 1.3522, 1.3530, 1.3535, 1.354,
              1.3545, 1.355,  1.3555, 1.356,  1.3568,
              1.3572, 1.358,  1.3585, 1.359,  1.3595,
              1.36,   1.3608};

  G4double Absorption1[nEntries] =
           {3.448*m,  4.082*m,  6.329*m,  9.174*m, 12.346*m, 13.889*m,
           15.152*m, 17.241*m, 18.868*m, 20.000*m, 26.316*m, 35.714*m,
           45.455*m, 47.619*m, 52.632*m, 52.632*m, 55.556*m, 52.632*m,
           52.632*m, 47.619*m, 45.455*m, 41.667*m, 37.037*m, 33.333*m,
           30.000*m, 28.500*m, 27.000*m, 24.500*m, 22.000*m, 19.500*m,
           17.500*m, 14.500*m };

  G4double ScintilFast[nEntries] =
            { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00 };
  G4double ScintilSlow[nEntries] =
            { 0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00,
              7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 4.00,
              3.00, 2.00, 1.00, 0.01, 1.00, 2.00, 3.00,
              4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00,
              7.00, 6.00, 5.00, 4.00 };

  G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();

  myMPT1->AddProperty("RINDEX",       PhotonEnergy, RefractiveIndex1,nEntries);
  myMPT1->AddProperty("ABSLENGTH",    PhotonEnergy, Absorption1,     nEntries);
  myMPT1->AddProperty("FASTCOMPONENT",PhotonEnergy, ScintilFast,     nEntries);
  myMPT1->AddProperty("SLOWCOMPONENT",PhotonEnergy, ScintilSlow,     nEntries);;
  
  myMPT1->AddConstProperty("SCINTILLATIONYIELD",50./MeV);
  myMPT1->AddConstProperty("RESOLUTIONSCALE",1.0);
  myMPT1->AddConstProperty("FASTTIMECONSTANT", 1.*ns);
  myMPT1->AddConstProperty("SLOWTIMECONSTANT",10.*ns);
  myMPT1->AddConstProperty("YIELDRATIO",0.8);

  const G4int NUMENTRIES_water = 60;

  G4double ENERGY_water[NUMENTRIES_water] = {
     1.56962*eV, 1.58974*eV, 1.61039*eV, 1.63157*eV,
     1.65333*eV, 1.67567*eV, 1.69863*eV, 1.72222*eV,
     1.74647*eV, 1.77142*eV, 1.7971 *eV, 1.82352*eV,
     1.85074*eV, 1.87878*eV, 1.90769*eV, 1.93749*eV,
     1.96825*eV, 1.99999*eV, 2.03278*eV, 2.06666*eV,
     2.10169*eV, 2.13793*eV, 2.17543*eV, 2.21428*eV,
     2.25454*eV, 2.29629*eV, 2.33962*eV, 2.38461*eV,
     2.43137*eV, 2.47999*eV, 2.53061*eV, 2.58333*eV,
     2.63829*eV, 2.69565*eV, 2.75555*eV, 2.81817*eV,
     2.88371*eV, 2.95237*eV, 3.02438*eV, 3.09999*eV,
     3.17948*eV, 3.26315*eV, 3.35134*eV, 3.44444*eV,
     3.54285*eV, 3.64705*eV, 3.75757*eV, 3.87499*eV,
     3.99999*eV, 4.13332*eV, 4.27585*eV, 4.42856*eV,
     4.59258*eV, 4.76922*eV, 4.95999*eV, 5.16665*eV,
     5.39129*eV, 5.63635*eV, 5.90475*eV, 6.19998*eV
  };

  //assume 100 times larger than the rayleigh scattering for now.
  G4double MIE_water[NUMENTRIES_water] = {
     167024.4*m, 158726.7*m, 150742  *m,
     143062.5*m, 135680.2*m, 128587.4*m,
     121776.3*m, 115239.5*m, 108969.5*m,
     102958.8*m, 97200.35*m, 91686.86*m,
     86411.33*m, 81366.79*m, 76546.42*m,
     71943.46*m, 67551.29*m, 63363.36*m,
     59373.25*m, 55574.61*m, 51961.24*m,
     48527.00*m, 45265.87*m, 42171.94*m,
     39239.39*m, 36462.50*m, 33835.68*m,
     31353.41*m, 29010.30*m, 26801.03*m,
     24720.42*m, 22763.36*m, 20924.88*m,
     19200.07*m, 17584.16*m, 16072.45*m,
     14660.38*m, 13343.46*m, 12117.33*m,
     10977.70*m, 9920.416*m, 8941.407*m,
     8036.711*m, 7202.470*m, 6434.927*m,
     5730.429*m, 5085.425*m, 4496.467*m,
     3960.210*m, 3473.413*m, 3032.937*m,
     2635.746*m, 2278.907*m, 1959.588*m,
     1675.064*m, 1422.710*m, 1200.004*m,
     1004.528*m, 833.9666*m, 686.1063*m
  };

  // gforward, gbackward, forward backward ratio
  G4double MIE_const[3]={0.99,0.99,0.8};

  myMPT1->AddProperty("MIEHG",ENERGY_water,MIE_water,NUMENTRIES_water);
  myMPT1->AddConstProperty("MIEHG_FORWARD",MIE_const[0]);
  myMPT1->AddConstProperty("MIEHG_BACKWARD",MIE_const[1]);
  myMPT1->AddConstProperty("MIEHG_FORWARD_RATIO",MIE_const[2]);

  scintillator->SetMaterialPropertiesTable(myMPT1);

  // Set the Birks Constant for the Water scintillator

  scintillator->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
}

}   //superfgd
}   //simulators
}   //esbroot