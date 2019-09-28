#ifndef ESBROOT_ESBDRECONSTRUCTION_FGD_PDG_FROM_PHOTONS_H
#define ESBROOT_ESBDRECONSTRUCTION_FGD_PDG_FROM_PHOTONS_H

#include "TObject.h"
#include <TVector3.h>
#include <vector>
#include <algorithm>

namespace esbroot {
namespace reconstruction {
namespace superfgd {


class PdgFromPhotons : public TObject
{
public:

    /** Default constructor **/ 
    PdgFromPhotons();
    PdgFromPhotons(Int_t p, Double_t ph, Double_t diff);

    ~PdgFromPhotons();

    PdgFromPhotons(const PdgFromPhotons& p);

    PdgFromPhotons& operator=(const PdgFromPhotons& p);

    Bool_t operator==(const PdgFromPhotons& p);

    Bool_t GetPdg(Double_t val, Int_t& pdgCode);

    Int_t fpdg;
    Double_t favgPhoto;
    Double_t fallowedDiff;

private:

    ClassDef(PdgFromPhotons, 2);
};

} //superfgd
} //reconstruction
} //esbroot

#endif