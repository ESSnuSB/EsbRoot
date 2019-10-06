#ifndef ESBROOT_GENERATOR_FGD_GENIE_GENERATOR_H
#define ESBROOT_GENERATOR_FGD_GENIE_GENERATOR_H 1

#include "EsbGenerators/GenieGenerator.h"

#include <cmath>

#include <FairRootManager.h>
#include "TGeoManager.h"
#include <TGeoVolume.h>

class TVector3;
class TLorentzVector;

namespace esbroot {
namespace generators {
namespace superfgd {


class FgdGenieGenerator : public GenieGenerator
{
public:
	//! Default constructor
	FgdGenieGenerator();
	
	// ! Basic constructor
	FgdGenieGenerator(const char* geoConfigFile
									, const char* nuFluxFile
									, unsigned int seed
									, TLorentzVector const& x4_nu
									, TGeoManager* gm = nullptr);

	//! Destructor
	~FgdGenieGenerator();
	
	//! Implementation of GenieGenerator::Init() which calls the Configure() function.
	virtual Bool_t Init() {return(Configure());};
	
	//! Post processes Genie events to choose vertex position
	virtual void PostProcessEvent(/*IN OUT*/ genie::GHepRecord* event);

	virtual Bool_t Configure() override; 

private:
	//! 4-position of neutrino interaction vertex
	TLorentzVector fVertexX4 = {std::nan("ni"), std::nan("ni"), std::nan("ni"), std::nan("ni")};

	TGeoManager* fgm;//!<!
	std::string fgeoConfigFile;//!<!
	std::string fnuFluxFile;//!<!
	unsigned int fseed;//!<!
	
	ClassDef(FgdGenieGenerator,2)
};

} //namespace superfgd 
} //namespace generators
} //namespace esbroot

#endif
