#include "EsbSimulators/EsbSuperFGD/EsbEventAction.h"
#include "EsbSimulators/EsbSuperFGD/EsbFileWriter.h"
#include "EsbSimulators/EsbSuperFGD/EsbFgdRunManager.h"

#include "Simulation/include/NFPrimaryGeneratorAction.hh"
#include "Simulation/include/NFParticleGunPrimaryGenerator.hh"

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4RunManager.hh"


namespace esbroot {
namespace simulators {
namespace superfgd {

FgdEventAction::FgdEventAction() 
{
}
 
FgdEventAction::~FgdEventAction() 
{
}
 
void FgdEventAction::BeginOfEventAction(const G4Event* event) 
{
}
 
void FgdEventAction::EndOfEventAction(const G4Event* event)
{
    G4RunManager *runmanager = G4RunManager::GetRunManager();
    G4VUserPrimaryGeneratorAction* prGA = const_cast<G4VUserPrimaryGeneratorAction*>(runmanager->GetUserPrimaryGeneratorAction());
    
    NFPrimaryGeneratorAction* nfPrga = dynamic_cast<NFPrimaryGeneratorAction*>(prGA);
    NFParticleGunPrimaryGenerator* nfPrGga = dynamic_cast<NFParticleGunPrimaryGenerator*>(prGA);

    shared_ptr<FileWriter> writer = ((FgdRunManager*)G4RunManager::GetRunManager())->GetFileWriter();
    writer->writeHit();

    if(nfPrga!=nullptr && !nfPrga->hasMoreEvents()
        || nfPrGga!=nullptr && !nfPrGga->hasMoreEvents())
    {
        G4cout << "FgdEventAction: No more events, aborting run ..." << G4endl;
        runmanager->AbortRun();
    }
}

}   //superfgd
}   //simulators
}   //esbroot