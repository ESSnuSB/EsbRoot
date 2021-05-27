#include "EsbTools/GenieAnaEventData.h"

#include <TTree.h>
#include <TObjString.h>

#include <iostream>
#include <cstddef>

namespace esbroot {
namespace tools {

void GenieAnaEventData::ConnectToTree(TTree* gtrac_tree)
{
	gtrac_tree->SetBranchAddress("EvtNum", &EvtNum);
	gtrac_tree->SetBranchAddress("EvtCode", &EvtCode);
	gtrac_tree->SetBranchAddress("StdHepN", &StdHepN); //Number of particles in event
	gtrac_tree->SetBranchAddress("StdHepPdg", StdHepPdg); //PDG codes of particles
	gtrac_tree->SetBranchAddress("StdHepStatus", StdHepStatus); //GENIE status of particles
	gtrac_tree->SetBranchAddress("StdHepP4", StdHepP4); //Four-momentum of particles (px, py, pz, E)
	gtrac_tree->SetBranchAddress("StdHepFd", StdHepFd); //First daughter index
	gtrac_tree->SetBranchAddress("StdHepLd", StdHepLd); //Last daughter index
	gtrac_tree->SetBranchAddress("StdHepFm", StdHepFm); //First mother index
	gtrac_tree->SetBranchAddress("StdHepLm", StdHepLm);//Last mother index		
}

void GenieAnaEventData::PrintEvent()
{
	using namespace std;
	
	cout << endl << "**** Event " << EvtNum << " ****" << endl;
	cout << "Event code: " << EvtCode->GetString() << endl;
	cout << "Number of Hep particles: " << StdHepN << endl;
	
	cout << right << setw(3) << "In" << setw(12) << "PDG Code"
			<< setw(4) << "St"
			<< setw(4) << "Fd" << setw(4) << "Ld"
			<< setw(4) << "Fm" << setw(4) << "Lm"
			<< setw(10) << "Px (GeV)" << setw(10) << "Py (GeV)"
			<< setw(10) << "Pz (Gev)" << setw(10) << "E (GeV)"
			<< endl;
			
	cout << "---------------------------------------------------------------------------" << endl;
	
	for(int part = 0; part < StdHepN; ++part) {
		cout << right << setw(3) << part << setw(12) << StdHepPdg[part]
			<< setw(4) << StdHepStatus[part]
			<< setw(4) << StdHepFd[part] << setw(4) << StdHepLd[part]
			<< setw(4) << StdHepFm[part] << setw(4) << StdHepLm[part]
			<< setprecision(3)
			<< setw(10) << StdHepP4[part][0] << setw(10) << StdHepP4[part][1]
			<< setw(10) << StdHepP4[part][2] << setw(10) << StdHepP4[part][3]
			<< endl;
	}	
}

}
}
