#include "EsbReconstruction/EsbSuperFGD/FgdMCEventRecord.h"




namespace esbroot {
namespace reconstruction {
namespace superfgd {


// -----   Constructors and Destructors  -----------------------------------
FgdMCEventRecord::FgdMCEventRecord()
{
}

FgdMCEventRecord::FgdMCEventRecord(const FgdMCEventRecord& c)
{
    this->fNu_pdg = c.fNu_pdg;
    this->fNu_E = c.fNu_E;
    this->fIsWeakCC = c.fIsWeakCC;
    this->fIsWeakNC = c.fIsWeakNC;
}

FgdMCEventRecord::~FgdMCEventRecord()
{
}
// -------------------------------------------------------------------------

// -----   Public methods   ------------------------------------------------
FgdMCEventRecord& FgdMCEventRecord::operator=(const FgdMCEventRecord& c)
{
    this->fNu_pdg = c.fNu_pdg;
    this->fNu_E = c.fNu_E;
    this->fIsWeakCC = c.fIsWeakCC;
    this->fIsWeakNC = c.fIsWeakNC;
    return *this;
}
// -------------------------------------------------------------------------


// -----   Private methods   -----------------------------------------------

// -------------------------------------------------------------------------

}// namespace superfgd
}// namespace reconstruction
}// namespace esbroot