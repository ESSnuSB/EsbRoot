#include "EsbGenerators/EsbSuperFGD/FgdGeomAnalyzer.h"

#include "EsbGeometry/EsbSuperFGD/EsbFgdDetectorParameters.h"
#include "EsbGeometry/EsbSuperFGD/Names.h"

#include <TGeoNode.h>

namespace esbroot {
namespace generators {
namespace superfgd {

FgdGeomAnalyzer::FgdGeomAnalyzer(TGeoManager* gm, const char* geoConfigFile)
                :ROOTGeomAnalyzer(gm) 
{
    // Set defaults
    //SetLengthUnits(genie::units::meter/CLHEP::meter);
    //SetDensityUnits(genie::units::kg_m3/(CLHEP::kg/CLHEP::m3));
    //SetNearestSourcePoint(TVector3(0, 0, -100*CLHEP::meter));

    esbroot::geometry::superfgd::FgdDetectorParameters detectorParams;
    detectorParams.LoadPartParams(geoConfigFile);
    
    SetLengthUnits(detectorParams.GetLenghtUnit());
}

const genie::PathLengthList& FgdGeomAnalyzer::ComputeMaxPathLengths()
{
    Double_t xmin = 0., xmax = 0.;
    Double_t ymin = 0., ymax = 0.;
    Double_t zmin = 0., zmax = 0.;
    Double_t lunits = this->LengthUnits();

    TGeoVolume* topVolume = this->GetGeometry()->GetTopVolume();

    const TGeoNode* node = topVolume->GetNode(esbroot::geometry::superfgd::fgdnames::superFGDName );

    const TGeoVolume* det = node->GetVolume();

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

} //namespace superfgd 
} //namespace generators
} //namespace esbroot