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

#include "EsbGeometry/EsbSuperFGD/EsbSuperFGDDetectorConstruction.h"
#include "EsbData/EsbSuperFGD/FgdDetectorPoint.h"

class FairVolume;
class TClonesArray;

namespace esbroot {

namespace geometry {
	class FgdDetector: public FairDetector
	{

	  public:

		/** Default constructor **/
		FgdDetector();

		/**      Name :  Detector Name
		 *       Active: kTRUE for active detectors (ProcessHits() will be called)
		 *               kFALSE for inactive detectors
		 * 		 geoConfigFile: Full path to the configuration file for the Superfgd geometry
		*/
	    FgdDetector(const char* Name, const char* geoConfigFile, double posX, double posY, double posZ, Bool_t Active = true);

		/**     Constructor using default name
		 * 		geoConfigFile: Full path to the configuration file for the Superfgd geometry
		*/
		FgdDetector(const char* geoConfigFile, double posX, double posY, double posZ, Bool_t Active = true);

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

		/**      Initializes a magnetic field for the volume and detector position     */
		void GetMagneticField(Double_t& Bx,Double_t& By, Double_t& Bz);
		void GetMagneticFieldRegion(Double_t& xMin, Double_t& xMax,
                                         Double_t& yMin, Double_t& yMax,
                                         Double_t& zMin, Double_t& zMax);


		/**      This method is an example of how to add your own point
		 *       of type EsbFgdDetectorPoint to the clones array
		*/
		data::superfgd::FgdDetectorPoint* AddHit(Int_t trackID, Int_t detID,
								 TVector3 detectorPos,
								 TVector3 pos, TVector3 posExit, TVector3 mom, TVector3 momExit,
								 Double32_t time, Double32_t edep, Double32_t trackLength, Int_t pdg
								 ,Double32_t trackLengthFromOrigin );

		/** The following methods can be implemented if you need to make
		 *  any optional action in your detector during the transport.
		*/

		virtual void   SetSpecialPhysicsCuts() override;
		virtual void   EndOfEvent();
		virtual void   FinishPrimary() {;}
		virtual void   FinishRun() {;}
		virtual void   BeginPrimary() {;}
		virtual void   PostTrack() {;}
		virtual void   PreTrack() {;}
		virtual void   BeginEvent() {;}

		TGeoVolume* GetVolume(){return fsuperFgdVol;}


	  private:

		/** Initialize amterials used in the detector construction **/
		void DefineMaterials();

		//** indicate that the materials have already been defined
		bool isDefinedMaterials;

		/** Track information to be stored until the track leaves the
		active volume.
		*/
		Int_t          fTrackID;           //!  track index
		Int_t          fVolumeID;          //!  volume id
		TLorentzVector fPos;               //!  position at entrance
		TLorentzVector fPosExit;           //!  position at exit
		TLorentzVector fMom;               //!  momentum at entrance
		TLorentzVector fMomExit;               //!  momentum at entrance
		Double32_t     fTime;              //!  time
		Double32_t     fLength;            //!  length
		Double32_t     fELoss;             //!  energy loss

		// Detector position
		double fposX;
		double fposY;
		double fposZ;

		std::set<Int_t> fpdgCodes;//!<!

		TGeoVolume* fsuperFgdVol;//!<!

		esbroot::geometry::superfgd::SuperFGDDetectorConstruction    fgdConstructor;	   //! SuperFgd Detector Constructor
		/** container for data points */

		TClonesArray*  fFgdDetectorPointCollection;  //! 

		FgdDetector(const FgdDetector&);
		FgdDetector& operator=(const FgdDetector&);		

		ClassDef(FgdDetector,2)
	};

} // namespace geometry
} // esbroot

#endif //ESBROOT_ESBGEOMETRY_WCDETECTOR_H
