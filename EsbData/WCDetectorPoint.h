/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ESBROOT_WCDETECTOR_POINT_H
#define ESBROOT_WCDETECTOR_POINT_H 1


#include "FairMCPoint.h"

#include "TObject.h"
#include "TVector3.h"

namespace esbroot {

namespace data {

class WCDetectorPoint : public FairMCPoint
{

  public:

    /** Default constructor **/
    WCDetectorPoint();


    /** Constructor with arguments
     *@param trackID  Index of MCTrack
     *@param detID    Detector ID
     *@param pos      Ccoordinates at entrance to active volume [cm]
     *@param mom      Momentum of track at entrance [GeV]
     *@param tof      Time since event start [ns]
     **/
    WCDetectorPoint(Int_t trackID, Int_t detID, TVector3 pos, TVector3 mom,
		       Double_t tof);
    
    /** Destructor **/
    virtual ~WCDetectorPoint();


    Bool_t IsSortable() const { return kTRUE; }

    /// To be able to sort the photon hits according to their z position
    Int_t Compare(const TObject *obj) const;
    
    /** Output to screen **/
    virtual void Print(const Option_t* opt) const;

  private:
    /** Copy constructor **/
    WCDetectorPoint(const WCDetectorPoint& point);
    WCDetectorPoint operator=(const WCDetectorPoint& point);

    ClassDef(WCDetectorPoint,2)
};

}//namespace data

} //namespace esbroot

#endif
