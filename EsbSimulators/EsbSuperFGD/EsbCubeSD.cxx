#include "EsbSimulators/EsbSuperFGD/EsbCubeSD.h"

#include "EsbSimulators/EsbSuperFGD/EsbFgdRunManager.h"
#include "Simulation/include/NFDetectorConstruction.hh"

namespace esbroot {
namespace simulators {
namespace superfgd {

CubeSD::CubeSD(G4String name, G4String detector_sd_type)
    :G4VSensitiveDetector(name),fverbose(false), fdetector_sd_type(detector_sd_type) 
{
    fhitBuffer =  = make_shared<CubeHit>();
}

CubeSD::~CubeSD()
{
}

void  CubeSD::Initialize(G4HCofThisEvent* HCE)
{

}

G4bool CubeSD::ProcessHits(G4Step* aStep,G4TouchableHistory* ROHist)
{
    return ProcessTotalHits(aStep, ROHist);
}

/*
Process total hits sums up all the deposited energy and
trackingLenght before the information is exported.
This is done to be more memory efficient for large simulations.
*/
G4bool CubeSD::ProcessTotalHits(G4Step* aStep,G4TouchableHistory* ROHist)
{
    if(fverbose)
        cout << "NFCubeSD::ProcessHitsSuperFGD " << endl;

    //==========================================================================
    //              Get Runmanager and detector to analyze the step data
    //==========================================================================
    FgdRunManager* man = dynamic_cast<FgdRunManager*>(G4RunManager::GetRunManager());
    G4VUserDetectorConstruction* g4dc = const_cast<G4VUserDetectorConstruction*>(man->GetUserDetectorConstruction());
    NFDetectorConstruction* detector = dynamic_cast<NFDetectorConstruction*>(g4dc);
    if(detector==nullptr) 
        return false;

    bool ignoreEdep = (man!=nullptr && man->GetIgnoreEdep());

    //==============================================
    //              Get Step data
    //==============================================
    G4double edep = aStep->GetTotalEnergyDeposit();
    if (!ignoreEdep && edep == 0.)
        return false;

    //
    G4double nonIon = aStep->GetNonIonizingEnergyDeposit();
    G4ThreeVector hitPosition   = aStep->GetPreStepPoint()->GetPosition();
    G4ThreeVector postHitPosition = aStep->GetPostStepPoint()->GetPosition();
    G4double      kineticEnergy = aStep->GetTrack()->GetKineticEnergy();
    G4ThreeVector momentum = aStep->GetPreStepPoint()->GetMomentum();

    double time = aStep->GetPreStepPoint()->GetGlobalTime();
    double postTime = aStep->GetPostStepPoint()->GetGlobalTime();

    G4Track* track = aStep->GetTrack();
    int trackID = track->GetTrackID();
    int parentID = track->GetParentID();
    int pdg = track->GetDefinition()->GetPDGEncoding();
    G4double charge = track->GetDefinition()->GetPDGCharge();

    double trackLength = aStep->GetStepLength();
    
    if(man!=nullptr)
    {
        fwriter = man->getFileWriter();
    }

    //================================================================
    //              Write data to be exported to file on end event
    //================================================================
    if(fwriter!=nullptr)
    {
        if(fverbose)
             cout<<" ======  NFCubeSD:fverbose ====="<< __FILE__ << " " << __LINE__ << endl;

        fhitBuffer->setEdep(edep);
        fhitBuffer->setNonIonizingEnergyDeposit(nonIon);

        fhitBuffer->setHitPostion(hitPosition.x(), hitPosition.y(), hitPosition.z());
        fhitBuffer->setPostHitPostion(postHitPosition.x(), postHitPosition.y(), postHitPosition.z());
        fhitBuffer->setKinEnergy(kineticEnergy);
        fhitBuffer->setHitMomentum(momentum.x(), momentum.y(), momentum.z());

        fhitBuffer->setTime(time);
        fhitBuffer->setPostTime(postTime);
        fhitBuffer->setTracklength(trackLength);

        fhitBuffer->setTrackId(trackID);
        fhitBuffer->setParentId(parentID);
        fhitBuffer->setPdg(pdg);
        fhitBuffer->setCharge(charge);

        //================================================================
        //     Check in which cube is the step position and accumulate it
        //================================================================
        NFDetectorParameters& dp = detector->getDetectorParams();
        f_writer->sumStep(fhitBuffer, dp);
    }
    else if(fverbose)
    {
        cout    << "NFFileWriter is nullptr in " 
                << __FILE__ << " " 
                << __LINE__ << endl;
    }
    
    //================================================================
    //              Output verbose data
    //================================================================
    if(fverbose)
    {
        // Get Material 
        G4String thisVolume = aStep->GetTrack()->GetVolume()->GetName() ;
        G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();

        G4cout << "*******************************  " << G4endl;
        G4cout << "             HIT                 " << G4endl;
        G4cout << "  Total_Energy_Deposit           " << edep/eV << " eV"  << G4endl;
        G4cout << "  Total_Energy_NonIoniziong      " << nonIon/eV << " eV"  << G4endl;
        G4cout << "  Pdg Code                       " << pdg  << G4endl;
        G4cout << "  Particle:                      " << particleName << G4endl;
        G4cout << "  Volume:                        " << thisVolume << G4endl;
        G4cout << "  TrackLength:                   " << trackLength / CLHEP::mm << " mm" << G4endl;
        G4cout << "  Charge:                        " << track->GetDefinition()->GetPDGCharge() << G4endl;
        G4cout << "  Energy (eV) :                  " << kineticEnergy/eV << G4endl;
        G4cout << "  POSITION (mm) :                " 
            << hitPosition.x()/mm << " " << hitPosition.y()/mm << " " << hitPosition.z()/mm << G4endl;
        G4cout << "*******************************" << G4endl;
    }

    return true;
}

}   //superfgd
}   //simulators
}   //esbroot