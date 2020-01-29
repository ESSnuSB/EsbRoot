#ifndef ESBROOT_DATA_SUPERFGD_DETECTOR_MATERIALS_H
#define ESBROOT_DATA_SUPERFGD_DETECTOR_MATERIALS_H 1

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"


using namespace CLHEP;

namespace esbroot {

namespace geometry {

namespace superfgd {

namespace materials 
{
    static const char* vacuum = "vacuum";
    static const char* air = "air";
    static const char* brass = "brass";
    static const char* bronze = "bronze";
    static const char* stainlessSteel = "stainlessSteel";
    static const char* methane = "methane";
    static const char* carbonDioxide = "CO2";
    static const char* carbontetraFloride = "CF4";
    static const char* titaniumDioxide = "TiO2";
    static const char* polystyrene = "polystyrene";
    static const char* scintillator = "scintillator";
    static const char* paraterphnyl = "paraterphnyl";
    static const char* scintilatorMix = "scintilatorMix";
    static const char* podscintillator = "podscintillator";
    static const char* polyethylene = "polyethylene";
    static const char* poduleEpoxy = "poduleEpoxy";
    static const char* polycarbonate = "polycarbonate";
    static const char* carbonFiber = "carbonFiber";
    static const char* fiberCore = "fiberCore";
    static const char* fiberCladding = "fiberCladding";
    static const char* scintillatorCoating = "scintillatorCoating";

    static int GetNextIndex()
    {
        static int index=10000;
        index++;
        return index;
    }

}   // materials
}   // superfgd
}   // geometry
}   // esbroot

#endif