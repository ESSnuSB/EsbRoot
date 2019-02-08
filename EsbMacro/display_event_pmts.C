/*
  
  Macro that can display the WC digits
  (e.g., produced by ess_reco.C)
  
*/

void display_event_pmts(const Char_t* file_name="dst.root", 
			Int_t event=0,
			const Double_t r_d = 300, // cm
			const Double_t l_d = 1000) // cm
{
  using namespace esbroot;

  gStyle->SetOptStat(0);
  
  TH2D* h_event_charge = 
    new TH2D("h_event_charge", Form("Event #%d: charge dist", event), 
	     100, -r_d-50, r_d + l_d + 2*r_d + 50,
	     100, -r_d*TMath::Pi()-50, r_d*TMath::Pi()+300);
  
  TProfile2D* h_event_time = 
    new TProfile2D("h_event_time", Form("Event #%d: time dist", event), 
		   100, -r_d-50, r_d + l_d + 2*r_d + 50,
		   100, -r_d*TMath::Pi()-50, r_d*TMath::Pi()+300);
  
  TFile* file = new TFile(file_name);
  TTree* tree = (TTree*)file->Get("cbmsim");
  
  Int_t n_event = (Int_t)tree->GetEntries();  
  cout << " Number of events = " << n_event << endl;
  
  TClonesArray* pmt_array = 0;
  tree->SetBranchAddress("PMTubeHit", &pmt_array);
  
  tree->GetEntry(event);
  
  Int_t n_pmt = (Int_t)pmt_array->GetEntries();
  
  cout << "Number of PMT signals: " << n_pmt << endl;
  
  for (Int_t n = 0; n < n_pmt; n++) {
    
    auto pmt = (data::PMTubeHit*)pmt_array->At(n);
    
    const Double_t x_pmt = pmt->GetX();
    const Double_t y_pmt = pmt->GetY();
    const Double_t z_pmt = pmt->GetZ();
    const Double_t r_pmt = TMath::Sqrt(x_pmt*x_pmt + y_pmt*y_pmt);
    
    // Find the location of the pmt hit
    Int_t location = -1;
    if(TMath::Abs(z_pmt - l_d/2.0) < 1) { // front
      location = 1;
    }
    else if(TMath::Abs(z_pmt + l_d/2) < 1) { // back
      location = 3;
    }
    else if (TMath::Abs(r_pmt - r_d) < 1) { // side
      
      location = 2;
    } else {
    
      cout << "Warning: hit was not on detector edge" << endl;
      continue;
    }

    // Now we need to find out where to fill the hit
    Double_t x=0, y=0;

    if(location == 3) { // back
      
      x = -x_pmt;
      y =  y_pmt;
    } else if (location == 1) { // front
      
      x = x_pmt + l_d + 2*r_d;
      y = y_pmt; 
    } else if (location == 2) { // frontside
      
      x = z_pmt + r_d + l_d/2.0; 
      y = TMath::ATan2(y_pmt, -x_pmt) * r_d; 
    }
    
    const Double_t q_pmt = pmt->GetCharge();
    h_event_charge->Fill(x, y, q_pmt);

    const Double_t t_pmt = pmt->GetTime();
    h_event_time->Fill(x, y, t_pmt);
  }

  TCanvas* c_charge = new TCanvas("c_charge", "PMTube charge distribution",
				 900, 600);
  c_charge->Draw();
  h_event_charge->Draw("COLZ");

  TCanvas* c_time = new TCanvas("c_time", "PMTube arrival time distribution",
			       900, 600);
  c_time->Draw();
  h_event_time->Draw("COLZ");
  
  TEllipse* ellipse = new TEllipse();
  ellipse->SetLineWidth(2);
  ellipse->SetFillStyle(0);
  c_charge->cd();  
  ellipse->DrawEllipse(0, 0, r_d, 0, 0, 360, 0);
  ellipse->DrawEllipse(r_d+l_d+r_d, 0, r_d, 0, 0, 360, 0);
  c_time->cd();  
  ellipse->DrawEllipse(0, 0, r_d, 0, 0, 360, 0);
  ellipse->DrawEllipse(r_d+l_d+r_d, 0, r_d, 0, 0, 360, 0);
  
  TBox* box = new TBox();
  box->SetLineWidth(2);
  box->SetFillStyle(0);
  c_charge->cd();  
  box->DrawBox(r_d, -r_d*TMath::Pi(), r_d+l_d, r_d*TMath::Pi());
  c_time->cd();  
  box->DrawBox(r_d, -r_d*TMath::Pi(), r_d+l_d, r_d*TMath::Pi());

  TLatex* latex = new TLatex();
  latex->SetTextSize(0.06);
  latex->SetTextAlign(22);
  c_charge->cd();  
  latex->DrawLatex(0          , r_d*TMath::Pi()+200, "BACK");
  latex->DrawLatex(r_d+l_d/2  , r_d*TMath::Pi()+200, "SIDE");
  latex->DrawLatex(r_d+l_d+r_d, r_d*TMath::Pi()+200, "FRONT");
  c_time->cd();  
  latex->DrawLatex(0          , r_d*TMath::Pi()+200, "BACK");
  latex->DrawLatex(r_d+l_d/2  , r_d*TMath::Pi()+200, "SIDE");
  latex->DrawLatex(r_d+l_d+r_d, r_d*TMath::Pi()+200, "FRONT");
}

