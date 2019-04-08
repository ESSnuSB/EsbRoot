#ifndef ESBROOT_DATA_SUPERFGD_CUBE_HIT_H
#define ESBROOT_DATA_SUPERFGD_CUBE_HIT_H 1

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"
#include "TObject.h"
#include <TVector3.h>

using namespace std;

namespace esbroot {

namespace data {

namespace superfgd {

namespace detector {

class CubeHit : public TObject
{
public:
    /** Default constructor **/
    CubeHit();

    /** Destructor **/
    virtual ~CubeHit();

    //====================================================================
    /** Get the total deposited energy in the cube **/
    double GetEdep(){return fedep;}

    /** Set the total deposited energy in the cube
     *@param edep - deposited energy
     **/
    void SetEdep(double edep){fedep=edep;}
    //====================================================================

    //====================================================================
    /** Get the total deposited non-ionizing energy in the cube **/
    double GetNonIonizingEnergyDeposit(){return fnonIonizingEnergyDeposit;}

    /** Set the total deposited energy in the cube
     *@param nonIon - deposited energy
     **/
    void SetNonIonizingEnergyDeposit(double nonIon){fnonIonizingEnergyDeposit=nonIon;}
    //====================================================================

    //====================================================================
    /** Get the hit position in the cube **/
    const TVector3& GetHitPosition(){return fhitPosition;}
    /** Set the hit position in the cube using vector
     *@param p - position vector
     **/
    void SetHitPostion(const TVector3& p) {fhitPosition.SetXYZ(p.x(),p.y(),p.z());}
    /** Set the hit position in the cube using coordinates
     *@param x - position x
     *@param y - position y
     *@param z - position z
     **/
    void SetHitPostion(double x,double y,double z ) {fhitPosition.SetXYZ(x,y,z);}
    //====================================================================

    //====================================================================
    /** Get the post hit position in the cube **/
    const TVector3& GetPostHitPostion(){return fpostHitPosition;}
    /** Set the post hit position in the cube using vector
     *@param p - position vector
     **/
    void SetPostHitPostion(const TVector3& p) {fpostHitPosition.SetXYZ(p.x(),p.y(),p.z());}
    /** Set the post hit position in the cube using coordinates
     *@param x - position x
     *@param y - position y
     *@param z - position z
     **/
    void SetPostHitPostion(double x,double y,double z ) {fpostHitPosition.SetXYZ(x,y,z);}
    //====================================================================

    //====================================================================
    /** Get the kinetic energy of the particle **/
    double GetKinEnergy(){return fkineticEnergy;}
    /** Set the kinetic energy of the particler
     *@param kineE - particle kinetic energy
     **/
    void SetKinEnergy(double kineE) {fkineticEnergy= kineE;}
    //====================================================================

    //====================================================================
    /** Get the particle momentum in the cube **/
    const TVector3& GetHitMomentum(){return fmomentum;}
    /** Set the momentum of the particle
     *@param hitP - momentum vector
     **/
    void SetHitMomentum(const TVector3& hitP) {fmomentum.SetXYZ(hitP.x(),hitP.y(),hitP.z());}
    /** Set the momentum of the particle
     *@param x - momentum px
     *@param y - momentum py
     *@param z - momentum pz
     **/
    void SetHitMomentum(double px,double py,double pz ) {fmomentum.SetXYZ(px,py,pz);}
    //====================================================================
    
    //====================================================================
    /** When constructing the detector geometry, the cube is copied for easier construction 
        , get the cube copy number **/
    int GetCubeCopyNo(){return fCubeCopyNo;}
    /** Set the cube copy number
     *@param ccn - cube copy number
     **/
    void SetCubeCopyNo(int ccn){fCubeCopyNo = ccn;}
    //====================================================================

    //====================================================================
    /** When constructing the detector geometry, the cube is copied into slabs for easier construction
        , get the cube`s slab copy number **/
    int GetslabCopyNo(){return fslabCopyNo;}
    /** Set the cube slab copy number
     *@param scn - slab copy number
     **/
    void SetslabCopyNo(int scn){fslabCopyNo = scn;}
    //====================================================================

    //====================================================================
    /** Get time of interaction **/
    double GetTime(){return ftime;}
    /** Set the time of interaction
     *@param time - interaction time
     **/
    void SetTime(double time) {ftime=time;}
    //====================================================================

    //====================================================================
    /** Get time of post interaction **/
    double GetPostTime(){return fpostTime;}
    /** Set the time of post interaction
     *@param t - post interaction time
     **/
    void SetPostTime(double t) {fpostTime=t;}
    //====================================================================

    //====================================================================
    /** Get the particle track lenght in the cube **/
    double GetTracklength(){return ftrackLength;}
    /** Set the particle track lenght
     *@param l - track lenght
     **/
    void SetTracklength(double l) {ftrackLength=l;}
    //====================================================================

    //====================================================================
    /** Get the trackId of the particle **/
    int GetTrackId(){return ftrackID;}
    /** Set trackId of the particle
     *@param tId - track lenght
     **/
    void SetTrackId(int tId){ftrackID = tId;}
    //====================================================================

    //====================================================================
    /** Get parent of the particle **/
    int GetParentId(){return fparentID;}
    /** Set parent of the particle
     *@param pId - parent Id
     **/
    void SetParentId(int pId){fparentID = pId;}
    //====================================================================

    //====================================================================
    /** Get the particle pdg code **/
    int GetPdg(){return fpdg;}
    /** Set the particle pdg code 
     *@param pdg - particle pdg code
     **/
    void SetPdg(int pdg){fpdg=pdg;}
    //====================================================================

    //====================================================================
    /** Get the particle charge **/
    double GetCharge(){return fcharge;}
    /** Set the particle charge 
     *@param c - particle charge 
     **/
    void SetCharge(int c){fcharge=c;}   
    //==================================================================== 

    /** Copy ROOT method **/
    void Copy(TObject& object) const;

    /** Print the object to stream **/
    void PrintToStream(ostream& stream) const;

private:

    double fedep;
    double fnonIonizingEnergyDeposit;
    TVector3 fhitPosition;
    TVector3 fpostHitPosition;
    double      fkineticEnergy;
    TVector3 fmomentum;
    
    int fCubeCopyNo;
    int fslabCopyNo;

    double ftime;
    double fpostTime;
    double ftrackLength;

    int ftrackID;
    int fparentID;
    int fpdg;
    double fcharge;

    ClassDef(CubeHit, 2);
};

}   // detector
}   // superfgd
}   // data
}   // esbroot

#endif