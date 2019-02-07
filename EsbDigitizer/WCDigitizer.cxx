#include "WCDigitizer.h"

#include <TClonesArray.h>
#include <TSpline.h>
#include <TRandom.h>

#include <FairRootManager.h>

#include "EsbData/WCDetectorPoint.h"
#include "EsbGeometry/PMTube.h"
#include "EsbData/PMTubeHit.h"

#include <iostream>
using std::cout;
using std::endl;

namespace esbroot {
namespace digitizer {

// -----   Default constructor   -------------------------------------------
WCDigitizer::WCDigitizer() :
  FairTask(),
  fPhotonArray(nullptr),
  fPMTubeArray(nullptr),
  fHitArray(nullptr),
  fQEffSpline(nullptr)
{ 
}
// -------------------------------------------------------------------------

// -----   Constructor   -------------------------------------------
WCDigitizer::WCDigitizer(const char* name, TClonesArray* pmt_array, Int_t i_verbose) :
  FairTask(name, i_verbose),
  fPhotonArray(nullptr),
  fPMTubeArray(pmt_array),
  fHitArray(nullptr),
  fQEffSpline(nullptr)
{ 
  fPMTubeArray->Sort();
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
WCDigitizer::~WCDigitizer() 
{
  if(fHitArray) {
    fHitArray->Delete();
    delete fHitArray;
  }
}
// -------------------------------------------------------------------------



// -----   Public method Init   --------------------------------------------
InitStatus WCDigitizer::Init() {

  // Get RootManager
  FairRootManager* manager = FairRootManager::Instance();
  if ( ! manager ) {
    cout << "-E- WCDigitizer::Init: "
	 << "FairRootManager not instantised!" << endl;
    return kFATAL;
  }

  // Get input array
  // TBD: ideally the name of this array should be defined a common place
  // (it is also used by EsbGeometry/WCDetector)
  fPhotonArray = (TClonesArray*) manager->GetObject("EsbWCDetectorPoint");
  if (!fPhotonArray) {
    cout << "-W- WCDigitizer::Init: "
	 << "No Photon array!" << endl;
    return kERROR;
  }

  // Create and register output array
  fHitArray = new TClonesArray(data::PMTubeHit::Class(), 1000);
  manager->Register("PMTubeHit", "WCDetector", fHitArray, kTRUE);

  // Create the MEMPHYS spline if no other spline is given
  if(!fQEffSpline) {

    // Taken from MEMPHYS code StackingAction.cpp:
    const Int_t nPoints = 20;
    Double_t wavelength[nPoints] = 
      { 280., 300., 320., 340., 360., 380., 400., 420., 440., 460.,  
	480., 500., 520., 540., 560., 580., 600., 620., 640., 660.};
    
    Double_t qEfficiency[nPoints] =
      { 0.00, .0139, .0854, .169,  .203,  .206,  .211,   .202,   .188,   .167, 
	.140,  .116, .0806, .0432, .0265, .0146, .00756, .00508, .00158, 0.00};
    
    fQEffSpline = new TSpline3("q_eff", wavelength, qEfficiency, nPoints);
  }
  
  cout << "-I- WCDigitizer: Intialisation successfull " << kSUCCESS<< endl;
  return kSUCCESS;
}


// -------------------------------------------------------------------------



// -----   Public method Exec   --------------------------------------------
void WCDigitizer::Exec(Option_t* opt) {
  
  // Reset output array
  fHitArray->Delete();
  
  if (!fPhotonArray) 
    Fatal("Exec", "No photon array");

  // sort the photon array so that we know that for index i > j, then fZ(i) >
  // fZ(j)
  fPhotonArray->Sort();

  const Int_t n_photons = fPhotonArray->GetEntries();
  const Int_t n_pmts    = fPMTubeArray->GetEntries();

  Int_t first_photon = 0;
 
  Int_t n_pmt_hits = 0;

  for (Int_t i = 0; i < n_pmts; i++) {
    
    geometry::PMTube* pmt = 
      (geometry::PMTube*)fPMTubeArray->At(i);
    
    Double_t pmt_charge = 0;
    Double_t pmt_time   = 0;
    // Loop over photon array
    for(Int_t n = first_photon; n < n_photons; n++) {
      
      data::WCDetectorPoint* photon = 
	(data::WCDetectorPoint*)fPhotonArray->At(n);
      
      const Double_t d_z = photon->GetZ() - pmt->GetZ();
      const Double_t r   = pmt->GetRadius();
      
      if(d_z > r)
	break; // the rest of the photons will, because they are sorted in fZ,
	       // all be unable to match the PMT
      if(-d_z> r) {
	first_photon = n+1; // the previous photons will not be able to match
			   // any of the next PMTs because the PMTs are also
			   // sorted according to fZ
	continue;
      }

      const Double_t d_x = photon->GetX() - pmt->GetX();
      const Double_t d_y = photon->GetY() - pmt->GetY();

      if(d_x*d_x + d_y*d_y + d_z*d_z > r*r)
	continue; // photon id not hit PMT

      if(kFALSE) { // Old primitive diitization
	
	// Now we know that we have a hit
	pmt_charge++;
	pmt_time += photon->GetTime();
      } else { 
	// MEMPHYS like digitization, inspired/Taken from WCDigitizer.cpp
	
	// First check that the photon produces a photo electron

	// Calculate wavelength:
	// lambda = hc/E ~ 2*pi*200eV*nm / E (in eV)
	const Double_t E = photon->GetP() * 1e9; // [eV]
	const Double_t wavelength = TMath::TwoPi()*197.33/E; // [nm]
	if(wavelength<fQEffSpline->GetXmin() ||
	   wavelength>fQEffSpline->GetXmax())
	  continue; // no photo electron
	if(gRandom->Rndm()>fQEffSpline->Eval(wavelength))
	  continue; // no photo electron
	
	// Now we know that we have a photo electron
	pmt_charge++;
	if(pmt_charge==1) { // first photo electron
	  
	  pmt_time = photon->GetTime();
	} else {
	  if(pmt_time > photon->GetTime())
	    pmt_time = photon->GetTime();
	}
      }
    } // end loop over photons
    
    // decide if we make a digit or not
    if(pmt_charge == 0)
      continue;

    if(kFALSE) { // primitive weighted average digitization
      pmt_time /= pmt_charge; 
    } else { // MEMPHYS Like digitization
      
      { // Smear the charge 
	// Peter: I modified the code here to require that the final signal is
	// > 0.1. One could instead have some threshold.
	Double_t pmt_charge_smeared = 0;
	do {
	  pmt_charge_smeared = 
	    gRandom->Gaus(pmt_charge, TMath::Sqrt(pmt_charge));
	} while (pmt_charge_smeared < 0.1 );
	pmt_charge = pmt_charge_smeared;
      }
      
      const Double_t timingConstant = 1.890; // MEMPHYS (12"?)
      const Double_t Q = (pmt_charge > 0.5) ? pmt_charge : 0.5;      
      Double_t timingResolution = 0.33 + TMath::Sqrt(timingConstant / Q);
      if (timingResolution < 0.58) 
	timingResolution=0.58;
      pmt_time += gRandom->Gaus(0, timingResolution);
    }
    
    new((*fHitArray)[n_pmt_hits]) 
      data::PMTubeHit(pmt->GetX(), pmt->GetY(), pmt->GetZ(),
		      pmt->GetRadius(), pmt_charge, pmt_time);
    n_pmt_hits++;
  } // end loop over PMTs
}
  // -------------------------------------------------------------------------
  
}
}
