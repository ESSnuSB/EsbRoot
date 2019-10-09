#include "EsbGenerators/EsbSuperFGD/FgdGenieGenerator.h"
#include "EsbGenerators/EsbSuperFGD/FgdFluxDriver.h"
#include "EsbGeometry/EsbSuperFGD/Names.h"

#include "FairLogger.h"
#include <Framework/Conventions/Units.h>

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
									, TVector3 detPos
									, TGeoManager* gm)
	 : GenieGenerator()
	 	, fgeoConfigFile(geoConfigFile)
		, fnuFluxFile(nuFluxFile)
		, fseed(seed)
		, fdetPos(detPos)
		, fgm(gm)
{
}


void FgdGenieGenerator::PostProcessEvent(/*IN OUT*/ genie::GHepRecord* event)
{
	// Move each vertex to the global geometry coordinate system
	TLorentzVector* v = event->Vertex();
	
	FgdFluxDriver* fluxD = dynamic_cast<FgdFluxDriver*>(GetFluxI().get());
	if(fluxD!=nullptr)
	{	
		*v += fluxD->AbsPosition();	
		event->SetVertex(*v);
	}
}


Bool_t FgdGenieGenerator::Configure()
{
	if(fgm==nullptr)
	{
		fgm = gGeoManager;
	}

	SetFluxI(std::make_shared<FgdFluxDriver>(fgeoConfigFile.c_str(), fnuFluxFile.c_str(), fseed, fdetPos));
	
	auto geomAnalyzer = std::make_shared<genie::geometry::ROOTGeomAnalyzer>(fgm);
	geomAnalyzer->SetLengthUnits(genie::units::centimeter);
	// geomAnalyzer->SetTopVolName((esbroot::geometry::superfgd::fgdnames::superFGDName));
	SetGeomI(geomAnalyzer);

	GenieGenerator::Configure();
}

} //namespace superfgd 
} //namespace generators
} //namespace esbroot
