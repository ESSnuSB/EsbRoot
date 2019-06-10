#ifndef ESBROOT_ESBGEOMETRY_SUPERFGD_FGD_DETECTOR_CONSTRUCTION_H
#define ESBROOT_ESBGEOMETRY_SUPERFGD_FGD_DETECTOR_CONSTRUCTION_H 1

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

#include "EsbGeometry/EsbSuperFGD/Materials.h"
#include "EsbGeometry/EsbSuperFGD/EsbFgdDetectorParameters.h"

#include "TObject.h"

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
    SuperFGDDetectorConstruction(std::string detectorFile, double posX, double posY, double posZ);

    /** Destructor **/
    virtual ~SuperFGDDetectorConstruction();

    /** Constructs the Geant4 physical volume of the detector**/
    virtual void Construct();

private:

    /** Class to hold the Detector parameters read from external file **/
    FgdDetectorParameters fdetector;

    /** Detector position **/
    double fposition_X;
    double fposition_Y;
    double fposition_Z;

    ClassDef(SuperFGDDetectorConstruction,2)
};

}   //superfgd
}   //geometry
}   //esbroot


#endif