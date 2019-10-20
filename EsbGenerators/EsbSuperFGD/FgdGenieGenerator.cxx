#include "EsbGenerators/EsbSuperFGD/FgdGenieGenerator.h"
#include "EsbGenerators/EsbSuperFGD/GenieFluxDriver.h"
#include "EsbGenerators/EsbSuperFGD/FgdGeomAnalyzer.h"
#include "EsbGeometry/EsbSuperFGD/Names.h"

#include "FairLogger.h"
#include <Framework/Conventions/Units.h>
#include "Framework/GHEP/GHepParticle.h"

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
									, TGeoManager* gm)
	 : GenieGenerator()
	 	, fgeoConfigFile(geoConfigFile)
		, fnuFluxFile(nuFluxFile)
		, fseed(seed)
		, fdetPos(detPos)
		, fnumEvents(numEvents)
		, fgm(gm)
		, fCurrentEvent(0)
		, fUseRandomVertex(false)
{
}


void FgdGenieGenerator::PostProcessEvent(/*IN OUT*/ genie::GHepRecord* event)
{
	// Move each vertex to the global geometry coordinate system
	TLorentzVector* v = event->Vertex();
	

	GenieFluxDriver* fluxD = dynamic_cast<GenieFluxDriver*>(GetFluxI().get());
	if(fUseRandomVertex && fluxD!=nullptr)
	{	
		*v += fluxD->AbsPosition();	
		event->SetVertex(*v);
	}
	else
	{
		*v += TLorentzVector(fdetPos,0);	
		event->SetVertex(*v);
	}
	
}


Bool_t FgdGenieGenerator::Configure()
{
	if(fgm==nullptr)
	{
		fgm = gGeoManager;
	}

	SetFluxI(std::make_shared<GenieFluxDriver>(fgeoConfigFile.c_str(), fnuFluxFile.c_str(), fseed, fdetPos));

	SetGeomI(std::make_shared<FgdGeomAnalyzer>(fgeoConfigFile.c_str(), fdetPos, fgm));
	FgdGeomAnalyzer* geomAnalyzer = dynamic_cast<FgdGeomAnalyzer*>(GetGeomI().get());
	geomAnalyzer->SetScannerFlux(GetFluxI().get());

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
				std::vector<genie::GHepParticle*> eventParticles(nParticles);
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
				if(!KeepThrowing(eventParticles))
				{
					fGenieEvents.emplace_back(*event);
					break;
				}
			}
			else
			{
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

} //namespace superfgd 
} //namespace generators
} //namespace esbroot
