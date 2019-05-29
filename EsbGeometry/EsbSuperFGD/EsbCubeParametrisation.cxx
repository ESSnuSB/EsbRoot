#include "EsbGeometry/EsbSuperFGD/EsbCubeParametrisation.h"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"

namespace esbroot {
namespace geometry {
namespace superfgd {

CubeParametrisation::CubeParametrisation(G4double cube_X, G4int cube_X_N, G4double cube_Y, G4int cube_Y_N)
:fcube_X(cube_X),fcube_X_N(cube_X_N),fcube_Y(cube_Y),fcube_Y_N(cube_Y_N)
{
  G4double startX = (-(fcube_X * fcube_X_N) + fcube_X)/2;
  for(int x=0; x< fcube_X_N; x++)
  {
    fxpos.push_back(startX + x*fcube_X);
  }

  G4double startY = (-(fcube_Y * fcube_Y_N) + fcube_Y)/2;
  for(int y=0; y < fcube_Y_N;y++)
  {
    fypos.push_back(startY + y*fcube_Y);
  }
}

CubeParametrisation::~CubeParametrisation()
{
}

void CubeParametrisation::ComputeTransformation (const G4int copyNo, G4VPhysicalVolume* physVol) const
{
  // Note: copyNo will start with zero!
  int x_num = (copyNo % fcube_X_N);
  G4double xposition = fxpos[x_num];
  int y_num = (copyNo / fcube_X_N);
  G4double yposition = fypos[y_num];
  
  G4ThreeVector origin(xposition,yposition,0);

  physVol->SetTranslation(origin);
  physVol->SetRotation(0); // No rotation
}

}   //superfgd
}   //geometry
}   //esbroot