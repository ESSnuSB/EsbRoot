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

void SetWCParameters()
{
   // Set Cherenkov parameters
  // Just some I found online for seawater for now
  // TGeo units are GeV and cm
  const Double_t eV = 1.0e-9;
  const Double_t m  = 100.;
  const Double_t nm = m / 1.0e9 ;
  
  static const Int_t nEnergies = 32;
  static Double_t photEnergies[nEnergies] = {
    2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
    2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
    2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
    2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
    2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
    3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
    3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
    3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV
  };
  static Double_t refractIdxSeaWater[nEnergies] = {
    1.3429, 1.3432, 1.3435, 1.3438, 1.3441,
    1.3445, 1.3448, 1.3452, 1.3456, 1.3461,
    1.3465, 1.3470, 1.3475, 1.3481, 1.3487,
    1.3493, 1.3500, 1.3507, 1.3514, 1.3522,
    1.3531, 1.3540, 1.3551, 1.3561, 1.3573,
    1.3586, 1.3600, 1.3615, 1.3632, 1.3650,
    1.3669, 1.3691
  };
  static Double_t absSeaWater[nEnergies] = {
    3.448*m,  4.082*m,  6.329*m,  9.174*m, 12.346*m, 13.889*m,
    15.152*m, 17.241*m, 18.868*m, 20.000*m, 26.316*m, 35.714*m,
    45.455*m, 47.619*m, 52.632*m, 52.632*m, 55.556*m, 52.632*m,
    52.632*m, 47.619*m, 45.455*m, 41.667*m, 37.037*m, 33.333*m,
    30.000*m, 28.500*m, 27.000*m, 24.500*m, 22.000*m, 19.500*m,
    17.500*m, 14.500*m
  };
  
  // efficiency: not sure what this is...
  static Double_t efficiency[nEnergies];
  for(Int_t i = 0; i < nEnergies; i++) {
    
    //~ efficiency[i] = 0;
    efficiency[i] = 1;
  }
  
  TVirtualMC *MC = TVirtualMC::GetMC();
  
  cout << MC << endl;
  
  const Int_t waterId  = MC->MediumId("WcWater");
  MC->SetCerenkov(waterId, nEnergies,
		   photEnergies, absSeaWater,
		   efficiency, refractIdxSeaWater);
}

void ess_sim(TString outFileName = "evetest.root",
             Int_t nStartEvent = 0, 
	     Int_t nEvents = 1)
{
   FairRunSim* fRun = new FairRunSim(); // create the FairRun Class

   // Choose the Geant Navigation System
   fRun->SetName("TGeant4"); // TGeant3

   // Set Material Definition file
   fRun->SetMaterials("media.geo");

	 cout << "\nHello!\n" << endl;

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

   //~ FairParticleGenerator* partGen = new FairParticleGenerator(13, 1, 0, 0, 1, 0, 0, 0);
   FairParticleGenerator* partGen = new FairParticleGenerator(13, 1, 0, 0, 1, 0, 0, 0);
   primGen->AddGenerator(partGen);

   fRun->SetOutputFile(outFileName.Data()); // set output file

   // skip magnetic field

   fRun->Init();
	 //~ SetWCParameters();


   // Transport nEvents
   fRun->Run(nEvents);
   
   fRun->CreateGeometryFile("geo_full.root");  // for additional full geometry file
}
