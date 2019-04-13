#ifndef ESBROOT_SIMULATION_SUPERFGD_SIMULATOR_H
#define ESBROOT_SIMULATION_SUPERFGD_SIMULATOR_H 1


#include <iostream>
#include <fstream>
#include <exception>
#include <map>

#include "EsbSimulators/EsbSuperFGD/EsbDetectorConstruction.h"
#include "EsbSimulators/EsbSuperFGD/EsbSuperFGDDetectorConstruction.h"
#include "EsbSimulators/EsbSuperFGD/EsbPrimaryGeneratorAction.h"
#include "EsbSimulators/EsbSuperFGD/EsbParticleGunPrimaryGenerator.h"
#include "EsbSimulators/EsbSuperFGD/EsbPhysicsList.h"
#include "EsbSimulators/EsbSuperFGD/EsbFgdStepAction.h"
#include "EsbSimulators/EsbSuperFGD/EsbRunAction.h"
#include "EsbSimulators/EsbSuperFGD/EsbEventAction.h"
#include "EsbSimulators/EsbSuperFGD/EsbFgdRunManager.h"
#include "EsbSimulators/EsbSuperFGD/EsbFileWriter.h"

#include "EsbTools/EsbDefines.h"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include <G4UItcsh.hh>
#include <G4UIterminal.hh>

#ifdef G4VIS_USE
    #include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
    #include "G4UIExecutive.hh"
#endif

#include "Randomize.hh"

class G4VUserPrimaryGeneratorAction;

namespace esbroot {
namespace simulators {

class EsbSuperFGDSimulator 
{
    /** Default constructor **/
    EsbSuperFGDSimulator();

    /** Destructor **/
    ~EsbSuperFGDSimulator();


};

}   //simulators
}   //esbroot

#endif