#include "EsbData/EsbSuperFGD/EsbDetector/EsbCubeHit.h"

namespace esbroot {

namespace data {

namespace superfgd {

namespace detector {

// -----   Default constructor   -------------------------------------------
CubeHit::CubeHit() : fedep(0.),fnonIonizingEnergyDeposit(0),fhitPosition(0,0,0)
    , fkineticEnergy(0.),fmomentum(0,0,0)
    ,fCubeCopyNo(0), fslabCopyNo(0),ftime(0.), fpostTime(0.),ftrackID(0),fparentID(0)
    ,fpdg(0),fcharge(0)
{
    
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CubeHit::~CubeHit()
{
    
}
// -------------------------------------------------------------------------

// -----   Public Root Copy method  ----------------------------------------
void CubeHit::Copy(TObject& object) const
{
    CubeHit* hit = dynamic_cast<CubeHit*>(&object);

    hit->SetEdep(this->fedep);
    hit->SetHitPostion(this->fhitPosition.x()
                        ,this->fhitPosition.y()
                        ,this->fhitPosition.z());
    hit->SetPostHitPostion(this->fpostHitPosition.x()
                        ,this->fpostHitPosition.y()
                        ,this->fpostHitPosition.z());
    hit->SetKinEnergy(this->fkineticEnergy);
    hit->SetHitMomentum(this->fmomentum.Px()
                        ,this->fmomentum.Py()
                        ,this->fmomentum.Pz());
    hit->SetCubeCopyNo(this->fCubeCopyNo);
    hit->SetslabCopyNo(this->fslabCopyNo);
    hit->SetTime(this->ftime);
    hit->SetPostTime(this->fpostTime);
    hit->SetTracklength(this->ftrackLength);
    hit->SetTrackId(this->ftrackID);
    hit->SetParentId(this->fparentID);
    hit->SetPdg(this->fpdg);
    hit->SetCharge(this->fcharge);
}

// -----   Public method Print  to stream  ---------------------------------
void CubeHit::PrintToStream(ostream& stream) const 
{
    stream << "----------------------------------------" << endl;
    stream << "                 CubeHit              " << endl;
    stream << "----------------------------------------" << endl;
    stream << "Edep = " << fedep << endl;
    stream << "Position.x = " << fhitPosition.x() << endl;
    stream << "Position.y = " << fhitPosition.y() << endl;
    stream << "Position.z = " << fhitPosition.z() << endl;
    stream << "PostPosition.x = " << fpostHitPosition.x() << endl;
    stream << "PostPosition.y = " << fpostHitPosition.y() << endl;
    stream << "PostPosition.z = " << fpostHitPosition.z() << endl;
    stream << "Kinetic Energy = " << fkineticEnergy << endl;
    stream << "Momentum.Px = " << fmomentum.Px() << endl;
    stream << "Momentum.Py = " << fmomentum.Py() << endl;
    stream << "Momentum.Pz = " << fmomentum.Pz() << endl;
    stream << "CubeCopyNo = " << fCubeCopyNo << endl;
    stream << "SlabCopyNo = " << fslabCopyNo << endl;
    stream << "Time = " << ftime << endl;
    stream << "Track_ID = " << ftrackID << endl;
    stream << "Parent_ID = " << fparentID << endl;
    stream << "Pdg = " << fpdg << endl;
    stream << "Charge = " << fcharge << endl;
  
    stream << endl;
}

}   // detector
}   // superfgd
}   // data
}   // esbroot