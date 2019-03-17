#ifndef ESBROOT_DATA_SUPERFGD_FIBER_HIT_H
#define ESBROOT_DATA_SUPERFGD_FIBER_HIT_H 1

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"
#include "TObject.h"
#include <TVector3.h>

using namespace std;

namespace esbroot {

namespace data {

namespace superfgd {

namespace detector {

class FiberHit : public TObject
{
public:

    /** Default constructor **/
    FiberHit();

    /** Destructor **/
    ~FiberHit();

    /** Get the total deposited energy in the fiber **/
    Double_t GetEdep(){return fedep;}
    /** Set the total deposited energy in the fiber **/
    void SetEdep(Double_t edep){fedep=edep;}

    /** Get bin position - the bin which contains the fiber **/
    TVector3& GetHitPosition(){return fhitPosition;}
    /** Set bin position - the bin which contains the fiber
     *@param hitPos - bin position
     **/
    void SetHitPostion(TVector3& hitPos) {fhitPosition.SetXYZ(hitPos.x(),hitPos.y(),hitPos.z());}
    /** Set bin position - the bin which contains the fiber
     *@param x - x bin position
     *@param y - y bin position
     *@param z - z bin position
     **/
    void SetHitPostion(Double_t x,Double_t y,Double_t z ) {fhitPosition.SetXYZ(x,y,z);}

    /** Get the kinetic energy of the particle in the fiber **/
    Double_t GetKinEnergy(){return fkineticEnergy;}
    /** Set the kinetic energy of the particle in the fiber
     *@param kineE - kinetic energy
     **/
    void SetKinEnergy(Double_t kineE) {fkineticEnergy= kineE;}

    /** Get the momentum of the particle in the fiber**/
    TVector3& GetHitMomentum(){return fmomentum;}
    /** Set the momentum of the particle in the fiber
     *@param hitP - momentum vector
     **/
    void SetHitMomentum(TVector3& hitP) {fmomentum.SetXYZ(hitP.x(),hitP.y(),hitP.z());}
    /** Set the momentum of the particle in the fiber
     *@param px - x momentum
     *@param py - y momentum
     *@param pz - z momentum
     **/
    void SetHitMomentum(Double_t px,Double_t py,Double_t pz ) {fmomentum.SetXYZ(px,py,pz);}
    
    /** Get the copy number of the fiber within the parent geometry **/
    Int_t GetfiberCopyNo(){return ffiberCopyNo;}
    /** Set the copy number of the fiber within the parent geometry
     *@param fcn - fiber copy number
     **/
    void SetfiberCopyNo(Int_t fcn){ffiberCopyNo = fcn;}

    /** Get the copy number of the slab that contains the fiber **/
    Int_t GetslabCopyNo(){return fslabCopyNo;}
    /** Set the copy number of the slab that contains the fiber
     *@param scn - slab copy number
     **/
    void SetslabCopyNo(Int_t scn){fslabCopyNo = scn;}

    /** Get the time of the event **/
    Double_t GetTime(){return ftime;}
     /** Set the time of the event
     *@param time - time of event
     **/
    void SetTime(Double_t time) {ftime=time;}

    /** Get trackid of the particle **/
    Int_t GetTrackId(){return ftrackID;}
    /** Set the trackid of the particle
     *@param tId - trackid
     **/
    void SetTrackId(Int_t tId){ftrackID = tId;}

    /** Get parent of the particle **/
    Int_t GetParentId(){return fparentID;}
    /** Set the parent of the particle
     *@param pId - parentId
     **/
    void SetParentId(Int_t pId){fparentID = pId;}

    /** Get pdg code of the particle **/
    Int_t GetPdg(){return fpdg;}
    /** Set the pdg of the particle
     *@param pdg - pdg code
     **/
    void SetPdg(Int_t pdg){fpdg=pdg;}

private:

    Double_t fedep;
    TVector3 fhitPosition;
    Double_t fkineticEnergy;
    TVector3 fmomentum;
    
    Int_t ffiberCopyNo;
    Int_t fslabCopyNo;

    Double_t ftime;

    Int_t ftrackID;
    Int_t fparentID;
    Int_t fpdg;

    ClassDef(FiberHit, 2);
};

}   // detector
}   // superfgd
}   // data
}   // esbroot

#endif