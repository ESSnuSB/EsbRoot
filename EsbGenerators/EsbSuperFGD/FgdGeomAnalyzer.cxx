#include "EsbGenerators/EsbSuperFGD/FgdGeomAnalyzer.h"
#include "EsbGeometry/EsbSuperFGD/Names.h"

#include <TGeoNode.h>
#include <TObjArray.h>

#include "FairLogger.h"

#include <sstream>
#include <iostream>

namespace esbroot {
namespace generators {
namespace superfgd {

FgdGeomAnalyzer::FgdGeomAnalyzer(const char* geoConfigFile , TVector3 detPos, TGeoManager* gm)
                : ROOTGeomAnalyzer(gm) ,fdetPos(detPos) , fFluxDirection(0,0,1)
{
    // fdetectorParams.LoadPartParams(geoConfigFile);
    // Double_t genie_m = genie::units::meter;
    // Double_t genie_cm = genie_m/100.;
    // Double_t decLunit = fdetectorParams.GetLenghtUnit(); // [cm]
    // SetLengthUnits(genie_cm * decLunit);
}

// const genie::PathLengthList& FgdGeomAnalyzer::ComputeMaxPathLengths()
// {
//     Double_t genie_m = genie::units::meter;
//     Double_t genie_cm = genie_m/100.;

//     Double_t xmin = 0., xmax = 0.;
//     Double_t ymin = 0., ymax = 0.;
//     Double_t zmin = 0., zmax = 0.;
//     Double_t lunits = this->LengthUnits();

//     TGeoVolume* topVolume = this->GetGeometry()->GetTopVolume();

//     std::stringstream ss;
//     ss << esbroot::geometry::superfgd::fgdnames::superFGDName << "_1"; // TGeoVolume adds '_1' as suffix to indicate the number of object
//     std::string nodeName = ss.str();
//     const TGeoNode* node = topVolume->GetNode(nodeName.c_str());

//     if(node==nullptr)
//     {
//         ss.clear();
//         ss << "Unable to retrieve volume node with name" << esbroot::geometry::superfgd::fgdnames::superFGDName;
//         LOG(fatal) << ss.str();
//         throw ss.str();
//     }

//     const TGeoVolume* det = node->GetVolume();

//     // Get bounding box of detector volume (in detector units)
//     det->GetShape()->GetAxisRange(1, xmin, xmax);
//     det->GetShape()->GetAxisRange(2, ymin, ymax);
//     det->GetShape()->GetAxisRange(3, zmin, zmax);
    
//     xmin = fdetPos.X() + xmin;
//     xmax = fdetPos.X() + xmax;

//     ymin = fdetPos.Y() + ymin;
//     ymax = fdetPos.Y() + ymax;

//     zmin = fdetPos.Z() + zmin;
//     zmax = fdetPos.Z() + zmax;;

//     // Create farthest point
//     TVector3 farthestDetectorPoint(xmax * genie_cm, ymax * genie_cm, zmax * genie_cm);

//     // set x4 in SI units
//     TLorentzVector x4(farthestDetectorPoint*genie_cm, 0);
//     // set p4 - only the direction matters (but not magnitude/units)
//     TLorentzVector p4(fFluxDirection, 0);

//     const genie::PathLengthList& pl = this->ComputePathLengths(x4, p4);
//     fCurrMaxPathLengthList->Copy(pl);

//     return *fCurrMaxPathLengthList;
// }

} //namespace superfgd 
} //namespace generators
} //namespace esbroot