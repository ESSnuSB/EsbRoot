#ifndef ESBROOT_GENERATOR_GENIENTPFLUXV1_H
#define ESBROOT_GENERATOR_GENIENTPFLUXV1_H

//EsbRoot includes
#include "EsbGeometry/CoordinateSystem.h"

//GENIE includes
#include <Framework/EventGen/GFluxI.h>
#include <Framework/ParticleData/PDGCodeList.h>

//ROOT includes
#include <TLorentzVector.h>
#include <TFile.h>
#include <TTree.h>

//STD includes
#include <cmath>

//This class reads files available at:
// https://essnusb.eu/DocDB/private/ShowDocument?docid=342

namespace esbroot {
namespace generators {

class GenieNtpFluxV1 : public genie::GFluxI
{
	public:
	GenieNtpFluxV1(
		TTree *in_tree,
		int pdg_code,
		esbroot::geometry::CoordinateSystem const& TSCS,
		esbroot::geometry::CoordinateSystem const& DTCS
	);
	
	GenieNtpFluxV1(
		const char* file_name,
		const char* tree_name,
		int pdg_code,
		esbroot::geometry::CoordinateSystem const& TSCS,
		esbroot::geometry::CoordinateSystem const& DTCS		
	);
	
	virtual ~GenieNtpFluxV1() {};
	
	virtual const		genie::PDGCodeList& FluxParticles(void) override {return fFluxParticles;}; ///< declare list of flux neutrinos that can be generated (for init. purposes)
	virtual double		MaxEnergy(void) override {return fMaxEnergy;}; ///< declare the max flux neutrino energy that can be generated (for init. purposes)
	virtual bool		GenerateNext(void) override; ///< generate the next flux neutrino (return false in err)
	virtual int			PdgCode(void) override {return(fFluxParticles.front());}; ///< returns the flux neutrino pdg code
	virtual double		Weight(void) override {return(1.0);}; ///< returns the flux neutrino weight (if any)
	virtual const		TLorentzVector& Momentum(void) override {return(fMomentum);}; ///< returns the flux neutrino 4-momentum 
	virtual const		TLorentzVector& Position(void) override {return(fPosition);}; ///< returns the flux neutrino 4-position (note: expect SI rather than physical units)
	virtual bool		End(void) override {return(fCurrIndex == fInTree->GetEntries());}; ///< true if no more flux nu's can be thrown (eg reaching end of beam sim ntuples)
	virtual long int 	Index(void) override {return(fCurrIndex);}; ///< returns corresponding index for current flux neutrino (e.g. for a flux ntuple returns the current entry number)
	virtual void		Clear(Option_t * opt) override { fCurrIndex = 0; }; ///< reset state variables based on opt
	virtual void		GenerateWeighted(bool gen_weighted) override {}; ///< set whether to generate weighted or unweighted neutrinos
	
	protected:
	void ConnectTree();
	double FindMaxEnergy();
	
	private:
	TFile fInFile; //In case the class opens the file itself
	
	TTree *fInTree = nullptr;
	long int fCurrIndex = -1; //Curent tree entry
	
	genie::PDGCodeList fFluxParticles;
	double fMaxEnergy = std::nan("ni");
	
	//Definition of Target Station CS
	esbroot::geometry::CoordinateSystem fTSCS;
	//Definition of Detector CS
	esbroot::geometry::CoordinateSystem fDTCS;
	
	//Vertex coordinates (TSCS), to be connected to tree
	float fX = std::nan("ni"), fY = std::nan("ni"), fZ = std::nan("ni");
	//Momentum coordinates (TSCS), to be connected to tree
	float fPx = std::nan("ni"), fPy = std::nan("ni"), fPz = std::nan("ni");
	
	//Momentum DTCS
	TLorentzVector fMomentum;
	//Position DTCS
	TLorentzVector fPosition;
	
	ClassDef(GenieNtpFluxV1, 2)
};

}
}

#endif
