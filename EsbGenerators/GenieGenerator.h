#ifndef ESBROOT_GENERATOR_GENIEGENERATOR_H
#define ESBROOT_GENERATOR_GENIEGENERATOR_H 1

//! @file EsbGenerators/GenieGenerator.h
//! @copyright ESSnuSB consortium. Released under GPLv3 <https://www.gnu.org/licenses/gpl-3.0.en.html>.

/* FairRoot headers */
#include "FairGenerator.h" 
#include "FairPrimaryGenerator.h"

/* Genie framework classes and tools headers */
#include "Framework/EventGen/GMCJDriver.h"
#include "Framework/EventGen/EventRecord.h"
#include "Tools/Flux/GMonoEnergeticFlux.h"
#include "Tools/Geometry/PointGeomAnalyzer.h"
#include "Tools/Geometry/ROOTGeomAnalyzer.h"

#include <memory>

//~ using namespace std;
//~ using namespace genie;
//~ using namespace genie::geometry;
//~ using namespace genie::flux;

namespace esbroot {

namespace generators {

//! @brief Basic Genie generator class
//!
//! This class wraps Genie MC generator interface. It provides functionality to configure Genie global state
//! and to configure Genie generator backend using user provided Genie flux and geometry interfaces.
//!
//! It implements the FairGenerator::ReadEvent(FairPrimaryGenerator* primGen) function
//! which makes it usable within the FairRoot framework.
//!
//! @authors Budimir Kliček
//! @authors Georgi Petkov
//! @copyright ESSnuSB consortium. Released under GPLv3 (<https://www.gnu.org/licenses/gpl-3.0.en.html>).
class GenieGenerator : public FairGenerator
{

public:
	
	static struct GlobalState_t {
		std::string fGenieTune = ""; //!< Genie tune to be used
		std::string fXsecSplineFileName = ""; //!< Path to XML file with GENIE cross sections
		long int fRandomSeed = -1; //!< Random seed for GENIE (NOT IMPLEMENTED YET)
	} GlobalState; //!< Global state of Genie generator
	
	static void InitGlobalState(); //!< Initialize the state of Genie backend
	
	//! Empty constructor
	GenieGenerator() {};
	
	//! Basic constructor
	GenieGenerator(genie::GFluxI *fluxI, genie::GeomAnalyzerI *geomI);

	//! Empty destructor
  	virtual ~GenieGenerator() {};

	//Setters
	//! Set Genie flux driver. Can't set if object already configured.
	void SetFluxI(std::shared_ptr<genie::GFluxI> FluxI) {assert(IsConfigured() == false); fFluxI = FluxI;};
	//! Set Genie geometry driver. Can't set if object already configured.
	void SetGeomI(std::shared_ptr<genie::GeomAnalyzerI> GeomI) {assert(IsConfigured() == false); fGeomI = GeomI;};

	//Getters
	//! Get Genie flux driver
	std::shared_ptr<genie::GFluxI> const& GetFluxI() const {return(fFluxI);};
	//! Get Genie geometry driver
	std::shared_ptr<genie::GeomAnalyzerI> const& GetGeomI() const {return(fGeomI);};

  	//!Method which initializes the Genie generator
  
  	//!FairRoot runs this from FairRunSim::Init().
  	//!Empty because we might want to init Genie after FairRunSim::Init()
  	//!e.g to pass the geometry
  	virtual Bool_t Init(void) {return true;};

	//!Configure the Genie backend
	virtual Bool_t Configure(); 
	
	//!Check if Genie backend is configured
	Bool_t IsConfigured() const {return fIsConfigured;};

 	 //!Implementation of the base method which generates the vertex particles
  	virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);
	
	//!Method to post process the genie event record
	virtual void PostProcessEvent(/*IN OUT*/ genie::GHepRecord* event) {};

  	/** Clone this object (used in MT mode only) */
  	virtual FairGenerator* CloneGenerator() const;

	
	void AddPdgCode(int pdg){fpdgCodesAllowed.push_back(pdg);}
	void AddPdgCode(std::vector<int> pdgCodes){fpdgCodesAllowed.insert(fpdgCodesAllowed.end(), pdgCodes.begin(), pdgCodes.end());}
	void ClearPfgCodes(){fpdgCodesAllowed.clear();}


protected:

	//These static members are initialized in the .cxx file so that
	//RootCint does not complain
	//! Tracks if global state has been initialized
	static bool fGlobalStateInit;

  	//! Driver for generating neutrino events
	//! NOTE: DO NOT remove the //!<! comment! This is flag
	//! to indicate to ROOT that this member is transient and should not be
	//! written to a root file!
  	std::shared_ptr<genie::GMCJDriver> fmcj_driver;//!<!
    
  	//! Flux driver for GMCJDriver
	//! NOTE: DO NOT remove the //!<! comment! This is flag
	//! to indicate to ROOT that this member is transient and should not be
	//! written to a root file!
  	std::shared_ptr<genie::GFluxI> fFluxI;//!<!
   
  	//! Geometry dirver for GMCJDriver
	//! NOTE: DO NOT remove the //!<! comment! This is flag
	//! to indicate to ROOT that this member is transient and should not be
	//! written to a root file!
  	std::shared_ptr<genie::GeomAnalyzerI> fGeomI;//!<!

	//! During simulation some particles may be exluded from the analysis
	//! for Monte Carlo analysis add the particle pdg code which is inluded 
	//! from the neutrino event interactions
	std::vector<int> fpdgCodesAllowed;//!<!

  	//! Tracks if object has been configured
  	Bool_t fIsConfigured = false;

	//! Check if the event particle is allowed by the chosen criteria fo the generator
	virtual Bool_t IsPdgAllowed(int pdg);

	//!These methods checks if to try to generate a new event
	//!if the particles in the have certain criteria
	virtual Bool_t KeepThrowing(std::vector<genie::GHepParticle*>& eventParticles );

  	ClassDef(GenieGenerator,6)
};

} //namespace generators

} //namespace esbroot
#endif
