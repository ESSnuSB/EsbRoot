/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

/// \file PMTubeGeoCreator.h
/// \brief Utility class for creating simple symmetric PMT configurations
/// \author Peter Christiansen
#ifndef ESBROOT_ESBTOOLS_PMTUBEGEOCREATOR_H
#define ESBROOT_ESBTOOLS_PMTUBEGEOCREATOR_H

#include <TClonesArray.h>

namespace esbroot {
namespace tools {

/// \class PMTubeGeoCreator
/// This is the definition of the PMT. For now it just contains the position and the 
class PMTubeGeoCreator 
{

 public:
  
  /** Default constructor **/
  PMTubeGeoCreator();
  
  /** Constructor with arguments
   *@param wc_radius       radius of WC
   *@param wc_half_length  half lengt of WC
   *@param n_z             number of PMTs along z for barrel
   *@param n_phi           number of PMTs along phi for barrel
   *@param n_r             number of PMT rings along r for endcaps
   *@param d_phi           min dist along ring between PMTs for endcaps
   *@param r               radius of PMT
   **/
  PMTubeGeoCreator(Double_t wc_radius, Double_t wc_half_length, 
		   Int_t n_z, Int_t n_phi, Int_t n_r, Int_t d_phi, 
		   Double_t r);
  
  /** Destructor **/
  virtual ~PMTubeGeoCreator();

  TClonesArray* CreateGeometry();
  
  /** Accessors **/
  Double_t GetRadius() const { return fRadius; }

  /** Modifiers **/
  void SetRadius(Double_t radius) { fRadius = radius; }
   
 protected:
  
  Double_t fWCRadius;       ///< radius of WC cylinder [cm] 
  Double_t fWCHalfLength;   ///< halflength of WC cylinder [cm]
  Int_t    fNZ;             ///< number of PMTs along z for barrel
  Int_t    fNPhi;           ///< number of PMTs along phi for barrel
  Int_t    fNR;             ///< number of PMT rings along r for endcaps
  Double_t fDPhi;           ///< min dist along ring between PMTs for endcaps [cm]
  Double_t fRadius;         ///< radius of all PMTs [cm]
  
  ClassDef(PMTubeGeoCreator, 2);
};
 
}
}

#endif // ESBROOT_ESBTOOLS_PMTUBEGEOCREATOR_H
