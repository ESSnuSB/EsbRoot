#ifndef ESBROOT_SIMULATION_SUPERFGD_FIBER_PARAMETRIZATION_H
#define ESBROOT_SIMULATION_SUPERFGD_FIBER_PARAMETRIZATION_H 1

#include "globals.hh"
#include "G4VPVParameterisation.hh"

class G4VPhysicalVolume;
class G4Box;

// Dummy declarations to get rid of warnings ...
class G4Trd;
class G4Trap;
class G4Cons;
class G4Orb;
class G4Sphere;
class G4Torus;
class G4Para;
class G4Hype;
class G4Tubs;
class G4Polycone;
class G4Polyhedra;

namespace esbroot {
namespace simulators {
namespace superfgd {

class FiberParametrisation :  public G4VPVParameterisation
{
public:
     /** Default constructor **/
    FiberParametrisation(G4double initialX, G4double spacing);

    /** Destructor **/
    virtual ~FiberParametrisation();

    /** To ease the detector construction we make one fiber and copy it
     * this computes the position of the copy
     *@param copyNo - the number of the copy from the original fiber
     *@param physVol - the physical volume
     **/
    void ComputeTransformation (const G4int copyNo,
                                G4VPhysicalVolume* physVol) const;
private: 
    G4double fSpacing;
    G4double finitialX;           
};

}   //superfgd
}   //simulators
}   //esbroot

#endif