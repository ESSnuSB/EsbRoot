/*
  
  Macro that can display the WC digits
  (e.g., produced by ess_reco.C)
  
*/

#include <TH2.h>
#include <TProfile2D.h>
#include <TObjArray.h>
#include <TCanvas.h>
#include <TEllipse.h>
#include <TBox.h>
#include <TLatex.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <TTree.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TVector3.h>

#include <EsbData/PMTubeHit.h>

TGraph* get_cone_projection(TVector3 pos,
			    TVector3 dir,
			    const Double_t theta, 
			    const Int_t n_steps = 100,
			    const Double_t r_d = 300, // cm
			    const Double_t l_d = 1000); // cm

void find_ring_pmts(const Char_t* file_name="dst.root", 
		    Int_t event=0,
		    const Double_t r_accept = 0,
		    const Int_t n_min_pmts_in_ring = 50, 
		    const Int_t max_n_rings = 2, 
		    const Double_t r_d = 300, // cm
		    const Double_t l_d = 1000) // cm
{

  using namespace esbroot;
  
  gStyle->SetOptStat(0);
  
  TFile* file = new TFile(file_name);
  TTree* tree = (TTree*)file->Get("cbmsim");
  
  Int_t n_event = (Int_t)tree->GetEntries();  
  cout << " Number of events = " << n_event << endl;
  
  TClonesArray* pmt_array = 0;
  tree->SetBranchAddress("PMTubeHit", &pmt_array);
  
  tree->GetEntry(event);
  
  data::PMTubeHit::sortOption = 1;
  
  pmt_array->Sort();
  
  Int_t n_pmt = (Int_t)pmt_array->GetEntries();
  
  cout << "Number of PMT signals: " << n_pmt << endl;
  
  if(n_pmt <= 0)
    return;
  
  // RefIndex is used to assign clusters to a ring. Start by "unsetting" it
  for (Int_t n = 0; n < n_pmt; n++) {
    
    auto pmt = (data::PMTubeHit*)pmt_array->At(n);
    pmt->SetRefIndex(0);
  }
  
  Int_t n_ring = 0;
  Bool_t more_rings = kTRUE;

  while(n_ring < max_n_rings && more_rings) {
    
    // Find first pmt to be checked
    TObjArray* ring_array = new TObjArray(100);
    more_rings = kFALSE;
    Int_t n_pmt_in_ring = 0;
    for (Int_t n = 0; n < n_pmt; n++) {
      
      auto pmt = (data::PMTubeHit*)pmt_array->At(n);
      if(pmt->GetRefIndex() == 0) {
	
	more_rings = kTRUE;
	ring_array->Add(pmt);
	pmt->SetRefIndex(1);
	n_pmt_in_ring = 1;
	break;
      }
    }
    
    Int_t n_checked = 0;
    Double_t mean_x = 0;
    Double_t mean_y = 0;
    Double_t mean_z = 0;
    Double_t mean_t = 0;
    
    while (n_checked < n_pmt_in_ring) {
      
      auto pmt_check = (data::PMTubeHit*)ring_array->At(n_checked);
      const Double_t x_check = pmt_check->GetX();
      const Double_t y_check = pmt_check->GetY();
      const Double_t z_check = pmt_check->GetZ();
      const Double_t t_check = pmt_check->GetTime();
      
      mean_x += x_check;
      mean_y += y_check;
      mean_z += z_check;
      mean_t += t_check;
      
      for (Int_t n = 0; n < n_pmt; n++) {
	
	auto pmt = (data::PMTubeHit*)pmt_array->At(n);
	
	if(pmt->GetRefIndex() == 1)
	  continue;
	
	//      cout << pmt->GetCharge() << endl;
	
	if(pmt == pmt_check) {
	  cout << "Skip " << n_checked << endl;
	}
	
	const Double_t dx  = pmt->GetX() - x_check;
	const Double_t dy  = pmt->GetY() - y_check;
	const Double_t dz  = pmt->GetZ() - z_check;
	const Double_t cdt = 30*(pmt->GetTime() - t_check); // c=30cm/ns
	
	if(dx*dx + dy*dy + dz*dz + cdt*cdt < r_accept*r_accept) {
	  //      if(dx*dx + dy*dy + dz*dz < r_accept*r_accept) {
	  
	  ring_array->Add(pmt);
	  pmt->SetRefIndex(1);
	  n_pmt_in_ring++;
	}
      }
      
      n_checked++;
    }
    
    mean_x /= n_checked;
    mean_y /= n_checked;
    mean_z /= n_checked;
    mean_t /= n_checked;
    
    cout << "N PMTs in ring: " << n_pmt_in_ring << endl;
    
    if(n_pmt_in_ring >= n_min_pmts_in_ring) {
      
      // Ring found
      n_ring++;

      TH2D* h_ring_charge = 
	new TH2D(Form("h_ring_charge_%d", n_ring), 
		 Form("Event #%d: ring %d charge dist", event, n_ring), 
		 100, -r_d-50, r_d + l_d + 2*r_d + 50,
		 100, -r_d*TMath::Pi()-50, r_d*TMath::Pi()+300);
      
      TProfile2D* h_ring_time = 
	new TProfile2D(Form("h_ring_time_%d", n_ring),
		       Form("Event #%d: ring %d time dist", event, n_ring), 
		       100, -r_d-50, r_d + l_d + 2*r_d + 50,
		       100, -r_d*TMath::Pi()-50, r_d*TMath::Pi()+300);
      
      
      // visualize ring
      for (Int_t n = 0; n < n_pmt_in_ring; n++) {
	
	auto pmt = (data::PMTubeHit*)ring_array->At(n);
	
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
	h_ring_charge->Fill(x, y, q_pmt);
	
	const Double_t t_pmt = pmt->GetTime();
	h_ring_time->Fill(x, y, t_pmt);
      }
      
      TCanvas* c_charge = new TCanvas(Form("c_charge_%d", n_ring), 
				      "PMTube charge distribution",
				      900, 600);
      c_charge->Draw();
      h_ring_charge->Draw("COLZ");
      
      TCanvas* c_time = new TCanvas(Form("c_time_%d", n_ring), 
				    "PMTube arrival time distribution",
				    900, 600);
      c_time->Draw();
      h_ring_time->Draw("COLZ");
      
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
      
      // Try to estimate start fit parameters
      // visualize ring
      
      Double_t mean_r = 0;
      for (Int_t n = 0; n < n_pmt_in_ring; n++) {
	
	auto pmt = (data::PMTubeHit*)ring_array->At(n);
      
	const Double_t dx  = pmt->GetX() - mean_x;
	const Double_t dy  = pmt->GetY() - mean_y;
	const Double_t dz  = pmt->GetZ() - mean_z;
	
	mean_r += TMath::Sqrt(dx*dx + dy*dy + dz*dz);
      }
      
      mean_r /= n_pmt_in_ring;
      
      
      // calculate cone guess
      TVector3 p_0(mean_x, mean_y, mean_z);
      const Double_t p_mag = p_0.Mag(); 
      p_0.SetMag(1);
      
      const Double_t cone_theta = TMath::ACos(1.0/1.33);
      const Double_t L = mean_r / TMath::Sin(cone_theta);
      
      const Double_t dL = p_mag - L;
      
      TVector3 x_0 = dL * p_0;
      
      cout << "(x, y, z) = (" << x_0.X() << ", " << x_0.Y() << ", " << x_0.Z() << ")" << endl;
      cout << "(px, py, pz) = (" << p_0.X() << ", " << p_0.Y() << ", " << p_0.Z() << ")" << endl;
      
      c_charge->cd();  
      TGraph* graph = get_cone_projection(x_0, p_0, cone_theta, 100, r_d, l_d);
      graph->SetLineWidth(3);
      graph->Draw("L");
    }
  }
}

