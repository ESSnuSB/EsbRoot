#ifndef ESBROOT_GENERATOR_FGD_FLUX_DRIVER_H
#define ESBROOT_GENERATOR_FGD_FLUX_DRIVER_H 1 

#include <fstream>
#include <random>
#include <vector>
#include <sstream>

#include "Framework/EventGen/GFluxI.h"
#include "Framework/ParticleData/PDGCodeList.h"

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

#include "TLorentzVector.h"
#include "TRandom3.h"
#include "TVector3.h"

#include "EsbGeometry/EsbSuperFGD/EsbFgdDetectorParameters.h"


namespace esbroot {
namespace generators {
namespace superfgd {

using namespace genie;

class FgdFluxDriver : public GFluxI
                    , public TObject 

{
public:
    // ~ctor
    FgdFluxDriver(const char* geoConfigFile
                  , const char* nuFluxFile
                  , unsigned int seed
                  , TVector3 detPos
                  , Double_t maxEnergy = 2. /* GeV */);

    class FLuxNeutrino
    {
      public:
        FLuxNeutrino(int pdg , Double_t energy, Double_t flux);
        ~FLuxNeutrino();

        FLuxNeutrino(const FLuxNeutrino& fn);
        FLuxNeutrino& operator=(const FLuxNeutrino& fn);

        void SetLowerVal(Double_t lv){fLowerVal = lv;}
        void SetUpperVal(Double_t uv){fUpperVal = uv;}

        Bool_t GetNeutrino(Double_t val, int& pdg, Double_t& energy);

        int GetPdg(){return fpdg;}
        Double_t GetEnergy(){return fEnergy;}
        Double_t GetFlux(){return fFluxValue;}

    private:
        int fpdg;
        Double_t fEnergy;
        Double_t fFluxValue;
        
        Double_t fLowerVal;
        Double_t fUpperVal;
    };

    // Methods implementing the GENIE GFluxI interface
    const PDGCodeList &     FluxParticles (void) { return fPdgCList;} ///< declare list of flux neutrinos that can be generated (for init. purposes)
    double                  MaxEnergy     (void) { return  fMaxEv;}   ///< declare the max flux neutrino energy that can be generated (for init. purposes)


    virtual bool            GenerateNext  (void);                        ///< generate the next flux neutrino (return false in err)
    int                     PdgCode       (void) { return  fpdgCode;}   ///< returns the flux neutrino pdg code
    double                  Weight        (void) { return  1.0;/* No idea what this is */} ///< returns the flux neutrino weight (if any)
    const TLorentzVector&   Momentum      (void) { return  f4momentum;}  ///< returns the flux neutrino 4-momentum
    const TLorentzVector&   Position      (void) { return  f4position;}  ///< returns the flux neutrino 4-position (note: expect SI rather than physical units)
    virtual bool            End           (void) { return  false;}       ///< true if no more flux nu's can be thrown (eg reaching end of beam sim ntuples)
    virtual long int        Index         (void){return fcurrentEvent;}  ///< returns corresponding index for current flux neutrino (e.g. for a flux ntuple returns the current entry number)
    virtual void            Clear            (Option_t * opt   ) {}      ///< reset state variables based on opt
    virtual void            GenerateWeighted (bool gen_weighted) {}      ///< set whether to generate weighted or unweighted neutrinos

    void                    SetMaxEnergy  (Double_t eMax) {fMaxEv = eMax;}

private:

    /* Detector parameters */
    esbroot::geometry::superfgd::FgdDetectorParameters fdetectorParams;

    /* Uniform random number generators for neutrino flux calculations */
    std::mt19937 fseed;//!<!
    std::uniform_real_distribution<Double_t> fdis;//!<!
    TVector3 fdetPos;


    PDGCodeList fPdgCList;

    // Temp values to store the data from the last interaction
    TLorentzVector f4momentum;
    TLorentzVector f4position;
    int fpdgCode;
    
    Double_t fMaxEv; // in [GeV]
    int fcurrentEvent;

    Double_t f_total_X;
    Double_t f_total_Y;
    Double_t f_total_Z;

    std::string fnuFluXFile;
    std::vector<FLuxNeutrino> fFlux;//!<!

    void CalculateNext4Momentum(Double_t energyOfNeutrino);
    void CalculateNext4position(Double_t rndVal);

    void InitPDGList(void);
    void InitDetectorParams(const char* configFile);
    void Init4Momentum(void);
    void Init4Position(void);

    void ReadNuFluxFile(const char* fluxFile);
    void CalculateProbability();

    ClassDef(FgdFluxDriver,6)
  };

} //namespace superfgd 
} //namespace generators
} //namespace esbroot
  
  #endif