#ifndef ESBROOT_SIMULATION_SUPERFGD_PHYSICS_LIST_H
#define ESBROOT_SIMULATION_SUPERFGD_PHYSICS_LIST 1

#include "G4VUserPhysicsList.hh"
#include "G4ParallelWorldScoringProcess.hh"
#include "globals.hh"

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpMieHG;
class G4OpBoundaryProcess;

namespace esbroot {
namespace simulators {
namespace superfgd {

class PhysicsList: public G4VUserPhysicsList
{
  public:
    /** Default constructor **/
    PhysicsList();

    /** Destructor **/
    ~PhysicsList();

    /** is physics list already defined **/
    bool IsDefined(G4int pid);

    /** Set verbosity level
    *@param verbose - verbosity level
    **/
    void SetVerbose(G4int verbose);

  protected:
    // Construct particle and physics process
    void ConstructParticle();
    void ConstructProcess();
    void SetCuts();

    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();
    void ConstructIons();

    void ConstructEM();
    void ConstructGeneral();
    void ConstructOp();

    G4Cerenkov*          theCerenkovProcess;
    G4Scintillation*     theScintillationProcess;
    G4OpAbsorption*      theAbsorptionProcess;
    G4OpRayleigh*        theRayleighScatteringProcess;
    G4OpMieHG*           theMieHGScatteringProcess;
    G4OpBoundaryProcess* theBoundaryProcess;

    void AddVirtualScoringProcess();
};

}   //superfgd
}   //simulators
}   //esbroot

#endif







