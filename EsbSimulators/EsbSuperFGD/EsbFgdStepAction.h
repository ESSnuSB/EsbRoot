#ifndef ESBROOT_SIMULATION_SUPERFGD_STEP_ACTION_H
#define ESBROOT_SIMULATION_SUPERFGD_STEP_ACTION_H 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "G4Step.hh"

namespace esbroot {
namespace simulators {
namespace superfgd {

class FgdStepAction : public G4UserSteppingAction
{
public:
    /** Default constructor **/
    FgdStepAction();

    /** Destructor **/
    virtual ~FgdStepAction();

    /** Static access method **/
    static FgdStepAction* Instance();

    /** Method from the base class **/
    virtual void UserSteppingAction(const G4Step*);

    /** Set the logical volume for this step
     *@param volume - step logical volume
     **/
    void SetVolume(G4LogicalVolume* volume) { ffVolume = volume; }

    /** Reset accumulated energy **/
    void Reset();
  
    /** Get the currently set logical volume **/
    G4LogicalVolume* GetVolume() const { return ffVolume; }

    /** Get the currently total energy **/
    G4double GetEnergy() const { return ffEnergy; }

private:

    static FgdStepAction* ffgInstance;  

    G4LogicalVolume* ffVolume;
    G4double  ffEnergy;
};

}   //superfgd
}   //simulators
}   //esbroot


#endif