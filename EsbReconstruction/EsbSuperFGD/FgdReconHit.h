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

    /** Default constructor **/  
    ReconHit();

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

    Bool_t operator==(const ReconHit& c);

    TVector3 operator-(const ReconHit& c);

    Bool_t IsAlone()
    {
        return fLocalHits.empty(); 
    } 

    Bool_t HasLocalHit(Int_t hitId)
    {
        return (find(fLocalHits.begin(), fLocalHits.end(), hitId)!=fLocalHits.end());
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
    Bool_t fIsLeaf;

    Double_t fZaxisAngle;
    Double_t fChangeAngle;

    Int_t fLocalId;
    Bool_t fIsVisited;
    std::vector<Int_t> fAllHits;//!<! 
    std::vector<Int_t> fLocalHits;//!<! 
    std::vector<Int_t> fLocalEdges;//!<! 
    std::vector<Int_t> fLocalCorner;//!<! 

private:

    ClassDef(ReconHit, 2);
};

} //superfgd
} //reconstruction
} //esbroot

#endif