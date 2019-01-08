/*
  .L EsbWCdetectorPoint.cxx+
  .L EsbWCdetector.cxx+
  .L ess_sim.C+
  ess_sim()
 */

#include <FairRunSim.h>
#include <FairModule.h>
#include <FairCave.h>
#include <FairDetector.h>
#include <FairPrimaryGenerator.h>
#include <FairParticleGenerator.h>

#include "EsbWCdetector.h"

void ess_sim(TString outFileName = "evetest.root",
             Int_t nStartEvent = 0, 
	     Int_t nEvents = 10)
{
   FairRunSim* fRun = new FairRunSim(); // create the FairRun Class

   // Choose the Geant Navigation System
   fRun->SetName("TGeant3"); // TGeant3

   // Set Material Definition file
   fRun->SetMaterials("media.geo");

   // Add Passive Modules
   FairModule *Cave= new FairCave("CAVE");
   Cave->SetGeometryFileName("cave.geo");
   fRun->AddModule(Cave);

   // Add Detectors
   FairDetector *nearWc = new EsbWCdetector("NearWcDetector", kTRUE);
   fRun->AddModule(nearWc);

   // Create and Set Event Generator
   FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
   fRun->SetGenerator(primGen);

   FairParticleGenerator* partGen = new FairParticleGenerator(13, 1, 0, 0, 1, 0, 0, 0);
   primGen->AddGenerator(partGen);

   fRun->SetOutputFile(outFileName.Data()); // set output file

   // skip magnetic field

   fRun->Init();

   // Transport nEvents
   fRun->Run(nEvents);
   
   fRun->CreateGeometryFile("geo_full.root");  // for additional full geometry file
}
