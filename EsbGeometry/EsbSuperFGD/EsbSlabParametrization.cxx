#include "EsbGeometry/EsbSuperFGD/EsbSlabParametrization.h"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"

namespace esbroot {
namespace geometry {
namespace superfgd {

SlabParametrisation::SlabParametrisation(G4double initialZ, G4double spacing)
:finitialZ(initialZ),fSpacing(spacing)
{
}

SlabParametrisation::~SlabParametrisation()
{
}

void SlabParametrisation::ComputeTransformation (const G4int copyNo, G4VPhysicalVolume* physVol) const
{
  // Note: copyNo will start with zero!
  G4double Zposition = finitialZ + copyNo * fSpacing;
  G4ThreeVector origin(0,0,Zposition);
  physVol->SetTranslation(origin);
  physVol->SetRotation(0); // No rotation
}

}   //superfgd
}   //geometry
}   //esbroot