int GenieNtpFluxV1Test(void)
{
	using namespace esbroot;
  
	//~ int nEvents = 100;
	int nEvents = 10;
	const char* out_file_name = "GenieNtpFluxV1Test.root";

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
	generators::GenieGenerator::GlobalState.fXsecSplineFileName =
	"/home/bklicek/sync/ESSnuSB/soft/genie_xsec/genie_xsec/v3_00_06/NULL/G1810a00000-k250-e1000/data/gxspl-FNALsmall.xml"; 

	//Define TS coordinate system, here it is at a centre of global CS
	esbroot::geometry::CoordinateSystem tscs(
		ROOT::Math::Rotation3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0),
		ROOT::Math::XYZVector(0,0,0)
	);

	//Define ND coordinate system
	//No rotation, just translation 250m from the proton target
	esbroot::geometry::CoordinateSystem ndcs(
		ROOT::Math::Rotation3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0),
		ROOT::Math::XYZVector(0,0,25000)
	);

	//Create GenieNtpFlux object
	//Parameters: name of the file with the flux, name of the tree within the file, neutrino PDG,
	//						TS coordinate system, ND coordinate system
	auto test_flux = new esbroot::generators::GenieNtpFluxV1("nuData_4x10e6_plus.root", "numuVtx", 14, tscs, ndcs);

	//Point geometry just for testing, can be replaced by real geometry
	auto geomI = new genie::geometry::PointGeomAnalyzer(1000080160);

	//Create an instance of the generator and configure it
	auto partGen = new generators::GenieGenerator(test_flux, geomI);
	partGen->Configure();
 
	//Set it as a primary generator
	primGen->AddGenerator(partGen);


	fRun->SetOutputFile(out_file_name); // set output file

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
	
	//~ while( test_flux.GenerateNext() ) {
		//~ test_flux.Momentum().Print();
		//~ test_flux.Position().Print();
		//~ cout << endl;
	//~ }

	//~ test_flux.Clear();

	return(0);
}

 
