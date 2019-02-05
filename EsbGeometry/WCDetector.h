/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ESBROOT_ESBGEOMETRY_WCDETECTOR_H
#define ESBROOT_ESBGEOMETRY_WCDETECTOR_H

#include "FairDetector.h"

#include "TVector3.h"
#include "TLorentzVector.h"

class FairVolume;
class TClonesArray;

//Inheritance: EsbWCDetector <- FairDetector <- FairModule <- TNamed

namespace esbroot {
class data::WCDetectorPoint;

namespace geometry {
	class WCDetector: public FairDetector
	{

	  public:

		/**      Name :  Detector Name
		 *       Active: kTRUE for active detectors (ProcessHits() will be called)
		 *               kFALSE for inactive detectors
		*/
	  WCDetector(const char* Name, Double_t wcRadius, Double_t wcHalflength, Bool_t Active);

		/**      default constructor    */
		WCDetector();

		/**       destructor     */
		virtual ~WCDetector();

		/**      Initialization of the detector is done here    */
		virtual void   Initialize();

		/**       this method is called for each step during simulation
		 *       (see FairMCApplication::Stepping())
		*/
		virtual Bool_t ProcessHits( FairVolume* v=0);

		/**       Registers the produced collections in FAIRRootManager.     */
		virtual void   Register();

		/** Gets the produced collections */
		virtual TClonesArray* GetCollection(Int_t iColl) const ;

		/**      has to be called after each event to reset the containers      */
		virtual void   Reset();

		/**      Create the detector geometry        */
		void ConstructGeometry();



		/**      This method is an example of how to add your own point
		 *       of type EsbWCDetectorPoint to the clones array
		*/
		data::WCDetectorPoint* AddHit(Int_t trackID, Int_t detID,
								 TVector3 pos, TVector3 mom,
								 Double_t time);

		/** The following methods can be implemented if you need to make
		 *  any optional action in your detector during the transport.
		*/

		virtual void   SetSpecialPhysicsCuts() {;}
		virtual void   EndOfEvent();
		virtual void   FinishPrimary() {;}
		virtual void   FinishRun() {;}
		virtual void   BeginPrimary() {;}
		virtual void   PostTrack() {;}
		virtual void   PreTrack() {;}
		virtual void   BeginEvent() {;}


	  private:

		/** Track information to be stored until the track leaves the
		active volume.
		*/
		Int_t          fTrackID;           //!  track index
		Int_t          fVolumeID;          //!  volume id
		TLorentzVector fPos;               //!  position at entrance
		TLorentzVector fMom;               //!  momentum at entrance
		Double32_t     fTime;              //!  time
		Double32_t     fLength;            //!  length
		Double32_t     fELoss;             //!  energy loss

		Double_t       fWCradius;          // radius of WC cylinder 
		Double_t       fWChalflength;      // halflength of WC cylinder
		
		/** container for data points */

		TClonesArray*  fWCDetectorPointCollection;  //!

		WCDetector(const EsbWCDetector&);
		WCDetector& operator=(const EsbWCDetector&);

		ClassDef(WCDetector,2)
	};

}
}

#endif //ESBROOT_ESBGEOMETRY_WCDETECTOR_H
