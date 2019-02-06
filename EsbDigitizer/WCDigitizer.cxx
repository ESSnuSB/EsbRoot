#include "WCDigitizer.h"

#include <TClonesArray.h>
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
  fHitArray(nullptr)
{ 
}
// -------------------------------------------------------------------------

// -----   Constructor   -------------------------------------------
WCDigitizer::WCDigitizer(const char* name, TClonesArray* pmt_array, Int_t i_verbose) :
  FairTask(name, i_verbose),
  fPhotonArray(nullptr),
  fPMTubeArray(pmt_array),
  fHitArray(nullptr)
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
      
      // Here we should check that the photon produces a hit (to be done)
      
      // Now we know that we have a hit
      pmt_charge++;
      pmt_time += photon->GetTime();
    } // end loop over photons
    
    // decide if we make a hit or not
    if(pmt_time > 0) {
      pmt_time /= pmt_charge; 
      
      new((*fHitArray)[n_pmt_hits]) 
	data::PMTubeHit(pmt->GetX(), pmt->GetY(), pmt->GetZ(),
			pmt->GetRadius(), pmt_charge, pmt_time);
      n_pmt_hits++;
    }

  } // end loop over PMTs
}
// -------------------------------------------------------------------------

}
}
