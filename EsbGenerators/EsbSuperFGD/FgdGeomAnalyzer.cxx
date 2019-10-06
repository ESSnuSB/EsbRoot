#include "EsbGenerators/EsbSuperFGD/FgdGeomAnalyzer.h"
#include "EsbGeometry/EsbSuperFGD/Names.h"

#include <TGeoNode.h>

namespace esbroot {
namespace generators {
namespace superfgd {

FgdGeomAnalyzer::FgdGeomAnalyzer(const char* geoConfigFile , TGeoManager* gm, TGeoVolume* geoVol)
                : ROOTGeomAnalyzer(gm) , fGeo(geoVol)
{
    fdetectorParams.LoadPartParams(geoConfigFile);
    SetLengthUnits(fdetectorParams.GetLenghtUnit());
}

const genie::PathLengthList& FgdGeomAnalyzer::ComputeMaxPathLengths()
{
    Double_t xmin = 0., xmax = 0.;
    Double_t ymin = 0., ymax = 0.;
    Double_t zmin = 0., zmax = 0.;
    Double_t lunits = this->LengthUnits();

    TGeoVolume* topVolume = this->GetGeometry()->GetTopVolume();

    const TGeoNode* node = topVolume->GetNode(esbroot::geometry::superfgd::fgdnames::superFGDName );

    if(node!=nullptr)
    {
        const TGeoVolume* det = node->GetVolume();

        // Get bounding box of detector volume (in detector units)
        det->GetShape()->GetAxisRange(1, xmin, xmax);
        det->GetShape()->GetAxisRange(2, ymin, ymax);
        det->GetShape()->GetAxisRange(3, zmin, zmax);
    }
    else if(fGeo!=nullptr)
    {
        // Get bounding box of detector volume (in detector units)
        fGeo->GetShape()->GetAxisRange(1, xmin, xmax);
        fGeo->GetShape()->GetAxisRange(2, ymin, ymax);
        fGeo->GetShape()->GetAxisRange(3, zmin, zmax);
    }
    else
    {
        Double_t lunit = fdetectorParams.GetLenghtUnit(); // [cm]

        Double_t step_X  = fdetectorParams.ParamAsDouble(esbroot::geometry::superfgd::DP::length_X) * lunit;
        Double_t step_Y  = fdetectorParams.ParamAsDouble(esbroot::geometry::superfgd::DP::length_Y) * lunit;
        Double_t step_Z  = fdetectorParams.ParamAsDouble(esbroot::geometry::superfgd::DP::length_Z) * lunit;

        Int_t bin_X = fdetectorParams.ParamAsInt(esbroot::geometry::superfgd::DP::number_cubes_X);
        Int_t bin_Y = fdetectorParams.ParamAsInt(esbroot::geometry::superfgd::DP::number_cubes_Y);
        Int_t bin_Z = fdetectorParams.ParamAsInt(esbroot::geometry::superfgd::DP::number_cubes_Z);

        xmax = bin_X * step_X;
        ymax = bin_Y * step_Y;
        zmax = bin_Z * step_Z;
    }
    

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