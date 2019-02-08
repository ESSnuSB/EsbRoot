#ifndef ESBROOT_GENERATOR_GENIEGENERATOR_H
#define ESBROOT_GENERATOR_GENIEGENERATOR_H 1

/* FairRoot headers */
#include "FairGenerator.h" 
#include "FairPrimaryGenerator.h"

/* Genie framework classes and tools headers */
#include "Framework/EventGen/GMCJDriver.h"
#include "Tools/Flux/GMonoEnergeticFlux.h"
#include "Tools/Geometry/PointGeomAnalyzer.h"
#include "Tools/Geometry/ROOTGeomAnalyzer.h"

#include <memory>

using namespace std;
using namespace genie;
using namespace genie::geometry;
using namespace genie::flux;

namespace esbroot {

namespace generators {

/*! Implementar of the FairRoot generator using the 
    Genie framework for neutrino event generations*/
class GenieGenerator : public FairGenerator
{

public:

    /** Geometry types **/
    enum GenieGeometry{
        GLOBAL = 0,
        POINT = 1
    };

    /** Constructor with pdgCode and max energy **/
    GenieGenerator(int pdgCode, double Emax);

    // /** Constructor with pdgCode, max energy and geometry type selection **/
    GenieGenerator(int pdgCode, double Emax, GenieGenerator::GenieGeometry geoType);

    /** Destructor  **/
    virtual ~GenieGenerator();

    /* method which initializes the Genie generator */
    Bool_t Init(void);

    Bool_t IsInit() {return fIsInit;}

    /* implementation of the base method which generates the vertex particles */
    virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);

    /** Clone this object (used in MT mode only) */
    virtual FairGenerator* CloneGenerator() const;

private:

    /** Driver for generating neutrino events **/
    std::shared_ptr<GMCJDriver> fmcj_driver;
    
     /** Initializes the Flux driver for GMCJDriver **/
    void FluxInit(void);
    std::shared_ptr<GMonoEnergeticFlux> fflux_driver;
   
    /** Used in GMCJDriver initialization to select geometry **/
    void GeometryInit(void);
    GenieGeometry fgeom;
    std::shared_ptr<PointGeomAnalyzer> fpointGeom;
    std::shared_ptr<ROOTGeomAnalyzer> fglobalGeom;

    /** Max energy of the neutrinos in eV **/
    double fmaxEv;

    /** pdg code of the flux generator **/
    int fpdgCode; 

    /* checks if the Init method has been called */
    Bool_t fIsInit;

    ClassDef(GenieGenerator,4)
};

} //namespace generators

} //namespace esbroot
#endif