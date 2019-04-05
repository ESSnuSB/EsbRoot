#ifndef ESBROOT_SIMULATION_SUPERFGD_FIBER_SD_H
#define ESBROOT_SIMULATION_SUPERFGD_FIBER_SD_H 1

#include <memory>

#include <G4VSensitiveDetector.hh>

#include "EsbData/EsbSuperFGD/EsbDetector/EsbFiberHit.h"
#include "EsbSimulators/EsbSuperFGD/EsbFileWriter.h"

namespace esbroot {
namespace simulators {
namespace superfgd {

class FiberSD : public G4VSensitiveDetector
{
public:
    /** Default constructor **/
    FiberSD(G4String name);

    /** Destructor **/
    virtual ~FiberSD();

    /** Process the hit from the geant4 simulation in the fiber
     * extacting simulation data
     *@param astep - step in the simulation
     *@param ROHist - geant4 history
     **/
    G4bool ProcessHits(G4Step* astep,G4TouchableHistory* ROHist);

    /** Initialization of Fiber Sensitive Detector
     *@param HCE - configuration parameter (not used)
     **/
    void Initialize(G4HCofThisEvent* HCE);

    /** Set verbosity level
     *@param verbose - true to verbose
     **/
    void SetVerbose(bool verbose) {fverbose = verbose;}

private:
    shared_ptr<data::superfgd::detector::FiberHit> fhitBuffer;
    shared_ptr<FileWriter> fwriter;
    bool fverbose;
};

}   //superfgd
}   //simulators
}   //esbroot

#endif