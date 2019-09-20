#include "EsbReconstruction/EsbSuperFGD/FgdReconTemplate.h"


namespace esbroot {
namespace reconstruction {
namespace superfgd {


FgdReconTemplate::FgdReconTemplate(const char* templateConfigFile)
{
}


FgdReconTemplate::~FgdReconTemplate()
{
}

Bool_t FgdReconTemplate::IsLeaf(Int_t ind, std::vector<ReconHit>& hits)
{
    // TODO Implement
    return false;
}

Bool_t FgdReconTemplate::GetNextHit(Int_t previousId, Int_t currentId, Int_t& nextId, std::vector<ReconHit>& hits)
{
    // TODO Implement
    return false;
}


} //superfgd
} //reconstruction
} //esbroot