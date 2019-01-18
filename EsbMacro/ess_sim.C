/*
  .L EsbWCDetectorPoint.cxroot -x+
  .L EsbWCDetector.cxx+
  .L ess_sim.C+
  ess_sim()
 */

//~ #include <FairRunSim.h>
//~ #include <FairModule.h>
//~ #include <FairCave.h>
//~ #include <FairDetector.h>
//~ #include <FairPrimaryGenerator.h>
//~ #include <FairParticleGenerator.h>

//~ #include <esb/EsbWCDetector.h>
//~ #include <passive/EsbCave.h>

void ess_sim(TString outFileName = "evetest.root",
             Int_t nStartEvent = 0, 
	     Int_t nEvents = 1)
{
   FairRunSim* fRun = new FairRunSim(); // create the FairRun Class

   // Choose the Geant Navigation System
   fRun->SetName("TGeant4"); // TGeant3/4
	 //~ fRun->SetIsMT(true);
	 
   // Set Material Definition file
   fRun->SetMaterials("media.geo");

   // Add Passive Modules
   FairModule *Cave= new EsbCave("CAVE");
   //~ FairModule *Cave= new FairCave("CAVE");
   Cave->SetGeometryFileName("cave.geo");
   fRun->AddModule(Cave);

   // Add Detectors
   FairDetector *nearWc = new EsbWCDetector("NearWcDetector", kTRUE);
   fRun->AddModule(nearWc);

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
   fRun->Run(nEvents);
   
   fRun->CreateGeometryFile("geo_full.root");  // for additional full geometry file
}
