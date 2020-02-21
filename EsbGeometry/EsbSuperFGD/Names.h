#ifndef ESBROOT_DATA_SUPERFGD_DETECTOR_NAMES_H
#define ESBROOT_DATA_SUPERFGD_DETECTOR_NAMES_H 1

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

using namespace CLHEP;

namespace esbroot {

namespace geometry {

namespace superfgd {

namespace fgdnames 
{
    static const char* cubeName = "CubeWithCoatingWhole"; // The name of the hole Cube - with scintilator, coating, fibers
    static const char* superFGDName = "Esb/SuperFGD"; // name of the constructed detector volume
    static const char* scintilatorVolume = "cubeScntVol"; // name of just the scintilator volume in the cube
    static const char* coatingVolume = "cubeCoatingVol"; // name of just the scintilator volume in the cube
}   // fgdnames
}   // superfgd
}   // geometry
}   // esbroot

#endif