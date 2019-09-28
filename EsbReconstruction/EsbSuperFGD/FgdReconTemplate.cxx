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
    : freconFile(""), fUseOnlyLeafTemplates(false)
{
}

FgdReconTemplate::FgdReconTemplate(const char* templateConfigFile, Bool_t useOnlyLeafTemp)
    : freconFile(templateConfigFile), fUseOnlyLeafTemplates(useOnlyLeafTemp)
{
}

FgdReconTemplate::~FgdReconTemplate()
{
}

Bool_t FgdReconTemplate::IsLeaf(ReconHit* hit, std::vector<ReconHit>& hits)
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
        GetHitVectors(hit, hits, vecs);
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

Bool_t FgdReconTemplate::GetNextHit(ReconHit* previous, ReconHit* current, ReconHit*& next, std::vector<ReconHit>& hits)
{
    Bool_t nextFound(false);
    Bool_t rc(true);

    if(current==nullptr)
    {
        throw "Invalid current hit";
    }

    // Check for size conditions
    if(!fUseOnlyLeafTemplates
        && (current->fLocalHits.size()>2
        || current->fLocalEdges.size()>2
        || current->fLocalCorner.size()>2
        || current->fAllHits.size()>6)
        )
    {
        rc = false;
        nextFound = false;
        next = nullptr;
    }

    // Check for 4 and 5 hits allowed templates - only if fUseOnlyLeafTemplates is false
    if( !fUseOnlyLeafTemplates && 
        (current->fAllHits.size()==4 || current->fAllHits.size()==5 || current->fAllHits.size()==6)
        )
    {
        Bool_t isTemplateAllowed(false);
        Int_t permutation(0);
        std::vector<TVector3> vecs;
        GetHitVectors(current, hits, vecs);
        for(size_t temp=0; !isTemplateAllowed && temp < fStrangeVectors.size(); ++temp)
        {
            if(fStrangeVectors[temp].hitVectors.size() == current->fAllHits.size())
            {
                std::vector<TVector3>& tempVecs = fStrangeVectors[temp].hitVectors;
                isTemplateAllowed = AreVectorsEqual(tempVecs, vecs, permutation);
            }
        }

        if(!isTemplateAllowed)
        {
            rc = false;
            nextFound = false;
            next = nullptr;
        }
    }
    


    // 1. If it is  leaf and has only one near hit, it is the next one
    if(rc 
        && current->fIsLeaf 
        && current->fAllHits.size()==1)
    {
        next = &hits[current->fAllHits[0]];
        nextFound = true;  
    }

    // 2. If it is  leaf and has multiple hits, return the nearest one
    if(rc 
        && !nextFound
        && current->fIsLeaf 
        && current->fAllHits.size()>1)
    {
        size_t nearestId(0);
        Int_t min_dist = std::numeric_limits<Int_t>::max();
        for(size_t nid = 0; nid< current->fAllHits.size(); ++nid)
        {
            ReconHit* toComp = &hits[current->fAllHits[nid]];
            TVector3 vecPosition = current->fmppcLoc - toComp->fmppcLoc;
            Int_t dist = vecPosition.X()*vecPosition.X() + vecPosition.Y()*vecPosition.Y() + vecPosition.Z()*vecPosition.Z();

            if(dist < min_dist)
            {
                min_dist = dist;
                nearestId = nid;
            }
        }

        next = &hits[current->fAllHits[nearestId]];
        nextFound = true;
    }

    // 3. If it has 1 local hits (And previous hit is not it) return it
    if(rc 
        && !current->fIsLeaf 
        && !nextFound
        && current->fLocalHits.size()==1)
    {
        ReconHit* candidateHit = &hits[current->fLocalHits[0]];
        if(!candidateHit->fIsVisited
            && candidateHit->fLocalId != previous->fLocalId
            && !candidateHit->fIsLeaf)
        {
            next = candidateHit;
            nextFound = true;
        }
    }

    // 4. If it has 2 local hits (And previous hit is one of them) return the other one
    if(rc 
        && !current->fIsLeaf 
        && !nextFound
        && current->fLocalHits.size()==2)
    {
        if(std::find(current->fLocalHits.begin(), current->fLocalHits.end(), previous->fLocalId)!= current->fLocalHits.end())
        {
            Int_t nid = (current->fLocalHits[0] == previous->fLocalId ) ? 1 : 0;
            ReconHit* candidateHit = &hits[current->fLocalHits[nid]];
            if(!candidateHit->fIsLeaf)
            {
                next = candidateHit;
                nextFound = true;
            }
        }
    }

    // 5. Check in edges - if it has only one edge and is not visited or the previous one -> return it
    if(rc 
        && !current->fIsLeaf 
        && !nextFound
        && current->fLocalEdges.size()==1)
    {
        ReconHit* candidateHit = &hits[current->fLocalEdges[0]];
        if(!candidateHit->fIsVisited
            && candidateHit->fLocalId != previous->fLocalId
            && !candidateHit->fIsLeaf)
        {
            next = candidateHit;
            nextFound = true;
        }
    }

    // 6. Check in edges - if it has multiple ednges and previous hit is one of them, return the other one
    if(rc 
        && !current->fIsLeaf 
        && !nextFound
        && current->fLocalEdges.size()==2)
    {
        if(std::find(current->fLocalEdges.begin(), current->fLocalEdges.end(), previous->fLocalId)!= current->fLocalEdges.end())
        {
            Int_t nid = (current->fLocalEdges[0] == previous->fLocalId ) ? 1 : 0;
            ReconHit* candidateHit = &hits[current->fLocalEdges[nid]];
            if(!candidateHit->fIsLeaf)
            {
                next = candidateHit;
                nextFound = true;
            }
        }
    }

    // 7.  If one of the edges is visited and the other is not, return the not visited one
    if(rc 
        && !current->fIsLeaf 
        && !nextFound
        && current->fLocalEdges.size()==2)
    {
        Int_t zeroInd = current->fLocalEdges[0];
        Int_t oneInd = current->fLocalEdges[1];
        ReconHit* zeroHit = &hits[zeroInd];
        ReconHit* oneHit = &hits[oneInd];
        
        if(!zeroHit->fIsVisited 
            && !zeroHit->fIsLeaf
            && oneHit->fIsVisited)
        {
            next = zeroHit;
            nextFound = true;
        }

        if(zeroHit->fIsVisited 
            && !oneHit->fIsVisited
            && !oneHit->fIsLeaf)
        {
            next = oneHit;
            nextFound = true;
        }
    }

    // 8. Check in corner - if it has only one corner which is not visite or the previous one -> return it
    if(rc 
        && !current->fIsLeaf 
        && !nextFound
        && current->fLocalCorner.size()==1)
    {
        ReconHit* candidateHit = &hits[current->fLocalCorner[0]];
        if(!candidateHit->fIsVisited
            && candidateHit->fLocalId != previous->fLocalId
            && !candidateHit->fIsLeaf)
        {
            next = candidateHit;
            nextFound = true;
        }
    }

    // 9. Check in corner - if it has multiple ednges and previous hit is one of them, return the other one
    if(rc 
        && !current->fIsLeaf 
        && !nextFound
        && current->fLocalCorner.size()==2)
    {
        if(std::find(current->fLocalCorner.begin(), current->fLocalCorner.end(), previous->fLocalId)!= current->fLocalCorner.end())
        {
            Int_t nid = (current->fLocalCorner[0] == previous->fLocalId ) ? 1 : 0;
            ReconHit* candidateHit = &hits[current->fLocalCorner[nid]];
            if(!candidateHit->fIsLeaf)
            {
                next = candidateHit;
                nextFound = true;
            }
        }
    }

    // 10. If one of the corners is visited and the other is not, return the not visited one
    if(rc 
        && !current->fIsLeaf 
        && !nextFound
        && current->fLocalCorner.size()==2)
    {
        Int_t zeroInd = current->fLocalCorner[0];
        Int_t oneInd = current->fLocalCorner[1];
        ReconHit* zeroHit = &hits[zeroInd];
        ReconHit* oneHit = &hits[oneInd];
        
        if(!zeroHit->fIsVisited 
            && !zeroHit->fIsLeaf
            && oneHit->fIsVisited)
        {
            next = zeroHit;
            nextFound = true;
        }

        if(zeroHit->fIsVisited 
            && !oneHit->fIsVisited
            && !oneHit->fIsLeaf)
        {
            next = oneHit;
            nextFound = true;
        }
    }

    return nextFound;
}



