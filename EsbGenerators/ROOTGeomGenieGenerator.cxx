#include "EsbGenerators/ROOTGeomGenieGenerator.h"

namespace esbroot{
namespace generators{
	ROOTGeomGenieGenerator::ROOTGeomGenieGenerator(int pdg_nu, double e_nu, TGeoManager* geom, std::string top_vol_name)
	{
		auto mono_flux = std::make_shared<genie::flux::GMonoEnergeticFlux>(e_nu, pdg_nu);
		SetFluxI(std::move(mono_flux));
		
		fGeom = geom;
		fTopVolumeName = top_vol_name;
	}
	
	Bool_t ROOTGeomGenieGenerator::LateInit() {
		fOrigTopVolume = fGeom->GetTopVolume();
		auto root_geom = std::make_shared<genie::geometry::ROOTGeomAnalyzer>(fGeom);
		root_geom->SetTopVolName(fTopVolumeName);
		Configure();
		fTopVolume = fGeom->GetTopVolume();
		fGeom->SetTopVolume(fTopVolume);
		
		return(true);
	}
	
	/* virtual */ Bool_t ROOTGeomGenieGenerator::ReadEvent(FairPrimaryGenerator* primGen) {
		fGeom->SetTopVolume(fTopVolume);
		GenieGenerator::ReadEvent(primGen);
		fGeom->SetTopVolume(fOrigTopVolume);
	}
	
}
}
