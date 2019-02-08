/*
  .L create_pmt_geometry.C
  create_pmt_geometry()

  // Near Detector Dense
  create_pmt_geometry(300, 500, 33, 62, 10, 30, 10, kFALSE, kTRUE);

  // Near Detector Sparse
  create_pmt_geometry(300, 500, 16, 31, 5, 60, 10, kFALSE, kTRUE);

*/

using namespace esbroot;

void draw_pmt_layout(const TClonesArray* pmt_array, 
		     const Double_t wc_radius, 
		     const Double_t wc_half_length);

TClonesArray* 
create_pmt_geometry(Double_t wc_radius = 300, // cm 
		    Double_t wc_half_length = 500, // cm 
		    Int_t n_z=33, 
		    Int_t n_phi=62, 
		    Int_t n_r=10, 
		    Int_t d_phi=30, // cm
		    Double_t r=10,
		    Bool_t check_overlaps=kFALSE,// to be implemented
		    Bool_t visualize=kTRUE) // cm 
{
  
  tools::PMTubeGeoCreator pmt_geo_creator(wc_radius, wc_half_length, 
					  n_z, n_phi, n_r, d_phi, r);
  TClonesArray* /* OUT */ pmt_array = 
    pmt_geo_creator.CreateGeometry();
  
  // Calculate PMT coverage
  const Double_t wc_surface = 
    TMath::TwoPi()* wc_radius * (2*wc_half_length) + // Barrel
    2* TMath::Pi() * wc_radius*wc_radius; // endcaps
  const Double_t pmt_surface =
    pmt_array->GetEntries() * TMath::Pi() * r*r;
  
  cout << "Coverage: " <<  pmt_surface / wc_surface << endl;
  
  if (visualize) 
    draw_pmt_layout(pmt_array, wc_radius, wc_half_length);

  return pmt_array;
}

//__________________________________________________________________
void draw_pmt_layout(const TClonesArray* pmt_array, 
		     const Double_t wc_radius, // cm 
		     const Double_t wc_half_length) // cm
{
  gStyle->SetOptStat(0);
  // The geometry is hardcoded for now
  const Double_t r_d = wc_radius;
  const Double_t l_d = 2*wc_half_length;
  TH2D* hDisplay = new TH2D("hDisplay", "WC PMT Layout Display", 
			    100, -r_d-50, r_d + l_d + 2*r_d + 50,
			    100, -r_d*TMath::Pi()-50, r_d*TMath::Pi()+300);
  
  TEllipse* ellipse = new TEllipse();
  ellipse->SetLineWidth(1);
  ellipse->SetFillStyle(0);

  TCanvas* cDisplay = new TCanvas("cDisplay", "WC PMT Layout Display", 
				  1200, 900);
  cDisplay->Draw();
  hDisplay->Draw();
  
  Int_t n_pmt = pmt_array->GetEntries();
  cout << "nPMTs: " << n_pmt << endl;

  for (Int_t n = 0; n < n_pmt; n++) {
    
    geometry::PMTube* pmtube = 
      (geometry::PMTube*)pmt_array->At(n);
    
    const Double_t x_pmt = pmtube->GetX();
    const Double_t y_pmt = pmtube->GetY();
    const Double_t z_pmt = pmtube->GetZ();
    const Double_t r_pmt = TMath::Sqrt(x_pmt*x_pmt + y_pmt*y_pmt);
    
    //    cout << x_pmt << ", " << y_pmt << ", " << z_pmt << ", " << r_pmt << endl;

    // Find the location of the pmt
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
    
    ellipse->DrawEllipse(x, y, pmtube->GetRadius(), 0, 0, 360, 0);
  }
  
  ellipse->SetLineWidth(2);
  ellipse->DrawEllipse(0, 0, r_d, 0, 0, 360, 0);
  ellipse->DrawEllipse(r_d+l_d+r_d, 0, r_d, 0, 0, 360, 0);
  
  TBox* box = new TBox();
  box->SetLineWidth(2);
  box->SetFillStyle(0);
  box->DrawBox(r_d, -r_d*TMath::Pi(), r_d+l_d, r_d*TMath::Pi());
  
  TLatex* latex = new TLatex();
  latex->SetTextSize(0.06);
  latex->SetTextAlign(22);
  latex->DrawLatex(0          , r_d*TMath::Pi()+200, "BACK");
  latex->DrawLatex(r_d+l_d/2  , r_d*TMath::Pi()+200, "SIDE");
  latex->DrawLatex(r_d+l_d+r_d, r_d*TMath::Pi()+200, "FRONT");
}
