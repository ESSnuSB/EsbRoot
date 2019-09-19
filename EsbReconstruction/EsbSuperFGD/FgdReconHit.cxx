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
        , fIsVisited(false)
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
        , fIsVisited(c.fIsVisited)
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
    fIsVisited = c.fIsVisited;
    return *this;
}

Bool_t ReconHit::operator==(const ReconHit& c)
{
    return (this->fHitPos.X() == c.fHitPos.X()
            && this->fHitPos.Y() == c.fHitPos.Y()
            && this->fHitPos.Z() == c.fHitPos.Z());
}

// return next if:
//  a) It is a leaf -> there is only one neightbour cube
//  b) It is empty -> no hits near it
//  c) It has more than 2 neightbour hits - it is in a vertex or a complicated interavtion
//      and the track cannot be extracted.
// bool ReconHit::GetNext(Int_t& previousId, Int_t& nextId)
// {
//     if(fLocalHits.size()==1 && previousId!=fLocalHits[0])
//     {
//         nextId = fLocalHits[0];
//         return true;
//     }

//     // The previous Id has to be in the LocalHits
//     // Since there are only two hits, 1 is the previous return teh other one
//     if(fLocalHits.size()==2 && std::find(fLocalHits.begin(), fLocalHits.end(),previousId)!=fLocalHits.end())
//     {
//         nextId = (fLocalHits[0] == previousId) ? fLocalHits[1] : fLocalHits[0];
//         return true;
//     }

//     if(fLocalHits.size()<=2
//         && fLocalEdges.size()==1 
//         && previousId!=fLocalEdges[0])
//     {
//         nextId = fLocalEdges[0];
//         return true;
//     }

//     if(fLocalHits.size()<=2 
//         && fLocalCorner.size()==1 
//         && previousId!=fLocalCorner[0])
//     {
//         nextId = fLocalCorner[0];
//         return true;
//     }

//     // if(fLocalHits.size()>2 && std::find(fLocalHits.begin(), fLocalHits.end(),previousId)!=fLocalHits.end())
//     // {
//     //     Int_t count(0);
//     //     for(Int_t n =0; n < fLocalHits.size(); ++n)
//     //     {
//     //         if(!hits[fLocalHits[n]].fIsVisited)
//     //         {
//     //             ++count;
//     //             nextId=fLocalHits[n];
//     //         }
//     //     }
//     //     nextId = (fLocalHits[0] == previousId) ? fLocalHits[1] : fLocalHits[0];
//     //     return (count==1);
//     // }

//     return false;
// }


} //superfgd
} //reconstruction
} //esbroot