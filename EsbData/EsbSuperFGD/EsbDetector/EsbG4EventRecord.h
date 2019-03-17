#ifndef ESBROOT_DATA_SUPERFGD_G4EVENT_RECORD_H
#define ESBROOT_DATA_SUPERFGD_G4EVENT_RECORD_H 1

#include <TObject.h>
#include <TClonesArray.h>
#include <TMap.h>
#include <map>

#include "EsbData/EsbSuperFGD/EsbDetector/EsbCubeHit.h"
#include "EsbData/EsbSuperFGD/EsbDetector/EsbG4StepsRecord.h"

#include <iostream>

#define MIN_TRACK_LENGHT 3
#define MIN_SEPARATION_TRACK_LENGHT 1
#define PI 3.14159265

using namespace std;

namespace esbroot {

namespace data {

namespace superfgd {

namespace detector {

#define OUT

class G4EventRecord : public TObject
{
public:

    /** Default constructor **/
    G4EventRecord();

    /** Constructor to initialize given number of cubes
     *@param sizeCubeHits - size of simulated cubes
     **/
    G4EventRecord(Int_t sizeCubeHits);

    /** Constructor to initialized with eventrecord
     *@param record - event record
     **/
    G4EventRecord(const G4EventRecord& record);

    /** Destructor **/
    ~G4EventRecord();

    /** Add cube hit to the container record
     *@param hit - cube hit
     **/
    void AddCubeHit(const CubeHit& hit);

    /** Get the cubehit at the given index
     *@param index - cube index
     **/
    CubeHit* GetCubeHit(Int_t index) const;

    /** Number of stored cube hits for this event **/
    unsigned int NumCubeHits() const;

    /** Add the step to the container event [summing its properties]
     *@param step - step to add
    **/
    void AddSumStep(const G4StepsRecord& step);

    /** Get steprecord at index
     *@param index - step index
    **/
    G4StepsRecord* GetSumStep(Int_t index) const;

    /** Number of stored steps for this event **/
    unsigned int NumSumSteps() const;

    /** Overriden Copy method from TObject**/
    void Copy(TObject& object) const;

    /** Overriden Clear method from TObject**/
    void Clear(Option_t* option = "");

    /** Overriden Print method from TObject**/
    void Print(Option_t* option = "") const;

    /** Initializes the event record **/
    void Init();

    /** Resets the event record **/
    void Reset();

    /** Prints the event record to the passed stream **/
    void PrintToStream(ostream& stream) const;

    /** Adds the calculated cos angle for the particle with pdg code
     *@param pdg - particle pdg code
     *@param cosAngle - calculated angle
    **/
    void AddCos(Long_t pdg,Double_t cosAngle);
    /** Get the map of the cosine angles for the particles in this event
     *@param particleCos - map of particle pdg-cosine angles
    **/
    Bool_t GetCos(OUT std::map<Long_t,Double_t>& particleCos){particleCos=fparticlePdgCos; return true;}

    /** Adds the calculated cos angle 
     *@param cosAngle - calculated angle
    **/
    void AddCos(Double_t cosAngle){ fparticlesCos.push_back(cosAngle);}
    /** Get the calculated cos angle 
     *@param cosvec - vector of calculated cosine angles
    **/
    Bool_t GetCos(OUT std::vector<Double_t>& cosvec){cosvec=fparticlesCos; return true;}

    /** Adds the calculated momentum of the particle with pdg code
     *@param pdg - particle pdg code
     *@param p - particle momentum
    **/
    void AddMomentum(Long_t pdg, Double_t p);
    /** Get the calculated momentum
     *@param pa - particle momentums
    **/
    Bool_t GetMomentum(OUT std::map<Long_t,Double_t>& pa){pa=fparticlePdgMom; return true;}

    /** Adds the calculated momentum of the particle 
     *@param p - calculated angle
    **/
    void AddMomentum(Double_t p){ fparticlesMomentum.push_back(p);}
    /** Get the calculated momentum
     *@param cosMom - particle momentums
    **/
    Bool_t GetMomentum(OUT std::vector<Double_t>& cosMom){cosMom=fparticlesMomentum; return true;}

