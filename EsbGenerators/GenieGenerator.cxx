#include "EsbGenerators/GenieGenerator.h"
#include "EsbTools/UtilityDefs.h"

/* Fair Root headers */
#include "TGeoManager.h" // for gGeoManager

/* Genie framework classes and tools */
#include "Framework/EventGen/EventRecord.h"
#include "Framework/Utils/XSecSplineList.h" // for cross_section.xml genie initialization
#include "Framework/GHEP/GHepParticle.h"
#include "Framework/GHEP/GHepStatus.h"      // for kIStStableFinalState

using namespace esbroot::generators;

GenieGenerator::GenieGenerator(int pdgCode, double Emax)
    : FairGenerator("GenieGenerator"),fpdgCode(pdgCode), fmaxEv(Emax), fgeom(GenieGeometry::POINT) , fIsInit(false)
{
}


GenieGenerator::GenieGenerator(int pdgCode, double Emax, GenieGenerator::GenieGeometry geoType)
    : FairGenerator("GenieGenerator"), fpdgCode(pdgCode), fmaxEv(Emax), fgeom(geoType) , fIsInit(false)
{
}

GenieGenerator::~GenieGenerator()
{
}

/*! Initialize the GenieGenrator:
*   1. Sets the fluxDriver
*   2. Sets the Geometry of the sensitive parts
*       where neutrinos should interact
*   3. Sets the cross_sections.xml file for the interactions
*/
Bool_t GenieGenerator::Init()
{
    // Initialize cross section xml data
    // file to cross section should be set 
    XSecSplineList * xspl = XSecSplineList::Instance();
    std::string xmlfile = std::string(std::getenv("CSFILE"));
    xspl->LoadFromXml(xmlfile, false);
    unsetenv("GSPLOAD"); // if it is not unset, the cross sections cannot be read

    fmcj_driver = make_shared<GMCJDriver>();

    FluxInit();
    GeometryInit();

    fmcj_driver->Configure();
    fmcj_driver->UseSplines();
    fmcj_driver->ForceSingleProbScale();

    fIsInit = true;

    return true; // No initialization checks done, return true
}
 
/*! Override the FairGenerator::ReadEvent:
*       Reads the events from the genie::GMCJDriver
*      instead of a passed in file.
*/
Bool_t GenieGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{
    //flag indicates that an event has been generated
    Bool_t rc(false);

    EventRecord* event = fmcj_driver->GenerateEvent();
    if(event!=nullptr)
    {
        TLorentzVector* v = event->Vertex();

        // Fire other final state particles
        int nParticles = event->GetEntries();
        for (int i = 0; i < nParticles; i++) 
        {
            GHepParticle *p = event->Particle(i);
            // kIStStableFinalState - Genie documentation: generator-level final state
            // particles to be tracked by the detector-level MC
            if ((p->Status() == EGHepStatus::kIStStableFinalState)) 
            {
                primGen->AddTrack(p->Pdg(), p->Px(), p->Py(), p->Pz(), v->X(), v->Y(), v->Z());
            }
        }
        
        delete event;
    }

    return rc;
}

FairGenerator* GenieGenerator::CloneGenerator() const
{
  // Clone for worker (used in MT mode only)
  return new GenieGenerator(*this);
}

/*! Set the flux driver to be used by the 
*   to be used in the Genie event generation.
*/
void GenieGenerator::FluxInit(void)
{
    fflux_driver = make_shared<GMonoEnergeticFlux>(
            fmaxEv, // max energy of the particles in eV
            fpdgCode // e.g. genie::kPdgNuMu = 14
    );
    fmcj_driver->UseFluxDriver(fflux_driver.get()); 
}

/*! Set the geometry to be used in the Genie
*   event generation.
*/
void GenieGenerator::GeometryInit()
{
    // No need for null check of fmcj_driver. Used only internally during initialization
    switch(fgeom)
    {
        case GenieGeometry::POINT:
                                fpointGeom = make_shared<PointGeomAnalyzer>(CARBON_12);
                                fmcj_driver->UseGeomAnalyzer(fpointGeom.get());
                                break;
        case GenieGeometry::GLOBAL:
        default:
                                fglobalGeom = make_shared<ROOTGeomAnalyzer>(gGeoManager);
                                fmcj_driver->UseGeomAnalyzer(fglobalGeom.get());
                                break;
    }
}