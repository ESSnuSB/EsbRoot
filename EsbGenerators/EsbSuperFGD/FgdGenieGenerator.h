#ifndef ESBROOT_GENERATOR_FGD_GENIE_GENERATOR_H
#define ESBROOT_GENERATOR_FGD_GENIE_GENERATOR_H 1

#include "EsbGenerators/GenieGenerator.h"

#include <cmath>

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
	FgdGenieGenerator(): GenieGenerator() {};
	
	//! Basic constructor
	FgdGenieGenerator(TGeoManager* gm
						, const char* geoConfigFile
						, const char* nuFluxFile
						, unsigned int seed
						, TLorentzVector const& x4_nu);
	
	//! Implementation of GenieGenerator::Init() which calls the Configure() function.
	virtual Bool_t Init() {return(Configure());};
	
	//! Post processes Genie events to choose vertex position
	virtual void PostProcessEvent(/*IN OUT*/ genie::GHepRecord* event);

private:
	//! 4-position of neutrino interaction vertex
	TLorentzVector fVertexX4 = {std::nan("ni"), std::nan("ni"), std::nan("ni"), std::nan("ni")};
	
	ClassDef(FgdGenieGenerator,2)
};

} //namespace superfgd 
} //namespace generators
} //namespace esbroot

#endif
