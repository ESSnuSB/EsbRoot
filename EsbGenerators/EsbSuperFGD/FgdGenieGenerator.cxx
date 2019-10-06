#include "EsbGenerators/EsbSuperFGD/FgdGenieGenerator.h"
#include "EsbGenerators/EsbSuperFGD/FgdFluxDriver.h"
#include "EsbGenerators/EsbSuperFGD/FgdGeomAnalyzer.h"

#include "FairLogger.h"

namespace esbroot {
namespace generators {
namespace superfgd {

FgdGenieGenerator::FgdGenieGenerator()
	: GenieGenerator()
{
}

FgdGenieGenerator::~FgdGenieGenerator()
{
}

FgdGenieGenerator::FgdGenieGenerator(const char* geoConfigFile
									, const char* nuFluxFile
									, unsigned int seed
									, TLorentzVector const& x4_nu
									, TGeoManager* gm
									, TGeoVolume* geoVol)
	 : GenieGenerator()
	 	, fgeoConfigFile(geoConfigFile)
		, fnuFluxFile(nuFluxFile)
		, fseed(seed)
		, fVertexX4(x4_nu)
		, fgm(gm)
		, fGeo(geoVol)
{
}


void FgdGenieGenerator::PostProcessEvent(/*IN OUT*/ genie::GHepRecord* event)
{
	// Move each vertex to the global geometry coordinate system
	TLorentzVector* v = event->Vertex();
	
	*v += fVertexX4;		
	event->SetVertex(*v);
}

Bool_t FgdGenieGenerator::Configure()
{
	if(fgm==nullptr)
	{
		fgm = gGeoManager;
	}

	SetFluxI(std::make_shared<FgdFluxDriver>(fgeoConfigFile.c_str(), fnuFluxFile.c_str(), fseed));
	SetGeomI(std::make_shared<FgdGeomAnalyzer>(fgeoConfigFile.c_str(), fgm, fGeo));

	GenieGenerator::Configure();
}

} //namespace superfgd 
} //namespace generators
} //namespace esbroot
