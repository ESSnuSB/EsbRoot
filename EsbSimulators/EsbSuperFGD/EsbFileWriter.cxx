#include "EsbSimulators/EsbSuperFGD/EsbFileWriter.h"

#include "EsbSimulators/EsbSuperFGD/EsbFgdRunManager.h"
#include "EsbSimulators/EsbSuperFGD/EsbDetectorConstruction.h"

#include <math.h>

namespace esbroot {
namespace simulators {
namespace superfgd {

FileWriter::FileWriter(const char* filename, int compressionLevel):fverbose(false)
{
    fcubeHits = make_shared<G4EventRecord>();

    // open output file
    ffile = make_shared<TFile>(filename, "RECREATE", "NF Geant4 Simulation Hits");
    ffile->SetCompressionLevel(compressionLevel);

    fhitsTree = make_shared<TTree>("hits", "G4 event hits");
    fhitsTree->Branch(data::superfgd::detector::DP::NF_CUBE_HIT_BRANCH.c_str(), &fcubeHits);
}

FileWriter::~FileWriter() 
{
    ffile->WriteTObject(fhitsTree);
    ffile->Close();
    std::cout << "Root file written.\n";
}

void FileWriter::AddEvent(genie::EventRecord* event)
{
    // if event is needed for later analysis 
    // add event adding and handling here
}

// Add the angle of the particle from the initial vertex
// this will be used to calculate the efficiency of detected
//  particles based on initial angle.
void FileWriter::AddGHepParticle(GHepParticle* p)
{
    if(p!=nullptr)
    {
        double px = p->Px();
        double py = p->Py();
        double pz = p->Pz();
        double tsquare = px*px + py*py + pz*pz;
        double total = std::sqrt(tsquare);

        double angle = std::acos(pz/total)* (180 / PI);

        fcubeHits->addCos(p->Pdg(), angle); // Add to map
        fcubeHits->addCos(angle); // Add to know all angles

        fcubeHits->addMomentum(p->Pdg(), total); // Add to map
        fcubeHits->addMomentum(total); // Add to all particle momentums
    }
}

void FileWriter::AddFiberHit(NFFiberHit* hit)
{
    // Fibers are not required, info is only gathered from the
    // cubes
    if(fverbose)
    {
        cout<<" =============  NFFileWriter ============" << endl;
        cout<<" Adding hit " << endl;
        cout<<" FiberCopyNo" << hit->getfiberCopyNo() << endl;
        cout<<" Momentum:X " << hit->getHitMomentum().x() << endl;
        cout<<" Momentum:Y " << hit->getHitMomentum().y() << endl;
        cout<<" Momentum:Z " << hit->getHitMomentum().z() << endl;
        cout<<" Pdg " << hit->getPdg() << endl;
        cout<<" ========================================" << endl;
    }
}

void FileWriter::AddCubeHit(CubeHit* hit)
{
    if(fverbose)
    {
        cout<<" =============  NFFileWriter ============" << endl;
        cout<<" Adding hit " << endl;
        cout<<" CubeCopyNo " << hit->getCubeCopyNo() << endl;
        cout<<" Momentum:X " << hit->getHitMomentum().x() << endl;
        cout<<" Momentum:Y " << hit->getHitMomentum().y() << endl;
        cout<<" Momentum:Z " << hit->getHitMomentum().z() << endl;
        cout<<" Pdg " << hit->getPdg() << endl;
        cout<<" ========================================" << endl;
    }

    fcubeHits->addCubeHit(*hit);
}

void FileWriter::SumStep(CubeHit* hit, data::superfgd::detector::DetectorParameters& dp)
{
    if(fverbose)
    {
        cout<<" =============  FileWriter ============" << endl;
        cout<<" Summing step " << endl;
        cout<<" ========================================" << endl;
    }

    //================================================================
    //    Retrive the cube position from the detector parameters
    //================================================================
    static double step_X  = dp.ParamAsDouble(data::superfgd::detector::DP::length_X) * dp.getLenghtUnit();
    static double step_Y  = dp.ParamAsDouble(data::superfgd::detector::DP::length_Y) * dp.getLenghtUnit();
    static double step_Z  = dp.ParamAsDouble(data::superfgd::detector::DP::length_Z) * dp.getLenghtUnit();

    static int bins_x = dp.ParamAsInt(data::superfgd::detector::DP::number_cubes_X);
    static int bins_y = dp.ParamAsInt(data::superfgd::detector::DP::number_cubes_Y);
    static int bins_z = dp.ParamAsInt(data::superfgd::detector::DP::number_cubes_Z);

    static double total_X = step_X * bins_x;
    static double total_Y = step_Y * bins_y;
    static double total_Z = step_Z * bins_z;

    int bin_x = (hit->getHitPosition().X() + total_X/2)/step_X;
    int bin_y = (hit->getHitPosition().Y() + total_Y/2)/step_Y;
    int bin_z = (hit->getHitPosition().Z() + total_Z/2)/step_Z;

    G4StepsRecord step;
    step.addPdg(hit->getPdg());
    step.setBinPosition(bin_x,bin_y,bin_z);

    std::vector<G4StepsRecord>::iterator iter = std::find(fsteps.begin(), fsteps.end(),step);
    if(iter==fsteps.end())
    {
        step.accStartTime(hit->getTime());
        step.accEndTime(hit->getPostTime());
        step.accEdep(hit->getEdep());
        step.accNonIon(hit->getNonIonizingEnergyDeposit());
        step.accTrackLenght(hit->getTracklength());
        step.addTrackId(hit->getTrackId());
        step.addParentId(hit->getParentId());
        //step.addPdg(hit->getPdg());
        fsteps.push_back(step);
    }
    else
    {
        G4StepsRecord& s = *iter;
        s.accStartTime(hit->getTime());
        s.accEndTime(hit->getPostTime());
        s.accEdep(hit->getEdep());
        s.accNonIon(hit->getNonIonizingEnergyDeposit());
        s.accTrackLenght(hit->getTracklength());
        s.addTrackId(hit->getTrackId());
        s.addParentId(hit->getParentId());
        s.addPdg(hit->getPdg());
    } 

    int trackId = hit->getTrackId();
    bool isCharged = hit->getCharge()!=0;
    if(isCharged)
    {
        std::map<int,std::vector<G4StepsRecord>>::iterator iterTrack = m_tracks.find(trackId);
        if(iterTrack!=m_tracks.end())
        {
            std::vector<G4StepsRecord>& vec = iterTrack->second;
            if(std::find(vec.begin(), vec.end(), step) == vec.end())
                vec.push_back(step);
        }
        else
        {
            std::vector<G4StepsRecord> cubesHitByParticle;
            cubesHitByParticle.push_back(step);
            m_tracks[trackId]=cubesHitByParticle;
        }
    }
}

void FileWriter::AddVertexPos(double xpos, double ypos, double zpos)
{
    FgdRunManager* man = dynamic_cast<FgdRunManager*>(G4RunManager::GetRunManager());
    G4VUserDetectorConstruction* g4dc = const_cast<G4VUserDetectorConstruction*>(man->GetUserDetectorConstruction());
    FgdDetectorConstruction* detector = dynamic_cast<FgdDetectorConstruction*>(g4dc);

    FgdDetectorParameters& dp = detector->GetDetectorParams();

    static double step_X  = dp.ParamAsDouble(data::superfgd::detector::DP::length_X) * dp.getLenghtUnit();
    static double step_Y  = dp.ParamAsDouble(data::superfgd::detector::DP::length_Y) * dp.getLenghtUnit();
    static double step_Z  = dp.ParamAsDouble(data::superfgd::detector::DP::length_Z) * dp.getLenghtUnit();

    static int bins_x = dp.ParamAsInt(data::superfgd::detector::DP::number_cubes_X);
    static int bins_y = dp.ParamAsInt(data::superfgd::detector::DP::number_cubes_Y);
    static int bins_z = dp.ParamAsInt(data::superfgd::detector::DP::number_cubes_Z);

    static double total_X = step_X * bins_x;
    static double total_Y = step_Y * bins_y;
    static double total_Z = step_Z * bins_z;

    int bin_x = (xpos + total_X/2)/step_X;
    int bin_y = (ypos + total_Y/2)/step_Y;
    int bin_z = (zpos + total_Z/2)/step_Z;

    fvertex.setBinPosition(bin_x,bin_y,bin_z);
}

void FileWriter::WriteHit()
{

    std::vector<G4StepsRecord>::iterator iter = fsteps.begin();
    while(iter!=fteps.end())
    {
        fcubeHits->addSumStep(*iter);
        iter++;
    }

    fcubeHits->extractTrackCos(ftracks, fvertex);

    fhitsTree->Fill();
    fcubeHits->reset();
    fsteps.clear();
    ftracks.clear();
}

}   //superfgd
}   //simulators
}   //esbroot