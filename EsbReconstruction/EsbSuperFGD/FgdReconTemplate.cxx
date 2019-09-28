#include "EsbReconstruction/EsbSuperFGD/FgdReconTemplate.h"

#include "FairLogger.h"

#include <fstream>
#include <iostream>
#include <limits>
#include <map>

namespace esbroot {
namespace reconstruction {
namespace superfgd {

using namespace std;

#define PERMUTATIONS 23

FgdReconTemplate::FgdReconTemplate() 
    : freconFile("")
{
}

FgdReconTemplate::FgdReconTemplate(const char* templateConfigFile)
    : freconFile(templateConfigFile)
{
}

FgdReconTemplate::~FgdReconTemplate()
{
}

Bool_t FgdReconTemplate::IsLeaf(ReconHit* hit)
{
    Bool_t isHitLeaf(false);

    if(hit->fAllHits.size()==1)
    {
        LOG(debug)<< "Single leaf " << hit->fLocalId;
        isHitLeaf = true;
    }
    else
    {
        Int_t permutation(0);
        std::vector<TVector3> vecs;
        GetHitVectors(hit, vecs);
        for(size_t temp=0; !isHitLeaf && temp < fLeafVectors.size(); ++temp)
        {
            if(fLeafVectors[temp].hitVectors.size() == hit->fAllHits.size())
            {
                std::vector<TVector3>& tempVecs = fLeafVectors[temp].hitVectors;
                isHitLeaf = AreVectorsEqual(tempVecs, vecs, permutation);
            }
        }

        if(isHitLeaf)
        {
            LOG(debug) << "Leaf  " << hit->fLocalId << " with local hits " << hit->fAllHits.size() << endl;
        }
    }
    
    return isHitLeaf;
}



Bool_t FgdReconTemplate::GetNextHit(ReconHit* previous, ReconHit* current, ReconHit*& next)
{
    Bool_t nextFound(false);
    next = nullptr;

    if(current==nullptr)
    {
        throw "Invalid current hit";
    }

    if(current->fAllHits.empty())
    {
        return nextFound;
    }

    Int_t previousId = (previous==nullptr) ? -1 : previous->fLocalId;
    size_t nearestId(0);
    Double_t min_dist = std::numeric_limits<Int_t>::max();

    for(size_t nid = 0; nid< current->fAllHits.size(); ++nid)
    {
        ReconHit* candidateHit = current->fAllHits[nid];

        if(!candidateHit->fIsVisited
            && candidateHit->fLocalId != previousId
            && !candidateHit->fIsLeaf)
        {
            TVector3 vecPosition = current->fmppcLoc - candidateHit->fmppcLoc;
            Double_t dist = vecPosition.X()*vecPosition.X() + vecPosition.Y()*vecPosition.Y() + vecPosition.Z()*vecPosition.Z();

            if(dist < min_dist)
            {
                min_dist = dist;
                nearestId = nid;
            }
        }
    }

    next = current->fAllHits[nearestId];
    nextFound = (min_dist!=std::numeric_limits<Int_t>::max());

    return nextFound;
}

void FgdReconTemplate::LoadTemplates()
{
    TVector3 center;
    std::vector<TVector3> leaves;

    std::ifstream file(freconFile);
    std::string line;
    
    while(std::getline(file,line))
    {
        size_t leafInd = line.find(ReconTemplates::LEAF);
        if( leafInd==0                       // String starts with The searched pattern
            && leafInd!=std::string::npos)   // Search found a result
        {
            FgdReconTemplate::HitTemplate hitTemp;
            for(Int_t y = 1; y >= -1; --y)
            {
                Int_t x = 1;
                Int_t z = -1;
                if(std::getline(file,line))
                {
                    for(size_t cube=0; cube<line.size(); ++cube)
                    {
                        if(line[cube]==ReconTemplates::SEPARATOR)
                        {
                            continue;
                        }

                        switch(line[cube])
                        {
                            case 'C':   center.SetX(x); 
                                        center.SetY(y); 
                                        center.SetZ(z);
                                        break;
                            case 'X':   leaves.emplace_back(x,y,z);
                                        break;
                            default:
                                        break;
                        }

                        
                        --x;
                        if(x==-2)
                        {
                            ++z;
                            x = 1;
                        } 
                    }
                }
            }
            for(size_t leaf=0; leaf<leaves.size(); leaf++)
            {
                hitTemp.hitVectors.emplace_back(center - leaves[leaf]);
            }

            if(!leaves.empty())
            {
                fLeafVectors.push_back(hitTemp);
            }
            
            leaves.clear();
        } 
    }

    LOG(debug) << " Leaf templates found " << fLeafVectors.size();
}

void FgdReconTemplate::GetHitVectors(ReconHit* hit , std::vector<TVector3>& vecs)
{
    for(size_t i=0; i< hit->fAllHits.size(); ++i)
    {
        ReconHit& neightbourHit = *hit->fAllHits[i];
        TVector3 result = hit->fmppcLoc - neightbourHit.fmppcLoc;
        vecs.emplace_back(result);
    }
}

// Compares if the two vectors are equal
// This also includes rotational symmetry
// Make a copy of the template since it will be modified
Bool_t FgdReconTemplate::AreVectorsEqual(const std::vector<TVector3>& tempVecs, const std::vector<TVector3>& vecs,  Int_t& foundPermutation )
{
    Bool_t areEqual(false);

    if(tempVecs.size()!=vecs.size())
    {
        return areEqual;
    }

    std::vector<TVector3> tempVecPermut = tempVecs;

    Int_t permutation(0);
    Int_t limitPermutations = PERMUTATIONS;

    while(!areEqual && permutation<=limitPermutations)
    {
        for(size_t i=0; i<tempVecs.size(); ++i)
        {
            TVector3 tmp =  GetPermutation(tempVecs[i],permutation);
            tempVecPermut[i].SetX(tmp.X());
            tempVecPermut[i].SetY(tmp.Y());
            tempVecPermut[i].SetZ(tmp.Z());
        }

        Bool_t allVecsAreEqual(true);
        for(size_t i=0; allVecsAreEqual && i<vecs.size(); ++i)
        {
            allVecsAreEqual = std::find(tempVecPermut.begin(), tempVecPermut.end(), vecs[i])     !=  tempVecPermut.end();
        }

        areEqual = allVecsAreEqual;

        if(areEqual)
        {
            foundPermutation = permutation;
            break;
        }

        ++permutation;
    }

    return areEqual;
}

TVector3 FgdReconTemplate::GetPermutation(TVector3 vec, Int_t numPermutation)
{
    Double_t rot90deg = TMath::Pi()/2;
    Double_t rot180deg = TMath::Pi();

    if(numPermutation<0 || numPermutation>PERMUTATIONS)
    {
        throw "Invalid permutation";
    }

    switch(numPermutation)
    {
        case 0:
                // No rotation
                break;
        case 1:
                vec.RotateZ(rot90deg);
                break;
        case 2:
                vec.RotateZ(2*rot90deg);
                break;
        case 3:
                vec.RotateZ(3*rot90deg);
                break;
        case 4:
                vec.RotateY(rot90deg);
                break;
        case 5:
                vec.RotateY(rot90deg);
                vec.RotateX(rot90deg);
                break;
        case 6:
                vec.RotateY(rot90deg);
                vec.RotateX(2*rot90deg);
                break;
        case 7:
                vec.RotateY(rot90deg);
                vec.RotateX(3*rot90deg);
                break;
        case 8:
                vec.RotateY(2*rot90deg);
                break;
        case 9:
                vec.RotateY(2*rot90deg);
                vec.RotateZ(rot90deg);
                break;
        case 10:
                vec.RotateY(2*rot90deg);
                vec.RotateZ(2*rot90deg);
                break;
        case 11:
                vec.RotateY(2*rot90deg);
                vec.RotateZ(3*rot90deg);
                break;
        case 12:
                vec.RotateY(3*rot90deg);
                break;
        case 13:
                vec.RotateY(3*rot90deg);
                vec.RotateX(rot90deg);
                break;
        case 14:
                vec.RotateY(3*rot90deg);
                vec.RotateX(2*rot90deg);
                break;
        case 15:
                vec.RotateY(3*rot90deg);
                vec.RotateX(3*rot90deg);
                break;
        case 16:
                vec.RotateX(rot90deg);
                break;
        case 17:
                vec.RotateX(rot90deg);
                vec.RotateY(rot90deg);
                break;
        case 18:
                vec.RotateX(rot90deg);
                vec.RotateY(2*rot90deg);
                break;
        case 19:
                vec.RotateX(rot90deg);
                vec.RotateY(3*rot90deg);
                break;
        case 20:
                vec.RotateX(-rot90deg);
                break;
        case 21:
                vec.RotateX(-rot90deg);
                vec.RotateY(rot90deg);
                break;
        case 22:
                vec.RotateX(-rot90deg);
                vec.RotateY(2*rot90deg);
                break;
        case 23:
                vec.RotateX(-rot90deg);
                vec.RotateY(3*rot90deg);
                break;
        default: 
                break;
    }

    Double_t&& x = std::round(vec.X());
    Double_t&& y = std::round(vec.Y());
    Double_t&& z = std::round(vec.Z());

    return TVector3(x,y,z);
}

} //superfgd
} //reconstruction
} //esbroot