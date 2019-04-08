#include "EsbSimulators/EsbSuperFGD/EsbFiberSD.h"
#include "EsbSimulators/EsbSuperFGD/EsbFgdRunManager.h"

namespace esbroot {
namespace simulators {
namespace superfgd {

FiberSD::FiberSD(G4String name)
    :G4VSensitiveDetector(name), fverbose(false)
{
    fhitBuffer = make_shared<data::superfgd::detector::FiberHit>(); 
}

FiberSD::~FiberSD()
{
}

void  FiberSD::Initialize(G4HCofThisEvent* HCE)
{
}

G4bool FiberSD::ProcessHits(G4Step* aStep,G4TouchableHistory* ROHist)
{
    if(fverbose)
        cout << "FiberSD::ProcessHits " << endl;
        
    G4double edep = aStep->GetTotalEnergyDeposit();
    if (edep == 0.)
        return false;

    G4ThreeVector hitPosition   = aStep->GetPreStepPoint()->GetPosition();
    G4double      kineticEnergy = aStep->GetTrack()->GetKineticEnergy();
    G4ThreeVector momentum = aStep->GetPreStepPoint()->GetMomentum();
    
    G4int fiberCopyNo = aStep->GetPreStepPoint()->GetTouchable()->GetVolume(0)->GetCopyNo();
    G4int slabCopyNo = aStep->GetPreStepPoint()->GetTouchable()->GetVolume(1)->GetCopyNo();

    double time = aStep->GetPreStepPoint()->GetGlobalTime();

    G4Track* track = aStep->GetTrack();
    int trackID = track->GetTrackID();
    int parentID = track->GetParentID();
    int pdg = track->GetDefinition()->GetPDGEncoding();

    FgdRunManager* man = dynamic_cast<FgdRunManager*>(G4RunManager::GetRunManager());
    if(man!=nullptr)
    {
        fwriter = man->GetFileWriter();
    }
        
    if(fwriter!=nullptr)
    {
        if(fverbose)
             cout<<" ======  FiberSD:fverbose ====="<< __FILE__ << " " << __LINE__ << endl;

        fhitBuffer->SetEdep(edep);

        fhitBuffer->SetHitPostion(hitPosition.x(), hitPosition.y(), hitPosition.z());
        fhitBuffer->SetKinEnergy(kineticEnergy);
        fhitBuffer->SetHitMomentum(momentum.x(), momentum.y(), momentum.z());

        fhitBuffer->SetfiberCopyNo(fiberCopyNo);
        fhitBuffer->SetslabCopyNo(slabCopyNo);

        fhitBuffer->SetTime(time);

        fhitBuffer->SetTrackId(trackID);
        fhitBuffer->SetParentId(parentID);
        fhitBuffer->SetPdg(pdg);

        fwriter->AddFiberHit(*fhitBuffer.get());
    }
    else if(fverbose)
    {
        cout    << "FileWriter is nullptr in " 
                << __FILE__ << " " 
                << __LINE__ << endl;
    }
    
    if(fverbose)
    {
        // Get Material 
        G4String thisVolume = aStep->GetTrack()->GetVolume()->GetName() ;
        G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();
        G4double nonIon = aStep->GetNonIonizingEnergyDeposit();

        G4cout << "*******************************  " << G4endl;
        G4cout << "             HIT                 " << G4endl;
        G4cout << "  Total_Energy_Deposit           " << edep/CLHEP::eV << " eV"  << G4endl;
        G4cout << "  Total_Energy_NonIoniziong      " << nonIon/CLHEP::eV << " eV"  << G4endl;
        G4cout << "  Particle:                      " << particleName << G4endl;
        G4cout << "  Volume:                        " << thisVolume << G4endl;
        G4cout << "  Energy (eV) :                  " << kineticEnergy/CLHEP::eV << G4endl;
        G4cout << "  POSITION (mm) :                " 
         << hitPosition.x()/CLHEP::mm << " " << hitPosition.y()/CLHEP::mm << " " << hitPosition.z()/CLHEP::mm << G4endl;
        G4cout << "*******************************" << G4endl;
    }

    return true;
}

}   //superfgd
}   //simulators
}   //esbroot