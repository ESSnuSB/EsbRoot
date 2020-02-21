#include "EsbReconstruction/EsbSuperFGD/PdgFromMomentumLoss.h"

namespace esbroot {
namespace reconstruction {
namespace superfgd {

PdgFromMomentumLoss::PdgFromMomentumLoss()
{
}

PdgFromMomentumLoss::PdgFromMomentumLoss(Int_t p, Double_t momLoss, Double_t diff)
: fpdg(p), favgMomLoss(momLoss) , fallowedDiff(diff)
{
}

PdgFromMomentumLoss::~PdgFromMomentumLoss()
{
}

PdgFromMomentumLoss::PdgFromMomentumLoss(const PdgFromMomentumLoss& p)
: fpdg(p.fpdg), favgMomLoss(p.favgMomLoss) , fallowedDiff(p.fallowedDiff)
{
}


PdgFromMomentumLoss& PdgFromMomentumLoss::operator=(const PdgFromMomentumLoss& p)
{
    fpdg = p.fpdg;
    favgMomLoss = p.favgMomLoss;
    fallowedDiff = p.fallowedDiff;
}

Bool_t PdgFromMomentumLoss::operator==(const PdgFromMomentumLoss& p)
{
    return (fpdg==p.fpdg);
}

Bool_t PdgFromMomentumLoss::GetPdg(Double_t val, Int_t& pdgCode)
{
    Bool_t result(false);
    Double_t lower = favgMomLoss - fallowedDiff;
    lower = (lower < 0 ) ? 0 : lower;
    Double_t upper = favgMomLoss + fallowedDiff;

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