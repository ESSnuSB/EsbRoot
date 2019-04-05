#include "EsbSimulators/EsbSuperFGD/EsbEventAction.h"
#include "EsbSimulators/EsbSuperFGD/EsbFileWriter.h"
#include "EsbSimulators/EsbSuperFGD/EsbFgdRunManager.h"

#include "EsbSimulators/EsbSuperFGD/EsbPrimaryGeneratorAction.h"
#include "EsbSimulators/EsbSuperFGD/EsbParticleGunPrimaryGenerator.h"

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
    
    PrimaryGeneratorAction* nfPrga = dynamic_cast<PrimaryGeneratorAction*>(prGA);
    ParticleGunPrimaryGenerator* nfPrGga = dynamic_cast<ParticleGunPrimaryGenerator*>(prGA);

    shared_ptr<FileWriter> writer = ((FgdRunManager*)G4RunManager::GetRunManager())->GetFileWriter();
    writer->WriteHit();

    if(nfPrga!=nullptr && !nfPrga->HasMoreEvents()
        || nfPrGga!=nullptr && !nfPrGga->HasMoreEvents())
    {
        G4cout << "FgdEventAction: No more events, aborting run ..." << G4endl;
        runmanager->AbortRun();
    }
}

}   //superfgd
}   //simulators
}   //esbroot