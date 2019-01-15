/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ESBWCDETECTORPOINT_H
#define ESBWCDETECTORPOINT_H 1


#include "FairMCPoint.h"

#include "TObject.h"
#include "TVector3.h"

class EsbWCdetectorPoint : public FairMCPoint
{

  public:

    /** Default constructor **/
    EsbWCdetectorPoint();


    /** Constructor with arguments
     *@param trackID  Index of MCTrack
     *@param detID    Detector ID
     *@param pos      Ccoordinates at entrance to active volume [cm]
     *@param mom      Momentum of track at entrance [GeV]
     *@param tof      Time since event start [ns]
     **/
    EsbWCdetectorPoint(Int_t trackID, Int_t detID, TVector3 pos, TVector3 mom,
		       Double_t tof);
    
    /** Destructor **/
    virtual ~EsbWCdetectorPoint();
    
    /** Output to screen **/
    virtual void Print(const Option_t* opt) const;

  private:
    /** Copy constructor **/
    EsbWCdetectorPoint(const EsbWCdetectorPoint& point);
    EsbWCdetectorPoint operator=(const EsbWCdetectorPoint& point);

    ClassDef(EsbWCdetectorPoint,1)
};

#endif
