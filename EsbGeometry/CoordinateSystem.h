#ifndef ESBROOT_ESBGEOMETRY_COORDINATESYSTEM_H
#define ESBROOT_ESBGEOMETRY_COORDINATESYSTEM_H

#include "Math/GenVector/Rotation3D.h"
#include "Math/Vector3D.h"

#include "Rtypes.h" //For ClassDef

namespace esbroot {
namespace geometry {

class CoordinateSystem
{
	public:
	CoordinateSystem() {};
	
	template <typename Matrix_t, typename Vector_t>
	CoordinateSystem(Matrix_t const& rot, Vector_t const& vec) :
			fRotation(rot), fInverseRotation(rot.Inverse()), fTranslation(vec) {IsProperOrDie();}
	
	ROOT::Math::Rotation3D const& GetRotation() const {return(fRotation);}
	ROOT::Math::Rotation3D const& GetInverseRotation() const {return(fInverseRotation);}
	
	ROOT::Math::XYZVector const& GetTranslation() const {return(fTranslation);}

	static bool IsProperRotation(ROOT::Math::Rotation3D const& rotation, double delta);
	
	protected:
	void IsProperOrDie() const;
	
	private:
	ROOT::Math::Rotation3D fRotation;
	ROOT::Math::Rotation3D fInverseRotation;
	
	ROOT::Math::XYZVector fTranslation;
	
	ClassDef(CoordinateSystem, 2)
};

class cstransforms
{
	public:
	template <typename V>
	static V l2g_pos(V const& pos, CoordinateSystem lcs)
	{
		return( add_vec(lcs.GetRotation()*pos, lcs.GetTranslation()) );
	}

	template <typename V>
	static V g2l_pos(V const& pos, CoordinateSystem lcs)
	{
		return( lcs.GetInverseRotation() * ( add_vec(pos, -lcs.GetTranslation()) ) );
	}
	
	template <typename V>
	static V l2l_pos(V const& pos, CoordinateSystem const& lcs_src, CoordinateSystem const& lcs_dest)
	{
		return( lcs_dest.GetInverseRotation() *
				add_vec(add_vec(lcs_src.GetRotation()*pos, lcs_src.GetTranslation()), -lcs_dest.GetTranslation()) );
	}

	template <typename V>
	static V l2g_vec(V const& pos, CoordinateSystem lcs)
	{
		return( lcs.GetRotation()*pos );
	}

	template <typename V>
	static V g2l_vec(V const& pos, CoordinateSystem lcs)
	{
		return( lcs.GetInverseRotation() * pos );
	}
	
	template <typename V>
	static V l2l_vec(V const& pos, CoordinateSystem const& lcs_src, CoordinateSystem const& lcs_dest)
	{
		return( (lcs_dest.GetInverseRotation() * lcs_src.GetRotation()) * pos );
	}


	private:
	template <typename V1, typename V2>
	static V1 add_vec(V1 const& v1, V2 const& v2)
	{
		return( V1(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z()) );
	}
	
};

} //namespace geometry
} //namespace esbroot

#endif //Header guard
