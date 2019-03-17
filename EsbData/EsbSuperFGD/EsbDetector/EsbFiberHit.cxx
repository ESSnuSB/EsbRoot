#include "EsbData/EsbSuperFGD/EsbDetector/EsbFiberHit.h"

namespace esbroot {

namespace data {

namespace superfgd {

namespace detector {

FiberHit::FiberHit() : fedep(0.),fhitPosition(0,0,0), fkineticEnergy(0.),fmomentum(0,0,0),
ffiberCopyNo(0), fslabCopyNo(0),ftime(0.),ftrackID(0),fparentID(0),fpdg(0)
{
    
}

FiberHit::~FiberHit()
{
    
}

}   // detector
}   // superfgd
}   // data
}   // esbroot