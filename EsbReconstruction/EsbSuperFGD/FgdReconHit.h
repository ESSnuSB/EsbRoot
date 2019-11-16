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
            , TVector3 mom
            , TVector3 momExit
            , Double_t trackLength
            , Int_t pdg
            , Int_t trackId
            , Double_t edep
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
        return fAllHits.empty(); 
    } 


    TVector3 fmppcLoc;
    TVector3 fHitPos;
    TVector3 fphotons;
    TVector3 fph1;
    TVector3 fmppc1;
    TVector3 fph2;
    TVector3 fmppc2;
    Double_t ftime;
    TVector3 fmom;
    TVector3 fmomExit;
    Double_t ftrackLength;
    Double_t fEdep;
    Int_t fpdg;
    Int_t ftrackId;
    Bool_t fIsLeaf;

    Double_t fZaxisAngle;
    Double_t fChangeAngle;

    Int_t fLocalId;
    Bool_t fIsVisited;
    std::vector<ReconHit*> fAllHits;//!<! 

private:

    ClassDef(ReconHit, 2);
};

} //superfgd
} //reconstruction
} //esbroot

#endif