/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "EsbTools/PMTubeGeoCreator.h"

#include "EsbGeometry/PMTube.h"

#include <TClonesArray.h>
#include <TMath.h>

#include <cmath>

#include <iostream>
using std::cout;
using std::endl;

namespace esbroot {
namespace tools {

// -----   Default constructor   -------------------------------------------
PMTubeGeoCreator::PMTubeGeoCreator()
  : fWCRadius(std::nan("NI")),
    fWCHalfLength(std::nan("NI")),
    fNZ(-1),
    fNPhi(-1),
    fNR(-1),
    fDPhi(std::nan("NI")),
    fRadius(std::nan("NI"))
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
PMTubeGeoCreator::PMTubeGeoCreator(Double_t wc_radius, Double_t wc_half_length, 
				   Int_t n_z, Int_t n_phi, Int_t n_r, 
				   Int_t d_phi, Double_t r)
  : fWCRadius(wc_radius),
    fWCHalfLength(wc_half_length),
    fNZ(n_z),
    fNPhi(n_phi),
    fNR(n_r),
    fDPhi(d_phi),
    fRadius(r)
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
PMTubeGeoCreator::~PMTubeGeoCreator() { }
// -------------------------------------------------------------------------
  
TClonesArray* PMTubeGeoCreator::CreateGeometry()
{
  TClonesArray* pmt_array = nullptr;
  { 
    geometry::PMTube pmtube;
    pmt_array = new TClonesArray(pmtube.Class(), 1000);
  }

  Int_t n_pmt = 0;
  // Create Barrel
  { 
    const Double_t d_z   = 2.0*fWCHalfLength / fNZ;
    const Double_t d_phi = TMath::TwoPi() / fNPhi;
    // Create Barrel
    for(Int_t n_z = 0; n_z < fNZ; n_z++) {
      for(Int_t n_phi = 0; n_phi < fNPhi; n_phi++) {
	
	const Double_t x = fWCRadius*TMath::Cos(n_phi*d_phi);
	const Double_t y = fWCRadius*TMath::Sin(n_phi*d_phi);
	// There is an offset by 0.5 as we want a gap in both ends
	const Double_t z = (n_z+0.5)*d_z - fWCHalfLength;
	new((*pmt_array)[n_pmt]) geometry::PMTube(x, y, z, fRadius);
	n_pmt++;
      }
    }
  }
  
  // Create End Caps
  {
    // Divide by fNR-0.5 because we want the first PMT at (0,0) so it only
    // needs half the length in radius
    const Double_t d_r = fWCRadius / (fNR-0.5);
    for(Int_t n_r = 0; n_r < fNR; n_r++) {
      
      const Double_t r = n_r * d_r;

      // Find put how many PMT detectors to do in phi
      Int_t n_pmt_phi = Int_t(TMath::TwoPi()*r/fDPhi);
      // we want n_pmt_phi to be even to make PMT configuration left-right
      // symmetric
      if(TMath::Odd(n_pmt_phi))
	n_pmt_phi--;
      
      // for n_r == 0 we have detectors at (0, 0, +-fWCHalfLength)
      if(n_r == 0)
	n_pmt_phi = 1;

      // here we should issues a warning and abort if n_pmt_phi == 0!
      if(n_pmt_phi == 0)
	continue;
      
      const Double_t d_phi = TMath::TwoPi() / n_pmt_phi;
      
      for(Int_t n_phi = 0; n_phi < n_pmt_phi; n_phi++) {
	
	const Double_t x = r*TMath::Cos(n_phi*d_phi);
	const Double_t y = r*TMath::Sin(n_phi*d_phi);
	const Double_t z = fWCHalfLength;
	// Place PMTs at both end caps
	new((*pmt_array)[n_pmt]) geometry::PMTube(x, y, z, fRadius);
	n_pmt++;
	new((*pmt_array)[n_pmt]) geometry::PMTube(x, y, -z, fRadius);
	n_pmt++;
      }
    }
  }
      
  return pmt_array;
}

}
}
