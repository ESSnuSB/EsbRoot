#ifndef SIMULATION_NFRUN_ACTION_H
#define SIMULATION_NFRUN_ACTION_H 1

#include <G4UserRunAction.hh>
#include <globals.hh>

class G4Run;

namespace esbroot {
namespace simulators {
namespace superfgd {

class FgdRunAction : public G4UserRunAction
{
public:

    /** Default constructor **/
    FgdRunAction();

    /** Destructor **/
    virtual ~FgdRunAction();

    /** Base method to begin the run action
     *@param G4Run - current run
     **/
    virtual void BeginOfRunAction(const G4Run*);

    /** Base method to end the run action
     *@param G4Run - current run
     **/
    virtual void   EndOfRunAction(const G4Run*);
};

}   //superfgd
}   //simulators
}   //esbroot

#endif