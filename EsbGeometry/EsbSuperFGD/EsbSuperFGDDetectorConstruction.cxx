#include "EsbGeometry/EsbSuperFGD/EsbSuperFGDDetectorConstruction.h"
#include "EsbGeometry/EsbSuperFGD/EsbSuperFGDConstructor.h"
#include "EsbGeometry/EsbSuperFGD/Names.h"

#include "FairLogger.h"

namespace esbroot {
namespace geometry {
namespace superfgd {


SuperFGDDetectorConstruction::SuperFGDDetectorConstruction(std::string detectorFile)
  : fSuperVol(nullptr), fSensVolume(nullptr)
{
  fParams.LoadPartParams(detectorFile);
}

SuperFGDDetectorConstruction::~SuperFGDDetectorConstruction()
{
}

TGeoVolume* SuperFGDDetectorConstruction::Construct()
{
    SuperFGDConstructor fSuperFGDConstructor;
    fSuperFGDConstructor.SetVisibility(fParams.ParamAsBool(DP::visdetail));

    Double_t lunit = fParams.GetLenghtUnit(); 
    // Detector is made of cubes 
    // length_X == length_Y ==length_Z
    Double_t edge = fParams.ParamAsDouble(DP::length_X) * lunit; 

    Int_t cube_X_N = fParams.ParamAsInt(DP::number_cubes_X);
    Int_t cube_Y_N = fParams.ParamAsInt(DP::number_cubes_Y);
    Int_t cube_Z_N = fParams.ParamAsInt(DP::number_cubes_Z);

    fSuperFGDConstructor.SetEdge(edge);
    fSuperFGDConstructor.SetCubeNumX(cube_X_N);
    fSuperFGDConstructor.SetCubeNumY(cube_Y_N);
    fSuperFGDConstructor.SetCubeNumZ(cube_Z_N);

    fSuperVol = fSuperFGDConstructor.Construct(); 
    fSensVolume = fSuperFGDConstructor.GetSensitiveVolume();
    return fSuperVol;
}

TVector3 SuperFGDDetectorConstruction::GetMagneticField()
{
  Double_t Bx = fParams.ParamAsInt(DP::magField_X);
  Double_t By = fParams.ParamAsInt(DP::magField_Y);
  Double_t Bz = fParams.ParamAsInt(DP::magField_Z);

  TVector3 magField;
  magField.SetX(Bx);
  magField.SetY(By);
  magField.SetZ(Bz);

  return magField;
}

}   //superfgd
}   //geometry
}   //esbroot