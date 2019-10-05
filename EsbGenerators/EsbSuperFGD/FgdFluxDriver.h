#ifndef ESBROOT_GENERATOR_FGD_FLUX_DRIVER_H
#define ESBROOT_GENERATOR_FGD_FLUX_DRIVER_H 1 

#include <stdio.h>
#include <iostream>

#include "Framework/EventGen/GFluxI.h"
#include "Framework/ParticleData/PDGCodeList.h"
#include "Framework/ParticleData/PDGCodes.h"

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

#include "TLorentzVector.h"
#include "TRandom3.h"

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
    FgdFluxDriver(const char* geoConfigFile, unsigned int seed);

    // Methods implementing the GENIE GFluxI interface
    const PDGCodeList &     FluxParticles (void) { return ffPdgCList;} ///< declare list of flux neutrinos that can be generated (for init. purposes)
    double                  MaxEnergy     (void) { return  fMaxEv;}   ///< declare the max flux neutrino energy that can be generated (for init. purposes)


    virtual bool            GenerateNext  (void);                        ///< generate the next flux neutrino (return false in err)
    int                     PdgCode       (void) { return  fpdgCode;}   ///< returns the flux neutrino pdg code
    double                  Weight        (void) { return  1.0;/* No idea what this is */} ///< returns the flux neutrino weight (if any)
    const TLorentzVector&   Momentum      (void) { return  f4momentum;}  ///< returns the flux neutrino 4-momentum
    const TLorentzVector&   Position      (void) { return  f4position;}  ///< returns the flux neutrino 4-position (note: expect SI rather than physical units)
    virtual bool            End           (void) { return  fcurrentEvent >=fnumEvents;} ///< true if no more flux nu's can be thrown (eg reaching end of beam sim ntuples)
    virtual long int        Index         (void){return fcurrentEvent;}  ///< returns corresponding index for current flux neutrino (e.g. for a flux ntuple returns the current entry number)
    virtual void            Clear            (Option_t * opt   ) {}       ///< reset state variables based on opt
    virtual void            GenerateWeighted (bool gen_weighted) {}       ///< set whether to generate weighted or unweighted neutrinos








    // Class method
    void                  initDefaultPDGList(void) { ffPdgCList.push_back(genie::kPdgNuMu); }
    void                  addPDGCodeParticle(int pdgCode) { ffPdgCList.push_back(pdgCode); }

    void                  setMaxEnergy     (double maxEv) { fMaxEv=maxEv; }

    Int_t                 getNumEvents     (void) { return fnumEvents; }
    void                  setNumEvents     (int numEvents) { fnumEvents = numEvents; }
    void                  resetLastEvent()  {fcurrentEvent--;}

    void                  setCurrentIndex(int& ind) {fcurrentEvent =  ind;}
    void                  getCurrentIndex(int& ind) {ind = fcurrentEvent;}

    void                  reset(void) {fcurrentEvent=0;}
    void                  setPdgCode(int newPdgCode){fpdgCode=newPdgCode;}

private:

    /* Detector parameters */
    esbroot::geometry::superfgd::FgdDetectorParameters fdetectorParams;

    unsigned int fseed;
    PDGCodeList ffPdgCList;

    // Temp values to store the data from the last interaction
    TLorentzVector f4momentum;
    TLorentzVector f4position;
    int fpdgCode;
    
    double fMaxEv; // in [GeV]
    int fnumEvents; // number of events to generate
    int fcurrentEvent;

    Double_t f_total_X;
    Double_t f_total_Y;
    Double_t f_total_Z;

    void calculateNext4Momentum(void);
    void calculateNext4position(void);
    void calculateNextPdgCode(void);

    ClassDef(FgdFluxDriver,6)
  };

} //namespace superfgd 
} //namespace generators
} //namespace esbroot
  
  #endif