Bool_t FgdReconTemplate::GetNextHit2(ReconHit* previous, ReconHit* current, ReconHit*& next, std::vector<ReconHit>& hits)
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
        ReconHit* candidateHit = &hits[current->fAllHits[nid]];

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

    next = &hits[current->fAllHits[nearestId]];
    nextFound = (min_dist!=std::numeric_limits<Int_t>::max());

    return nextFound;
}

void FgdReconTemplate::LoadTemplates()
{
    TVector3 center;
    std::vector<TVector3> leaves;
    std::vector<TVector3> strangeNodes;

    std::ifstream file(freconFile);
    std::string line;
    
    while(std::getline(file,line))
    {
        // 1. Check for leaf
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


        // 2. Check for getnext
        size_t stopInd = line.find(ReconTemplates::STRANGE_NODE);
        if( stopInd==0                       // String starts with The searched pattern
            && stopInd!=std::string::npos)   // Search found a result
        {
            TVector3 previousNode;
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
                            case 'X':   strangeNodes.emplace_back(x,y,z);
                                        break;
                            case 'P':   previousNode.SetX(x); 
                                        previousNode.SetY(y); 
                                        previousNode.SetZ(z);
                                        strangeNodes.emplace_back(x,y,z);
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

            for(size_t node=0; node<strangeNodes.size(); node++)
            {
                hitTemp.hitVectors.emplace_back(center - strangeNodes[node]);
            }

            if(!strangeNodes.empty())
            {
                hitTemp.previousHit = center - previousNode;
                fStrangeVectors.push_back(hitTemp);
            }
            
            strangeNodes.clear();
        }
    }

    LOG(debug) << " Leaf templates found " << fLeafVectors.size();
    LOG(debug) << " Strange node templates found " << fStrangeVectors.size();
}

void FgdReconTemplate::GetHitVectors(ReconHit* hit, std::vector<ReconHit>& hits, std::vector<TVector3>& vecs)
{
    for(size_t i=0; i< hit->fAllHits.size(); ++i)
    {
        ReconHit& neightbourHit = hits[hit->fAllHits[i]];
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