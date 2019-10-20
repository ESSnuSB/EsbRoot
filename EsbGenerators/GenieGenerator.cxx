#include "EsbGenerators/GenieGenerator.h"
#include "EsbTools/UtilityDefs.h"

/* Fair Root headers */
#include "TGeoManager.h" // for gGeoManager

/* Genie framework classes and tools */
#include "Framework/EventGen/EventRecord.h"
#include "Framework/Utils/XSecSplineList.h" // for cross_section.xml genie initialization
#include "Framework/GHEP/GHepParticle.h"
#include "Framework/GHEP/GHepStatus.h"      // for kIStStableFinalState
#include "Framework/Utils/RunOpt.h"      
#include "Framework/Utils/AppInit.h" //For Random Seed

#include <iostream>
#include <cstdlib>
#include <cassert>

//~ using namespace esbroot::generators;
namespace esbroot {
namespace generators {

/*static*/ GenieGenerator::GlobalState_t GenieGenerator::GlobalState;
/*static*/ bool GenieGenerator::fGlobalStateInit = false;

//! Constructor which takes user defined Genie flux and geometry drivers.
//! The Genie backend is not configured until a call to Configure()
//! @param fluxI user supplied Genie flux driver
//! @param geomI user supplied Genie geometry driver
GenieGenerator::GenieGenerator(genie::GFluxI *fluxI, genie::GeomAnalyzerI *geomI) {
	SetFluxI(std::shared_ptr<genie::GFluxI>(fluxI));
	SetGeomI(std::shared_ptr<genie::GeomAnalyzerI>(geomI));
}

//! Initializes the state of Genie backend using GenieGenerator::GlobalState variable.
//! Called automatically from Configure().
/*static*/ void GenieGenerator::InitGlobalState()
{
	assert(fGlobalStateInit == false);

	auto xspl = genie::XSecSplineList::Instance();
	xspl->LoadFromXml(GlobalState.fXsecSplineFileName, false);
	
	genie::RunOpt::Instance()->SetTuneName(GlobalState.fGenieTune);
	genie::RunOpt::Instance()->BuildTune();
	
	fGlobalStateInit = true;
}

//!This function actually initializes the Genie generator backend using
//!supplied geometry and flux drivers. Calls InitGlobalState().
/*virtual*/ Bool_t GenieGenerator::Configure()
{
	assert(IsConfigured() == false);
	
	InitGlobalState();
	
	fmcj_driver = std::make_shared<genie::GMCJDriver>();
	
	fmcj_driver->UseFluxDriver(GetFluxI().get());
	fmcj_driver->UseGeomAnalyzer(GetGeomI().get());
	fmcj_driver->Configure();

	fIsConfigured = true;

	return true; // No initialization checks done, return true	
}
	
 
/*! Override the FairGenerator::ReadEvent:
*       Reads the events from the genie::GMCJDriver
*      instead of a passed in file.
*/
/*virtual*/ Bool_t GenieGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{
    //!Budimir: this function is not entirely correct. The vertex position is
    //!always at (0,0,0). However, each track starts at the position
    //!genie::EventRecord::Vertex() relative
    //!to FairRoot's "vertex coordinate (0,0,0)", so in the end simulation works as required.
    //!This is due to limitations of FairRoot. We can live with this for now,
    //!but it definitely needs to be fixed in the future.

	Bool_t flaGkeepThrowing(true); // Flag to indicate when to stop generating events if there is a condition for the generated particles

	while(flaGkeepThrowing)
	{
		genie::EventRecord* event = fmcj_driver->GenerateEvent();
		if(event == nullptr) return false;

		PostProcessEvent(event);

		std::vector<genie::GHepParticle*> eventParticles;
		
		event->Print(std::cout);
		TLorentzVector* v = event->Vertex();
			
		// Fire other final state particles
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
					primGen->AddTrack(p->Pdg(), p->Px(), p->Py(), p->Pz(), v->X(), v->Y(), v->Z());
					eventParticles.push_back(p);
				}
			}
		}

		flaGkeepThrowing = KeepThrowing(eventParticles);

		delete event;
	}
	
    
    return true;
}


Bool_t GenieGenerator::IsPdgAllowed(int pdg)
{
	
	// Workaround for GENIE bug (or "feature") that treats nuclear energy as trackable particle
	if(pdg >= 2000000000)
	{
		return false;
	}

	// If there are no codes to filter any valid pdg is allowed to be tracked
	if(fpdgCodesAllowed.empty())
	{
		return true;
	}

	Bool_t isAllowed = std::find(fpdgCodesAllowed.begin(), fpdgCodesAllowed.end(), pdg) != fpdgCodesAllowed.end();
	return isAllowed;
}

Bool_t GenieGenerator::KeepThrowing(std::vector<genie::GHepParticle*>& eventParticles )
{
	// No implementation - decendent should decide
	return false;
}

FairGenerator* GenieGenerator::CloneGenerator() const
{
  // Clone for worker (used in MT mode only)
  // TODO: check that this actually works
  return new GenieGenerator(*this);
}

}
}
