#include "EsbGenerators/EsbSuperFGD/FgdGeomAnalyzer.h"
#include "EsbGenerators/EsbSuperFGD/GenieFluxDriver.h"
#include "EsbGeometry/EsbSuperFGD/Names.h"

#include <TGeoNode.h>
#include <TObjArray.h>

#include "FairLogger.h"

#include "Framework/ParticleData/PDGCodeList.h"
#include "Framework/EventGen/GFluxI.h"

#include <sstream>
#include <iostream>

namespace esbroot {
namespace generators {
namespace superfgd {

FgdGeomAnalyzer::FgdGeomAnalyzer(const char* geoConfigFile , TVector3 detPos, TGeoManager* gm)
                : ROOTGeomAnalyzer(gm) ,fdetPos(detPos) , fFluxDirection(0,0,1)
                    , fFgdVol(nullptr), fTopVol(nullptr), fGm(gm) 
{
    fdetectorParams.LoadPartParams(geoConfigFile);
    this->Initialize();

    //ROOTGeomAnalyzer::SetLengthUnits(genie::units::centimeter);
	//ROOTGeomAnalyzer::SetTopVolName((esbroot::geometry::superfgd::fgdnames::superFGDName));
}


FgdGeomAnalyzer::~FgdGeomAnalyzer()
{
    Reset();
}

void FgdGeomAnalyzer::Reset()
{
    if(fGm!=nullptr && fTopVol!=nullptr)
    {
        //ROOTGeomAnalyzer::SetLengthUnits(genie::units::meter);
        //ROOTGeomAnalyzer::SetTopVolName(fTopVol->GetName());
    }
}

const genie::PathLengthList & FgdGeomAnalyzer::ComputePathLengths(const TLorentzVector & x, const TLorentzVector & p)
{
    esbroot::generators::superfgd::GenieFluxDriver* fDriver = dynamic_cast<esbroot::generators::superfgd::GenieFluxDriver*>(fFlux);
    //TLorentzVector tDist = fDriver->AbsPosition();

    // Rescale the coordinates. Genie works in meters, while the Fgd uses centimeters
    static const double CM_TO_M_SCALE = 1/100.;

    const TLorentzVector absPos = fDriver->AbsPosition();
    TVector3 rescaled(absPos.X() * CM_TO_M_SCALE 
                        , absPos.Y()* CM_TO_M_SCALE 
                        , absPos.Z() * CM_TO_M_SCALE );
    TLorentzVector tDist(rescaled,0);
    const genie::PathLengthList& pl = ROOTGeomAnalyzer::ComputePathLengths(tDist, p);

    if(pl.AreAllZero())
    {
        return *flengthList;
    }

    return pl;
}


void FgdGeomAnalyzer::Initialize()
{
    fTopVol = fGm->GetTopVolume();
    fFgdVol = fGm->GetVolume(esbroot::geometry::superfgd::fgdnames::superFGDName);

    const genie::PDGCodeList& pdgList = ROOTGeomAnalyzer::ListOfTargetNuclei();

    map<int,double> tgtmap;
    for(size_t i = 0; i < pdgList.size(); ++i)
    {
        tgtmap[pdgList[i]]=1.0;// We only want the interaction , the vertex we are setting it in the flux generator
    }

    flengthList = new genie::PathLengthList(tgtmap);
}


} //namespace superfgd 
} //namespace generators
} //namespace esbroot