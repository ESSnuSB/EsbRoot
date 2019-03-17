#ifndef ESBROOT_DATA_SUPERFGD_DIGIT_READOUT_H
#define ESBROOT_DATA_SUPERFGD_DIGIT_READOUT_H 1

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"
#include "TObject.h"
#include <TVector3.h>
#include <set>

using namespace std;

namespace esbroot {

namespace data {

namespace superfgd {

namespace detector {

class DigitReadout : public TObject
{
public:
    /** Default constructor **/
    DigitReadout();
    /** Copy parameter constructor **/
    DigitReadout(TVector3 binPosition, TVector3 MPPCLocalPosition, TVector3 PhotoElectrons);

    /** Destructor **/
    ~DigitReadout();

    /** Get the bin position of the read cube [in x,y,z number position] **/
    const TVector3 &GetBinPosition() const {return fbinPosition;}
    /** Set the bin position for the read cube [in x,y,z number position]
     *@param binPos - bin position
     **/
    void SetBinPosition(TVector3 &binPos){fbinPosition = binPos;}

    const TVector3 &GetMPPCLocalPosition() {return fMPPCLocalPosition;}
    void SetMPPCLocalPosition(TVector3 &mppc){fMPPCLocalPosition=mppc;}

    /** Get the generated photoelectrons in each direction **/
    const TVector3 &GetPhotoElectrons() {return fPhotoElectrons;}
    /** Set the generated photoelectrons in each direction **/
    void SetPhotoElectrons(TVector3 & pe){fPhotoElectrons = pe;}

    /** Get the total deposited energy in the cube **/
    Double_t GetTotalEdep() {return ftotalEdep;}
    /** Set the total deposited energy in the cube **/
    void SetTotalEdep(Double_t edep){ftotalEdep = edep;}
    /** Add deposited energy in the cube **/
    void AddTotalEdep(Double_t edep){ftotalEdep = ftotalEdep + edep;}

    /** Get the event number for which the cube contains photons **/
    Int_t GetEventNumber() {return feventNumber;}
    /** Set the event number for which the cube contains photons **/
    void SetEventNumber(Double_t n){feventNumber = n;}

    /** equals operator, to enable usage in stl containers **/
    bool operator==(const DigitReadout& rhs) const;

    /** Root copy method
     *@param object - obj to copy from
     **/
    void Copy(TObject& object) const;

    /** Dump readout information into stream
     *@param stream - stream to dump info to
     **/
    void PrintToStream(ostream& stream) const;

    /** Get the pdg codes of the particles that deposited energy in the cube **/
    std::set<int>& GetPdgs(){return fpdgs;}
    /** Set the pdg codes of the particles that deposited energy in the cube
     *@param pdgs - std::set to hold the pdg codes
     **/
    void SetPdgs(std::set<int>& pdgs){fpdgs = pdgs;}
    
    /** Get the trackids of the particles that deposited energy in the cube **/
    std::set<int>& GetTrackIds(){return ftrackIds;}
    /** Set the trackids of the particles that deposited energy in the cube
     *@param trackids - std::set to hold the trackids
     **/
    void SetTrackIds(std::set<int>& trackids){ftrackIds = trackids;}
    
    /** Get the parent pdg codes of the particles that deposited energy in the cube **/
    std::set<int>& GetParentIds(){return fparentIds;}
    /** Set the parent pdg codes of the particles that deposited energy in the cube
     *@param parentIds - std::set to hold the pdg codes
     **/
    void SetParentIds(std::set<int>& parentIds){fparentIds = parentIds;}

private:

    TVector3 fbinPosition;
    TVector3 fMPPCLocalPosition;
    TVector3 fPhotoElectrons;
    Double_t ftotalEdep;
    Int_t feventNumber;

    std::set<int> ftrackIds;
    std::set<int> fparentIds;
    std::set<int> fpdgs;

    ClassDef(DigitReadout, 2);
};

}   // detector
}   // superfgd
}   // data
}   // esbroot

#endif