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
    SuperFGDDetectorConstruction(std::string detectorFile);

    /** Destructor **/
    virtual ~SuperFGDDetectorConstruction();

    /** Return a Root volume **/
    TGeoVolume* GetPiece(void);

private:

    /** Constructs the TgeoVolume of the detector**/
    void Construct();

    /** Class to hold the Detector parameters read from external file **/
    FgdDetectorParameters fParams;

    /**  The pointer to the root volume **/
    TGeoVolume* fVolume;

    ClassDef(SuperFGDDetectorConstruction,2)
};

}   //superfgd
}   //geometry
}   //esbroot


#endif