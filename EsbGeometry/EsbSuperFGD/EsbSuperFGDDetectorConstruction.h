#ifndef ESBROOT_ESBGEOMETRY_SUPERFGD_FGD_DETECTOR_CONSTRUCTION_H
#define ESBROOT_ESBGEOMETRY_SUPERFGD_FGD_DETECTOR_CONSTRUCTION_H 1

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

#include "EsbGeometry/EsbSuperFGD/Materials.h"
#include "EsbGeometry/EsbSuperFGD/Names.h"
#include "EsbGeometry/EsbSuperFGD/EsbFgdDetectorParameters.h"

#include "TVector3.h"
#include "TObject.h"
#include "TGeoManager.h"

class FgdDetectorParameters;

namespace esbroot {
namespace geometry {
namespace superfgd {

class SuperFGDDetectorConstruction : public TObject
{
public:

    /** Constructor
     *@param detectorFile - name (fullpath) of file from which to read the detector parameters
     **/
    SuperFGDDetectorConstruction(std::string detectorFile);

    /** Destructor **/
    virtual ~SuperFGDDetectorConstruction();

    /** Constructs the TgeoVolume of the detector**/
    void Construct();

    /** Returns the magnetic field read from the file **/ 
    TVector3 GetMagneticField();

    Double_t GetLengthUnit() {return fParams.GetLenghtUnit();}
    Double_t GetEdge() {return fParams.ParamAsDouble(DP::length_X);}

    Int_t GetCubeNX(){return fParams.ParamAsInt(DP::number_cubes_X);}
    Int_t GetCubeNY(){return fParams.ParamAsInt(DP::number_cubes_Y);}
    Int_t GetCubeNZ(){return fParams.ParamAsInt(DP::number_cubes_Z);}

private:
    /** Class to hold the Detector parameters read from external file **/
    FgdDetectorParameters fParams;

    ClassDef(SuperFGDDetectorConstruction,2)
};

}   //superfgd
}   //geometry
}   //esbroot


#endif