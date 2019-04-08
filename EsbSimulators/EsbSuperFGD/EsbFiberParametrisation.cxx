#include "EsbSimulators/EsbSuperFGD/EsbFiberParametrisation.h"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"


namespace esbroot {
namespace simulators {
namespace superfgd {

FiberParametrisation::FiberParametrisation(G4double initialX, G4double spacing)
:finitialX(initialX),fSpacing(spacing)
{
}
FiberParametrisation::~FiberParametrisation()
{
}

void FiberParametrisation::ComputeTransformation (const G4int copyNo, G4VPhysicalVolume* physVol) const
{
  // Note: copyNo will start with zero!
  G4double Xposition = finitialX + copyNo * fSpacing;
  G4ThreeVector origin(Xposition,0,0);
  physVol->SetTranslation(origin);

  G4RotationMatrix* rotmY = new G4RotationMatrix();
  rotmY->rotateX(90*CLHEP::deg);

  physVol->SetRotation(rotmY); // Rotate by 90 in Y
}

}   //superfgd
}   //simulators
}   //esbroot