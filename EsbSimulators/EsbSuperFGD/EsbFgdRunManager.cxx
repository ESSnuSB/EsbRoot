#include "EsbSimulators/EsbSuperFGD/EsbFgdRunManager.h"

namespace esbroot {
namespace simulators {
namespace superfgd {

FgdRunManager::FgdRunManager():G4RunManager(),ffileWriter(nullptr),fignoreEdep(false)
{
}

FgdRunManager::~FgdRunManager()
{
}

}   //superfgd
}   //simulators
}   //esbroot