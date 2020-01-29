#include "EsbGenerators/SimpleGenieGenerator.h"

namespace esbroot {
namespace generators {

//! @param[in] pdg_tgt	PDG code of the target
//! @param[in] pdg_nu		PDG code of neutrino
//! @param[in] e_nu			Energy of neutrino (GeV)
//! @param[in] dir_nu		Direction of incoming neutrino (normalization of this vector does not matter)
//! @param[in] x4_nu		4-position of neutrino interaction vertex (cm)
SimpleGenieGenerator::SimpleGenieGenerator(int pdg_tgt, int pdg_nu, double e_nu,
		TVector3 const& dir_nu, TLorentzVector const& x4_nu) : GenieGenerator(), fVertexX4(x4_nu)
{
		auto mono_flux = std::make_shared<genie::flux::GMonoEnergeticFlux>(e_nu, pdg_nu);
		mono_flux->SetDirectionCos(dir_nu.X(), dir_nu.Y(), dir_nu.Z());
		SetFluxI(std::move(mono_flux));
		
		SetGeomI(std::make_shared<genie::geometry::PointGeomAnalyzer>(pdg_tgt));
}

//! @bug This if to be considered quick&dirty implementation.
/*virtual*/ void SimpleGenieGenerator::PostProcessEvent(/*IN OUT*/ genie::GHepRecord* event)
{
		TLorentzVector* v = event->Vertex(); //TODO: each track has its own origin
		
		*v += fVertexX4;		
		event->SetVertex(*v);
}

}
}
