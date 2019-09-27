/*

  Macro that shows how to run a WC simulation.

*/

void simulate_1_fairgenerator(TString outFileName = "evetest.root",
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
  //FairDetector *nearWc = new geometry::WCDetector("NearWcDetector", 300, 500, kTRUE);
  //fRun->AddModule(nearWc);

  FairDetector* fgd = new geometry::FgdDetector("Granular Detector","../../EsbGeometry/EsbSuperFGD/EsbConfig/geometry",0,0,0, kTRUE);
  fRun->AddModule(fgd);

  double Bx(0), By(0), Bz(0);
  ((geometry::FgdDetector*)fgd)->GetMagneticField(Bx, By, Bz);
  double xMin(0), xMax(0), yMin(0), yMax(0), zMin(0), zMax(0);
  ((geometry::FgdDetector*)fgd)->GetMagneticFieldRegion(xMin, xMax, yMin, yMax, zMin, zMax);
  FairConstField* fgdField = new FairConstField();
  fgdField->SetField(Bx, By, Bz);
  fgdField->SetFieldRegion(xMin, xMax, yMin, yMax, zMin, zMax);
  fRun->SetField(fgdField);
  
  
  // Far Detector
  // FairDetector *farWc = new EsbWCDetector("FarWcDetector", 1000, 2000, kTRUE);
  // fRun->AddModule(farWc);
  
  // Create and Set Event Generator
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  fRun->SetGenerator(primGen);
  
  //~ FairParticleGenerator* partGen = new FairParticleGenerator(2212, 1, 0, 0, 1, 0, 0, 0);
  //FairParticleGenerator* partGen = new FairParticleGenerator(13, 1, 0, 0, 0.4, 0, 0, 150);
  //FairParticleGenerator* partGen = new FairParticleGenerator(2212, 1, 0.1, 0.1, 0.4, 0.5, 0.5, -50); // Initial
  FairParticleGenerator* partGen = new FairParticleGenerator(2212, 1, 0., 0., 1.606, 0.5, 0.5, -50);
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
