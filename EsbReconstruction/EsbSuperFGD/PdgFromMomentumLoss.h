#ifndef ESBROOT_ESBDRECONSTRUCTION_FGD_PDG_FROM_MOMENTUM_LOSS_H
#define ESBROOT_ESBDRECONSTRUCTION_FGD_PDG_FROM_MOMENTUM_LOSS_H

#include "TObject.h"
#include <TVector3.h>
#include <vector>
#include <algorithm>

namespace esbroot {
namespace reconstruction {
namespace superfgd {


class PdgFromMomentumLoss : public TObject
{
public:

    /** Default constructor **/ 
    PdgFromMomentumLoss();
    PdgFromMomentumLoss(Int_t p, Double_t momLoss, Double_t diff);

    ~PdgFromMomentumLoss();

    PdgFromMomentumLoss(const PdgFromMomentumLoss& p);

    PdgFromMomentumLoss& operator=(const PdgFromMomentumLoss& p);

    Bool_t operator==(const PdgFromMomentumLoss& p);

    Bool_t GetPdg(Double_t val, Int_t& pdgCode);

    Int_t fpdg;
    Double_t favgMomLoss;
    Double_t fallowedDiff;

private:

    ClassDef(PdgFromMomentumLoss, 2);
};

} //superfgd
} //reconstruction
} //esbroot

#endif