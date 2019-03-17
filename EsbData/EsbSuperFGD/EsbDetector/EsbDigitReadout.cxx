#include "EsbData/EsbSuperFGD/EsbDetector/EsbDigitReadout.h"

namespace esbroot {

namespace data {

namespace superfgd {

namespace detector {

DigitReadout::DigitReadout()
    :TObject(), ftotalEdep(0), feventNumber(0)
{
}

DigitReadout::DigitReadout(TVector3 binPosition, TVector3 MPPCLocalPosition, TVector3 PhotoElectrons)
    :TObject(), fbinPosition(binPosition), fMPPCLocalPosition(MPPCLocalPosition)
    , fPhotoElectrons(PhotoElectrons), ftotalEdep(0), feventNumber(0)
{
}

DigitReadout::~DigitReadout()
{
}

void DigitReadout::Copy(TObject& object) const
{
    DigitReadout* ro = dynamic_cast<DigitReadout*>(&object);

    ro->SetBinPosition(const_cast<TVector3&>(this->fbinPosition));
    ro->SetMPPCLocalPosition(const_cast<TVector3&>(this->fMPPCLocalPosition));
    ro->SetPhotoElectrons(const_cast<TVector3&>(this->fPhotoElectrons));
    ro->SetTotalEdep(this->ftotalEdep);
    ro->SetEventNumber(this->feventNumber);

    ro->SetPdgs(const_cast<std::set<int>&>(this->fpdgs));
    ro->SetTrackIds(const_cast<std::set<int>&>(this->ftrackIds));
    ro->SetParentIds(const_cast<std::set<int>&>(this->fparentIds));
}

bool DigitReadout::operator==(const DigitReadout& rhs) const
{
    return rhs.GetBinPosition().X()==this->GetBinPosition().X()
            && rhs.GetBinPosition().Y()==this->GetBinPosition().Y()
            && rhs.GetBinPosition().Z()==this->GetBinPosition().Z();
}

void DigitReadout::PrintToStream(ostream& stream) const 
{
    stream << "----------------------------------------" << endl;
    stream << "                 DigitReadout           " << endl;
    stream << "----------------------------------------" << endl;
    stream << "Edep = " << ftotalEdep << endl;
    stream << "EventNumber = " << feventNumber << endl;
    stream << "Bin X = " << fbinPosition.X() << endl;
    stream << "Bin Y = " << fbinPosition.Y() << endl;
    stream << "Bin Z = " << fbinPosition.Z() << endl;
  
    stream << endl;
}

}   // detector
}   // superfgd
}   // data
}   // esbroot