/*
#include <TFile.h>
#include <TTree.h>
#include <TError.h>
#include <TMath.h>
#include <TH2.h>
#include <TStyle.h>
#include <TEllipse.h>
#include <TBox.h>
#include <TLatex.h>

#include <iostream>

using namespace std;
*/

void display_event_ND(const Char_t* fileName="evetest.root", Int_t event=0)
{
  gStyle->SetOptStat(0);
  
  // if(!geomLoaded)
  //   load_geom(fileName);

  // The geometry is hardcoded for now
  const Double_t rND = 300;
  const Double_t lND = 1000;
  TH2D* hEvent = new TH2D("hEvent", Form("Event #%d", event), 
			  100, -rND-50, rND + lND + 2*rND + 50,
			  100, -rND*TMath::Pi()-50, rND*TMath::Pi()+300);
  
  TFile* file = new TFile(fileName);
  TTree* tree = (TTree*)file->Get("cbmsim");
  
  Int_t nEvent = (Int_t)tree->GetEntries();  
  cout << " nEvents = " << nEvent << endl;

  TClonesArray* photonArray = 0;
  tree->SetBranchAddress("EsbWCdetectorPoint"  , &photonArray);

  tree->GetEntry(event);

  Int_t nPhotons = (Int_t)photonArray->GetEntries();

  cout << "nPhotons: " << nPhotons << endl;
  
  for (Int_t n = 0; n < nPhotons; n++) {
    
    EsbWCdetectorPoint* photon = 
      (EsbWCdetectorPoint*)photonArray->At(n);
    
    const Double_t xP = photon->GetX();
    const Double_t yP = photon->GetY();
    const Double_t zP = photon->GetZ();
    const Double_t rP = TMath::Sqrt(xP*xP + yP*yP);
    
    // Find the location of the photon hit
    Int_t location = -1;
    if(TMath::Abs(zP - lND/2.0) < 1) { // front
      location = 1;
    }
    else if(TMath::Abs(zP + lND/2) < 1) { // back
      location = 3;
    }
    else if (TMath::Abs(rP - rND) < 1) { // side
      
      location = 2;
    } else {
    
      cout << "Warning: hit was not on detector edge" << endl;
      continue;
    }

    // Now we need to find out where to fill the hit
    Double_t x=0, y=0;

    if(location == 3) { // back
      
      x = -xP;
      y =  yP;
    } else if (location == 1) { // front
      
      x = xP + lND + 2*rND;
      y = yP; 
    } else if (location == 2) { // frontside
      
      x = zP + rND + lND/2.0; 
      y = TMath::ATan2(yP, -xP) * rND; 
    }
    
    hEvent->Fill(x, y);
  }

  hEvent->Draw("COLZ");
  
  TEllipse* ellipse = new TEllipse();
  ellipse->SetLineWidth(2);
  ellipse->SetFillStyle(0);
  ellipse->DrawEllipse(0, 0, rND, 0, 0, 360, 0);
  ellipse->DrawEllipse(rND+lND+rND, 0, rND, 0, 0, 360, 0);
  
  TBox* box = new TBox();
  box->SetLineWidth(2);
  box->SetFillStyle(0);
  box->DrawBox(rND, -rND*TMath::Pi(), rND+lND, rND*TMath::Pi());

  TLatex* latex = new TLatex();
  latex->SetTextSize(0.06);
  latex->SetTextAlign(22);
  latex->DrawLatex(0          , rND*TMath::Pi()+200, "BACK");
  latex->DrawLatex(rND+lND/2  , rND*TMath::Pi()+200, "SIDE");
  latex->DrawLatex(rND+lND+rND, rND*TMath::Pi()+200, "FRONT");
}

