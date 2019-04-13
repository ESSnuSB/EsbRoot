#include "EsbSimulators/EsbSuperFGDSimulator.h"

#include <sstream>

namespace esbroot {
namespace simulators {

SuperFGDSimulator::SuperFGDSimulator(std::string detectorGeomFile, std::string outputFile,
    std::string genieFile, std::string geant4GunFile, int compLvl)
    : fDetectorGeomFile(detectorGeomFile), fOutputFile(outputFile),
        fGenieFile(genieFile), fGunFile(geant4GunFile), fCompressionLevel(compLvl),
        fNumberOfEvents(0)
{

}

SuperFGDSimulator::~SuperFGDSimulator()
{

}

SuperFGDSimulator::Init()
{
    // ValidateSet parameters
    Validate();

    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

    //Construct the default run manager
    fRunManager = std::make_shared<superfgd::FgdRunManager>();

    // 1. Set the file writer - writes results to the output file
    std::shared_ptr<superfgd::FileWriter> file_writer 
                = std::make_shared<superfgd::FileWriter>(fOutputFile.c_str(), fCompressionLevel);
    fRunManager->setFileWriter(file_writer);

    // 2. Initialize detector construction
    superfgd::SuperFGDDetectorConstruction* detector 
                = new superfgd::SuperFGDDetectorConstruction(fDetectorGeomFile);
    detector->Construct();
    fRunManager->SetUserInitialization(detector);

    // 3. Initialize the physics list
    superfgd::PhysicsList* physicsList = new superfgd::PhysicsList;
    fRunManager->SetUserInitialization(physicsList);

    // 4. Initialize the events reader 
    //  a) file with events generated from genie - fGenieFile
    //  b) use the Geant4 gunGenerator - fGunFile
    //  if 'fGenieFile' is not empty uses genie events, else checks if 'fGunFile' is not empty
    if(!fGenieFile.empty())
    {
        superfgd::PrimaryGeneratorAction* primaryAction =  new superfgd::PrimaryGeneratorAction(fGenieFile, physicsList);
        fRunManager->SetUserAction(primaryAction);
        fNumberOfEvents = primaryAction->getNumberOfEvents();
    }
    else if(!fGunFile.empty())
    {
        superfgd::ParticleGunPrimaryGenerator* primaryAction 
            = new superfgd::ParticleGunPrimaryGenerator(fDetectorGeomFile, fGunFile);
        primaryAction->setKinematics();
        fRunManager->SetUserAction(primaryAction);
        fNumberOfEvents = primaryAction->getNumberOfEvents();
    }

    // 5. set Stepping action
    fRunManager->SetUserAction(new superfgd::FgdStepAction());  

    // 6. Set Event action 
    fRunManager->SetUserAction(new superfgd::FgdEventAction());

    // 7. Set Run action
    fRunManager->SetUserAction(new superfgd::FgdRunAction());

    // 8. Initialize Geant4 kernel
    fRunManager->Initialize();
}

void SuperFGDSimulator::Validate()
{
    std::stringstream ss;

    // The detector geometry file must not be empty and must exist
    if(fDetectorGeomFile.empty())
    {
        THROW("Detector file option is not set.");
    }else if(!FileExists(fDetectorGeomFile))
    {
        ss << fDetectorGeomFile << ": file does not exist!" << endl;
        THROW(ss.str());
    }

    // We need at least one file from which to read events to simulate
    if(fGenieFile.empty() && fGunFile.empty())
    {
        THROW("No events file to read events from - both GenieFile and Geant4Gun files are empty");
    }

    // If Genie file is set, it must exists
    if(!fGenieFile.empty() && !FileExists(fGenieFile))
    {
        ss << fGenieFile << ": file does not exist!" << endl;
        THROW(ss.str());
    }

    // If Geant4File file is set, it must exists
    if(!fGunFile.empty() && !FileExists(fGunFile))
    {
        ss << fGunFile << ": file does not exist!" << endl;
        THROW(ss.str());
    }
}

bool EsbSuperFGDSimulator::FileExists(std::string file)
{
    std::ifstream fs(file.c_str());
    return fs.good();
}

}   //simulators
}   //esbroot