#include "EsbSimulators/EsbSuperFGD/EsbFgdStepAction.h"


namespace esbroot {
namespace simulators {
namespace superfgd {

FgdStepAction::FgdStepAction()
: G4UserSteppingAction(),
  ffVolume(0),
  ffEnergy(0.)
{ 
    ffgInstance = this;
}

FgdStepAction::~FgdStepAction()
{
    ffgInstance = 0;
}

FgdStepAction* FgdStepAction::ffgInstance = 0;

FgdStepAction* FgdStepAction::Instance()
{
    // Static acces function via G4RunManager 
    if(!ffgInstance)
    {
        return ffgInstance;
    }else
    {
        return nullptr;
    }
        
}

void FgdStepAction::UserSteppingAction(const G4Step* step)
{
  // get volume of the current step
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
      
  // check if we are in scoring volume
  if (volume != ffVolume ) return;

  // collect energy and track length step by step
  G4double edep = step->GetTotalEnergyDeposit();
  ffEnergy += edep;
}

void FgdStepAction::Reset()
{
  ffEnergy = 0.;
}

}   //superfgd
}   //simulators
}   //esbroot