#include "EsbGenerators/GenieNtpFluxV1.h"

#include <TMath.h>
#include <TFile.h>
#include <TTree.h>

#include <iostream>
#include <cassert>

namespace esbroot {
namespace generators {

GenieNtpFluxV1::GenieNtpFluxV1(TTree *in_tree, int pdg_code,
		esbroot::geometry::CoordinateSystem const& TSCS,
		esbroot::geometry::CoordinateSystem const& DTCS
) : fInTree(in_tree), fCurrIndex(0), fTSCS(TSCS), fDTCS(DTCS)
{
	fFluxParticles.push_back(pdg_code);
	ConnectTree();
	fMaxEnergy = FindMaxEnergy();
}

GenieNtpFluxV1::GenieNtpFluxV1(
	const char* file_name,
	const char* tree_name,
	int pdg_code,
	esbroot::geometry::CoordinateSystem const& TSCS,
	esbroot::geometry::CoordinateSystem const& DTCS		
) : fInFile(file_name), fCurrIndex(0), fTSCS(TSCS), fDTCS(DTCS)
{
	fInFile.GetObject(tree_name, fInTree);
	assert(fInTree != nullptr);

	fFluxParticles.push_back(pdg_code);
	ConnectTree();
	fMaxEnergy = FindMaxEnergy();	
}


/*virtual*/ bool GenieNtpFluxV1::GenerateNext(void) /*override*/
{
	if(End()) return false;
	fInTree->GetEntry(fCurrIndex);
	
	TVector3 mom(fPx, fPy, fPz);
	mom = esbroot::geometry::cstransforms::l2l_vec(mom, fTSCS, fDTCS);

	TVector3 pos(fX, fY, fZ);
	pos = esbroot::geometry::cstransforms::l2l_pos(pos, fTSCS, fDTCS);
	pos *= 1.0/100.0; //Genie wants SI units for coordinates

	fMomentum.SetVect(mom);
	fMomentum.SetT(TMath::Sqrt(fPx*fPx + fPy*fPy + fPz*fPz));
	
	fPosition.SetVect(pos);
	fPosition.SetT(0.0);
	
	++fCurrIndex;
	return(true);
}

void GenieNtpFluxV1::ConnectTree()
{
	fInTree->SetBranchAddress("Xp", &fX);
	fInTree->SetBranchAddress("Yp", &fY);
	fInTree->SetBranchAddress("Zp", &fZ);

	fInTree->SetBranchAddress("Pxp", &fPx);
	fInTree->SetBranchAddress("Pyp", &fPy);
	fInTree->SetBranchAddress("Pzp", &fPz);	
}

double GenieNtpFluxV1::FindMaxEnergy()
{
	double max_e = -HUGE_VAL;

	for(long int a = 0; a < fInTree->GetEntries(); ++a) {
		fInTree->GetEntry(a);		
		double e = TMath::Sqrt(fPx*fPx + fPy*fPy + fPz*fPz);
		if(e > max_e) max_e = e;
	}
	return(max_e);
}

}
}
