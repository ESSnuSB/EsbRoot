#include "EsbReconstruction/EsbSuperFGD/FgdReconHit.h"



namespace esbroot {
namespace reconstruction {
namespace superfgd {

ReconHit::ReconHit()
    :     fIsVisited(false)
        , fIsLeaf(false)
        , fZaxisAngle(0.)
        , fChangeAngle(0.)
{
}

ReconHit::ReconHit(TVector3 mppcLoc
            , TVector3 hitPosition
            , TVector3 photons
            , Double_t time
            , TVector3 mom
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
        , fmom(mom)
        , fpdg(pdg)
        , ftrackId(trackId)
        , fph1(ph1)
        , fmppc1(mppc1)
        , fph2(ph2)
        , fmppc2(mppc2)
        , fIsVisited(false)
        , fIsLeaf(false)
        , fZaxisAngle(0.)
        , fChangeAngle(0.)
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
        , fmom(c.fmom)
        , fpdg(c.fpdg)
        , ftrackId(c.ftrackId)
        , fAllHits(c.fAllHits)
        , fIsVisited(c.fIsVisited)
        , fIsLeaf(c.fIsLeaf)
        , fZaxisAngle(c.fZaxisAngle)
        , fChangeAngle(c.fChangeAngle)
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
    fmom = c.fmom;
    fpdg = c.fpdg;
    ftrackId = c.ftrackId;
    fAllHits = c.fAllHits;
    fIsVisited = c.fIsVisited;
    fIsLeaf = c.fIsLeaf;
    fZaxisAngle = c.fZaxisAngle;
    return *this;
}

Bool_t ReconHit::operator==(const ReconHit& c)
{
    return (this->fHitPos.X() == c.fHitPos.X()
            && this->fHitPos.Y() == c.fHitPos.Y()
            && this->fHitPos.Z() == c.fHitPos.Z());
}


TVector3 ReconHit::operator-(const ReconHit& c)
{
    Int_t&& x = this->fmppcLoc.X()-c.fmppcLoc.X();
    Int_t&& y = this->fmppcLoc.Y()-c.fmppcLoc.Y();
    Int_t&& z = this->fmppcLoc.Z()-c.fmppcLoc.Z();
    return TVector3(x,y,z);
}

} //superfgd
} //reconstruction
} //esbroot