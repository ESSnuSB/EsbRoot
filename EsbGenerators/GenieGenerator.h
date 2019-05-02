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

//~ using namespace std;
//~ using namespace genie;
//~ using namespace genie::geometry;
//~ using namespace genie::flux;

namespace esbroot {

namespace generators {

/*! Implementar of the FairRoot generator using the 
    Genie framework for neutrino event generations*/
class GenieGenerator : public FairGenerator
{

public:

	static struct GlobalState_t {
		std::string fGenieTune = "";
		std::string fXsecSplineFileName = "";
		long int fRandomSeed = -1;
	} GlobalState;
	static void InitGlobalState();

	GenieGenerator() {};
	
	template <typename Vec>
	GenieGenerator(int pdg_tgt, int const& pdg_nu, double const& e_nu, Vec const& p_nu, 
			double const& time, Vec const& pos_nu) : fVtxTime(time), fVtxPos(pos_nu)
	{
		assert(GenieGenerator::fGlobalStateInit == true);
		
		auto mono_flux = std::make_shared<genie::flux::GMonoEnergeticFlux>(e_nu, pdg_nu);
		mono_flux->SetDirectionCos(p_nu.x(), p_nu.y(), p_nu.z());
		
		fFluxI = std::move(mono_flux);
		fGeomI = std::make_shared<genie::geometry::PointGeomAnalyzer>(pdg_tgt);
	}
	
	GenieGenerator(genie::GFluxI *fluxI, genie::GeomAnalyzerI *geomI);

  /** Destructor  **/
  virtual ~GenieGenerator();

	//Setters
	void SetFluxI(std::shared_ptr<genie::GFluxI> FluxI) {fFluxI = FluxI;};
	void SetGeomI(std::shared_ptr<genie::GeomAnalyzerI> GeomI) {fGeomI = GeomI;};

	//Getters
	std::shared_ptr<genie::GFluxI> const& GetFluxI() {return(fFluxI);};
	std::shared_ptr<genie::GeomAnalyzerI> const& GetGeomI() {return(fGeomI);};

  /* method which initializes the Genie generator */
  virtual Bool_t Init(void);
	Bool_t IsInit() const {return fIsInit;}

  /* implementation of the base method which generates the vertex particles */
  virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);

  /** Clone this object (used in MT mode only) */
  virtual FairGenerator* CloneGenerator() const;

private:

	//These static members are initialized in the .cxx file so that
	//RootCint does not complain
	static bool fGlobalStateInit;

  /** Driver for generating neutrino events **/
  std::shared_ptr<genie::GMCJDriver> fmcj_driver;
    
  /** Initializes the Flux driver for GMCJDriver **/
  std::shared_ptr<genie::GFluxI> fFluxI;
   
  /** Used in GMCJDriver initialization to select geometry **/
  std::shared_ptr<genie::GeomAnalyzerI> fGeomI;

	double fVtxTime;
	TVector3 fVtxPos;

  /* checks if the Init method has been called */
  Bool_t fIsInit;

  ClassDef(GenieGenerator,5)
};

} //namespace generators

} //namespace esbroot
#endif
