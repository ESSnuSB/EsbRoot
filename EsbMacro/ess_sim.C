/*

  Macro that shows how to run a WC simulation.

*/

void ess_sim(TString outFileName = "evetest.root",
             Int_t nStartEvent = 0, 
	     Int_t nEvents = 1)
{
  using namespace esbroot;
  
  FairRunSim* fRun = new FairRunSim(); // create the FairRun Class
  // Peter: SetStoreTraj seems to be needed for the official Eve
  // visualization. It creates the branch GeoTracks in the output tree.
  fRun->SetStoreTraj();
  
  // Choose the Geant Navigation System
  fRun->SetName("TGeant4"); // TGeant3/4
  //~ fRun->SetIsMT(true);
  
  // Set Material Definition file
  fRun->SetMaterials("media.geo");
  
  // Add Passive Modules
  FairModule *cave= new geometry::Cave("CAVE");
  //~ FairModule *Cave= new FairCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  fRun->AddModule(cave);
  
  // Add Detectors
  FairDetector *nearWc = new geometry::WCDetector("NearWcDetector", 300, 500, kTRUE);
  fRun->AddModule(nearWc);

  FgdDetector* fgd = new geometry::FgdDetector("/home/georgi/opt/Essnusb/ESSnuSB-soft/EsbGeometry/EsbSuperFGD/EsbConfig/geometry");
  fRun->AddModule(fgd);
  
  // Far Detector
  // FairDetector *farWc = new EsbWCDetector("FarWcDetector", 1000, 2000, kTRUE);
  // fRun->AddModule(farWc);
  
  // Create and Set Event Generator
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  fRun->SetGenerator(primGen);
  
  //~ FairParticleGenerator* partGen = new FairParticleGenerator(2212, 1, 0, 0, 1, 0, 0, 0);
  FairParticleGenerator* partGen = new FairParticleGenerator(13, 1, 0, 0, 0.4, 0, 0, 150);
  primGen->AddGenerator(partGen);

  fRun->SetOutputFile(outFileName.Data()); // set output file
  
  // skip magnetic field
  
  fRun->Init();
  //~ cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << nearWc->svList->GetEntries() << endl;
  
  // Transport nEvents
  
  // Peter: for the event display to work one need to create a "parameter"
  // file. The code is taken from the example from Konstantin Gertsenberger.
  // Peter: this is also needed for the reconstruction.
  FairRuntimeDb *rtdb = fRun->GetRuntimeDb();
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* output = new FairParRootFileIo(kParameterMerged);
  output->open("params.root");
  rtdb->setOutput(output);
  rtdb->saveOutput();
  
  fRun->Run(nEvents);
  
  fRun->CreateGeometryFile("geo_full.root");  // for additional full geometry file
}
