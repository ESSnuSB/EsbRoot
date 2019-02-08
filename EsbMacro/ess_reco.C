/*

  Macro that shows how to run the WC digitization on an existing simulation
  (e.g., produced with ess_sim.C).

  Based on the example in the presentation from
  Konstantin Gertsenberger
  
  .L create_pmt_geometry.C
  .L ess_reco.C
  ess_reco()
  
*/

void ess_reco(TString inFile = "evetest.root", 
	      TString parFile = "params.root",
	      TString outFile = "dst.root",
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

  // Create PMTs
  TClonesArray* pmt_array =
    create_pmt_geometry(300, 500, 16, 31, 5, 60, 10, kFALSE, kTRUE);

  // Set Tasks for Reconstruction
  FairTask* digitizer = new digitizer::WCDigitizer("Task1", pmt_array);
  //  FairTask* clustering = new ClusteringTask(“Task2");
  //  FairTask* tracking = new TrackingTask(“Task3");
  fRun->AddTask(digitizer);   //fRun->AddTask(clustering);   fRun->AddTask(tracking);
  fRun->Init(); // initializing
  fRun->Run(nStartEvent, nStartEvent + nEvents);
}
