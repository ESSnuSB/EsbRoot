#include "EsbReconstruction/EsbSuperFGD/PdgFromPhotons.h"



namespace esbroot {
namespace reconstruction {
namespace superfgd {

PdgFromPhotons::PdgFromPhotons()
{
}

PdgFromPhotons::PdgFromPhotons(Int_t p, Double_t ph, Double_t diff)
: fpdg(p), favgPhoto(ph) , fallowedDiff(diff)
{
}

PdgFromPhotons::~PdgFromPhotons()
{
}

PdgFromPhotons::PdgFromPhotons(const PdgFromPhotons& p)
: fpdg(p.fpdg), favgPhoto(p.favgPhoto) , fallowedDiff(p.fallowedDiff)
{
}


PdgFromPhotons& PdgFromPhotons::operator=(const PdgFromPhotons& p)
{
    fpdg = p.fpdg;
    favgPhoto = p.favgPhoto;
    fallowedDiff = p.fallowedDiff;
}

Bool_t PdgFromPhotons::operator==(const PdgFromPhotons& p)
{
    return (fpdg==p.fpdg);
}

Bool_t PdgFromPhotons::GetPdg(Double_t val, Int_t& pdgCode)
{
    Bool_t result(false);
    Double_t lower = favgPhoto - fallowedDiff;
    lower = (lower < 0 ) ? 0 : lower;
    Double_t upper = favgPhoto + fallowedDiff;

    if(val >=lower && val<=upper)
    {
        pdgCode = fpdg;
        result = true;
    }
    return result;
}


} //superfgd
} //reconstruction
} //esbroot