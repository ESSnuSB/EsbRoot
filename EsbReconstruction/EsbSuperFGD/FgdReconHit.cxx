#include "EsbReconstruction/EsbSuperFGD/FgdReconHit.h"



namespace esbroot {
namespace reconstruction {
namespace superfgd {


ReconHit::ReconHit(TVector3 mppcLoc
            , TVector3 hitPosition
            , TVector3 photons
            , Double_t time
            , Int_t pdg
            , Int_t trackId
            , TVector3 ph1
            , TVector3 mppc1
            , TVector3 ph2
            , TVector3 mppc2)
        : fmppcLoc(mppcLoc)
        , fHitPos(hitPosition)
        , fphotons(photons)
        , ftime(time)
        , fpdg(pdg)
        , ftrackId(trackId)
        , fph1(ph1)
        , fmppc1(mppc1)
        , fph2(ph2)
        , fmppc2(mppc2)
{
    
}

ReconHit::~ReconHit()
{

}

ReconHit::ReconHit(const ReconHit& c)
        : fmppcLoc(c.fmppcLoc)
        , fHitPos(c.fHitPos)
        , fphotons(c.fphotons)
        , fph1(c.fph1)
        , fmppc1(c.fmppc1)
        , fph2(c.fph2)
        , fmppc2(c.fmppc2)
        , ftime(c.ftime)
        , fpdg(c.fpdg)
        , ftrackId(c.ftrackId)
        , fLocalHits(c.fLocalHits)
{
    
}

ReconHit& ReconHit::operator=(const ReconHit& c)
{
    fmppcLoc = c.fmppcLoc;
    fHitPos = c.fHitPos;
    fphotons = c.fphotons;
    fph1 = c.fph1;
    fmppc1 = c.fmppc1;
    fph2 = c.fph2;
    fmppc2 = c.fmppc2;
    ftime = c.ftime;
    fpdg = c.fpdg;
    ftrackId = c.ftrackId;
    fLocalHits = c.fLocalHits;
    return *this;
}

bool ReconHit::operator==(const ReconHit& c)
{
    return (this->fHitPos.X() == c.fHitPos.X()
            && this->fHitPos.Y() == c.fHitPos.Y()
            && this->fHitPos.Z() == c.fHitPos.Z());
}


} //superfgd
} //reconstruction
} //esbroot