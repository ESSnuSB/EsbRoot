/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ESBROOT_ESBGEOMETRY_FGDDETECTOR_H
#define ESBROOT_ESBGEOMETRY_FGDDETECTOR_H

#include "FairDetector.h"

#include "TVector3.h"
#include "TLorentzVector.h"
#include "EsbGeometry/EsbSuperFGD/EsbDetectorConstruction.h"

class FairVolume;
class TClonesArray;
class G4VPhysicalVolume;

//Inheritance: EsbWCDetector <- FairDetector <- FairModule <- TNamed

namespace esbroot {

namespace data {class FgdDetectorPoint;}; 

namespace geometry {
	class FgdDetector: public FairDetector
	{

	  public:

		/**      Name :  Detector Name
		 *       Active: kTRUE for active detectors (ProcessHits() will be called)
		 *               kFALSE for inactive detectors
		 * 		 geoConfigFile: Full path to the configuration file for the Superfgd geometry
		*/
	    FgdDetector(const char* Name, std::string geoConfigFile, Bool_t Active);

		/**     Constructor using default name
		 * 		geoConfigFile: Full path to the configuration file for the Superfgd geometry
		*/
		FgdDetector(std::string geoConfigFile);

		/**       destructor     */
		virtual ~FgdDetector();

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
		 *       of type EsbFgdDetectorPoint to the clones array
		*/
		data::FgdDetectorPoint* AddHit(Int_t trackID, Int_t detID,
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

		esbroot::geometry::superfgd::FgdDetectorConstruction    fgdConstructor;	   //! SuperFgd Detector Constructor
		/** container for data points */

		TClonesArray*  fFgdDetectorPointCollection;  //! 

		FgdDetector(const FgdDetector&);
		FgdDetector& operator=(const FgdDetector&);

		ClassDef(FgdDetector,2)
	};

}
}

#endif //ESBROOT_ESBGEOMETRY_WCDETECTOR_H
