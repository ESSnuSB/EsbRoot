#include "EsbSimulators/EsbSuperFGD/EsbPrimaryGeneratorAction.h"

#include "EsbSimulators/EsbSuperFGD/EsbFileWriter.h"
#include "EsbSimulators/EsbSuperFGD/EsbFgdRunManager.h"

#include "EsbData/EsbSuperFGD/EsbDetector/EsbFgdDetectorParameters.h"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"

#include <Framework/EventGen/EventRecord.h>
#include <Framework/ParticleData/PDGUtils.h>

#include <Framework/Conventions/Units.h>
#include <TLorentzVector.h>

#include <iostream>
#include <exception>
#include <memory>

#include "EsbTools/EsbDefines.h"
   
using namespace std;

namespace esbroot {
namespace simulators {
namespace superfgd {

PrimaryGeneratorAction::PrimaryGeneratorAction(std::string eventsFile)
: G4VUserPrimaryGeneratorAction(),feventsFile(eventsFile), fverboseMode(false)
{
    foldbuffer = new genie::NtpMCEventRecord();
    fbuffer = new genie::NtpMCEventRecord();
    InitializeTChain();
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(!ReadEvent())
    cout << "No more events to process!" << endl;

  const genie::NtpMCEventRecord* ntpMCEvent = PopEvent();
  if (ntpMCEvent == nullptr) 
  {
    std::string errMsg = "ERROR: PrimaryGeneratorAction::GeneratePrimaries(): PrimaryGeneratorAction::PopEvent() returned 0.";
    THROW(errMsg);
  }
  
  //const genie::EventRecord* event = ntpMCEvent->event;
  genie::EventRecord* event = ntpMCEvent->event;

  TLorentzVector* v = event->Vertex();
  //Double_t normalization = (genie::units::meter/CLHEP::meter);
  Double_t normlunit =  data::superfgd::detector::FgdDetectorParameters::GetLenghtUnit();
  G4PrimaryVertex* vertex = new G4PrimaryVertex(v->X() * normlunit, v->Y() * normlunit, v->Z() * normlunit, 0.);

  if (fverboseMode) {
        cout << *event << endl;
        cout << "Primary_Vertex " << vertex->GetPosition() << endl;
        cout << *(event->Summary()) << endl;
  }
  
  // find fspl (nu elastic scattering on electron has nu set as fspl => need to find e-)
  int fspl_index = event->FinalStatePrimaryLeptonPosition();
  if (event->Summary()->ProcInfo().IsNuElectronElastic()) {
        fspl_index = event->ParticlePosition(11, genie::kIStStableFinalState, 0);
  }
  genie::GHepParticle* fspl = event->Particle(fspl_index);
  AddParticleToVertex(vertex, fspl);

  shared_ptr<FileWriter> writer = ((FgdRunManager*)G4RunManager::GetRunManager())->GetFileWriter();
  if(writer!=nullptr)
  {
    writer->AddEvent(event);
    if(fspl->Charge()!=0)
    {
        cout << "Primary_Vertex_pdg_fspl " << fspl->Pdg() << endl;
        writer->AddGHepParticle(fspl);
    } 
    writer->AddVertexPos(v->X() * normlunit, v->Y() * normlunit, v->Z() * normlunit);
  }

  // Fire other final state particles
  int nParticles = event->GetEntries();
  for (int i = 0; i < nParticles; i++) 
  {
      // skip fspl
      if (i == fspl_index) continue;

      genie::GHepParticle *p = event->Particle(i);
      // kIStStableFinalState - Genie documentation: generator-level final state
      // particles to be tracked by the detector-level MC
      if ((p->Status() == genie::kIStStableFinalState) && (p->Pdg() < 2000000000) &&
          (abs(p->Pdg()) != 12) && (abs(p->Pdg()) != 14) && (abs(p->Pdg()) != 16)) {
                
            AddParticleToVertex(vertex, p);

            if(writer!=nullptr && p->Charge()!=0)
            {
                cout << "Primary_Vertex_pdg " << p->Pdg() << endl;
                writer->AddGHepParticle(p);
            }
      }
  }

  anEvent->AddPrimaryVertex(vertex);
}

void PrimaryGeneratorAction::AddParticleToVertex(G4PrimaryVertex* v, genie::GHepParticle *p)
{
    G4PrimaryParticle* primaryParticle = nullptr;

    if (p->Pdg() < 1000000000) {
        primaryParticle = new G4PrimaryParticle(p->Pdg());
        primaryParticle->Set4Momentum(p->Px()*CLHEP::GeV, p->Py()*CLHEP::GeV, p->Pz()*CLHEP::GeV, p->E()*CLHEP::GeV);
    }
    else {
        G4IonTable* ionTable = G4ParticleTable::GetParticleTable()->GetIonTable();
        int Z = genie::pdg::IonPdgCodeToZ(p->Pdg());
        int A = genie::pdg::IonPdgCodeToA(p->Pdg());
        G4ParticleDefinition* particle = ionTable->GetIon(Z,A,0,0);
        //G4ParticleDefinition* particle = particleTable->GetIon(6,12,0,0);
        primaryParticle = new G4PrimaryParticle(particle);
        primaryParticle->SetMomentum(p->Px()*CLHEP::GeV, p->Py()*CLHEP::GeV, p->Pz()*CLHEP::GeV);
        primaryParticle->SetMass(particle->GetPDGMass());

        if (fverboseMode) 
        {
            cout << "PdgCode_AddParticleToVertex_Ion " << p->Pdg() << endl;
        }
    }

    primaryParticle->SetCharge(p->Charge()*(CLHEP::eplus/3));
    TVector3 polz;
    p->GetPolarization(polz);
    primaryParticle->SetPolarization(polz.X(), polz.Y(), polz.Z());
    v->SetPrimary(primaryParticle);

    if (fverboseMode) {
        cout << "PdgCode_AddParticleToVertex " << p->Pdg() << endl;
        primaryParticle->Print();
        cout << "  > " << p->Name() << "     KinE = " << p->KinE()*1000 << " MeV    " << "theta = " << p->P4()->Theta() << endl;
    }
}


void PrimaryGeneratorAction::InitializeTChain()
{
    fchain = new TChain("gtree"); // create the chain with tree "gtree"
    fchain->Add(feventsFile.c_str()); // add the file
    cout << "INFO: Added file \"" << feventsFile << "\" to chain." << endl;

    fchain->SetBranchAddress("gmcrec", &fbuffer);

    fnextEventNo = 0;
    fchainNEvents = fchain->GetEntries();
    if (fchainNEvents <= 0) 
    {
        std::string errMsg = "ERROR: File ";
        errMsg+=feventsFile;
        errMsg+=" has no events in chain.";
        THROW(errMsg);
    }
    cout << "INFO: entries in file \"" << feventsFile << "\" ->" << fchainNEvents << endl;
}

bool PrimaryGeneratorAction::ReadEvent() 
{
  if (fnextEventNo == fchainNEvents) {
      fbuffer = nullptr;
      return false;
  }
  cout << "INFO: Event N" << fnextEventNo << " is " << *fbuffer << endl;

  fchain->GetEntry(fnextEventNo);
  fnextEventNo++;
  return true;
}

const genie::NtpMCEventRecord* PrimaryGeneratorAction::PopEvent()
{
  if (fbuffer == nullptr)
        return nullptr;

  foldbuffer->Copy(*fbuffer);
  return foldbuffer;
}

}   //superfgd
}   //simulators
}   //esbroot