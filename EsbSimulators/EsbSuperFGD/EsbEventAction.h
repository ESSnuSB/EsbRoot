#ifndef ESBROOT_SIMULATION_SUPERFGD_EVENT_ACTION_H
#define ESBROOT_SIMULATION_SUPERFGD_EVENT_ACTION_H 1

#include <G4Event.hh>
#include <G4UserEventAction.hh>

namespace esbroot {
namespace simulators {
namespace superfgd {

class FgdEventAction : public G4UserEventAction
{
public:
    /** Default constructor **/
    FgdEventAction();

    /** Destructor **/
    virtual ~FgdEventAction();

    /** Base method to begin the event
     *@param event - current event
     **/
    virtual void BeginOfEventAction(const G4Event* event);

    /** Base method to end the event
     *@param event - current event
     **/
    virtual void EndOfEventAction(const G4Event* event);
};

}   //superfgd
}   //simulators
}   //esbroot
    
#endif
