#ifndef ESBROOT_ESBDRECONSTRUCTION_FGD_MUON_RECON_H
#define ESBROOT_ESBDRECONSTRUCTION_FGD_MUON_RECON_H

// EsbRoot headers
#include "EsbReconstruction/EsbSuperFGD/FgdMCGenFitRecon.h"


namespace esbroot {
namespace reconstruction {
namespace superfgd {

class FgdMuonRecon : public FgdMCGenFitRecon
{

 public:

    /** Default constructor **/  
    FgdMuonRecon();

    /** Constructor with argument
     *@param name       Name of task
    *@param geoConfigFile  - Configuration file detector
    *@param mediaFile  - Configuration file for the used mediums
    *@param outputFile  - path to the root file to write the output to
    *@param nuEFile  - path to the file generated from the Fgd generatr containing neutrino energy per event
    *@param verbose  - Verbosity level
    *@param debugLlv - debug level for genfit
    *@param visualize -  to visualize the event using genfit::EventDisplay
    *@param visOption -  option to be passed to genfit::EventDisplay
    **/  
    FgdMuonRecon(const char* name
                , const char* geoConfigFile
                , const char* mediaFile
                , const char* outputFile
                , const char* nuEFile
                , Int_t verbose = 1
                , double debugLlv = 0
                , bool visualize = false
                , std::string visOption ="D");

    /** Destructor **/
    virtual ~FgdMuonRecon();

    virtual InitStatus Init() override;
    virtual void FinishEvent() override;
    virtual void FinishTask() override;

    /** For decendents who want to write some data to output file */
    virtual void WriteOutput( Int_t pdg
                            ,   const TVector3& fitMom
                            ,   const TVector3& mcMom
                            ,   const genfit::Track& fitTrack
                            ,   genfit::FitStatus*& fiStatuStatus) override;

protected:
    
    void ReadEnuFile();

    /** Path to be used to write an output root file **/
    std::string foutputFile;//!<!

    /** Path to the energies of the neutrinos per event **/
    std::string fnuEFile;//!<!

    /** Write results (momentum for each fitted muon track) into root file **/
    TFile* fTfile;//!<!
    TTree* fTtree;//!<!
    std::string fMomOutFile;//!<!
    Double_t fx;//!<!
    Double_t fy;//!<!
    Double_t fz;//!<!
    Double_t fp;//!<!
    Double_t fx_fit;//!<!
    Double_t fy_fit;//!<!
    Double_t fz_fit;//!<!
    Double_t fp_fit;//!<!
    Double_t fpMC_min_pFit;//!<!
    Double_t fE_mu;//!<!
    //Double_t fE_mu_initial;//!<!
    Double_t fxy;//!<!
    Double_t fzMC_min_zFit;//!<!
    Double_t fcosTheta;//!<!
    bool fshouldCOnvergeToExport;//!<!

    Int_t fEventCount;//!<!
    std::vector<Double_t> fEnu;//!<!
        
    ClassDef(FgdMuonRecon, 2);

};

} //superfgd
} //reconstruction
} //esbroot

#endif // ESBROOT_ESBDIGITIZER_FGD_MPPC_DISPLAY_H
