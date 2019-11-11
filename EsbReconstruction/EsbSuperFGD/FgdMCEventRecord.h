#ifndef ESBROOT_ESBDRECONSTRUCTION_FGD_MC_EVENT_RECORDT_H
#define ESBROOT_ESBDRECONSTRUCTION_FGD_MC_EVENT_RECORDT_H

#include <TObject.h>


namespace esbroot {
namespace reconstruction {
namespace superfgd {

class FgdMCEventRecord : public TObject
{
public:
    FgdMCEventRecord();

    /* Copy constructor */
    FgdMCEventRecord(const FgdMCEventRecord& c);

    virtual ~FgdMCEventRecord();

    /* Operators */
    FgdMCEventRecord& operator=(const FgdMCEventRecord& c);

    void SetNuPdg(Int_t pdg){fNu_pdg = pdg;}
    void SetNuE(Double_t e){fNu_E = e;}
    void SetWeakCC(Int_t cc){fIsWeakCC = cc;}
    void SetWeakNC(Int_t nc){fIsWeakNC = nc;}

    Int_t GetNuPdg(void){return fNu_pdg;}
    Double_t GetNuE(void){return fNu_E;}
    Int_t GetWeakCC(void){return fIsWeakCC;}
    Int_t GetWeakNC(void){return fIsWeakNC;}

protected:

    Int_t fNu_pdg;
    Double_t fNu_E;
    Int_t fIsWeakCC;
    Int_t fIsWeakNC;

    ClassDef(FgdMCEventRecord, 2);

};

} //superfgd
} //reconstruction
} //esbroot

#endif // ESBROOT_ESBDRECONSTRUCTION_FGD_MC_GENFIT_H