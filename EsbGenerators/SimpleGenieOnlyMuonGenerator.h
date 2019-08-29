#ifndef ESBROOT_GENERATOR_SIMPLEGENIE_ONLY_MUON_GENERATOR_H
#define ESBROOT_GENERATOR_SIMPLEGENIE_ONLY_MUON_GENERATOR_H

#include "EsbGenerators/GenieOnlyMuonGenerator.h"

#include <cmath>

class TVector3;
class TLorentzVector;

namespace esbroot {
namespace generators {

//! @brief Generates monoenergetic neutrino interactions on a single target material
//!
//! Generates interaction vertices of a single species of neutrino with a specified
//! energy and on a target which composes of a single isotope. Meant to be used as an example
//! and for debugging of the detector simulation.
//!
//! @authors Budimir Kliček
//! @copyright ESSnuSB consortium (<https://essnusb.eu>). Released under GPLv3 (<https://www.gnu.org/licenses/gpl-3.0.en.html>).
class SimpleGenieOnlyMuonGenerator : public GenieOnlyMuonGenerator
{
	public:
	//! Empty constructor
	SimpleGenieOnlyMuonGenerator(): GenieOnlyMuonGenerator() {};
	
	//! Basic constructor
	SimpleGenieOnlyMuonGenerator(int pdg_tgt, int pdg_nu, double e_nu,
		TVector3 const& dir_nu, TLorentzVector const& x4_nu);
	
	//! Implementation of GenieOnlyMuonGenerator::Init() which calls the Configure() function.
	virtual Bool_t Init() {return(Configure());};
	
	//! Post processes Genie events to choose vertex position
	virtual void PostProcessEvent(/*IN OUT*/ genie::GHepRecord* event);

	private:
	//! 4-position of neutrino interaction vertex
	TLorentzVector fVertexX4 = {std::nan("ni"), std::nan("ni"), std::nan("ni"), std::nan("ni")};
	
	ClassDef(SimpleGenieOnlyMuonGenerator,2)
};

}
}

#endif
