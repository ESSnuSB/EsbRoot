#include "EsbData/EsbSuperFGD/EsbDetector/EsbG4EventRecord.h"
#include "Framework/ParticleData/PDGCodes.h"
#include "Framework/ParticleData/PDGCodeList.h"

ClassImp(esbroot::data::superfgd::detector::G4EventRecord)

namespace esbroot {

namespace data {

namespace superfgd {

namespace detector {

ostream & operator << (ostream & stream, const G4EventRecord & rec) 
{
    rec.PrintToStream(stream);
    return stream;
}

G4EventRecord::G4EventRecord()
:TObject(),fisMuon(false), fisProton(false), fmuonMomentum(0),fprotonMomentum(0)
    ,fmuonAngle(0),fprotonAngle(0)
{
    fcubeHits = new TClonesArray("CubeHit");
    fcubeHits->SetOwner(true);

    fsumSteps = new TClonesArray("G4StepsRecord");
    fsumSteps->SetOwner(true);

    passedTracks=0;

    Init();
}

G4EventRecord::G4EventRecord(int sizeCubeHits)
:TObject(),fisMuon(false),fisProton(false), fmuonMomentum(0),fprotonMomentum(0)
    ,fmuonAngle(0),fprotonAngle(0)
{
    fcubeHits = new TClonesArray("CubeHit", sizeCubeHits);
    fcubeHits->SetOwner(true);

    fsumSteps = new TClonesArray("G4StepsRecord");
    fsumSteps->SetOwner(true);

    Init();
}

G4EventRecord::G4EventRecord(const G4EventRecord& record)
:TObject(),fisMuon(false),fisProton(false), fmuonMomentum(0),fprotonMomentum(0)
    ,fmuonAngle(0),fprotonAngle(0)
{
    fcubeHits = new TClonesArray("CubeHit", record.fcubeHits->GetEntries());
    fcubeHits->SetOwner(true);

    fsumSteps = new TClonesArray("G4StepsRecord");
    fsumSteps->SetOwner(true);

    record.Copy(*this);
}

G4EventRecord::~G4EventRecord() 
{
    Clear("C");
    delete fcubeHits;
    delete fsumSteps;
}

CubeHit* G4EventRecord::GetCubeHit(Int_t index) const 
{
    if ((index >= 0) && (index < fcubeHits->GetEntriesFast())) 
    {
        CubeHit* hit = (CubeHit*)(*fcubeHits)[index];
        if (hit && hit!=nullptr && hit!=NULL)
        {
            return hit;
        }
    }
    
    return NULL;
}

G4StepsRecord* G4EventRecord::GetSumStep(Int_t index) const 
{
    if ((index >= 0) && (index < fsumSteps->GetEntriesFast())) 
    {
        G4StepsRecord* step = (G4StepsRecord*)(*fsumSteps)[index];
        if (step && step!=nullptr && step!=NULL)
        {
            return step;
        }
    }
    
    return NULL;
}

unsigned int G4EventRecord::NumCubeHits() const 
{
    return fcubeHits->GetEntries();
}

void G4EventRecord::AddCubeHit(const CubeHit& hit) 
{
    unsigned int pos = fcubeHits->GetEntries();
    new((*fcubeHits)[pos]) CubeHit(hit);
}

unsigned int G4EventRecord::NumSumSteps() const 
{
    return fsumSteps->GetEntries();
}

void G4EventRecord::AddSumStep(const G4StepsRecord& step) 
{
    unsigned int pos = fsumSteps->GetEntries();
    new((*fsumSteps)[pos]) G4StepsRecord(step);
}

void G4EventRecord::Copy(TObject& object) const 
{
    G4EventRecord* record = dynamic_cast<G4EventRecord*>(&object);
    record->Reset();

    // copy cubeHits
    TClonesArray& srcCubeHits = *(this->fcubeHits);;
    TClonesArray& destCubeHits = *(record->fcubeHits);

    int nc = fcubeHits->GetEntries();
    for (int i = 0; i < nc; i++) 
    {
        new(destCubeHits[i]) CubeHit(*((CubeHit*)srcCubeHits[i]));
    }

    // copy particle angles
    record->fparticlePdgCos.clear();
    record->fparticlePdgCos = this->fparticlePdgCos;

    record->fparticlesCos.clear();
    record->fparticlesCos = this->fparticlesCos;

    record->fparticlePdgMom.clear();
    record->fparticlePdgMom = this->fparticlePdgMom;

    record->fparticlesMomentum.clear();
    record->fparticlesMomentum = this->fparticlesMomentum;

    record->ftrackCos.clear();
    record->ftrackCos = this->ftrackCos;

    record->ftrackMom.clear();
    record->ftrackMom = this->ftrackMom;

    record->passedTracks = this->passedTracks;
}

void G4EventRecord::Clear(Option_t* /*option*/) 
{
    // Call Clear/Delete methods of TClonesArray/TRefArray/TBits/... fields
    fcubeHits->Clear("C");
    fsumSteps->Clear("C");
    fparticlesCos.clear();
    fparticlePdgCos.clear();
    fparticlePdgMom.clear();
    fparticlesMomentum.clear();
    ftrackCos.clear();
    ftrackMom.clear();
    passedTracks=0;
    fisMuon = false;
    fisProton = false;
    fmuonMomentum = 0;
    fprotonMomentum = 0;
    fmuonAngle = 0;
    fprotonAngle = 0;
}

void G4EventRecord::Print(Option_t* option) const 
{
    PrintToStream(cout);
}

void G4EventRecord::Init() 
{
}

void G4EventRecord::Reset() 
{
    Clear();
    Init();
}

void G4EventRecord::PrintToStream(ostream& stream) const 
{
    stream << "========================================" << endl;
    stream << "          G4EventRecord                 " << endl;
    stream << "========================================" << endl;
    int nc = fcubeHits->GetEntries();
    stream << "Number of cube hits = " << nc << "\n";
    for (int i = 0; i < nc; i++) {
        TObject* item = (*fcubeHits)[i];
        ((CubeHit*)item)->PrintToStream(stream);
    }
    nc = fsumSteps->GetEntries();
    stream << "Number of steps = " << nc << "\n";
    for (int i = 0; i < nc; i++) {
        TObject* item = (*fsumSteps)[i];
        ((G4StepsRecord*)item)->PrintToStream(stream);
    }
}

void G4EventRecord::AddCos(long pdg,double cosAngle)
{
    fparticlePdgCos.emplace(pdg,cosAngle);

    // Record only the first muon - this will probably be the primary
    // muon from the initial interaction
    if(fmuonAngle == 0 && genie::kPdgMuon==pdg)
    {
        fisMuon = true;
        fmuonAngle = cosAngle;
    }

    // Record only the first proton - this will probably be the primary
    // proton from the initial interaction
    if(fprotonAngle == 0 && genie::kPdgProton==pdg)
    {
        fisProton = true;
        fprotonAngle = cosAngle;
    }
}

void G4EventRecord::AddMomentum(long pdg, double p)
{ 
    fparticlePdgMom.emplace(pdg,p);

    // Record only the first muon - this will probably be the primary
    // muon from the initial interaction
    if(fmuonMomentum==0 && genie::kPdgMuon==pdg)
    {
        fmuonMomentum = p;
    }

    // Record only the first proton - this will probably be the primary
    // proton from the initial interaction
    if(fprotonMomentum==0 && genie::kPdgProton==pdg)
    {
        fprotonMomentum = p;
    }
}

/*
    The cos is extracted with two criteria:
    1) the track has to be at least 3 bins long (to pass atleast 3 cubes)
    2) the track has to be at some point at least 1 cube appart from other tracks
    The cos angle is calculated from the initial 3 cubes (from the primary vertex)
    by arc cos = (length_z/total_length)
    -> length_z is selected since z is the particle direction chosen for simulation
*/
void G4EventRecord::ExtractTrackCos(std::map<int,std::vector<G4StepsRecord>>& tracks, G4StepsRecord& vertex)
{
    std::map<int,std::vector<G4StepsRecord>>::iterator iterTrack = tracks.begin();
    while(iterTrack!=tracks.end())
    {
        int trackId = iterTrack->first;

        std::vector<G4StepsRecord> bins = iterTrack->second;
        // Process only tracks which emanate(start) from the primary vertex
        bool isFromVertex = std::find(bins.begin(), bins.end(),vertex) != bins.end();

        if(isFromVertex
             && bins.size()>=MIN_TRACK_LENGHT
             && ValidateSeperation(trackId,bins,tracks)) // Apply criteria
        {
            passedTracks++;
        }
        iterTrack++;
    }
}


bool G4EventRecord::ValidateSeperation(int& binTrackid,
    vector<G4StepsRecord>& trackBins,
    std::map<int,std::vector<G4StepsRecord>>& tracks)
{
    bool rc(false);
    std::map<int,std::vector<G4StepsRecord>>::iterator iterTrack = tracks.begin();
    int numberOfTracks=1;
    while(iterTrack!=tracks.end())
    {
        int trackId = iterTrack->first;
        if(trackId!=binTrackid) // compare with other tracks but not with itself
        {
            std::vector<G4StepsRecord> bins = iterTrack->second;
            if(bins.size()>=MIN_TRACK_LENGHT) // Apply first criteria -> to be more than 3 bins
            {
                // to have a correct comparison 
                // both tracks should start the comparison from the initial
                // bins of the primary vertex
                std::vector<G4StepsRecord>::iterator binIter = bins.begin();
                while(binIter!=bins.end())
                {
                    int ind=0;
                    while(ind < trackBins.size())
                    {
                        G4StepsRecord& compBin = *binIter;
                        G4StepsRecord& trackBin = trackBins[ind];

                        int distX = std::abs(compBin.GetBinX() - trackBin.GetBinX());
                        int distY = std::abs(compBin.GetBinY() - trackBin.GetBinY());
                        int distZ = std::abs(compBin.GetBinZ() - trackBin.GetBinZ());

                        // Bins should have at least one coordinate match
                        // this ensures that they will be compared e.g.
                        //    '*' <- compare this bin
                        //   *
                        //  **'*' <- with this bin
                        // in the example above the bins '*' have the same coordinate
                        // in the vertical plane and they are seperated by at least one
                        // * in the vertical direction

                        // Compare in X direction
                        if( (compBin.GetBinX() == trackBin.GetBinX())
                            && (distY>= MIN_SEPARATION_TRACK_LENGHT || distZ>= MIN_SEPARATION_TRACK_LENGHT)
                            )
                            {
                                rc=true;
                                break; 
                            }

                        // Compare in Y direction
                        if( (compBin.GetBinY() == trackBin.GetBinY())
                            && (distX>= MIN_SEPARATION_TRACK_LENGHT || distZ>= MIN_SEPARATION_TRACK_LENGHT)
                            )
                            {
                                rc=true;
                                break; 
                            }

                        // Compare in Z direction
                        if( (compBin.GetBinZ() == trackBin.GetBinZ())
                            && (distX>= MIN_SEPARATION_TRACK_LENGHT || distY>= MIN_SEPARATION_TRACK_LENGHT)
                            )
                            {
                                rc=true;
                                break; 
                            }   

                        ind++;
                    }
                    if(rc) 
                        break;

                    binIter++;
                }
            }

            numberOfTracks++;
        }

        if(rc)
            break;

        iterTrack++;
    }

    if(!rc && tracks.size()==1) // return true if only one track exists
        rc=true;

    return rc;
}

}   // detector
}   // superfgd
}   // data
}   // esbroot