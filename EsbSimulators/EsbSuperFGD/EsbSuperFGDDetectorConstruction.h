#ifndef ESBROOT_SIMULATION_SUPERFGD_FGD_DETECTOR_CONSTRUCTION_H
#define ESBROOT_SIMULATION_SUPERFGD_FGD_DETECTOR_CONSTRUCTION_H 1

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

#include "EsbData/EsbSuperFGD/EsbDetector/Materials.h"
#include "EsbData/EsbSuperFGD/EsbDetector/EsbFgdDetectorParameters.h"

#include "EsbSimulators/EsbSuperFGD/EsbDetectorConstruction.h"

#include <G4SDManager.hh>
#include <G4Tubs.hh>
#include "G4VUserDetectorConstruction.hh"
#include "G4ParticleGun.hh"
#include "G4NistManager.hh"
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include "globals.hh"

class G4VPhysicalVolume;
class FgdDetectorParameters;

namespace esbroot {
namespace simulators {
namespace superfgd {

class SuperFGDDetectorConstruction : public  FgdDetectorConstruction
{
public:

    /** Default Constructor **/
    SuperFGDDetectorConstruction();

    /** Constructor
     *@param detectorFile - name (fullpath) of file from which to read the detector parameters
     **/
    SuperFGDDetectorConstruction(std::string detectorFile);

    /** Destructor **/
    virtual ~SuperFGDDetectorConstruction();

public:
    /** Constructs the Geant4 physical volume of the detector**/
    virtual G4VPhysicalVolume* Construct();
};

}   //superfgd
}   //simulators
}   //esbroot


#endif