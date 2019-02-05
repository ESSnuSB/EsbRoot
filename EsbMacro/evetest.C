#include "FairLogger.h" 

void evetest(void)
{
	using namespace esbroot;
	
	fair::Logger::SetConsoleSeverity("DEBUG");	
	auto infile = new TFile("evetest.root");
	TTree *simtree; infile->GetObject("cbmsim", simtree);
	
	//~ int ntrack;
	//~ simtree->SetBranchAddress("MCTrack", &ntrack);

	//~ TClonesArray *mctrack_arr = new TClonesArray("EsbMCTrack");
	TClonesArray *mctrack_arr = nullptr;
	simtree->SetBranchAddress("MCTrack", &mctrack_arr);
	//~ TClonesArray *detpoint_arr = new TClonesArray("EsbWCDetectorPoint");
	TClonesArray *detpoint_arr = nullptr;
	simtree->SetBranchAddress("EsbWCDetectorPoint", &detpoint_arr);

	simtree->GetEntry(0);
	cout << mctrack_arr->GetEntries() << endl;
	cout << detpoint_arr->GetEntries() << endl;

	exit(0);

	for(int a = 0; a < mctrack_arr->GetEntries(); ++a) {
		EsbMCTrack *mctrack = (EsbMCTrack*) mctrack_arr->At(a);
		if(mctrack->GetPdgCode() == 50000050) continue; //cherenkov photon
		cout << a << " " 
			<< mctrack->GetPdgCode() << " "
			<< mctrack->GetMotherId() << " "
			<< "(" << mctrack->GetPx() << " " << mctrack->GetPy() << " " << mctrack->GetPz() << ") "
			<< "(" << mctrack->GetStartX() << " " << mctrack->GetStartY() << " " << mctrack->GetStartZ() << ")"
			<< endl;
		//~ mctrack->Print(a);
	}


	for(int a = 0; a < detpoint_arr->GetEntries(); ++a) {
		EsbWCDetectorPoint *detpoint = (EsbWCDetectorPoint*) detpoint_arr->At(a);
		//~ cout << a << " " 
			//~ << mctrack->GetPdgCode() << " "
			//~ << mctrack->GetMotherId() << " "
			//~ << "(" << mctrack->GetPx() << " " << mctrack->GetPy() << " " << mctrack->GetPz() << ") "
			//~ << "(" << mctrack->GetStartX() << " " << mctrack->GetStartY() << " " << mctrack->GetStartZ() << ")"
			//~ << endl;
		detpoint->Print("");
	}

	
	//~ TestTrack testtrack;
	
}

   //~ Int_t    GetPdgCode()  const { return fPdgCode; }
    //~ Int_t    GetMotherId() const { return fMotherId; }
    //~ Double_t GetPx()       const { return fPx; }
    //~ Double_t GetPy()       const { return fPy; }
    //~ Double_t GetPz()       const { return fPz; }
    //~ Double_t GetStartX()   const { return fStartX; }
    //~ Double_t GetStartY()   const { return fStartY; }
    //~ Double_t GetStartZ()   const { return fStartZ; }