//__________________________________________________________________________
TGraph* get_cone_projection(TVector3 pos,
			    TVector3 dir,
			    const Double_t theta, 
			    const Int_t n_steps,
			    const Double_t r_d,
			    const Double_t l_d)
{
  TVector3 p_perp_1 = dir.Orthogonal();
  TVector3 p_perp_2 = dir.Cross(p_perp_1);
  p_perp_1.SetMag(TMath::Sin(theta));
  p_perp_2.SetMag(TMath::Sin(theta));
  
  TGraph* graph = new TGraph(n_steps);
  
  for(Int_t n = 0; n < n_steps; n++) {
    
    // we want to create an ensemble of unit vectors that will all lie on the
    // cone
    const Double_t phi = n * TMath::TwoPi() / n_steps;
    TVector3 p_line = dir + TMath::Cos(phi)*p_perp_1
      + TMath::Sin(phi)*p_perp_2;
    p_line.SetMag(1);
    
    // Find the location (front, side, back) and distance (steps) of the
    // intersect
    Int_t location = -1;
    Double_t steps = 0;
    {
      const Double_t x  = pos.X(); 
      const Double_t y  = pos.Y(); 
      const Double_t z  = pos.Z(); 
      const Double_t px = p_line.X(); 
      const Double_t py = p_line.Y(); 
      const Double_t pz = p_line.Z(); 
      
      //      cout << "(px, py, pz) = (" << px << ", " << py << ", " << pz << ")" << endl;
      
      Double_t steps_front_back = kMaxLong;
      if(TMath::Abs(pz) > 0.000001) {
	if(pz > 0)
	  steps_front_back = (l_d/2.0 - z) / pz; 
	else
	  steps_front_back = (-l_d/2.0 - z) / pz; 
      }
      R__ASSERT(steps_front_back >= 0);
      
      Double_t steps_side = kMaxLong;
      if(1-TMath::Abs(pz) > 0.000001) {
      
	const Double_t pt_norm_sq = px*px + py*py; 
	const Double_t help_1 = (px*x + py*y)/pt_norm_sq;
	const Double_t help_2 = help_1*help_1 + 
	  (r_d*r_d -x*x - y*y)/pt_norm_sq;
	R__ASSERT(help_2 >= 0);
	// This should always be the correct solution as help_2 >= |help_1|
	const Double_t steps_side_1 = -help_1 + TMath::Sqrt(help_2);
	R__ASSERT(steps_side_1 >= 0);
	const Double_t steps_side_2 = -help_1 - TMath::Sqrt(help_2);
	R__ASSERT(steps_side_1*steps_side_2 <= 0);
	steps_side = steps_side_1;
      }
      
      
      // Find the location of the pmt hit
      if(steps_front_back <= steps_side) {
	steps = steps_front_back;
	if(pz > 0) { // front
	  location = 1;
	} else { // back
	  location = 3;
	}
      } else {
	steps = steps_side;
	location = 2;
      } 
    }
     
    TVector3 pos_hit = pos + steps * p_line;
    const Double_t x_hit = pos_hit.X();
    const Double_t y_hit = pos_hit.Y();
    const Double_t z_hit = pos_hit.Z();
    
    //    cout << location << ", (x_hit, y_hit, z_hit) = (" << x_hit << ", " << y_hit << ", " << z_hit << ")" << endl;
    
    
    Double_t x = 0, y = 0;
    if(location == 3) { // back
      
      x = -x_hit;
      y =  y_hit;
    } else if (location == 1) { // front
      
      x = x_hit + l_d + 2*r_d;
      y = y_hit; 
    } else if (location == 2) { // frontside
      
      x = z_hit + r_d + l_d/2.0; 
      y = TMath::ATan2(y_hit, -x_hit) * r_d; 
    }

    graph->SetPoint(n, x, y);
  }
  
  return graph;
 }
