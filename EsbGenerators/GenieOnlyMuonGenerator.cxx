#include "EsbGenerators/GenieOnlyMuonGenerator.h"
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

/*static*/ GenieOnlyMuonGenerator::GlobalState_t GenieOnlyMuonGenerator::GlobalState;
/*static*/ bool GenieOnlyMuonGenerator::fGlobalStateInit = false;

//! Constructor which takes user defined Genie flux and geometry drivers.
//! The Genie backend is not configured until a call to Configure()
//! @param fluxI user supplied Genie flux driver
//! @param geomI user supplied Genie geometry driver
GenieOnlyMuonGenerator::GenieOnlyMuonGenerator(genie::GFluxI *fluxI, genie::GeomAnalyzerI *geomI) 
{
	SetFluxI(std::shared_ptr<genie::GFluxI>(fluxI));
	SetGeomI(std::shared_ptr<genie::GeomAnalyzerI>(geomI));
}


GenieOnlyMuonGenerator::~GenieOnlyMuonGenerator()
{
}


//! Initializes the state of Genie backend using GenieOnlyMuonGenerator::GlobalState variable.
//! Called automatically from Configure().
/*static*/ void GenieOnlyMuonGenerator::InitGlobalState()
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
/*virtual*/ Bool_t GenieOnlyMuonGenerator::Configure()
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
/*virtual*/ Bool_t GenieOnlyMuonGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{
    //!Budimir: this function is not entirely correct. The vertex position is
    //!always at (0,0,0). However, each track starts at the position
    //!genie::EventRecord::Vertex() relative
    //!to FairRoot's "vertex coordinate (0,0,0)", so in the end simulation works as required.
    //!This is due to limitations of FairRoot. We can live with this for now,
    //!but it definitely needs to be fixed in the future.
    
    //flag indicates that an event has been generated
    Bool_t rc(false);

	// Generate event untill it has a generated muon for a track
	while(!rc)
	{
		genie::EventRecord* event = fmcj_driver->GenerateEvent();
    	if(event == nullptr) return false;

		PostProcessEvent(event);
		
		event->Print(std::cout);
		TLorentzVector* v = event->Vertex();
			
		// Fire other final state particles
		int nParticles = event->GetEntries();
		for (int i = 0; i < nParticles; i++) 
		{
			genie::GHepParticle *p = event->Particle(i);
			// kIStStableFinalState - Genie documentation: generator-level final state
			// particles to be tracked by the detector-level MC
			if ((p->Status() == genie::EGHepStatus::kIStStableFinalState) // particles should be stable in the final state
				&& (p->Pdg() < 2000000000)// Particles above 2000000000 are supersimetric particles and should be exclude from the simulation
				&& (p->Pdg() == 13 || p->Pdg()==-13) // Track only muons or antimuons
				) 
			{
				primGen->AddTrack(p->Pdg(), p->Px(), p->Py(), p->Pz(), v->X(), v->Y(), v->Z());
				rc = true;

				if(!GlobalState.fOutputMuonFileName.empty())
				{
					std::ofstream fMuonfile(GlobalState.fOutputMuonFileName, std::ios::app);
					if(fMuonfile.is_open())
					{
						fMuonfile << p->Px() << " " << p->Py() << " " << p->Pz() << " " << v->X() << " " << v->Y()<< " " << v->Z() << std::endl;
					}
					fMuonfile.close();
				}
				
			}
		}
			
		delete event;
	}
    
    
    return true;
}

FairGenerator* GenieOnlyMuonGenerator::CloneGenerator() const
{
  // Clone for worker (used in MT mode only)
  // TODO: check that this actually works
  return new GenieOnlyMuonGenerator(*this);
}

}
}
