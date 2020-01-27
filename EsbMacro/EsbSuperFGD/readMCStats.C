void readMCStats()
{
    TFile* tf = new TFile("statsRoot.root");
    TTree *tr = (TTree*)tf->Get("FgdMCLeptonStatsReconstructionData");

    esbroot::reconstruction::superfgd::FgdMCEventRecord* dat = nullptr;
    tr->SetBranchAddress("FgdMCLeptonStatsBranch", &dat);

    int entries = tr->GetEntries();
    for(int i=0; i < entries; ++i) 
    { 
        cout << "               Event " << i << "           "<< endl;
        tr->GetEntry(i); 
        dat->ReadEventData();
        const std::vector<std::pair<Int_t, TVector3>>& primPars =  dat->GetPrimaryParticles();
        cout << "          Muon Momentum " << dat->GetMuonMom().Mag() << endl;
        cout << "        Neutrino Energy " << dat->GetNuE() << endl;
        cout << "    IsPrimaryLeptonMuon " << dat->IsPrimaryLeptonMuon() << endl;
        cout << "               IsWeakCC " << dat->IsWeakCC() << endl;
        cout << "               IsWeakNC " << dat->IsWeakNC() << endl;
        cout << "         IsQuasiElastic " << dat->IsQuasiElastic() << endl;
        for(int j = 0; j < primPars.size(); ++j)
        {
            cout << "                        Pdg " << primPars[j].first << endl;
        }
        cout << "=======================================" << endl;
    }
}
