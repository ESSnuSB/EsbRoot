/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

/// \file PMTube.h
/// \brief Definition PMT
/// \author Peter Christiansen
#ifndef ESBROOT_ESBGEOMETRY_PMTUBE_H
#define ESBROOT_ESBGEOMETRY_PMTUBE_H

#include <TObject.h>

namespace esbroot {
namespace geometry {

/// \class PMTube
/// This is the definition of the PMT. For now it just contains the position and the 
class PMTube : public TObject
{

 public:
  
  /** Default constructor **/
  PMTube();
  
  /** Constructor with arguments
   *@param x  center of PMT
   *@param y  center of PMT
   *@param z  center of PMT
   *@param r  radius of PMT
   **/
  PMTube(Double_t x, Double_t y, Double_t z, Double_t r);
  
  /** Destructor **/
  virtual ~PMTube();
  
  /// Make ROOT aware that the class is sortable
  Bool_t IsSortable() const { return kTRUE; }

  /// To be able to sort the PMTs according to their z position
  Int_t Compare(const TObject *obj) const;
  
  /** Accessors **/
  Double_t GetX() const      { return fX; }
  Double_t GetY() const      { return fY; }
  Double_t GetZ() const      { return fZ; }
  Double_t GetRadius() const { return fRadius; }

  /** Modifiers **/
  void SetX(Double_t x) { fX = x; }
  void SetY(Double_t y) { fY = y; }
  void SetZ(Double_t z) { fZ = z; }
  void SetRadius(Double_t radius) { fRadius = radius; }
   
  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;
  
 protected:
  
  Double_t fX;      ///< x coordinate of PMT [cm]
  Double_t fY;      ///< y coordinate of PMT [cm]
  Double_t fZ;      ///< z coordinate of PMT [cm]
  Double_t fRadius; ///< Radius of PMT [cm]

  ClassDef(PMTube, 2);
};
 
}
}

#endif // ESBROOT_ESBGEOMETRY_PMTUBE_H
