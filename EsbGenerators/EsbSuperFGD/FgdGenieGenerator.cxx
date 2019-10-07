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
									, TVector3 detPos
									, TGeoManager* gm)
	 : GenieGenerator()
	 	, fgeoConfigFile(geoConfigFile)
		, fnuFluxFile(nuFluxFile)
		, fseed(seed)
		, fVertexX4(x4_nu)
		, fdetPos(detPos)
		, fgm(gm)
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

	SetFluxI(std::make_shared<FgdFluxDriver>(fgeoConfigFile.c_str(), fnuFluxFile.c_str(), fseed, fdetPos));
	// auto mono_flux = std::make_shared<genie::flux::GMonoEnergeticFlux>(1, 14);
	// mono_flux->SetDirectionCos(0, 0, 1);
	// SetFluxI(std::move(mono_flux));



	SetGeomI(std::make_shared<FgdGeomAnalyzer>(fgeoConfigFile.c_str(), fgm));
	// SetGeomI(std::make_shared<genie::geometry::PointGeomAnalyzer>(1000080160));

	GenieGenerator::Configure();
}

} //namespace superfgd 
} //namespace generators
} //namespace esbroot
