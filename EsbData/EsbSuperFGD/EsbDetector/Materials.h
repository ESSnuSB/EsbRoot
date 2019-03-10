#ifndef ESBROOT_DATA_SUPERFGD_DETECTOR_H
#define ESBROOT_DATA_SUPERFGD_DETECTOR_H 1

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

using namespace CLHEP;

namespace esbroot {

namespace data {

namespace superfgd {

namespace detector {

namespace materials 
{
    static const int H_Z = 1;
    static const double H1_ATOMIC_MASS = 1.007825*g/mole;

    static const int C_Z = 6;
    static const double C12_ATOMIC_MASS = 12.0000*g/mole;

    static const int N_Z = 7;
    static const double N_ATOMIC_MASS = 14.0067*g/mole;

    static const int O_Z = 8;
    static const double O_ATOMIC_MASS = 15.9994*g/mole;

    static const double AIR_DENSITY = 1.29*mg/cm3;
    static const int AIR_NCOMPONENTS = 2;
    static const double AIR_FRACTION_N = 0.70;
    static const double AIR_FRACTION_O = 0.30;

    static const double POLYSTYRENE_DENSITY = 1.06*g/cm3;
    static const int POLYSTYRENE_NCOMPONENTS = 2;
    static const int POLYSTYRENE_NATOMS_C = 8;
    static const int POLYSTYRENE_NATOMS_H = 8;

}   // materials
}   // detector
}   // superfgd
}   // data
}   // esbroot

#endif