    /** Adds the track cos relative to detector coordinate system
     *@param cosAngle - calculated track cosine angle
    **/
    void AddTrackCos(Double_t cosAngle){ ftrackCos.push_back(cosAngle);}
    /** Get the track cos relative to detector coordinate system
     *@param trackCos - vector of calculated track cosine angle for event
    **/
    Bool_t GetTrackCos(OUT std::vector<Double_t>& trackCos){trackCos=ftrackCos; return true;}

    /** Adds the calculated track momentum  
     *@param p - track momentum
    **/
    void AddTrackMomentum(Double_t p){ ftrackMom.push_back(p);}
    /** Get the calculated track momentum
     *@param pv - vector of track particle momentums
    **/
    Bool_t GetTrackMomentum(std::vector<Double_t>& pv){pv=ftrackMom;return true;}

    /** Calculate the cosines of the particles eminating from the step vertex
     *@param tracks - tracks of the particles
     *@param vertex - vertex of the interaction
    **/
    void ExtractTrackCos(std::map<Int_t,std::vector<G4StepsRecord>>& tracks, G4StepsRecord& vertex);

    /** Checks if the path should be added to the record, based on the track lenght and
     * its seperation from other tracks
     * 
     *@param binTrackid - the bin which to validate
     *@param trackBins - tracks from this bin
     *@param tracks - recordred tracks
    **/
    Bool_t ValidateSeperation(Int_t& binTrackid,
                            vector<G4StepsRecord>& trackBins,
                            std::map<Int_t,std::vector<G4StepsRecord>>& tracks);

    /** Return the number of tracks that have passed the validation seperation **/
    Long_t GetPassedTracks(){return passedTracks;}

    /** Set the number of passed tracks
     *@param trackNum - number of passed tracks
    **/
    void SetPassedTracks(Long_t trackNum){passedTracks = trackNum;}

    /** If the efficiency is greater than one that means that
    * in the vertex there has been generated one or more
    * particles that have left a valid track in the detection
    * and the number of tracks can exceed the number of primary particles
    **/
    Double_t GetEffTracks(){return fparticlesCos.size()>0? (passedTracks/(Double_t)fparticlesCos.size()): 0;}

    /** Returns true of the events contains muon **/
    Bool_t IsMuon(){return fisMuon;}
    /** Returns the muon angle in the event **/
    Double_t GetMuonAngle(){return fmuonAngle;}
    /** Returns the muon momentum in the event **/
    Double_t GetMuonMomentum(){return fmuonMomentum;}

    /** Returns true of the event contains proton **/
    Bool_t IsProton(){return fisProton;}
    /** Returns the proton angle in the event **/
    Double_t GetProtonAngle(){return fprotonAngle;}
    /** Returns the proton momentum in the event **/
    Double_t GetProtonMomentum(){return fprotonMomentum;}

private:
    TClonesArray* fcubeHits;
    TClonesArray* fsumSteps;

    std::vector<Double_t> fparticlesCos; // hold the angles of the initial particles
    std::vector<Double_t> fparticlesMomentum; // hold the momentum of the initial particles

    std::vector<Double_t> ftrackCos; // hold the angles of the detected particles particles
    std::vector<Double_t> ftrackMom; // hold the angles of the detected particles particles

    std::map<Long_t,Double_t> fparticlePdgCos; // holds the angles associated with the given pdg code
    std::map<Long_t,Double_t> fparticlePdgMom; // holds the momentum associated with the given pdg code

    // Counts all tracks that are validated to the pass criteria
    // Used to pass the weight as (passed tracks/ all tracks) to calculate
    // the efficiency of the angle and momentum
    Long_t passedTracks; 

    Bool_t fisMuon;
    Double_t fmuonAngle;
    Double_t fmuonMomentum;

    Bool_t fisProton;
    Double_t fprotonAngle;
    Double_t fprotonMomentum;

    ClassDef(G4EventRecord, 2);
};

}   // detector
}   // superfgd
}   // data
}   // esbroot

#endif