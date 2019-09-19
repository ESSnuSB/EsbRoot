#ifndef ESBROOT_ESBDRECONSTRUCTION_FGD_RECONHIT_H
#define ESBROOT_ESBDRECONSTRUCTION_FGD_RECONHIT_H

#include "TObject.h"
#include <TVector3.h>
#include <vector>
#include <algorithm>

namespace esbroot {
namespace reconstruction {
namespace superfgd {

class ReconHit : public TObject
{

public:
    ReconHit(TVector3 mppcLoc
            , TVector3 hitPosition
            , TVector3 photons
            , Double_t time
            , Int_t pdg
            , Int_t trackId
            , TVector3 ph1
            , TVector3 mppc1
            , TVector3 ph2
            , TVector3 mppc2);

    ~ReconHit();

    ReconHit(const ReconHit& c);

    ReconHit& operator=(const ReconHit& c);

    bool operator==(const ReconHit& c);

    bool IsLeaf()
    {
        return (fLocalHits.size()==1); 
    }

    bool IsAlone()
    {
        return fLocalHits.empty(); 
    }

    // return next if:
    //  a) It is a leaf -> there is only one neightbour cube
    //  b) It is empty -> no hits near it
    //  c) It has more than 2 neightbour hits - it is in a vertex or a complicated interavtion
    //      and the track cannot be extracted.
    bool GetNext(Int_t& previousId, Int_t& nextId)
    {
        if(fLocalHits.size()==1 && previousId!=fLocalHits[0])
        {
            nextId = fLocalHits[0];
            return true;
        }

        // The previous Id has to be in the LocalHits
        if(fLocalHits.size()==2 && std::find(fLocalHits.begin(), fLocalHits.end(),previousId)!=fLocalHits.end())
        {
            nextId = (fLocalHits[0] == previousId) ? fLocalHits[1] : fLocalHits[0];
            return true;
        }

        return false;
    }

    
    TVector3 fmppcLoc;
    TVector3 fHitPos;
    TVector3 fphotons;
    TVector3 fph1;
    TVector3 fmppc1;
    TVector3 fph2;
    TVector3 fmppc2;
    Double_t ftime;
    Int_t fpdg;
    Int_t ftrackId;

    Int_t fLocalId;
    Bool_t fIsVisited;
    std::vector<Int_t> fLocalHits;//!<! 

private:

    ClassDef(ReconHit, 2);
};

} //superfgd
} //reconstruction
} //esbroot

#endif