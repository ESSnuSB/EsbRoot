#ifndef ESBROOT_ESBGEOMETRY_ONAXISCS_H
#define ESBROOT_ESBGEOMETRY_ONAXISCS_H

#include "EsbGeometry/CoordinateSystem.h"

#include <TObject.h>
#include <TMath.h>
#include <Math/Vector3D.h>

#include <cmath>

namespace esbroot {
namespace geometry {

//! @brief On-axis coordinate system construction class
class OnAxisCS : public TObject
{
	public:
	
	//! Constant conecting ECS with TCS
	static constexpr double StdTheta_E = 28.5 * TMath::DegToRad();
	
	//! Empty constructor for ROOT I/O
	OnAxisCS() {};
	
	//! Constructor from parameters
	template <typename vec>
	OnAxisCS(double theta_E, double theta_N, vec T_TS, double L_ND, double gamma, double L_FD) : 
			ftheta_E(theta_E), ftheta_N(theta_N), fT_TS(T_TS), fL_ND(L_ND), fgamma(gamma), fL_FD(L_FD),
			fECS(BuildECS(ftheta_E)),
			fTSCS(BuildTSCS(ftheta_E, ftheta_N, fT_TS)),
			fNDCS(BuildNDCS(ftheta_E, ftheta_N, fT_TS, fL_ND, fgamma)),
			fFDCS(BuildFDCS(ftheta_E, ftheta_N, fT_TS, fL_ND, fgamma, fL_FD))
			{};
	
	//! Empty destructor
	virtual ~OnAxisCS() {};
	
	//Getters		
	double GetTheta_E() const {return ftheta_E;};
	double GetTheta_N() const {return ftheta_N;};
	ROOT::Math::XYZVector const& GetT_TS() const {return fT_TS;};
	double GetL_ND() const {return fL_ND;};
	double GetGamma() const {return fgamma;};
	double GetL_FD() const {return fL_FD;};
	
	CoordinateSystem const& GetECS() const {return fECS;} //!<Get ECS
	CoordinateSystem const& GetTSCS() const {return fTSCS;} //!<Get TSCS
	CoordinateSystem const& GetNDCS() const {return fNDCS;} //!<Get NDCS
	CoordinateSystem const& GetFDCS() const {return fFDCS;} //!<Get FDCS

	//! Static function to construct ECS
	static CoordinateSystem BuildECS(double theta_E);
	//! Static function to construct TSCS
	static CoordinateSystem BuildTSCS(double theta_E, double theta_N, ROOT::Math::XYZVector const& T_TS);
	//! Static function to construct NDCS
	static CoordinateSystem BuildNDCS(double theta_E, double theta_N, ROOT::Math::XYZVector const& T_TS, double L_ND, double gamma);
	//! Static function to construct FDCS
	static CoordinateSystem BuildFDCS(double theta_E, double theta_N, ROOT::Math::XYZVector const& T_TS, double L_ND, double gamma, double L_FD);
	
	private:
	double ftheta_E = std::nan("ni"); //rad
	double ftheta_N = std::nan("ni"); //rad
	ROOT::Math::XYZVector fT_TS = ROOT::Math::XYZVector(std::nan("ni"), std::nan("ni"), std::nan("ni")); //solve init
	double fL_ND = std::nan("ni"); //cm
	double fgamma = std::nan("ni"); //rad
	double fL_FD = std::nan("ni"); //cm
	
	CoordinateSystem fECS; //!< ECS coordinate system
	CoordinateSystem fTSCS; //!< TSCS coordinate system
	CoordinateSystem fNDCS; //!< NDCS coordinate system
	CoordinateSystem fFDCS; //!< FDCS coordinate system
	
	ClassDef(OnAxisCS, 2)
};

} //namespace geometry
} //namespace esbroot

#endif //ESBROOT_ESBGEOMETRY_ONAXISCS_H
