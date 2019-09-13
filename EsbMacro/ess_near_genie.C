/*

  Macro that shows how to run a WC simulation.

*/

void ess_near_genie(TString outFileName = "ess_near_genie.root",
             Int_t nStartEvent = 0, 
	     Int_t nEvents = 10)
{
  using namespace esbroot;
  
  FairRunSim* fRun = new FairRunSim(); // create the FairRun Class
  // Peter: SetStoreTraj seems to be needed for the official Eve
  // visualization. It creates the branch GeoTracks in the output tree.
  fRun->SetStoreTraj();
  
  // Choose the Geant Navigation System
  fRun->SetName("TGeant4"); // TGeant3/4
  
  // Set Material Definition file
  fRun->SetMaterials("media.geo");
  
  // Add Passive Modules
  FairModule *cave= new geometry::Cave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  fRun->AddModule(cave);
  
  // Add Detectors
  FairDetector *nearWc = new geometry::WCDetector("NearWcDetector", 300, 500, kTRUE);
  fRun->AddModule(nearWc);
  
  FairDetector* fgd = new geometry::FgdDetector(
		"Granular Detector",
		"../EsbGeometry/EsbSuperFGD/EsbConfig/geometry",
		0,0,-550, kTRUE
	);
  fRun->AddModule(fgd);

  double Bx(0), By(0), Bz(0);
  ((geometry::FgdDetector*)fgd)->GetMagneticField(Bx, By, Bz);
  double xMin(0), xMax(0), yMin(0), yMax(0), zMin(0), zMax(0);
  ((geometry::FgdDetector*)fgd)->GetMagneticFieldRegion(xMin, xMax, yMin, yMax, zMin, zMax);
  FairConstField* fgdField = new FairConstField();
  fgdField->SetField(Bx, By, Bz);
  fgdField->SetFieldRegion(xMin, xMax, yMin, yMax, zMin, zMax);
  fRun->SetField(fgdField);
  
  
  // Create and Set Event Generator
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  fRun->SetGenerator(primGen);
  
	// *** Set global genie parameters ***
	//Genie tune, this is the recommended one
  generators::GenieGenerator::GlobalState.fGenieTune = "G18_10a_00_000";
  //File with cross-section splines (see: http://scisoft.fnal.gov/scisoft/packages/genie_xsec/)
  generators::GenieGenerator::GlobalState.fXsecSplineFileName = "../../xsec/xsec_essnusb.xml"; 
 
	//Create simple genie generator
	auto partGen = new generators::SimpleGenieGenerator(
		1000080160, //Target PDG (O16)
		14, //Neutrino PDG (nu_mu)
		0.6, //Neutrino energy (GeV)
		TVector3(1.0,0.0,0.0), //Neutrino directon (normalization of this vector is not important)
		TLorentzVector(0.0, 0.0, -550, 0.0) //4-position of the neutrino vertex (x, y, z, t) (cm, s)
  );

  //Add to list of generators
  primGen->AddGenerator(partGen);
  
  fRun->SetOutputFile(outFileName.Data()); // set output file
  
  fRun->Init();
  
	fRun->GetSink()->WriteObject(gGeoManager, "geometry");
  
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
  
  
  //~ fRun->CreateGeometryFile("geo_full.root");  // for additional full geometry file
}
