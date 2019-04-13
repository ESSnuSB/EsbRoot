#include "EsbSimulators/EsbSuperFGD/EsbFileWriter.h"

#include "EsbSimulators/EsbSuperFGD/EsbFgdRunManager.h"
#include "EsbSimulators/EsbSuperFGD/EsbDetectorConstruction.h"

#include <math.h>

using namespace std;

namespace esbroot {
namespace simulators {
namespace superfgd {

FileWriter::FileWriter(const char* filename, int compressionLevel):fverbose(false)
{
    fcubeHits = make_shared<data::superfgd::detector::G4EventRecord>();

    // open output file
    ffile = make_shared<TFile>(filename, "RECREATE", "NF Geant4 Simulation Hits");
    ffile->SetCompressionLevel(compressionLevel);

    fhitsTree = make_shared<TTree>("hits", "G4 event hits");
    fhitsTree->Branch(data::superfgd::detector::DP::NF_CUBE_HIT_BRANCH.c_str(), &fcubeHits);
}

FileWriter::~FileWriter() 
{
    ffile->WriteTObject(fhitsTree.get());
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
void FileWriter::AddGHepParticle(genie::GHepParticle* p)
{
    if(p!=nullptr)
    {
        double px = p->Px();
        double py = p->Py();
        double pz = p->Pz();
        double tsquare = px*px + py*py + pz*pz;
        double total = std::sqrt(tsquare);

        double angle = std::acos(pz/total)* (180 / PI);

        fcubeHits->AddCos(p->Pdg(), angle); // Add to map
        fcubeHits->AddCos(angle); // Add to know all angles

        fcubeHits->AddMomentum(p->Pdg(), total); // Add to map
        fcubeHits->AddMomentum(total); // Add to all particle momentums
    }
}

void FileWriter::AddFiberHit(data::superfgd::detector::FiberHit& hit)
{
    // Fibers are not required, info is only gathered from the
    // cubes
    if(fverbose)
    {
        cout<<" =============  FileWriter ============" << endl;
        cout<<" Adding hit " << endl;
        cout<<" FiberCopyNo" << hit.GetfiberCopyNo() << endl;
        cout<<" Momentum:X " << hit.GetHitMomentum().x() << endl;
        cout<<" Momentum:Y " << hit.GetHitMomentum().y() << endl;
        cout<<" Momentum:Z " << hit.GetHitMomentum().z() << endl;
        cout<<" Pdg " << hit.GetPdg() << endl;
        cout<<" ========================================" << endl;
    }
}

void FileWriter::AddCubeHit(data::superfgd::detector::CubeHit& hit)
{
    if(fverbose)
    {
        cout<<" =============  FileWriter ============" << endl;
        cout<<" Adding hit " << endl;
        cout<<" CubeCopyNo " << hit.GetCubeCopyNo() << endl;
        cout<<" Momentum:X " << hit.GetHitMomentum().x() << endl;
        cout<<" Momentum:Y " << hit.GetHitMomentum().y() << endl;
        cout<<" Momentum:Z " << hit.GetHitMomentum().z() << endl;
        cout<<" Pdg " << hit.GetPdg() << endl;
        cout<<" ========================================" << endl;
    }

    fcubeHits->AddCubeHit(hit);
}

void FileWriter::SumStep(data::superfgd::detector::CubeHit& hit, data::superfgd::detector::FgdDetectorParameters& dp)
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
    static double step_X  = dp.ParamAsDouble(data::superfgd::detector::DP::length_X) * dp.GetLenghtUnit();
    static double step_Y  = dp.ParamAsDouble(data::superfgd::detector::DP::length_Y) * dp.GetLenghtUnit();
    static double step_Z  = dp.ParamAsDouble(data::superfgd::detector::DP::length_Z) * dp.GetLenghtUnit();

    static int bins_x = dp.ParamAsInt(data::superfgd::detector::DP::number_cubes_X);
    static int bins_y = dp.ParamAsInt(data::superfgd::detector::DP::number_cubes_Y);
    static int bins_z = dp.ParamAsInt(data::superfgd::detector::DP::number_cubes_Z);

    static double total_X = step_X * bins_x;
    static double total_Y = step_Y * bins_y;
    static double total_Z = step_Z * bins_z;

    int bin_x = (hit.GetHitPosition().X() + total_X/2)/step_X;
    int bin_y = (hit.GetHitPosition().Y() + total_Y/2)/step_Y;
    int bin_z = (hit.GetHitPosition().Z() + total_Z/2)/step_Z;

    data::superfgd::detector::G4StepsRecord step;
    step.AddPdg(hit.GetPdg());
    step.SetBinPosition(bin_x,bin_y,bin_z);

    std::vector<data::superfgd::detector::G4StepsRecord>::iterator iter = std::find(fsteps.begin(), fsteps.end(),step);
    if(iter==fsteps.end())
    {
        step.AccStartTime(hit.GetTime());
        step.AccEndTime(hit.GetPostTime());
        step.AccEdep(hit.GetEdep());
        step.AccNonIon(hit.GetNonIonizingEnergyDeposit());
        step.AccTrackLenght(hit.GetTracklength());
        step.AddTrackId(hit.GetTrackId());
        step.AddParentId(hit.GetParentId());
        //step.addPdg(hit->GetPdg());
        fsteps.push_back(step);
    }
    else
    {
        data::superfgd::detector::G4StepsRecord& s = *iter;
        s.AccStartTime(hit.GetTime());
        s.AccEndTime(hit.GetPostTime());
        s.AccEdep(hit.GetEdep());
        s.AccNonIon(hit.GetNonIonizingEnergyDeposit());
        s.AccTrackLenght(hit.GetTracklength());
        s.AddTrackId(hit.GetTrackId());
        s.AddParentId(hit.GetParentId());
        s.AddPdg(hit.GetPdg());
    } 

    int trackId = hit.GetTrackId();
    bool isCharged = hit.GetCharge()!=0;
    if(isCharged)
    {
        std::map<int,std::vector<data::superfgd::detector::G4StepsRecord>>::iterator iterTrack = ftracks.find(trackId);
        if(iterTrack!=ftracks.end())
        {
            std::vector<data::superfgd::detector::G4StepsRecord>& vec = iterTrack->second;
            if(std::find(vec.begin(), vec.end(), step) == vec.end())
                vec.push_back(step);
        }
        else
        {
            std::vector<data::superfgd::detector::G4StepsRecord> cubesHitByParticle;
            cubesHitByParticle.push_back(step);
            ftracks[trackId]=cubesHitByParticle;
        }
    }
}

void FileWriter::AddVertexPos(double xpos, double ypos, double zpos)
{
    FgdRunManager* man = dynamic_cast<FgdRunManager*>(G4RunManager::GetRunManager());
    G4VUserDetectorConstruction* g4dc = const_cast<G4VUserDetectorConstruction*>(man->GetUserDetectorConstruction());
    FgdDetectorConstruction* detector = dynamic_cast<FgdDetectorConstruction*>(g4dc);

    data::superfgd::detector::FgdDetectorParameters& dp = detector->GetDetectorParams();

    static double step_X  = dp.ParamAsDouble(data::superfgd::detector::DP::length_X) * dp.GetLenghtUnit();
    static double step_Y  = dp.ParamAsDouble(data::superfgd::detector::DP::length_Y) * dp.GetLenghtUnit();
    static double step_Z  = dp.ParamAsDouble(data::superfgd::detector::DP::length_Z) * dp.GetLenghtUnit();

    static int bins_x = dp.ParamAsInt(data::superfgd::detector::DP::number_cubes_X);
    static int bins_y = dp.ParamAsInt(data::superfgd::detector::DP::number_cubes_Y);
    static int bins_z = dp.ParamAsInt(data::superfgd::detector::DP::number_cubes_Z);

    static double total_X = step_X * bins_x;
    static double total_Y = step_Y * bins_y;
    static double total_Z = step_Z * bins_z;

    int bin_x = (xpos + total_X/2)/step_X;
    int bin_y = (ypos + total_Y/2)/step_Y;
    int bin_z = (zpos + total_Z/2)/step_Z;

    fvertex.SetBinPosition(bin_x,bin_y,bin_z);
}

void FileWriter::WriteHit()
{

    std::vector<data::superfgd::detector::G4StepsRecord>::iterator iter = fsteps.begin();
    while(iter!=fsteps.end())
    {
        fcubeHits->AddSumStep(*iter);
        iter++;
    }

    fcubeHits->ExtractTrackCos(ftracks, fvertex);

    fhitsTree->Fill();
    fcubeHits->Reset();
    fsteps.clear();
    ftracks.clear();
}

}   //superfgd
}   //simulators
}   //esbroot