/*
  
  Macro that can display the WC detector points
  (e.g., produced by ess_sim.C)
  
*/

void display_event_photons(const Char_t* file_name="evetest.root", 
			   Int_t event=0,
			   const Double_t r_d = 300, // cm
			   const Double_t l_d = 1000) // cm
{
  using namespace esbroot;
  
  gStyle->SetOptStat(0);
  
  TH2D* h_event_hit = 
    new TH2D("h_event_hit", Form("Event #%d: hit dist", event), 
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
  
  TClonesArray* photon_array = 0;
  tree->SetBranchAddress("EsbWCDetectorPoint", &photon_array);
  
  tree->GetEntry(event);
  
  Int_t n_photon = (Int_t)photon_array->GetEntries();
  
  cout << "Number of PHOTON signals: " << n_photon << endl;
  
  for (Int_t n = 0; n < n_photon; n++) {
    
    auto photon = (data::WCDetectorPoint*)photon_array->At(n);
    
    const Double_t x_photon = photon->GetX();
    const Double_t y_photon = photon->GetY();
    const Double_t z_photon = photon->GetZ();
    const Double_t r_photon = TMath::Sqrt(x_photon*x_photon + y_photon*y_photon);
    
    // Find the location of the photon hit
    Int_t location = -1;
    if(TMath::Abs(z_photon - l_d/2.0) < 1) { // front
      location = 1;
    }
    else if(TMath::Abs(z_photon + l_d/2) < 1) { // back
      location = 3;
    }
    else if (TMath::Abs(r_photon - r_d) < 1) { // side
      
      location = 2;
    } else {
    
      cout << "Warning: hit was not on detector edge" << endl;
      continue;
    }

    // Now we need to find out where to fill the hit
    Double_t x=0, y=0;

    if(location == 3) { // back
      
      x = -x_photon;
      y =  y_photon;
    } else if (location == 1) { // front
      
      x = x_photon + l_d + 2*r_d;
      y = y_photon; 
    } else if (location == 2) { // frontside
      
      x = z_photon + r_d + l_d/2.0; 
      y = TMath::ATan2(y_photon, -x_photon) * r_d; 
    }
    
    h_event_hit->Fill(x, y);

    const Double_t t_photon = photon->GetTime();
    h_event_time->Fill(x, y, t_photon);
  }

  TCanvas* c_hit = new TCanvas("c_hit", "PHOTON hit distribution",
				 900, 600);
  c_hit->Draw();
  h_event_hit->Draw("COLZ");

  TCanvas* c_time = new TCanvas("c_time", "PHOTON arrival time distribution",
			       900, 600);
  c_time->Draw();
  h_event_time->Draw("COLZ");
  
  TEllipse* ellipse = new TEllipse();
  ellipse->SetLineWidth(2);
  ellipse->SetFillStyle(0);
  c_hit->cd();  
  ellipse->DrawEllipse(0, 0, r_d, 0, 0, 360, 0);
  ellipse->DrawEllipse(r_d+l_d+r_d, 0, r_d, 0, 0, 360, 0);
  c_time->cd();  
  ellipse->DrawEllipse(0, 0, r_d, 0, 0, 360, 0);
  ellipse->DrawEllipse(r_d+l_d+r_d, 0, r_d, 0, 0, 360, 0);
  
  TBox* box = new TBox();
  box->SetLineWidth(2);
  box->SetFillStyle(0);
  c_hit->cd();  
  box->DrawBox(r_d, -r_d*TMath::Pi(), r_d+l_d, r_d*TMath::Pi());
  c_time->cd();  
  box->DrawBox(r_d, -r_d*TMath::Pi(), r_d+l_d, r_d*TMath::Pi());

  TLatex* latex = new TLatex();
  latex->SetTextSize(0.06);
  latex->SetTextAlign(22);
  c_hit->cd();  
  latex->DrawLatex(0          , r_d*TMath::Pi()+200, "BACK");
  latex->DrawLatex(r_d+l_d/2  , r_d*TMath::Pi()+200, "SIDE");
  latex->DrawLatex(r_d+l_d+r_d, r_d*TMath::Pi()+200, "FRONT");
  c_time->cd();  
  latex->DrawLatex(0          , r_d*TMath::Pi()+200, "BACK");
  latex->DrawLatex(r_d+l_d/2  , r_d*TMath::Pi()+200, "SIDE");
  latex->DrawLatex(r_d+l_d+r_d, r_d*TMath::Pi()+200, "FRONT");
}

