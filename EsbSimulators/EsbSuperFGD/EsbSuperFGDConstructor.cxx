#include "EsbSimulators/EsbSuperFGD/EsbSuperFGDConstructor.h"
#include "EsbSimulators/EsbSuperFGD/EsbCubeScintilatorConstructor.h"

#include "EsbData/EsbSuperFGD/EsbDetector/EsbFgdDetectorParameters.h"

#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4PVReplica.hh>
#include "G4NistManager.hh"

#include <G4Tubs.hh>

namespace esbroot {
namespace simulators {
namespace superfgd {

ND280SuperFGDConstructor::ND280SuperFGDConstructor(G4String n)
{
    DefineMaterials();
    SetName(n);
    Init();
}
  
ND280SuperFGDConstructor::~ND280SuperFGDConstructor()
{
}

void ND280SuperFGDConstructor::Init(void) 
{
  /// The edge of a cube of the SuperFGD detector 
  //fEdge = 1*CLHEP::cm;

  // Number of cubes along each axis 
  //fCubeNumX = 1;
  //fCubeNumY = 1;
  //fCubeNumZ = 1;

  SetWidth( fCubeNumX * fEdge);
  SetHeight(fCubeNumY * fEdge);
  SetLength(fCubeNumZ * fEdge);

  //SetFiberRadius(0.5*mm);
  //SetFiberMaterial("FiberCore");

  // Position of the center of the SuperFGD detector
  fPosX = 0.;
  fPosY = 0.;
  fPosZ = 0.;

  //
  // Note:
  // GetName() is the SuperFGD name given in DetectorConstruction  
  // i.e. /t2k/OA/.../SuperFGD
  //

  G4String nameRepXYZ = "RepY"; // replica of single cube along Z
  G4String nameRepXZ  = nameRepXYZ+"/RepX"; // replica of single row  along X
  G4String nameRepZ   = nameRepXZ +"/RepZ"; // replica of single layer along Y
  G4String nameCube   = nameRepZ+"/CubeScint";
  
  SetNameRepXYZ(nameRepXYZ);
  SetNameRepXZ(nameRepXZ);
  SetNameRepZ(nameRepZ);
  SetNameCube(nameCube);
}


G4LogicalVolume *ND280SuperFGDConstructor::GetPiece(void) 
{
  // Set total size of the Super-FGD

  double TotWidth  = fCubeNumX * fEdge; 
  double TotHeight = fCubeNumY * fEdge; 
  double TotLength = fCubeNumZ * fEdge; 

  SetWidth(TotWidth);
  SetLength(TotLength);
  SetHeight(TotHeight);

  // Build the plastic scintillator cube
  NFND280CubeScintConstructor cube(GetNameCube());
  cube.SetVisibility(true);
  cube.SetName(GetName() + "/cube");
  
  cube.SetBase(fEdge);
  cube.SetLength(fEdge);
  cube.SetHeight(fEdge);  
  cube.SetHoleRadius(0.7*CLHEP::mm);
  cube.SetFiberRadius(0.5*CLHEP::mm);
  cube.SetCoatingThickness(0.25*CLHEP::mm);
  cube.SetGap(0.0*CLHEP::mm);
  
  double shift = 1.5*CLHEP::mm; //cube.GetHoleRadius();
  G4ThreeVector HolePosAlongX = G4ThreeVector(0.    , shift , shift); // hole along X
  G4ThreeVector HolePosAlongY = G4ThreeVector(shift , 0.    , -shift); // hole along Y
  G4ThreeVector HolePosAlongZ = G4ThreeVector(-shift, -shift, 0.    ); // hole along Z
  cube.SetHolePosition_X(HolePosAlongX); 
  cube.SetHolePosition_Y(HolePosAlongY); 
  cube.SetHolePosition_Z(HolePosAlongZ); 

  G4LogicalVolume* cube_logical = cube.GetPiece();
                
  //
  // Make the logical volumes for the replicas
  //
 
  // Build row of cubes along Z (replica of single cube along Z)

  G4VSolid *repZ_solid 
    = new G4Box(GetName()+"/"+GetNameRepZ(), 
		cube.GetBase()/2, 
		cube.GetHeight()/2, 
		GetLength()/2); 
  G4LogicalVolume *repZ_logical 
    = new G4LogicalVolume(repZ_solid,
			  ND280SuperFGDConstructor::FindMaterial("Air"),
			  GetName()+"/"+GetNameRepZ());
  //repZ_logical->SetVisAttributes(G4VisAttributes::Invisible); 
     
  // Build layer of cubes XZ (replica of single row of cubes along X)  

  G4VSolid *repXZ_solid 
    = new G4Box(GetName()+"/"+GetNameRepXZ(), 
		GetWidth()/2, 
		cube.GetHeight()/2, 
		GetLength()/2);   
  G4LogicalVolume *repXZ_logical 
    = new G4LogicalVolume(repXZ_solid,
			  ND280SuperFGDConstructor::FindMaterial("Air"),
			  GetName()+"/"+GetNameRepXZ());
  //repXZ_logical->SetVisAttributes(G4VisAttributes::Invisible);  

  // Build box XYZ of cubes

  G4VSolid *repXYZ_solid 
    = new G4Box(GetName()+"/"+GetNameRepXYZ(), 
		GetWidth()/2, 
		GetHeight()/2, 
		GetLength()/2); 
  G4LogicalVolume *repXYZ_logical 
    = new G4LogicalVolume(repXYZ_solid,
			  ND280SuperFGDConstructor::FindMaterial("Air"),
			  GetName()+"/"+GetNameRepXYZ());
  //repXYZ_logical->SetVisAttributes(G4VisAttributes::Invisible);  

  //
  // Place the cubes:
  // 1) replica of cubes along Z --> single row
  // 2) replica of rows along X --> single layer
  // 3) replica of layers along Y --> SuperFGD
  //
  
  new G4PVReplica(GetName()+"/"+GetNameRepZ(),        // its name
		  cube_logical,     // its logical volume
		  repZ_logical,     // its mother volume
		  kZAxis,           // axis along replication applied
		  fCubeNumZ,        // number of replicated volumes
		  cube.GetLength()  // width of single replica along axis 
		  );

  new G4PVReplica(GetName()+"/"+GetNameRepXZ(),        // its name
		  repZ_logical,     // its logical volume
		  repXZ_logical,    // its mother volume
		  kXAxis,           // axis along replication applied
		  fCubeNumX,        // number of replicated volumes
		  cube.GetBase()    // width of single replica along axis 
		  );

  new G4PVReplica(GetName()+"/"+GetNameRepXYZ(),        // its name
		  repXZ_logical,      // its logical volume
		  repXYZ_logical,     // its mother volume
		  kYAxis,             // axis along replication applied
		  fCubeNumY,          // number of replicated volumes
		  cube.GetHeight()    // width of single replica along axis 
		  );

  return repXYZ_logical;
}

void ND280SuperFGDConstructor::DefineMaterials() 
{

  // Method copied from ND280 software

  //ND280RootGeometryManager* gMan = ND280RootGeometryManager::Get();
  
  G4double a, density;
  G4String name, symbol;
  G4double temperature, pressure;
  G4int nel, ncomponents, natoms;
  G4double fractionmass;

  G4NistManager* nistMan = G4NistManager::Instance();

  G4Element* elH = nistMan->FindOrBuildElement(1);
  G4Element* elB = nistMan->FindOrBuildElement(5);
  G4Element* elC = nistMan->FindOrBuildElement(6);
  G4Element* elN = nistMan->FindOrBuildElement(7);
  G4Element* elO = nistMan->FindOrBuildElement(8);
  G4Element* elF = nistMan->FindOrBuildElement(9);
  G4Element* elNa = nistMan->FindOrBuildElement(11);
  G4Element* elAl = nistMan->FindOrBuildElement(13);
  G4Element* elSi = nistMan->FindOrBuildElement(14);
  G4Element* elCl = nistMan->FindOrBuildElement(17);
  G4Element* elAr = nistMan->FindOrBuildElement(18);
  G4Element* elTi = nistMan->FindOrBuildElement(22);
  G4Element* elFe = nistMan->FindOrBuildElement(26);
  G4Element* elCo = nistMan->FindOrBuildElement(27);
  G4Element* elCu = nistMan->FindOrBuildElement(29);
  G4Element* elZn = nistMan->FindOrBuildElement(30);
  G4Element* elSn = nistMan->FindOrBuildElement(50);
  G4Element* elPb = nistMan->FindOrBuildElement(82);

#ifdef Define_Vacuum
  //Vacuum (set as default material)
  density     = universe_mean_density;
  pressure    = 3.e-18*CLHEP::pascal;
  temperature = 2.73*CLHEP::kelvin;
  vacuum = new G4Material(name="Vacuum", z=1., a=1.01*g/mole,
			  density,kStateGas,temperature,pressure);
  //gMan->SetColor(vacuum->GetName(),-1); // ND280 class
#endif

  //Air
  density = 1.29*CLHEP::mg/CLHEP::cm3;
  pressure    = 1*CLHEP::atmosphere;
  temperature = 293.15*CLHEP::kelvin;
  G4Material* air =  new G4Material(name="Air", density,
				    nel=2,kStateGas,temperature,
				    pressure);
  air->AddElement(elN, fractionmass = 70*CLHEP::perCent);
  air->AddElement(elO, fractionmass = 30*CLHEP::perCent);
  fDefaultMaterial = air;
  //gMan->SetDrawAtt(air,kGray+3); // ND280 class

  // //Earth
  // density = 2.15*g/CLHEP::cm3;
  // G4Material* earth = new G4Material(name="Earth", density, nel=2);
  // earth->AddElement(elSi, natoms=1);
  // earth->AddElement(elO, natoms=2);
  // gMan->SetDrawAtt(earth,49,0.2); // ND280 class
    
  // //Cement
  // density = 2.5*g/CLHEP::cm3;
  // G4Material* cement = new G4Material(name="Cement", density, nel=2);
  // cement->AddElement(elSi, natoms=1);
  // cement->AddElement(elO, natoms=2);
  // gMan->SetDrawAtt(cement,14,0.2); // ND280 class

  //Water
  density = 1.0*CLHEP::g/CLHEP::cm3;
  G4Material* water = new G4Material(name="Water", density, nel=2);
  water->AddElement(elH, natoms=2);
  water->AddElement(elO, natoms=1);
  //gMan->SetDrawAtt(water,kBlue); // ND280 class

  //Aluminum
  density = 2.70*CLHEP::g/CLHEP::cm3;
  G4Material* aluminum = new G4Material(name="Aluminum",density, nel=1);
  aluminum->AddElement(elAl,100.*CLHEP::perCent);
  //gMan->SetDrawAtt(aluminum,kYellow-5); // ND280 class

  //Iron.
  density = 7.87*CLHEP::g/CLHEP::cm3;
  G4Material* iron = new G4Material(name="Iron",density, nel=1);
  iron->AddElement(elFe,100.*CLHEP::perCent);
  //gMan->SetDrawAtt(iron,kRed+2,0.3); // ND280 class

  //Copper
  density = 8.94*CLHEP::g/CLHEP::cm3;
  G4Material* copper = new G4Material(name="Copper",density, nel=1);
  copper->AddElement(elCu,100.*CLHEP::perCent);
  //gMan->SetDrawAtt(copper,kRed+1,0.3);// ND280 class

  //Lead
  density = 11.35*CLHEP::g/CLHEP::cm3;
  G4Material* lead = new G4Material(name="Lead",density, nel=1);
  lead->AddElement(elPb,100.*CLHEP::perCent);
  //gMan->SetDrawAtt(lead,kGray+1);// ND280 class

  //Brass -- The density is from simetric.co.uk is 8400 -- 8730 gm/cm3
  density = 8.50*CLHEP::g/CLHEP::cm3;
  G4Material* brass = new G4Material(name="Brass", density, nel=2);
  brass->AddElement(elCu, fractionmass = 90*CLHEP::perCent);
  brass->AddElement(elZn, fractionmass = 10*CLHEP::perCent);
  //gMan->SetDrawAtt(brass,kOrange-2);// ND280 class

  //Bronze -- The density is from simetric.co.uk is 7700 -- 8920 gm/cm3
  density = 8.5*CLHEP::g/CLHEP::cm3;
  G4Material* bronze = new G4Material(name="Bronze", density, nel=2);
  bronze->AddElement(elCu, fractionmass = 60*CLHEP::perCent);
  bronze->AddElement(elSn, fractionmass = 40*CLHEP::perCent);
  //gMan->SetDrawAtt(bronze,kOrange-3);// ND280 class

  //Stainless Steel.  The density is taken from average 304 grade SS.
  density = 8.0*CLHEP::g/CLHEP::cm3;
  G4Material* steel = new G4Material(name="Steel", density, nel=3);
  steel->AddElement(elC,  fractionmass =  4*CLHEP::perCent);
  steel->AddElement(elFe, fractionmass = 88*CLHEP::perCent);
  steel->AddElement(elCo, fractionmass =  8*CLHEP::perCent);
  //gMan->SetDrawAtt(steel,kBlue-10);// ND280 class

  //Argon
  density     = 1.66*CLHEP::mg/CLHEP::cm3;    
  pressure    = 1*CLHEP::atmosphere;
  temperature = 293.15*CLHEP::kelvin;
  G4Material* argon =  new G4Material(name="Ar", density,
				      nel=1,kStateGas,temperature,
				      pressure);
  argon->AddElement(elAr, natoms=1);
  //gMan->SetDrawAtt(argon,kBlue-10,0.1);// ND280 class

  //Methane
  density     = 0.667*CLHEP::mg/CLHEP::cm3;
  pressure    = 1*CLHEP::atmosphere;
  temperature = 293.15*CLHEP::kelvin;
  G4Material* methane = new G4Material(name="CH4", density,
				       nel=2,kStateGas,temperature,
				       pressure);
  methane->AddElement(elC, natoms=1);
  methane->AddElement(elH, natoms=4);
  //gMan->SetDrawAtt(methane,kBlue-10,0.1);// ND280 class

  //Argon + 10% Methane
  density     = 2.33*CLHEP::mg/CLHEP::cm3;
  G4Material* gasMixture 
    = new G4Material(name="GasMixture", density, ncomponents=2);
  gasMixture->AddMaterial(argon, fractionmass = 90*CLHEP::perCent);
  gasMixture->AddMaterial(methane, fractionmass = 10*CLHEP::perCent);
  //gMan->SetDrawAtt(gasMixture,kBlue-10,0.1);// ND280 class

  // CarbonDioxide
  density     = 1.828*CLHEP::mg/CLHEP::cm3;
  pressure    = 1.*CLHEP::atmosphere;
  temperature = 293.15*CLHEP::kelvin;
  G4Material* CO2 = new G4Material(name="CO2", density,
				   nel=2,kStateGas,temperature,
				   pressure);
  CO2->AddElement(elC, natoms=1);
  CO2->AddElement(elO, natoms=2);
  //gMan->SetDrawAtt(CO2,kBlue-10,0.1);// ND280 class

  // CarbonTetrafluoride
  density     = 3.66*CLHEP::mg/CLHEP::cm3;
  pressure    = 1.*CLHEP::atmosphere;
  temperature = 293.15*CLHEP::kelvin;
  G4Material* CF4 = new G4Material(name="CF4", density,
				   nel=2,kStateGas,temperature,
				   pressure);
  CF4->AddElement(elC, natoms=1);
  CF4->AddElement(elF, natoms=4);
  //gMan->SetDrawAtt(CF4,kBlue-10,0.1);// ND280 class

  // Isobutane
  density     = 2.47*CLHEP::mg/CLHEP::cm3;
  pressure    = 1.*CLHEP::atmosphere;
  temperature = 293.15*CLHEP::kelvin;
  G4Material* C4H10 = new G4Material(name="C410", density,
                                     nel=2,kStateGas,temperature,
                                     pressure);
  C4H10->AddElement(elC, natoms=4);
  C4H10->AddElement(elH, natoms=10);
  //gMan->SetDrawAtt(C4H10,kBlue-10,0.1);// ND280 class

  // // Ar-CO2
  // density     = 1.958*CLHEP::mg/CLHEP::cm3;
  // G4Material* gasMixtureArCO2
  //   = new G4Material(name="GasMixtureArCO2", density, ncomponents=2);
  // gasMixtureArCO2->AddMaterial(argon, fractionmass = 90*CLHEP::perCent);
  // gasMixtureArCO2->AddMaterial(CO2, fractionmass = 10*CLHEP::perCent);
  // gMan->SetDrawAtt(gasMixtureArCO2,kBlue-10,0.1);// ND280 class

  // TPC Gas
#ifdef OLD_TPC_DENSITY
  density     = 1.485*CLHEP::mg/CLHEP::cm3;
#else
  // Gas density calculated by R. Openshaw [See bug 402 for a full
  // discussion].  The density is calculated for a Ar/CF4/iBu 95:03:02
  // mixture at Normal Pressure and Temperature (NTP).
  density     = 1.738*CLHEP::mg/CLHEP::cm3;
#endif
  G4Material* gasMixtureTPC
    = new G4Material(name="GasMixtureTPC", density, ncomponents=3);
  gasMixtureTPC->AddMaterial(argon, fractionmass = 95*CLHEP::perCent);
  gasMixtureTPC->AddMaterial(CF4, fractionmass = 3*CLHEP::perCent);
  gasMixtureTPC->AddMaterial(C4H10, fractionmass = 2*CLHEP::perCent);
  //gMan->SetDrawAtt(gasMixtureTPC,kBlue-10,0.1);// ND280 class

  // Water-SSteel-Air Mixture
  density     = 2.646*CLHEP::g/CLHEP::cm3;
  G4Material* waterSystem
    = new G4Material(name="WaterSystem", density, ncomponents=3);
  waterSystem->AddMaterial(water, fractionmass = 18*CLHEP::perCent);
  waterSystem->AddMaterial(steel, fractionmass = 32*CLHEP::perCent);
  waterSystem->AddMaterial(CO2, fractionmass = 50*CLHEP::perCent);
  //gMan->SetDrawAtt(waterSystem,kBlue-7);// ND280 class
    
  // add TPC field cage mixture NB rough guesses !!!!
  density = 0.221*CLHEP::g/CLHEP::cm3; // this gives 1.4 10-2 X0 for 2.2 cm
  a = 16.*CLHEP::g/CLHEP::mole;
  G4Material* tpcFieldCage 
    = new G4Material(name="TPCWallMaterial",8,a,density);
  //gMan->SetDrawAtt(tpcFieldCage,kYellow-7);// ND280 class
    
  // Titanium Dioxide -- Used in coextruded scintillator.
  density     = 4.26*CLHEP::g/CLHEP::cm3;
  G4Material* TIO2 = new G4Material(name="TIO2", density, nel=2);
  TIO2->AddElement(elTi, natoms=1);
  TIO2->AddElement(elO , natoms=2);
  //gMan->SetDrawAtt(TIO2,kWhite);// ND280 class
    
  // Polystyrene -- This is polystyrene defined in the PDG C6H5CH=CH2 (this
  // is a net C8H8)
  density = 1.050*CLHEP::g/CLHEP::cm3; // the density of the "extruded polystyrene"
  G4Material* polystyrene 
    = new G4Material(name="Polystyrene", density, nel=2);
  polystyrene->AddElement(elC, 8);
  polystyrene->AddElement(elH, 8);
  //gMan->SetDrawAtt(polystyrene,kGray+3);// ND280 class
    
  // Scintillator -- This is the average polystyrene plastic scintillator as
  // defined in the PDG C6H5CH=CH2 (this is a net C8H8).  The SMRD and ECal
  // geometries have been tuned to this value.  The 1.05 value represents a
  // typical extruded polystyrene.
  density = 1.050*CLHEP::g/CLHEP::cm3; 
  G4Material* scintillator 
    = new G4Material(name="Scintillator", density, nel=2);
  scintillator->AddElement(elC, 8);
  scintillator->AddElement(elH, 8);
  //gMan->SetDrawAtt(scintillator,kGreen);// ND280 class

  // P0D Scintillator -- This has the average polystyrene plastic
  // scintillator composition as defined in the PDG C6H5CH=CH2 (this is a
  // net C8H8).  The density of the P0D scintillator is taken from the assay
  // of the MINERvA scintillator which is manufactured on the same equipment
  // as the P0D scintillator and is chemically the same.  The measured
  // density of the MINERvA strips are 1.058 +- 0.003, including the Ti02
  // coating.  This corresponds to a scintillator density of 1.0506, which
  // has been rounded to 1.051
  density = 1.051*CLHEP::g/CLHEP::cm3; 
  G4Material* p0dScintillator 
    = new G4Material(name="P0DScintillator", density, nel=2);
  p0dScintillator->AddElement(elC, 8);
  p0dScintillator->AddElement(elH, 8);
  //gMan->SetDrawAtt(p0dScintillator,kGreen);// ND280 class

  // FGD Scintillator -- This is the average polystyrene scintillator for
  // the FGD.  The FGD density is based on our measurements of individual
  // bars, in combination with the measurements of the cross-sectional area
  // of the bars.
  density = 1.032*CLHEP::g/CLHEP::cm3; 
  G4Material* fgdScintillator 
    = new G4Material(name="FGDScintillator", density, nel=2);
  fgdScintillator->AddElement(elC, 8);
  fgdScintillator->AddElement(elH, 8);
  //gMan->SetDrawAtt(fgdScintillator,kGreen);// ND280 class

  // Scintillator coating.  This is the coating that goes around the average
  // plastic scintillator. It is 15% TiO2 and 85% polystyrene by weight.
  // rho = (m_Sc + m_Ti) / (V_Sc + V_Ti)
  //     = (0.85 + 0.15) / ( 0.85/1.032 + 0.15/4.26 )
  //     = 1.164 g/cm^3
  density = 1.164*CLHEP::g/CLHEP::cm3;
  G4Material* scintillatorCoating
    = new G4Material(name="ScintillatorCoating", density, ncomponents=2);
  scintillatorCoating->AddMaterial(TIO2        ,fractionmass = 15*CLHEP::perCent);
  scintillatorCoating->AddMaterial(scintillator,fractionmass = 85*CLHEP::perCent);
  //gMan->SetDrawAtt(scintillatorCoating,kGreen);// ND280 class
    
  // PVC -- Polyvinyl Chloride CH2=CHCl = C3H3Cl
  density = 1.38*CLHEP::g/CLHEP::cm3;
  G4Material* pvc
    = new G4Material(name="PVC", density, nel=3);
  pvc->AddElement(elC, 3);
  pvc->AddElement(elH, 3);
  pvc->AddElement(elCl, 1);
  //gMan->SetDrawAtt(pvc,kGray+1);// ND280 class

  // HDPE -- High Density Polyethylene used in P0D water bag
  density = 0.94*CLHEP::g/CLHEP::cm3;
  G4Material* hdpe
    = new G4Material(name="HDPE", density, nel=2);
  hdpe->AddElement(elC, natoms=1);
  hdpe->AddElement(elH, natoms=2);
  //gMan->SetDrawAtt(hdpe,kCyan-10);// ND280 class
    
  // P0DuleEpoxy -- The Epoxy used to hold the P0Dule together.  Density is
  // from the Henkel Hysol data sheet.
  density = 1.36*CLHEP::g/CLHEP::cm3;
  G4Material* p0duleEpoxy
    = new G4Material(name="P0DuleEpoxy", density, nel=3);
  p0duleEpoxy->AddElement(elC, 3);
  p0duleEpoxy->AddElement(elH, 3);
  p0duleEpoxy->AddElement(elCl, 1);
  //gMan->SetDrawAtt(p0duleEpoxy,kBlue+4);// ND280 class
    
  // // FGD Glue
  // density = 1.365*CLHEP::g/CLHEP::cm3;
  // G4Material* fgdGlue
  //   = new G4Material(name="FGDGlue", density, nel=4);
  // fgdGlue->AddElement(elO,fractionmass=62.5*CLHEP::perCent);
  // fgdGlue->AddElement(elC,fractionmass=27.9*CLHEP::perCent);
  // fgdGlue->AddElement(elH,fractionmass= 8.4*CLHEP::perCent);
  // fgdGlue->AddElement(elN,fractionmass= 1.2*CLHEP::perCent);
  // gMan->SetDrawAtt(fgdGlue,kOrange);// ND280 class
    
  // // FGD Water Module Epoxy
  // density = 0.6573*CLHEP::g/CLHEP::cm3;
  // G4Material* fgdWaterModuleEpoxy
  //   = new G4Material(name="FGDWaterModuleEpoxy", density, nel=4);
  // fgdWaterModuleEpoxy->AddElement(elO,fractionmass=62.5*CLHEP::perCent);
  // fgdWaterModuleEpoxy->AddElement(elC,fractionmass=27.9*CLHEP::perCent);
  // fgdWaterModuleEpoxy->AddElement(elH,fractionmass= 8.4*CLHEP::perCent);
  // fgdWaterModuleEpoxy->AddElement(elN,fractionmass= 1.2*CLHEP::perCent);
  // gMan->SetDrawAtt(fgdWaterModuleEpoxy,kOrange);// ND280 class
     
    
  // // Polypropylene
  // density = 0.9*CLHEP::g/CLHEP::cm3;
  // G4Material* polypropylene
  //   = new G4Material(name="Polypropylene", density, nel=2);
  // polypropylene->AddElement(elC, 3);
  // polypropylene->AddElement(elH, 6);
  // gMan->SetDrawAtt(polypropylene,kYellow-5);// ND280 class

  // Polycarbonate
  density = 1.2*CLHEP::g/CLHEP::cm3;
  G4Material* polycarbonate
    = new G4Material(name="Polycarbonate", density, nel=3);
  polycarbonate->AddElement(elH,fractionmass= 5.5491*CLHEP::perCent);
  polycarbonate->AddElement(elC,fractionmass=75.5751*CLHEP::perCent);
  polycarbonate->AddElement(elO,fractionmass=18.8758*CLHEP::perCent);
  //gMan->SetDrawAtt(polycarbonate,kYellow-2);// ND280 class

  //carbon fibre
  // NB : chemical composition may not be completely right but  close 
  density = 1.75*CLHEP::g/CLHEP::cm3;
  G4Material* carbonFibre
    = new G4Material(name="CarbonFibre", density, nel=2);
  carbonFibre->AddElement(elC,6);
  carbonFibre->AddElement(elO,1);
  //gMan->SetDrawAtt(carbonFibre,kGray+3);// ND280 class

  // G10 - by volume 57% glass, 43% epoxy (CH2)
  density = 1.70*CLHEP::g/CLHEP::cm3;
  G4Material* g10
    = new G4Material(name="G10", density, nel=6);
  g10->AddElement(elH,6.2*CLHEP::perCent);
  g10->AddElement(elC,36.8*CLHEP::perCent);
  g10->AddElement(elO,30.7*CLHEP::perCent);
  g10->AddElement(elSi,21.9*CLHEP::perCent);
  g10->AddElement(elB,2.2*CLHEP::perCent);
  g10->AddElement(elNa,2.2*CLHEP::perCent);
  //gMan->SetDrawAtt(g10,kGreen+3);// ND280 class

  // Diluted G10-FGD1
  density = 0.365*CLHEP::g/CLHEP::cm3;
  G4Material* g10fgd1
    = new G4Material(name="G10FGD1", density, nel=6);
  g10fgd1->AddElement(elH,6.2*CLHEP::perCent);
  g10fgd1->AddElement(elC,36.8*CLHEP::perCent);
  g10fgd1->AddElement(elO,30.7*CLHEP::perCent);
  g10fgd1->AddElement(elSi,21.9*CLHEP::perCent);
  g10fgd1->AddElement(elB,2.2*CLHEP::perCent);
  g10fgd1->AddElement(elNa,2.2*CLHEP::perCent);
  //gMan->SetDrawAtt(g10fgd1,kGreen+3);// ND280 class

  // // Diluted G10-FGD2
  // density = 0.171*CLHEP::g/CLHEP::cm3;
  // G4Material* g10fgd2
  //   = new G4Material(name="G10FGD2", density, nel=6);
  // g10fgd2->AddElement(elH,6.2*CLHEP::perCent);
  // g10fgd2->AddElement(elC,36.8*CLHEP::perCent);
  // g10fgd2->AddElement(elO,30.7*CLHEP::perCent);
  // g10fgd2->AddElement(elSi,21.9*CLHEP::perCent);
  // g10fgd2->AddElement(elB,2.2*CLHEP::perCent);
  // g10fgd2->AddElement(elNa,2.2*CLHEP::perCent);
  // gMan->SetDrawAtt(g10fgd2,kGreen+3);// ND280 class

  // Rohacell (polymethacrylimide, chemical formula to be confirmed from 
  // "Polymethyl methacrylate" C5O2H8)
  density = 0.052*CLHEP::g/CLHEP::cm3;
  G4Material* rohacell 
    = new G4Material(name="Rohacell", density, nel=3);
  // Rohacell C:H:O=5:8:2
  rohacell->AddElement(elC,5);
  rohacell->AddElement(elH,8);
  rohacell->AddElement(elO,2);
  //gMan->SetDrawAtt(rohacell,kGreen-9);// ND280 class
    
  // add Al/Rohacell TPC cage wall material
  // Outer box 15.2mm Al / Roha Panels (2 x 0.76mm Al, and 13.68mm Rohacell)
  // rho = (m_Al + m_Roha)/(V_Al + V_Roha)
  //     = (rho_Al * A*1.52mm + rho_Roha * A*13.68 mm)/(A*1.52mm + A*13.68mm)
  //     = (2.7g/cm3 * 1.52mm + 0.052g/cm3 * 13.68 mm) / 15.2mm
  //     = 0.3168 g/cm3

  density = 0.3168*CLHEP::g/CLHEP::cm3;

  G4Material* alroha
    = new G4Material(name="AlRoha", density, ncomponents=2);

  // Fraction by weight:
  //  w_Al = m_Al / m_total
  //       = (rho_Al*A*1.52mm) / (rho*A*15.2mm)
  //       = (2.7g/cm3*1.52mm) / (0.3168g/cm3*15.2mm)
  //       = 0.8523
  //  w_Ro = m_Roha / m_total
  //       = (rho_Roha*A*1.52mm) / (rho*A*15.2mm)
  //       = (0.052g/cm3*13.68mm) / (0.3168g/cm3*15.2mm)
  //       = 0.1477
  alroha->AddMaterial(aluminum, fractionmass = 85.23*CLHEP::perCent);
  alroha->AddMaterial(rohacell, fractionmass = 14.77*CLHEP::perCent);
  //gMan->SetDrawAtt(alroha,kGreen-9);// ND280 class

  // add Al/Rohacell TPC cage wall material
  // (same as above but only 13.2mm thick)
  density = 0.3569*CLHEP::g/CLHEP::cm3;

  G4Material* alroha2
    = new G4Material(name="AlRoha2", density, ncomponents=2);

  alroha2->AddMaterial(aluminum, fractionmass = 87.11*CLHEP::perCent);
  alroha2->AddMaterial(rohacell, fractionmass = 12.89*CLHEP::perCent);
  //gMan->SetDrawAtt(alroha2,kGreen-9);// ND280 class

  // G10/Roha/Cu TPC cage wall material
  //
  // Inner box 13.2mm G10 / Roha / Cu Panels (2 x 0.76mm G10, 11.68mm
  // Rohacell, and 2* 0.035mm Cu)
  //
  // Density:
  // rho = ( m_G10 + m_Roha + m_Cu ) / ( V_G10 + V_Roha +V_Cu )
  //    = ( rho_G10*A*1.52mm + rho_Roha*A*11.68mm + rho_Cu*A*0.07mm ) 
  //           / ( A*1.52mm +A*11.68mm+A*0.07mm )
  //    = ( 1.7g/cm^3*1.52mm + 0.052g/cm^3*11.68mm + 8.94g/cm^3*0.07mm) 
  //                    / 13.27mm
  //    = 0.2877 g/cm^3
  density = 0.2877*CLHEP::g/CLHEP::cm3;
  G4Material* g10roha
    = new G4Material(name="G10Roha", density, ncomponents=3);

  //Fraction by weight:
  //  w_G10 = m_G10 / m_total
  //       = (rho_G10*A*1.52mm) / (rho*A*13.27mm)
  //       = (1.7g/cm^3*1.52mm) / (0.2877g/cm^3*13.27mm)
  //       = 0.6769
  //  w_Ro = m_Roha / m_total
  //       = (rho_Roha*A*11.68mm) / (rho*A*13.27mm)
  //       = (0.052g/cm^3*11.68mm) / (0.2877g/cm^3*13.27mm)
  //       = 0.1591
  //  w_Cu = m_Cu / m_total
  //       = (rho_Cu*A*0.07mm) / (rho*A*13.27mm)
  //       = (8.94g/cm^3*0.07mm) / (0.2877g/cm^3*13.27mm)
  //       = 0.1640
  g10roha->AddMaterial(g10, fractionmass = 67.69*CLHEP::perCent);  
  g10roha->AddMaterial(rohacell, fractionmass = 15.91*CLHEP::perCent);
  g10roha->AddMaterial(copper, fractionmass = 16.40*CLHEP::perCent);
  //gMan->SetDrawAtt(g10roha,kGreen-9);// ND280 class

  // FGD Electronics card (95mm x 252mm x 8mm) with total mass of 400g. The
  // composition is 50% Al and 50% G10 by weight.  -> density =
  // 400g/191.52cm3
  density = 2.0885*CLHEP::g/CLHEP::cm3;

  G4Material* alg10
    = new G4Material(name="AlG10", density, ncomponents=2);

  alg10->AddMaterial(aluminum, fractionmass = 50.0*CLHEP::perCent);
  alg10->AddMaterial(g10, fractionmass = 50.0*CLHEP::perCent);
  //gMan->SetDrawAtt(alg10,kGreen-9);// ND280 class

  // Material for the fiber core.
  density = 1.05*CLHEP::g/CLHEP::cm3;
  G4Material* fiberCore
    = new G4Material(name="FiberCore", density, nel=2);
  fiberCore->AddElement(elC, 9);
  fiberCore->AddElement(elH, 10);
  //gMan->SetDrawAtt(fiberCore,kCyan-4);// ND280 class
    
  // Material for the fiber cladding
  density = 1.19*CLHEP::g/CLHEP::cm3;
  G4Material* fiberCladding
    = new G4Material(name="FiberCladding", density, nel=3);
  fiberCladding->AddElement(elH,8);
  fiberCladding->AddElement(elC,5);
  fiberCladding->AddElement(elO,2);
  //gMan->SetDrawAtt(fiberCladding,kBlue);// ND280 class

  // Fluorinated fiber cladding
  density = 1.43*CLHEP::g/CLHEP::cm3;
  G4Material* fiberCladdingFluor
    = new G4Material(name="FiberCladdingFluor", density, nel=2);
  fiberCladdingFluor->AddElement(elH,4);
  fiberCladdingFluor->AddElement(elC,2);
  //gMan->SetDrawAtt(fiberCladdingFluor,kBlue);// ND280 class
    
  // ActiveWater for the FGD.
  //
  // It is not necessarily activated, it is just a combination of water and
  // polycarbonate that holds the water in place.  Daniel's document says
  // the panel is 342mg/cm2 of PC (for most of the panels), and 2.2226 g/cm2
  // of water (average of the 6 deployed panels).  water+PC layer by mass is
  // 0.342/(0.342+2.2226)=0.1335 polycarbonate, so rho = 1 / (0.1335/1.2 +
  // 0.8666/1.0) = 1.0226 g/cm^3
  density = 1.0226*CLHEP::g/CLHEP::cm3;
  G4Material* activeWater 
    = new G4Material("ActiveWater", density, ncomponents=2);
  activeWater->AddMaterial(water        ,fractionmass = 86.672*CLHEP::perCent);
  activeWater->AddMaterial(polycarbonate,fractionmass = 13.328*CLHEP::perCent);
  //gMan->SetDrawAtt(activeWater,kAzure+8);// ND280 class

  // Uniform material for the FGD2 
  // 
  // 7 layers of 2.02cm of FGDScintillator + 6 layers of 2.5cm of ActiveWater
  // density for ActiveWater = 1.0226*g/cm3
  // density for FGDScintillator = 1.032*g/cm3
  // length ActiveWater = 6 (layers) * 2.5cm (thickness) (NIM-FGD)
  // length FGDScintillator = 7 (layers) * 2.02cm (thickness) (NIM-FGD)
  // rho = (1.0226*6*2.5 + 1.032*7*2.02) / (6*2.5 + 7*2.02) = 1.027 g/cm3
  // fractionmass (ActiveWater) = 51.247%
  // fractionmass (FGDScintillator) = 48.753%
  density = 1.027*CLHEP::g/CLHEP::cm3;
  G4Material *FGD2Uniform 
    = new G4Material("FGD2Uniform",  density, ncomponents=2);
  FGD2Uniform->AddMaterial(activeWater,    fractionmass = 51.247*CLHEP::perCent);
  FGD2Uniform->AddMaterial(fgdScintillator,fractionmass = 48.753*CLHEP::perCent);
  //gMan->SetDrawAtt(FGD2Uniform,kAzure+8);
  
  // WAGASCI
  
  //Scintillator // B.Q
  density = 1.032*CLHEP::g/CLHEP::cm3;
  G4Material *WAGASCIScint 
    = new G4Material(name="WAGASCIScintillator", density, nel=2);
  WAGASCIScint->AddElement(elC, 8);
  WAGASCIScint->AddElement(elH, 8);
  //gMan->SetDrawAtt(WAGASCIScint,kAzure+8);//ND280 class

  //Scintillator empty --> average density ~40% of normal one
  density = 1.032*CLHEP::g/CLHEP::cm3 * 0.3;
  G4Material *WAGASCIScint_Empty 
    = new G4Material(name="WAGASCIScintillatorEmpty", density,nel=2);
  WAGASCIScint_Empty->AddElement(elC, 9);
  WAGASCIScint_Empty->AddElement(elH, 10);
  //gMan->SetDrawAtt(WAGASCIScint_Empty,kGreen);//ND280 class
  
}

G4Material* ND280SuperFGDConstructor::FindMaterial(G4String name)
{
    G4Material* material = G4Material::GetMaterial(name,true);
    if(material == NULL || material == 0 || material == nullptr)
    {
        std::string errMsg = "Material is undefined: " + name;
        THROW(errMsg);
    }
    return material;
}

}   //superfgd
}   //simulators
}   //esbroot