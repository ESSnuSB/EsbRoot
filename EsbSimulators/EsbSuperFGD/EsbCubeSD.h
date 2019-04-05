#ifndef ESBROOT_SIMULATION_SUPERFGD_CUBE_SD_H
#define ESBROOT_SIMULATION_SUPERFGD_CUBE_SD_H 1

#include <memory>

#include <G4VSensitiveDetector.hh>

#include "EsbData/EsbSuperFGD/EsbDetector/EsbCubeHit.h"
#include "EsbData/EsbSuperFGD/EsbDetector/EsbFgdDetectorParameters.h"

#include "EsbSimulators/EsbSuperFGD/EsbFileWriter.h"

namespace esbroot {
namespace simulators {
namespace superfgd {

class CubeSD : public G4VSensitiveDetector
{
public:

    /** Default constructor **/
    CubeSD(G4String name, G4String detector_sd_type);

    /** Destructor **/
    virtual ~CubeSD();

    /** Process the hit from the geant4 simulation in the cube
     * extacting simulation data
     *@param astep - step in the simulation
     *@param ROHist - geant4 history
     **/
    G4bool ProcessHits(G4Step* astep,G4TouchableHistory* ROHist);

    /** Initialization of Cube Sensitive Detector
     *@param HCE - configuration parameter (not used)
     **/
    void    Initialize(G4HCofThisEvent* HCE);

    /** Set verbosity level
     *@param verbose - true to verbose
     **/
    void SetVerbose(bool verbose) {fverbose = verbose;}

private:

    G4bool ProcessTotalHits(G4Step* aStep,G4TouchableHistory* ROHist);

    bool fverbose;
    G4String fdetector_sd_type;

    shared_ptr<data::superfgd::detector::CubeHit>  fhitBuffer;
    shared_ptr<FileWriter> fwriter;
};

}   //superfgd
}   //simulators
}   //esbroot

#endif