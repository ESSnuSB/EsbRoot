#ifndef ESBROOT_SIMULATION_SUPERFGD_DETECTOR_CONSTRUCTION_H
#define ESBROOT_SIMULATION_SUPERFGD_DETECTOR_CONSTRUCTION_H 1

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

#include "EsbData/EsbSuperFGD/EsbDetector/EsbFgdDetectorParameters.h"
#include "EsbData/EsbSuperFGD/EsbDetector/Materials.h"

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

class FgdDetectorConstruction : public  G4VUserDetectorConstruction
{
public:
    /** Default constructor **/
    FgdDetectorConstruction();
    
    /** Constructor from external file [full path of file] **/
    FgdDetectorConstruction(std::string detectorFile);

    /** Destructor **/
    virtual ~FgdDetectorConstruction();

public:
    /** Construct the Detector **/
    virtual G4VPhysicalVolume* Construct();

    /** Get the currently set detector parameters **/
    data::superfgd::detector::FgdDetectorParameters& GetDetectorParams(){return fdetector;}

protected:
    data::superfgd::detector::FgdDetectorParameters fdetector;
    void AddScintillatonProperties(G4Material* scintillator);
};

}   //superfgd
}   //simulators
}   //esbroot

#endif