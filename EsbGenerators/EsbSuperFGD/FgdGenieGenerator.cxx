#include "EsbGenerators/EsbSuperFGD/FgdGenieGenerator.h"
#include "EsbGenerators/EsbSuperFGD/GenieFluxDriver.h"
#include "EsbGenerators/EsbSuperFGD/FgdGeomAnalyzer.h"
#include "EsbGeometry/EsbSuperFGD/Names.h"

#include "FairLogger.h"
#include <Framework/Conventions/Units.h>
#include "Framework/GHEP/GHepParticle.h"
#include "Framework/Interaction/ProcessInfo.h"
#include "Framework/Interaction/Interaction.h"
#include "Framework/Interaction/InitialState.h"

#include <Tools/Geometry/ROOTGeomAnalyzer.h>

namespace esbroot {
namespace generators {
namespace superfgd {

FgdGenieGenerator::FgdGenieGenerator()
	: GenieGenerator()
{
}

FgdGenieGenerator::~FgdGenieGenerator()
{
}

FgdGenieGenerator::FgdGenieGenerator(const char* geoConfigFile
									, const char* nuFluxFile
									, unsigned int seed
									, TVector3 detPos
									, Int_t numEvents
									, genie::GFluxI* extFlux
									, TGeoManager* gm)
	 : GenieGenerator()
	 	, fgeoConfigFile(geoConfigFile)
		, fnuFluxFile(nuFluxFile)
		, fseed(seed)
		, fdetPos(detPos)
		, fnumEvents(numEvents)
		, fgm(gm)
		, fExtFlux(extFlux)
		, fCurrentEvent(0)
		, fUseRandomVertex(false)
{
}


void FgdGenieGenerator::PostProcessEvent(/*IN OUT*/ genie::GHepRecord* event)
{

}


Bool_t FgdGenieGenerator::Configure()
{
	if(fgm==nullptr)
	{
		fgm = gGeoManager;
	}

	SetGeomI(std::make_shared<FgdGeomAnalyzer>(fgm));
	FgdGeomAnalyzer* geomAnalyzer = dynamic_cast<FgdGeomAnalyzer*>(GetGeomI().get());

	if(fExtFlux!=nullptr)
	{
		LOG(info) << "Using external flux driver ";
		SetFluxI(shared_ptr<genie::GFluxI>(fExtFlux));
	}
	else
	{
		LOG(info) << "Using flux driver - GenieFluxDriver [Fdg implementation]";
		SetFluxI(std::make_shared<GenieFluxDriver>(fgeoConfigFile.c_str(), fnuFluxFile.c_str(), fseed, fdetPos));
		geomAnalyzer->SetScannerFlux(GetFluxI().get()); // Force to use MaxPathLengthsFluxMethod, otherwise it uses MaxPathLengthsBoxMethod
	}

	GenieGenerator::Configure();
	GenerateEvents();
	
	geomAnalyzer->Reset();	// Revert initial Top Volume Geometry
}

Bool_t FgdGenieGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{
	if(fCurrentEvent>=fnumEvents) return false;

	genie::EventRecord& event = fGenieEvents[fCurrentEvent++];
	
	event.Print(std::cout);
	TLorentzVector* v = event.Vertex();
		
	// Fire other final state particles
	int nParticles = event.GetEntries();
	for (int i = 0; i < nParticles; i++) 
	{
		genie::GHepParticle *p = event.Particle(i);
		// kIStStableFinalState - Genie documentation: generator-level final state
		// particles to be tracked by the detector-level MC
		if ((p->Status() == genie::EGHepStatus::kIStStableFinalState)) 
		{
			if(IsPdgAllowed(p->Pdg()))
			{
				primGen->AddTrack(p->Pdg(), p->Px(), p->Py(), p->Pz(), v->X(), v->Y(), v->Z());
			}
		}
	}

	if(!GlobalState.fOutputFileName.empty())
	{
		WriteToOutputFile(&event, false /* flaGkeepThrowing - check made in GenerateEvents*/);
	}
		
    return true;
}

void FgdGenieGenerator::GenerateEvents()
{
	for(Int_t eventId = 0; eventId < fnumEvents; ++eventId)
	{
		while(true)
		{
			genie::EventRecord* event = fmcj_driver->GenerateEvent();
			if(event != nullptr)
			{
				PostProcessEvent(event);
				int nParticles = event->GetEntries();
				std::vector<genie::GHepParticle*> eventParticles;
				for (int i = 0; i < nParticles; i++) 
				{
					genie::GHepParticle *p = event->Particle(i);
					// kIStStableFinalState - Genie documentation: generator-level final state
					// particles to be tracked by the detector-level MC
					if ((p->Status() == genie::EGHepStatus::kIStStableFinalState)) 
					{
						if(IsPdgAllowed(p->Pdg()))
						{
							eventParticles.push_back(p);
						}
					}
				}
				if(!KeepThrowing(eventParticles) && !eventParticles.empty())
				{
					fGenieEvents.emplace_back(*event);
					delete event;
					break;
				}
			}
			else
			{
				--eventId; // try again
				break;
			}
			
			delete event;
		}
		
	}
}

Bool_t FgdGenieGenerator::KeepThrowing(std::vector<genie::GHepParticle*>& eventParticles )
{
	static int count = 0;
	Bool_t throwAgain(false);

	if(eventParticles.empty() && count <5 /* try only 5 times */)
	{
		++count;
		throwAgain = true;
	}
	else
	{
		count = 0;
	}
	

	return throwAgain;
}

void FgdGenieGenerator::WriteToOutputFile(const genie::EventRecord* event, Bool_t flaGkeepThrowing ) 
{
	static int eventCounter = 0;
	std::ofstream outputFile(GlobalState.fOutputFileName, std::ios::app);
	if(outputFile.is_open())
	{
		const Interaction* inter = event->Summary();
		const ProcessInfo& procInfo = inter->ProcInfo();
		LOG(debug) << "IsWeakCC " << procInfo.IsWeakCC() << "; IsWeakNC " << procInfo.IsWeakNC();

		const InitialState& initSt = inter->InitState();

		// Write the interaction info to be used in alter analysis
		outputFile << initSt.ProbePdg() 
					<< " " << initSt.ProbeE(kRfLab) 
					<< " " << procInfo.IsWeakCC() 
					<< " " << procInfo.IsWeakNC() 
					<< " " << procInfo.IsQuasiElastic()
					<< " " << procInfo.IsDeepInelastic()
					<< " " << procInfo.IsResonant()
					<< " " << procInfo.IsCoherent()
					<< " " << procInfo.IsMEC()
					<< " " << procInfo.IsNuElectronElastic()
					<< " " << procInfo.IsElectronScattering();

		TLorentzVector* v = event->Vertex();

		outputFile << " " << v->X() << " " << v->Y()<< " " << v->Z();

		int nParticles = event->GetEntries();
		for (int i = 0; i < nParticles; i++) 
		{
			genie::GHepParticle *p = event->Particle(i);
			// kIStStableFinalState - Genie documentation: generator-level final state
			// particles to be tracked by the detector-level MC
			if ((p->Status() == genie::EGHepStatus::kIStStableFinalState)) 
			{
				if(IsPdgAllowed(p->Pdg()))
				{
					outputFile << " " << p->Pdg() << " " <<  p->Px() << " " << p->Py() << " " << p->Pz();
				}
			}
		}
		//outputFile << "EndEvent " << eventCounter++ << "=====================" << endl;
		outputFile << endl;
	}
	outputFile.close();
}

} //namespace superfgd 
} //namespace generators
} //namespace esbroot
