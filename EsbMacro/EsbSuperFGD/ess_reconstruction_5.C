/*

  Macro that shows how to run the WC digitization on an existing simulation
  (e.g., produced with ess_sim.C).

  Based on the example in the presentation from
  Konstantin Gertsenberger
  
  .L ess_dig_fgd.C
  ess_dig_fgd()
  
*/

void ess_reconstruction_5(TString inFile = "fgd_dig.root", 
	      TString parFile = "params.root",
	      TString outFile = "fgd_recon.root",
              Int_t nStartEvent = 0, Int_t nEvents = 1)
{
  using namespace esbroot;

  FairRunAna *fRun= new FairRunAna();
  // Set Input Source and Output file
  FairFileSource *fFileSource = new FairFileSource(inFile);
  fRun->SetSource(fFileSource);

  fRun->SetSink(new FairRootFileSink(outFile));

  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();

  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  parIo1->open(parFile);
  rtdb->setFirstInput(parIo1);
  
  rtdb->setOutput(parIo1); 
  rtdb->saveOutput();

  // Set Tasks for Reconstruction
  TVector3 mom(0.5,0.7,0.45);
  TVector3 pos(0.5,0.5,-50);

  FairTask* recon = new reconstruction::superfgd::FgdGenFitRecon(
    "Reconstruction Task","../../EsbGeometry/EsbSuperFGD/EsbConfig/geometry"
    ,"../../geometry/media.geo"
    , pos, mom, 1, 0.0);

  fRun->AddTask(recon);   
  fRun->Init(); // initializing
  fRun->Run(nStartEvent, nStartEvent + nEvents);
}
