/*

  Macro that shows how to run the WC digitization on an existing simulation
  (e.g., produced with ess_sim.C).

  Based on the example in the presentation from
  Konstantin Gertsenberger
  
  .L ess_dig_fgd.C
  ess_dig_fgd()
  
*/

void ess_only_muon_reconstruction_5(TString inFile = "fgd_dig.root", 
	      TString parFile = "params.root",
	      TString outFile = "fgd_recon.root",
              Int_t nStartEvent = 0,
              Int_t nEvents = 500)
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

  double debugLvl = 0.0; 

  FairTask* recon = new reconstruction::superfgd::FgdMuonOnlyGenFitRecon(
    "Reconstruction Task"             // name of the task
    ,"../../EsbGeometry/EsbSuperFGD/EsbConfig/geometry"  //FIle with detector configuration
    ,"../../geometry/media.geo"       // Media file with defined materials
    , 0.,0.,0.                        // x,y,z detector position
    , 1                               // Verbose level
    , debugLvl                        // debug level of genfit (0 - little, 1 - debug info, 2 - detailed)
    , "../../EsbMacro/tests/muon_mom.dat"  // output file with Monte Carlo and genfit momentum
    , "../../EsbMacro/tests/genie_muons.dat"  // input file with initial starting condistions
    , false                           // To visualize the tracks using genfit::Eventdisplay
    , "D"                             // Option to be passed for genfit::Eventdisplay if used
    ,"../../EsbMacro/tests/MC_Fit_momentum.root"); // root file containing the monte carlo and fitter momentums

  ((reconstruction::superfgd::FgdMuonOnlyGenFitRecon*)recon)->SetInitialEmu(0.6);                      
  
  fRun->AddTask(recon);   
  fRun->Init(); // initializing
  fRun->Run(nStartEvent, nStartEvent + nEvents);
  fRun->CreateGeometryFile("geo_recon.root");  // for additional full geometry file
}
