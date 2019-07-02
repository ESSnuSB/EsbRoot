#include "EsbGeometry/CoordinateSystem.h"

#include <Math/SMatrix.h>

#include <cmath>
#include <cstdlib>

namespace esbroot {
namespace geometry {

//Budimir: is this function somewhere standard in ROOT?
inline static bool EqErr(double lhs, double rhs, double err)
{
	return( std::abs(lhs-rhs) < err );
}

/* static */ bool CoordinateSystem::IsProperRotation(ROOT::Math::Rotation3D const& rotation,double delta)
{
	using SMatrix33 = ROOT::Math::SMatrix<double, 3>;
	
	SMatrix33 rot;
	rotation.GetRotationMatrix(rot);

	double det;
	int det_status = rot.Det2(det);

	//~ std::cout << det_status << std::endl;
	//~ std::cout << det << std::endl;

	//Is determinant == 1?
	if(!(det_status == 1 && EqErr(det, 1.0, delta) )) return(false);

	//Is transpose*rot == 1?
	SMatrix33 zero_cand = (rot*ROOT::Math::Transpose(rot)) - SMatrix33(ROOT::Math::SMatrixIdentity());

	//~ rot.Print(std::cout); std::cout << std::endl;
	//~ SMatrix33(zero_cand).Print(std::cout); std::cout << std::endl;	

	for(auto elem : zero_cand) {
		if( !EqErr(elem, 0.0, delta) ) {
			return(false);
		}
	}
	
	return(true);
}

void CoordinateSystem::IsProperOrDie() const {
		if( !IsProperRotation(GetRotation(), 1e-6) ) {
			std::cerr << "FATAL: (" << __PRETTY_FUNCTION__
					<< ") Attempting to create CoordinateSystem object with a matrix which is not a proper rotation. Aborting."
					<< std::endl;
			std::exit(1);
		}
}


} //namespace geometry
} //namespace esbroot
