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
{
    LoadTemplates();
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
    // ==============================
    // 1. Single leaf templates
    // ==============================
    //     Leaf 
    // --- --- ---
    // --- -C- -X-
    // --- --- ---
    TVector3 leaf1_1(0,0,-1);
    FgdReconTemplate::HitTemplate leaf1_1_temp;
    leaf1_1_temp.hitVectors.emplace_back(leaf1_1);

    fLeafVectors.push_back(leaf1_1_temp);

    // Leaf 
    // --- --- -X-
    // --- -C- ---
    // --- --- ---
    TVector3 leaf1_2(0,-1,-1);
    FgdReconTemplate::HitTemplate leaf1_2_temp;
    leaf1_2_temp.hitVectors.emplace_back(leaf1_2);

    fLeafVectors.push_back(leaf1_2_temp);

    // Leaf 
    // --- --- --X
    // --- -C- ---
    // --- --- ---
    TVector3 leaf1_3(1,-1,-1);
    FgdReconTemplate::HitTemplate leaf1_3_temp;
    leaf1_3_temp.hitVectors.emplace_back(leaf1_3);

    fLeafVectors.push_back(leaf1_3_temp);


    // ==============================
    // 2. Two hits leaf templates
    // ==============================

    // Leaf 
    // --- --- -X-
    // --- -C- -X-
    // --- --- ---
    TVector3 leaf2_1_1(0,-1,-1);
    TVector3 leaf2_1_2(0,0,-1);
    FgdReconTemplate::HitTemplate leaf2_1_temp;
    leaf2_1_temp.hitVectors.emplace_back(leaf2_1_1);
    leaf2_1_temp.hitVectors.emplace_back(leaf2_1_2);

    fLeafVectors.push_back(leaf2_1_temp);

    // Leaf 
    // --- --- --X
    // --- -C- -X-
    // --- --- ---
    TVector3 leaf2_2_1(1,-1,-1);
    TVector3 leaf2_2_2(0,0,-1);
    FgdReconTemplate::HitTemplate leaf2_2_temp;
    leaf2_2_temp.hitVectors.emplace_back(leaf2_2_1);
    leaf2_2_temp.hitVectors.emplace_back(leaf2_2_2);

    fLeafVectors.push_back(leaf2_2_temp);

    // Leaf 
    // --- --- X--
    // --- -C- X--
    // --- --- ---
    TVector3 leaf2_3_1(-1,-1,-1);
    TVector3 leaf2_3_2(-1,0,-1);
    FgdReconTemplate::HitTemplate leaf2_3_temp;
    leaf2_3_temp.hitVectors.emplace_back(leaf2_3_1);
    leaf2_3_temp.hitVectors.emplace_back(leaf2_3_2);

    fLeafVectors.push_back(leaf2_3_temp);

    // Leaf 
    // --- --- --X
    // --- -C- --X
    // --- --- ---
    TVector3 leaf2_4_1(1,-1,-1);
    TVector3 leaf2_4_2(1,0,-1);
    FgdReconTemplate::HitTemplate leaf2_4_temp;
    leaf2_4_temp.hitVectors.emplace_back(leaf2_4_1);
    leaf2_4_temp.hitVectors.emplace_back(leaf2_4_2);

    fLeafVectors.push_back(leaf2_4_temp);

    // ==============================
    // 3. Three hits leaf templates
    // ==============================

    // Leaf 
    // --- --- -XX
    // --- -C- -X-
    // --- --- ---
    TVector3 leaf3_1_1(0,-1,-1);
    TVector3 leaf3_1_2(1,-1,-1);
    TVector3 leaf3_1_3(0,0,-1);
    FgdReconTemplate::HitTemplate leaf3_1_temp;
    leaf3_1_temp.hitVectors.emplace_back(leaf3_1_1);
    leaf3_1_temp.hitVectors.emplace_back(leaf3_1_2);
    leaf3_1_temp.hitVectors.emplace_back(leaf3_1_3);

    fLeafVectors.push_back(leaf3_1_temp);


    // Leaf 
    // --- --- XX-
    // --- -C- -X-
    // --- --- ---
    TVector3 leaf3_2_1(-1,-1,-1);
    TVector3 leaf3_2_2(0,-1,-1);
    TVector3 leaf3_2_3(0,0,-1);
    FgdReconTemplate::HitTemplate leaf3_2_temp;
    leaf3_2_temp.hitVectors.emplace_back(leaf3_2_1);
    leaf3_2_temp.hitVectors.emplace_back(leaf3_2_2);
    leaf3_2_temp.hitVectors.emplace_back(leaf3_2_3);

    fLeafVectors.push_back(leaf3_2_temp);

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