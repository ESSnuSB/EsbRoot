#ifndef ESBROOT_ESBGEOMETRY_COORDINATESYSTEM_H
#define ESBROOT_ESBGEOMETRY_COORDINATESYSTEM_H

#include "Math/GenVector/Rotation3D.h"
#include "Math/Vector3D.h"

#include "TObject.h"
#include "Rtypes.h"

namespace esbroot {
namespace geometry {

//! @brief Class which defines ESSnuSB coordinate system
//!
//! This class contains the rotation and translation part of the ESSnuSB
//! coordinate system definition described in https://essnusb.eu/DocDB/private/ShowDocument?docid=102
//! version v3. It is used by static functions of cstransforms class to transform
//! vectors and positions from one CS to another.
//!
//! The class automatically checks that the supplied matrix is an element of proper
//! orthonormal group.
class CoordinateSystem : public TObject
{
	public:
	//!Empty constructor for ROOT I/O
	CoordinateSystem() {};
	
	//! Basic constructor
	template <typename Matrix_t, typename Vector_t>
	CoordinateSystem(Matrix_t const& rot, Vector_t const& vec, double float_delta = 1e-6) :
			fRotation(rot), fInverseRotation(rot.Inverse()), fTranslation(vec),
			fFloatDelta(float_delta) {IsProperOrDie();}
		
	//! Empty virtual destructor
	virtual ~CoordinateSystem() {};
	
	//! Get rotation matrix
	ROOT::Math::Rotation3D const& GetRotation() const {return(fRotation);}
	//! Get inverse rotation matrix
	ROOT::Math::Rotation3D const& GetInverseRotation() const {return(fInverseRotation);}
	
	//! Get translation vector
	ROOT::Math::XYZVector const& GetTranslation() const {return(fTranslation);}

	//! Static function which checks whether the matrix is a proper rotation
	static bool IsProperRotation(ROOT::Math::Rotation3D const& rotation, double delta);
	
	protected:
	//! Function which terminates the program if rotation is not proper.
	void IsProperOrDie() const;
	
	private:
	//! Rotation matrix
	ROOT::Math::Rotation3D fRotation;
	//! Inverse of rotation matrix
	ROOT::Math::Rotation3D fInverseRotation;
	
	//! Translation vector
	ROOT::Math::XYZVector fTranslation;
	
	//! @brief Delta when checking doubles for equality.
	//!
	//! Two doubles a and b are considered equal if |a - b| < delta.
	double fFloatDelta = 1e-6;
	
	ClassDef(CoordinateSystem, 2)
};

//! @brief Coordinate system transformation functions
//!
//! This class contains static templated functions to transform vectors and
//! positions from one coordinate system to another. In principle, it should be
//! a namespace, but it is a class because of ROOT.
class cstransforms
{
	public:
	//! @brief Local to global position transformation
	//!
	//! @tparam V Vector-like class which must have V::x(), V::y(), and V::z() functions
	//! @param pos Position vector in LCS
	//! @param lcs LCS definition
	template <typename V>
	static V l2g_pos(V const& pos, CoordinateSystem lcs)
	{
		return( add_vec(lcs.GetRotation()*pos, lcs.GetTranslation()) );
	}

	//! @brief Global to local position transformation
	//!
	//! @tparam V Vector-like class which must have V::x(), V::y(), and V::z() functions
	//! @param pos Position vector in GCS
	//! @param lcs LCS definition
	template <typename V>
	static V g2l_pos(V const& pos, CoordinateSystem lcs)
	{
		return( lcs.GetInverseRotation() * ( add_vec(pos, -lcs.GetTranslation()) ) );
	}
	
	//! @brief Local to local position transformation
	//!
	//! @tparam V Vector-like class which must have V::x(), V::y(), and V::z() functions
	//! @param pos Position vector in LCS
	//! @param lcs_src Definition of source LCS
	//! @param lcs_dest Definition of destination LCS
	template <typename V>
	static V l2l_pos(V const& pos, CoordinateSystem const& lcs_src, CoordinateSystem const& lcs_dest)
	{
		return( lcs_dest.GetInverseRotation() *
				add_vec(add_vec(lcs_src.GetRotation()*pos, lcs_src.GetTranslation()), -lcs_dest.GetTranslation()) );
	}

	//! Local to global vector transformation
	template <typename V>
	static V l2g_vec(V const& pos, CoordinateSystem lcs)
	{
		return( lcs.GetRotation()*pos );
	}

	//! Global to local vector transformation
	template <typename V>
	static V g2l_vec(V const& pos, CoordinateSystem lcs)
	{
		return( lcs.GetInverseRotation() * pos );
	}
	
	//! Local to local vector transformation
	template <typename V>
	static V l2l_vec(V const& pos, CoordinateSystem const& lcs_src, CoordinateSystem const& lcs_dest)
	{
		return( (lcs_dest.GetInverseRotation() * lcs_src.GetRotation()) * pos );
	}


	private:
	//! @brief Template to add two vectors.
	//!
	//! @tparam V1 Vector-like class which needs to have defined
	//! 	- Constructor accepting 3 coordinates V1::V1(x, y, z)
	//! 	- V1::x(), V1::y(), and V1::z() functions
	//! @tparam V2 Vector-like class which needs to have V1::x(), V1::y(), and V1::z() functions
	template <typename V1, typename V2>
	static V1 add_vec(V1 const& v1, V2 const& v2)
	{
		return( V1(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z()) );
	}
	
};

} //namespace geometry
} //namespace esbroot

#endif //ESBROOT_ESBGEOMETRY_COORDINATESYSTEM_H
