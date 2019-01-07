#include <FairCave.h>
#include <FairPipe.h>
#include <FairRunSim.h>

TString geoDetectorName = "test";

void geometry(FairRunSim *fRun);

void test() {
	FairRunSim* fRun = new FairRunSim(); // create the FairRun Class

  // Choose the Geant Navigation System
  fRun->SetName("TGeant4"); // TGeant3
	
	fRun->SetMaterials("media.geo");

}

void geometry(FairRunSim *fRun)
{
    // Set Material Definition file
    fRun->SetMaterials("media.geo");
  
    // Add Passive Modules
    FairModule *Cave= new FairCave("CAVE");
    Cave->SetGeometryFileName("cave.geo");
    fRun->AddModule(Cave);

    //~ FairModule *Pipe= new FairPipe("PIPE");
    //~ Pipe->SetGeometryFileName("pipe.geo");
    //~ fRun->AddModule(Pipe);
  
    //~ FairModule *Magnet= new FairMagnet("MAGNET");
    //~ Magnet->SetGeometryFileName("magnet.root");
    //~ fRun->AddModule(Magnet);

    //~ // Add Detectors
    //~ FairDetector *Tpc = new TpcDetector("TPC");
    //~ Tpc->SetGeometryFileName("tpc.root");
    //~ fRun->AddModule(Tpc);
}
