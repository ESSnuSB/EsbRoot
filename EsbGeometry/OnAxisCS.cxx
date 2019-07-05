#include "EsbGeometry/OnAxisCS.h"

#include <TMath.h>
#include <Math/GenVector/Rotation3D.h>
#include <Math/Vector3D.h>

namespace esbroot {
namespace geometry {

/*static*/ CoordinateSystem OnAxisCS::BuildECS(double theta_E)
{
	return(
		CoordinateSystem(
			ROOT::Math::Rotation3D
			(
				-TMath::Cos(theta_E),	0, TMath::Sin(theta_E),
				TMath::Sin(theta_E),	0, TMath::Cos(theta_E),
				0.0, 1.0, 0.0
			),
			ROOT::Math::XYZVector(0.0, 0.0, 0.0)
		)
	);
}

/*static*/ CoordinateSystem OnAxisCS::BuildTSCS(double theta_E,
		double theta_N, ROOT::Math::XYZVector const& T_TS)
{
	return(
		CoordinateSystem(
			ROOT::Math::Rotation3D(
				-TMath::Cos(theta_E + theta_N),	0, TMath::Sin(theta_E + theta_N),
				TMath::Sin(theta_E + theta_N),	0, TMath::Cos(theta_E + theta_N),
				0.0, 1.0, 0.0 
			),
			T_TS
		)
	);
}

/*static*/ CoordinateSystem OnAxisCS::BuildNDCS(double theta_E,
		double theta_N, ROOT::Math::XYZVector const& T_TS, double L_ND, double gamma)
{
	return(
		CoordinateSystem(
			ROOT::Math::Rotation3D(
				-TMath::Cos(theta_E + theta_N),	0, TMath::Sin(theta_E + theta_N),
				TMath::Sin(theta_E + theta_N),	0, TMath::Cos(theta_E + theta_N),
				0.0, 1.0, 0.0 
			),
			ROOT::Math::XYZVector(
				L_ND*TMath::Cos(gamma)*TMath::Sin(theta_E+theta_N) + T_TS.x(),
				L_ND*TMath::Cos(gamma)*TMath::Cos(theta_E+theta_N) + T_TS.y(),
				-L_ND*TMath::Sin(gamma) + T_TS.z()
			)
		)
	);
}

/*static*/ CoordinateSystem OnAxisCS::BuildFDCS(double theta_E,
		double theta_N, ROOT::Math::XYZVector const& T_TS, double L_ND, double gamma, double L_FD)
{
	return(
		CoordinateSystem(
			ROOT::Math::Rotation3D(
				-TMath::Cos(theta_E + theta_N),
				TMath::Sin(2*gamma)*TMath::Sin(theta_E+theta_N),
				TMath::Cos(2*gamma)*TMath::Sin(theta_E+theta_N), //end first row
				TMath::Sin(theta_E + theta_N),
				TMath::Sin(2*gamma)*TMath::Cos(theta_E+theta_N),
				TMath::Cos(2*gamma)*TMath::Cos(theta_E+theta_N), //end second row
				0.0,
				TMath::Cos(2*gamma),
				-TMath::Sin(2*gamma) //end third row
			),
			ROOT::Math::XYZVector(
				L_FD*TMath::Cos(gamma)*TMath::Sin(theta_E+theta_N) + T_TS.x(),
				L_FD*TMath::Cos(gamma)*TMath::Cos(theta_E+theta_N) + T_TS.y(),
				-L_FD*TMath::Sin(gamma) + T_TS.z()				
			)
		)
	);
}


} //namespace geometry
} //namespace esbroot
