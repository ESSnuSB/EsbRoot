#include "EsbGenerators/EsbSuperFGD/FgdGenieGenerator.h"
#include "EsbGenerators/EsbSuperFGD/FgdFluxDriver.h"
#include "EsbGenerators/EsbSuperFGD/FgdGeomAnalyzer.h"


namespace esbroot {
namespace generators {
namespace superfgd {


FgdGenieGenerator::FgdGenieGenerator(TGeoManager* gm
									, const char* geoConfigFile
									, const char* nuFluxFile
									, unsigned int seed
									, TLorentzVector const& x4_nu)
	 : GenieGenerator(), fVertexX4(x4_nu)
{
	SetFluxI(std::make_shared<FgdFluxDriver>(geoConfigFile, nuFluxFile, seed));
	SetGeomI(std::make_shared<FgdGeomAnalyzer>(gm, geoConfigFile));
}


void FgdGenieGenerator::PostProcessEvent(/*IN OUT*/ genie::GHepRecord* event)
{
	// Move each vertex to the global geometry coordinate system
	TLorentzVector* v = event->Vertex();
	
	*v += fVertexX4;		
	event->SetVertex(*v);
}

} //namespace superfgd 
} //namespace generators
} //namespace esbroot
