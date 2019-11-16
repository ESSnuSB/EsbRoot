#ifndef ESBROOT_ESBDRECONSTRUCTION_FGD_MC_EVENT_RECORDT_H
#define ESBROOT_ESBDRECONSTRUCTION_FGD_MC_EVENT_RECORDT_H

#include <TObject.h>
#include <TVector3.h>

#include <vector>
#include <ostream>

namespace esbroot {
namespace reconstruction {
namespace superfgd {

class FgdMCEventRecord : public TObject
{
public:

    FgdMCEventRecord(std::string eventData);

    FgdMCEventRecord();

    /* Copy constructor */
    FgdMCEventRecord(const FgdMCEventRecord& c);

    virtual ~FgdMCEventRecord();

    /* Operators */
    FgdMCEventRecord& operator=(const FgdMCEventRecord& c);

    void SetEventData(std::string data);

    Int_t GetNuPdg(void);
    Double_t GetNuE(void);
    TVector3 GetVertex(void);
    Bool_t IsWeakCC(void);
    Bool_t IsWeakNC(void);
    Bool_t IsQuasiElastic(void);
    const std::vector<std::pair<Int_t, TVector3>>& GetPrimaryParticles();

    Bool_t IsPrimaryLeptonMuon();
    TVector3 GetMuonMom();
    Bool_t IsPrimaryLeptonElectron();
    TVector3 GetElectronMom();

    std::string GetEventData(){return feventData; }

    void PrintData(std::ostream & stream);

protected:

    enum Data : int   // Position in the fDataTokens which value represents which data 
    {
        NEUTRINO_PDG = 0,
        NEUTRINO_ENERGY = 1,
        IS_WEAK_CC = 2,
        IS_WEACK_NC = 3,
        IS_QUASI_ELASTIC = 4,
        VERTEX_POSTION_X = 5,
        VERTEX_POSTION_Y = 6,
        VERTEX_POSTION_Z = 7,
        PRIMARY_PARTICLES = 8   // The primary particles are written as 4 values - 1st pdg then momentum X, momentum Y, momentum Z
                                // all data above 8th position are primary particle data
    };

    void Init();
    void InitMembers();
    std::string feventData;

    
    Int_t fnuPdg;
    Double_t fNuEnergy;
    TVector3 fvertex;
    Bool_t fIsWeakCC;
    Bool_t fIsWeakNC;
    Bool_t fIsQuasiElastic;

    Bool_t fIsPrimaryMuon;
    Bool_t fIsPrimaryElectron;

    TVector3 fPrimaryMuonMom;
    TVector3 fPrimaryElectronMom;

    
    std::vector<std::string> fDataTokens;
    std::vector<std::pair<Int_t, TVector3>> fPrimaryParticles;

    ClassDef(FgdMCEventRecord, 1);

};

} //superfgd
} //reconstruction
} //esbroot

#endif // ESBROOT_ESBDRECONSTRUCTION_FGD_MC_GENFIT_H