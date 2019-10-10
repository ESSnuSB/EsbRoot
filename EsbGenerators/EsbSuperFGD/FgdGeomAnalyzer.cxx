#include "EsbGenerators/EsbSuperFGD/FgdGeomAnalyzer.h"
#include "EsbGeometry/EsbSuperFGD/Names.h"

#include <TGeoNode.h>
#include <TObjArray.h>

#include "FairLogger.h"

#include "Framework/ParticleData/PDGCodeList.h"

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
    Initialize();

    ROOTGeomAnalyzer::SetLengthUnits(genie::units::centimeter);
	ROOTGeomAnalyzer::SetTopVolName((esbroot::geometry::superfgd::fgdnames::superFGDName));
}


FgdGeomAnalyzer::~FgdGeomAnalyzer()
{
    Reset();
}

void FgdGeomAnalyzer::Reset()
{
    if(fGm!=nullptr && fTopVol!=nullptr)
    {
        ROOTGeomAnalyzer::SetLengthUnits(genie::units::meter);
        ROOTGeomAnalyzer::SetTopVolName(fTopVol->GetName());
    }
}

const genie::PathLengthList & FgdGeomAnalyzer::ComputePathLengths(const TLorentzVector & x, const TLorentzVector & p)
{
  return *flengthList;
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