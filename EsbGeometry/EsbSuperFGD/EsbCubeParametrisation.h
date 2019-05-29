#ifndef ESBROOT_ESBGEOMETRY_SUPERFGD_CUBE_PARAMETRIZATION_H
#define ESBROOT_ESBGEOMETRY_SUPERFGD_CUBE_PARAMETRIZATION_H 1

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
namespace geometry {
namespace superfgd {

class CubeParametrisation :  public G4VPVParameterisation
{
public:

    /** Default constructor **/
    CubeParametrisation(G4double cube_X, G4int cube_X_N, G4double cube_Y, G4int cube_Y_N);

    /** Destructor **/
    virtual ~CubeParametrisation();

    /** To ease the detector construction we make one cube and copy it
     * this computes the position of the copy
     *@param copyNo - the number of the copy from the original cube
     *@param physVol - the physical volume
     **/
    void ComputeTransformation (const G4int copyNo,
                                G4VPhysicalVolume* physVol) const;
private:

    
    G4double fcube_X;
    G4int fcube_X_N;

    G4double fcube_Y;   
    G4int fcube_Y_N;  
    
    std::vector<G4double> fxpos;
    std::vector<G4double> fypos;
};

}   //superfgd
}   //geometry
}   //esbroot

#endif