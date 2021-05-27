#ifndef ESBROOT_GENERATOR_ROOTGEOMGENIEGENERATOR_H
#define ESBROOT_GENERATOR_ROOTGEOMGENIEGENERATOR_H

#include "EsbGenerators/GenieGenerator.h"

#include <string>

#include <TGeoManager.h>
#include <TGeoVolume.h>

namespace esbroot{
namespace generators{

class ROOTGeomGenieGenerator : public GenieGenerator
{
	public:
	ROOTGeomGenieGenerator(int pdg_nu, double e_nu, TGeoManager* geom, std::string top_vol);

	virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);

	Bool_t LateInit();
	
	private:
	TGeoManager* fGeom;
	std::string fTopVolumeName;
	std::string fOrigTopVolumeName;
	
	TGeoVolume *fTopVolume;
	TGeoVolume *fOrigTopVolume;

	ClassDef(ROOTGeomGenieGenerator, 2);
};

}
}

#endif
