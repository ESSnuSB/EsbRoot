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

GenieGenerator::GenieGenerator(genie::GFluxI *fluxI, genie::GeomAnalyzerI *geomI) {
	SetFluxI(std::shared_ptr<genie::GFluxI>(fluxI));
	SetGeomI(std::shared_ptr<genie::GeomAnalyzerI>(geomI));
}

GenieGenerator::~GenieGenerator()
{
}

/*static*/ void GenieGenerator::InitGlobalState()
{
	assert(fGlobalStateInit == false);

	auto xspl = genie::XSecSplineList::Instance();
	xspl->LoadFromXml(GlobalState.fXsecSplineFileName, false);
	
	genie::RunOpt::Instance()->SetTuneName(GlobalState.fGenieTune);
	genie::RunOpt::Instance()->BuildTune();
	
	fGlobalStateInit = true;
}


/*! Initialize the GenieGenrator:
*   1. Sets the fluxDriver
*   2. Sets the Geometry of the sensitive parts
*       where neutrinos should interact
*   3. Sets the cross_sections.xml file for the interactions
*/
/*virtual*/ Bool_t GenieGenerator::Init()
{
    fmcj_driver = std::make_shared<genie::GMCJDriver>();

	fmcj_driver->UseFluxDriver(GetFluxI().get());
	fmcj_driver->UseGeomAnalyzer(GetGeomI().get());
	fmcj_driver->Configure();

    //~ FluxInit();
    //~ GeometryInit();

    //~ fmcj_driver->Configure();
    //~ fmcj_driver->UseSplines();
    //~ fmcj_driver->ForceSingleProbScale();

    fIsInit = true;

    return true; // No initialization checks done, return true
}
 
/*! Override the FairGenerator::ReadEvent:
*       Reads the events from the genie::GMCJDriver
*      instead of a passed in file.
*/
/*virtual*/ Bool_t GenieGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{
    //flag indicates that an event has been generated
    Bool_t rc(false);

    genie::EventRecord* event = fmcj_driver->GenerateEvent();
    event->Print(std::cout);
    if(event!=nullptr)
    {
        TLorentzVector* v = event->Vertex(); //TODO: each track has its own origin

				v->SetVect(v->Vect() + fVtxPos);
				v->SetT(v->T() + fVtxTime);

        // Fire other final state particles
        int nParticles = event->GetEntries();
        for (int i = 0; i < nParticles; i++) 
        {
            genie::GHepParticle *p = event->Particle(i);
            // kIStStableFinalState - Genie documentation: generator-level final state
            // particles to be tracked by the detector-level MC
            if ((p->Status() == genie::EGHepStatus::kIStStableFinalState)) 
            {
                primGen->AddTrack(p->Pdg(), p->Px(), p->Py(), p->Pz(), v->X(), v->Y(), v->Z());
            }
        }
        
        delete event;
    }

    return true;
}

FairGenerator* GenieGenerator::CloneGenerator() const
{
  // Clone for worker (used in MT mode only)
  // TODO: check that this actually works
  return new GenieGenerator(*this);
}

}
}
