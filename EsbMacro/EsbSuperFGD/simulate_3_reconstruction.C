/*

  Macro that shows how to run the WC digitization on an existing simulation
  (e.g., produced with ess_sim.C).

  Based on the example in the presentation from
  Konstantin Gertsenberger
  
  .L ess_dig_fgd.C
  ess_dig_fgd()
  
*/

void simulate_3_reconstruction(TString inFile = "fgd_dig.root", 
	      TString parFile = "params.root",
	      TString outFile = "fgd_recon.root",
              Int_t nStartEvent = 4, Int_t nEvents = 1)
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
  TVector3 mom(-0.107,0.211,-0.220);
  TVector3 pos(0.,0.,0.);

  double debugLvl = 0.0; 

  fair::Logger::SetConsoleSeverity(fair::Severity::debug);

  FairTask* recon = new reconstruction::superfgd::FgdGenFitRecon(
    "Reconstruction Task"             // name of the task
    ,"../../EsbGeometry/EsbSuperFGD/EsbConfig/geometry"  //FIle with detector configuration
    ,"../../geometry/media.geo"       // Media file with defined materials
    // ,"../../EsbGeometry/EsbSuperFGD/EsbConfig/reconstructionTemplates"       // Reconstruction templates
    ,"../../EsbGeometry/EsbSuperFGD/EsbConfig/reconTemps"       // Reconstruction templates
    , pos                             // initial position estimation (will be removed later on)
    , mom                             // initial momentum estimation (will be removed later on)
    , 1                               // Verbose level
    , debugLvl                        // debug level of genfit (0 - little, 1 - debug info, 2 - detailed)
    , true                           // To visualize the tracks using genfit::Eventdisplay
    , "D");                           // Option to be passed for genfit::Eventdisplay if used

  ((reconstruction::superfgd::FgdGenFitRecon*)recon)->SetMinHits(10);
  ((reconstruction::superfgd::FgdGenFitRecon*)recon)->SetUseTracker(reconstruction::superfgd::FgdGenFitRecon::TrackFinder::HOUGH_PATHFINDER_ALL);
  // ((reconstruction::superfgd::FgdGenFitRecon*)recon)->SetUseTracker(reconstruction::superfgd::FgdGenFitRecon::TrackFinder::HOUGH_PATHFINDER_ABOVE_BELOW);
  // ((reconstruction::superfgd::FgdGenFitRecon*)recon)->SetUseTracker(reconstruction::superfgd::FgdGenFitRecon::TrackFinder::HOUGH_PATHFINDER_TIME_INTERVALS);
  // ((reconstruction::superfgd::FgdGenFitRecon*)recon)->SetUseTracker(reconstruction::superfgd::FgdGenFitRecon::TrackFinder::USE_GRAPH);
  
  fRun->AddTask(recon);   
  fRun->Init(); // initializing
  fRun->Run(nStartEvent, nStartEvent + nEvents);
  fRun->CreateGeometryFile("geo_recon.root");  // for additional full geometry file
}
