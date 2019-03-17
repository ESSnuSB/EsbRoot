#ifndef ESBROOT_DATA_SUPERFGD_GEOM_ANALYZER__H
#define ESBROOT_DATA_SUPERFGD_GEOM_ANALYZER__H 1

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

#include <Tools/Geometry/ROOTGeomAnalyzer.h>
#include <Framework/EventGen/PathLengthList.h>
#include <Framework/Conventions/Units.h>

#include <TGeoVolume.h>
#include <TVector3.h>
#include <TLorentzVector.h>

using namespace std;

namespace esbroot {

namespace data {

namespace superfgd {

namespace detector {

// Use ROOTGeomAnalyzer since it has implemented the methods
// overwrite only the ComputeMaxPathLength method
class GeomAnalyzer : public genie::geometry::ROOTGeomAnalyzer  {

public:
    /** Default constructor **/
    GeomAnalyzer(TGeoManager* gm);

    /** Calculate the maximum travel path through the detector   **/
    virtual const genie::PathLengthList& ComputeMaxPathLengths();

    /** Sets the nearest source point
     *@param point - nearest point
     **/
    void SetNearestSourcePoint(const TVector3& point) { fNearestSourcePoint = point; };

private:

    TVector3 fNearestSourcePoint;
};

}   // detector
}   // superfgd
}   // data
}   // esbroot

#endif