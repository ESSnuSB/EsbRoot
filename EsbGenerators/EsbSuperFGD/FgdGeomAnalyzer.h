#ifndef ESBROOT_GENERATOR_FGD_ROOTGEOM_ANALYZER_H
#define ESBROOT_GENERATOR_FGD_ROOTGEOM_ANALYZER_H 1

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

#include <Tools/Geometry/ROOTGeomAnalyzer.h>
#include <Framework/EventGen/PathLengthList.h>
#include <Framework/Conventions/Units.h>

#include <TGeoVolume.h>
#include <TVector3.h>
#include <TLorentzVector.h>

namespace esbroot {
namespace generators {
namespace superfgd {

using namespace std;

// Use ROOTGeomAnalyzer since it has implemented the methods
// overwrite only the ComputeMaxPathLength method
class FgdGeomAnalyzer : public genie::geometry::ROOTGeomAnalyzer
                    ,   public TObject  {

public:

    FgdGeomAnalyzer(TGeoManager* gm, const char* geoConfigFile);

    virtual const genie::PathLengthList& ComputeMaxPathLengths();
    void SetNearestSourcePoint(const TVector3& point) { fNearestSourcePoint = point; };

private:
    TVector3 fNearestSourcePoint;//!<!

    ClassDef(FgdGeomAnalyzer,6)
};

} //namespace superfgd 
} //namespace generators
} //namespace esbroot

#endif