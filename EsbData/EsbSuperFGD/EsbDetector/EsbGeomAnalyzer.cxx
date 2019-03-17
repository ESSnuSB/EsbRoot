#include "EsbData/EsbSuperFGD/EsbDetector/EsbGeomAnalyzer.h"

namespace esbroot {

namespace data {

namespace superfgd {

namespace detector {

GeomAnalyzer::GeomAnalyzer(TGeoManager* gm)
:ROOTGeomAnalyzer(gm) 
{
    // Set defaults
    //SetLengthUnits(genie::units::meter/CLHEP::meter);
    //SetDensityUnits(genie::units::kg_m3/(CLHEP::kg/CLHEP::m3));
    //SetNearestSourcePoint(TVector3(0, 0, -100*CLHEP::meter));
}

const genie::PathLengthList& GeomAnalyzer::ComputeMaxPathLengths()
{
    double xmin = 0., xmax = 0.;
    double ymin = 0., ymax = 0.;
    double zmin = 0., zmax = 0.;
    double lunits = this->LengthUnits();

    TGeoVolume* det = this->GetGeometry()->GetTopVolume();

    // Get bounding box of detector volume (in detector units)
    det->GetShape()->GetAxisRange(1, xmin, xmax);
    det->GetShape()->GetAxisRange(2, ymin, ymax);
    det->GetShape()->GetAxisRange(3, zmin, zmax);

    // Create farthest point
    TVector3 farthestDetectorPoint(xmax, ymax, zmax);

    // set x4 in SI units
    TLorentzVector x4(farthestDetectorPoint*lunits, 0);
    // set p4 - only the direction matters (but not magnitude/units)
    TLorentzVector p4(fNearestSourcePoint - farthestDetectorPoint, 0);

    const genie::PathLengthList& pl = this->ComputePathLengths(x4, p4);
    fCurrMaxPathLengthList->Copy(pl);

    return *fCurrMaxPathLengthList;
}

}   // detector
}   // superfgd
}   // data
}   // esbroot