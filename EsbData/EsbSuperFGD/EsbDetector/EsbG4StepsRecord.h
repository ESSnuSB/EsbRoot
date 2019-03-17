#ifndef ESBROOT_DATA_SUPERFGD_G4STEP_RECORD_H
#define ESBROOT_DATA_SUPERFGD_G4STEP_RECORD_H 1

#include <TObject.h>
#include <TVector3.h>

#include "EsbData/EsbSuperFGD/EsbDetector/EsbCubeHit.h"

#include <iostream>
#include <set>

using namespace std;

namespace esbroot {

namespace data {

namespace superfgd {

namespace detector {

class G4StepsRecord : public TObject
{
public:
    /** Default constructor **/
    G4StepsRecord();

    /** Destructor **/
    ~G4StepsRecord();

    /** Get the bin x position **/
    Int_t GetBinX() const {return fbin_x;}

    /** Get the bin y position **/
    Int_t GetBinY() const {return fbin_y;}

    /** Get the bin z position **/
    Int_t GetBinZ() const {return fbin_z;}

    /** Set the bin position for this step
     *@param x - x position
     *@param y - y position
     *@param z - z position
     **/
    void SetBinPosition(Int_t x,Int_t y,Int_t z ) { fbin_x=x;fbin_y=y;fbin_z=z;}

    /** Get key for step **/
    Int_t GetKey(){return fkey;}
    /** Set key for step **/
    void SetKey(Int_t key){fkey=key;}

    /** Get start time for step **/
    Double_t GetStartTime(){return fstartTime;}
    /** Set start time for this step
     *@param t - start time
     **/
    void SetStartTime(Double_t t) {fstartTime=t;}
    /** Accumulate (add to) start time for this step
     *@param t - start time to accumulate
     **/
    void AccStartTime(Double_t t) {if(fstartTime==0) {fstartTime=t;}}

    /** Get end time for step **/
    Double_t GetEndTime(){return fendTime;}
    /** Set end time for this step
     *@param t - end time
     **/
    void SetEndTime(Double_t t) {fendTime=t;}
    /** Accumulate (add to) end time for this step
     *@param t - end time to accumulate
     **/
    void AccEndTime(Double_t t){ if(fendTime<t) fendTime=t;}

    /** Get deposited energy for step **/
    Double_t GetEdep(){return fedep;}
    /** Set the deposited energy for step
     *@param e - deposited energy
     **/
    void SetEdep(Double_t e){fedep=e;}
    /** Accumulate deposited energy for step
     *@param e - deposited energy
     **/
    void AccEdep(Double_t e) {fedep+=e;}

    /** Get track length for step **/
    Double_t GetTrackLenght(){return ftrackLenght;}
    /** Set track length for step
     *@param l - deposited energy
     **/
    void SetTrackLenght(Double_t l){ftrackLenght=l;}
    /** Accumulate track length for step
     *@param l - deposited energy
     **/
    void AccTrackLenght(Double_t l) {ftrackLenght+=l;}

    /** Get deposited non-ionization energy for step **/
    Double_t GetNonIon(){return fnonIon;}
    /** Set non-ionization energy for step
     *@param e - non-ionization energyy
     **/
    void SetNonIon(Double_t e){fnonIon=e;}
     /** Accumulate non-ionization energy for step
     *@param e - non-ionization energyy
     **/
    void AccNonIon(Double_t e) {fnonIon+=e;}

    /** Get trackids for step **/
    std::set<Int_t>& GetTrackIds(){return ftrackIds;}
    /** Set trackids for step
     *@param pIds - particle trackIds
     **/
    void SetTrackIds(std::set<Int_t>& pIds){ftrackIds=pIds;}
    /** Add track id
     *@param id - particle trackId
     **/
    void AddTrackId(Int_t id){ftrackIds.insert(id);}
    
    /** Get parentIds for step **/
    std::set<Int_t>& GetParentIds(){return fparentIds;}
    /** Set parentIds for step
     *@param pIds - particle parentIds
     **/
    void SetParentIds(std::set<Int_t>& pIds){fparentIds=pIds;}
    /** Add parentId for step
     *@param id - particle parentId
     **/
    void AddParentId(Int_t id){fparentIds.insert(id);}

    /** Get particle pdg for step **/
    std::set<Int_t>& GetPdgs(){return fpdgs;}
    /** Set pdg codes for step
     *@param pdgs - particle pdg codes
     **/
    void SetPdgs(std::set<Int_t>& pdgs){fpdgs=pdgs;}
    /** Add pdg code for step
     *@param pdg - particle pdg code
     **/
    void AddPdg(Int_t pdg){fpdgs.insert(pdg);}
    
    /** Override == opearator, to be used in stl containers **/
    bool operator==(const G4StepsRecord& rhs) const;

    /** Overriden Copy method from TObject**/
    void Copy(TObject& object) const;

    /** Print object to stream **/
    void PrintToStream(ostream& stream) const;

private:

    Int_t fbin_x;
    Int_t fbin_y;
    Int_t fbin_z;

    Int_t fkey; // to be used in maps

    Double_t fstartTime;
    Double_t fendTime;
    Double_t fedep;
    Double_t ftrackLenght;
    Double_t fnonIon;
    std::set<Int_t> ftrackIds;
    std::set<Int_t> fparentIds;
    std::set<Int_t> fpdgs;

    ClassDef(G4StepsRecord, 2);
};

}   // detector
}   // superfgd
}   // data
}   // esbroot

#endif