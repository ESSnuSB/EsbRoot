#ifndef ESBROOT_ESBDRECONSTRUCTION_FGD_GRAPH_STATS_H
#define ESBROOT_ESBDRECONSTRUCTION_FGD_GRAPH_STATS_H

// Root headers
#include "TDatabasePDG.h"

// FairRoot headers
#include <FairTask.h>

// EsbRoot headers
#include "EsbData/EsbSuperFGD/FgdHit.h"
#include "EsbGeometry/EsbSuperFGD/EsbFgdDetectorParameters.h"
#include "EsbGeometry/EsbSuperFGD/EsbSuperFGDDetectorConstruction.h"
#include "EsbReconstruction/EsbSuperFGD/FgdReconHit.h"
#include "EsbReconstruction/EsbSuperFGD/PdgFromMomentumLoss.h"

namespace esbroot {
namespace reconstruction {
namespace superfgd {

class FgdGraphStats : public FairTask
{

 public:

  /** Default constructor **/  
  FgdGraphStats();

  /** Constructor with argument
   *@param name       Name of task
   *@param geoConfigFile  - Configuration file detector
   *@param mediaFile  - Configuration file for the used mediums
   *@param eventData  - events data file (generated from fgd generator)
   *@param outputRootFile - full path to the output root file
   *@param verbose  - Verbosity level
   *@param debugLlv - debug level for genfit
  **/  
  FgdGraphStats(const char* name
              , const char* geoConfigFile
              , const char* mediaFile
              , const char* eventData
              , const char* outputRootFile
              , Int_t verbose = 1
              , double debugLlv = 0);

  /** Destructor **/
  virtual ~FgdGraphStats();

  /** Virtual method Init **/
  virtual InitStatus Init() override;

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt) override;

  virtual void FinishTask() override;

protected:

    void CompareTracks(std::vector<std::vector<ReconHit>>& mcTracks, std::vector<std::vector<ReconHit*>>& grTracks);
    /** Get all hits **/
    Bool_t GetMCHits(std::vector<ReconHit>& allHits);
    Bool_t GetGraphHits(std::vector<ReconHit>& allHits);

    void SplitMCTrack(std::vector<ReconHit>& allHits, std::vector<std::vector<ReconHit>>& splitTracks);

    Bool_t FindUsingGraph(std::vector<ReconHit>& hits
                  , std::vector<std::vector<ReconHit*>>& foundTracks);

    void BuildGraph(std::vector<ReconHit>& hits);
    void CalculateGrad(std::vector<std::vector<ReconHit*>>& tracks);
    void SplitGRTrack(std::vector<std::vector<ReconHit*>>& originalTracks, std::vector<std::vector<ReconHit*>>& splitTracks);
    Bool_t CalculateInitialMomentum(const std::vector<TVector3>& track,const TVector3& magField, TVector3& momentum, TVector3& momentumLoss);
    Bool_t CalculateMomentum(const TVector3& p1, const TVector3& p2, const TVector3& p3 , const TVector3& magField, TVector3& momentum);
    Double_t GetRadius(const TVector3& p1, const TVector3& p2, const TVector3& p3);

    Bool_t IsChargedParticle(ReconHit& hit);
    Double_t CmpGrToMCTrack(std::vector<ReconHit>& mc, std::vector<ReconHit*>& gr);
    Bool_t ContainsHit(std::vector<ReconHit>& mctrack, ReconHit& hit);

    /** Define materials used in the reconstruction phase **/
    void DefineMaterials();

    Long_t ArrInd(int i, int j, int k);

    Bool_t isParticleNeutral(Int_t pdg);

    /** Class to hold the Detector parameters read from external file **/
    esbroot::geometry::superfgd::FgdDetectorParameters fParams;//!<!

    /** Class containing the TGeometry for reconstruction of the tracks **/
    esbroot::geometry::superfgd::SuperFGDDetectorConstruction    fgdConstructor;//!<!	   //! SuperFgd Detector Constructor

    /** Detector geometry pointer**/
    TGeoVolume* fsuperFgdVol;//!<!

    /** Input array of FgdDetectorPoint(s)**/
    TClonesArray* fHitArray;     //! 

    /** Output array with genfit::Track(s) **/
    TClonesArray* fTracksArray;        //!

    /** Detector dimentions **/
    Double_t flunit;//!<!
    double f_step_X;//!<!
    double f_step_Y;//!<!
    double f_step_Z;//!<!
    int f_bin_X;//!<!
    int f_bin_Y;//!<!
    int f_bin_Z;//!<!
    double f_total_X;//!<!
    double f_total_Y;//!<!
    double f_total_Z;//!<!

    /** Start position and momentum **/
    //  0 - no additional info
    //  1 - debug info
    //  2- detail info
    double fDebuglvl_genfit;//!<!
    std::shared_ptr<TDatabasePDG> fpdgDB;//!<!

    /** Path to the used media.geo file - containing definitions of materials **/
    std::string fmediaFile;//!<!

    Int_t fMinTrackLenght;


    /** Are materials already defined **/
    bool isDefinedMaterials;
  	   
    ClassDef(FgdGraphStats, 2);

};

} //superfgd
} //reconstruction
} //esbroot

#endif // ESBROOT_ESBDRECONSTRUCTION_FGD_GRAPH_STATS_H
