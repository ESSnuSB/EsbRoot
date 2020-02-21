/*

  Macro that shows how to run the WC digitization on an existing simulation
  (e.g., produced with ess_sim.C).

  Based on the example in the presentation from
  Konstantin Gertsenberger
  
  .L ess_dig_fgd.C
  ess_dig_fgd()
  
*/

void simulate_3_MC_lepton_stats(TString inFile = "fgd_dig.root", 
	      TString parFile = "params.root",
	      TString outFile = "fgd_mc_recon_stats.root",
              Int_t nStartEvent = 0, Int_t nEvents = 50)
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

  fair::Logger::SetConsoleSeverity(fair::Severity::debug2);
  fair::Logger::SetConsoleColor(true);

  FairTask* recon = new reconstruction::superfgd::FgdMCLeptonStats(
    "Reconstruction MC Task"             // name of the task
    ,"../../EsbGeometry/EsbSuperFGD/EsbConfig/fgdconfig"  //File with detector configuration
    ,"../../geometry/media.geo"       // Media file with defined materials
    ,"../../EsbMacro/tests/eventsData.dat"       // events data file
    ,"../../EsbMacro/tests/statsRoot.root"       // output root file
    , 1                               // Verbose level
    , debugLvl                        // debug level of genfit (0 - little, 1 - debug info, 2 - detailed)
    );                           

  ((reconstruction::superfgd::FgdMCLeptonStats*)recon)->SetMinHits(3);

  
  fRun->AddTask(recon);   
  fRun->Init(); // initializing
  fRun->Run(nStartEvent, nStartEvent + nEvents);
  fRun->CreateGeometryFile("geo_recon_mc_lepton.root");  // for additional full geometry file
}
