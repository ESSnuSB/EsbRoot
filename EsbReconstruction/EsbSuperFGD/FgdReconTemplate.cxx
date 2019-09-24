#include "EsbReconstruction/EsbSuperFGD/FgdReconTemplate.h"

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
}

FgdReconTemplate::FgdReconTemplate(const char* templateConfigFile)
    : freconFile(templateConfigFile)
{
}

FgdReconTemplate::~FgdReconTemplate()
{
}

Bool_t FgdReconTemplate::IsLeaf(ReconHit* hit, std::vector<ReconHit>& hits)
{
    Bool_t isHitLeaf(false);

    if(hit->fLocalHits.size()==1)
    {
        isHitLeaf = true;
    }
    else
    {
        Int_t permutation(0);
        std::vector<TVector3> vecs;
        GetHitVectors(hit, hits, vecs);
        for(size_t temp=0; !isHitLeaf && temp < fLeafVectors.size(); ++temp)
        {
            if(fLeafVectors[temp].hitVectors.size() == hit->fLocalHits.size())
            {
                std::vector<TVector3>& tempVecs = fLeafVectors[temp].hitVectors;
                isHitLeaf = AreVectorsEqual(tempVecs, vecs, permutation);
            }
        }
    }
    
    return isHitLeaf;
}

// Bool_t FgdReconTemplate::GetNextHit(ReconHit* previous, ReconHit* current, ReconHit*& next, std::vector<ReconHit>& hits)
// {
//     Bool_t nextFound(false);

//     if(current==nullptr)
//     {
//         // cout   << endl;
//         // cout << " previous==nullptr " << (previous==nullptr) << " current->fLocalHits.size() " << current->fLocalHits.size() << endl;
//         // cout << "======================"  << endl;
//         throw "Invalid condition";
//     }

//     if(current->fIsLeaf)
//     {
//         // 1. If it has only one near hit, it is the next one
//         if(current->fLocalHits.size()==1)
//         {
//             next = &hits[current->fLocalHits[0]];
//             nextFound = true;
//         }
//         // 2. If more than 1 hit is a neighbour - choose the nearest one (for a leaf)
//         else
//         {
//             if(current->fLocalHits.empty())
//             {
//                 return nextFound;
//             }

//             size_t nearestId(0);
//             Int_t min_dist = std::numeric_limits<Int_t>::max();
//             for(size_t nid = 0; nid< current->fLocalHits.size(); ++nid)
//             {
//                 ReconHit* toComp = &hits[current->fLocalHits[nid]];
//                 TVector3 vecPosition = current->fmppcLoc - toComp->fmppcLoc;
//                 Int_t dist = vecPosition.X()*vecPosition.X() + vecPosition.Y()*vecPosition.Y() + vecPosition.Z()*vecPosition.Z();

//                 if(dist < min_dist)
//                 {
//                     min_dist = dist;
//                     nearestId = nid;
//                 }
//             }

//             next = &hits[current->fLocalHits[nearestId]];
//             nextFound = true;
//         }
//     }
//     // The logic is the following:
//     //  A) If it has two hits, 1 is the previous return the next one
//     //  B) If it has more hits, check if it has on opposite to the previous 
//     //      e.g. if previous vec was (0,0,1) check if there is (0,0,-1)
//     //      if yes, return it else check for the nearest hit
//     else if(current->fLocalHits.size()==2)
//     {
//         for(size_t nid = 0; nid< current->fLocalHits.size(); ++nid)
//         {
//             if(nid!=previous->fLocalId)
//             {
//                 nextFound = true;
//                 next = &hits[current->fLocalHits[nid]];
//                 break;
//             }
//         }
//     }
//     else
//     {
//         TVector3 prevVec = current->fmppcLoc - previous->fmppcLoc;
//         TVector3 oppositeVec(-prevVec.X(),-prevVec.Y(),-prevVec.Z());

//         for(size_t nid = 0; nid< current->fLocalHits.size(); ++nid)
//         {
//             ReconHit* localHit = &hits[current->fLocalHits[nid]];
//             TVector3 localDiff = current->fmppcLoc -localHit->fmppcLoc;
//             if(localDiff==oppositeVec)
//             {
//                 nextFound = true;
//                 next = localHit;
//                 break;
//             }
//         }

//         if(!nextFound)
//         {
//             size_t nearestId(0);
//             Int_t min_dist = std::numeric_limits<Int_t>::max();
//             for(size_t nid = 0; nid < current->fLocalHits.size(); ++nid)
//             {
//                 ReconHit* toComp = &hits[current->fLocalHits[nid]];
//                 TVector3 vecPosition = current->fmppcLoc - toComp->fmppcLoc;
//                 Int_t dist = vecPosition.X()*vecPosition.X() + vecPosition.Y()*vecPosition.Y() + vecPosition.Z()*vecPosition.Z();

//                 if(toComp->fLocalId!=previous->fLocalId
//                     &&  dist < min_dist)
//                 {
//                     min_dist = dist;
//                     nearestId = nid;
//                 }
//             }

//             next = &hits[current->fLocalHits[nearestId]];
//             nextFound = true;
//         }
//     }  
    
//     return nextFound;
// }

Bool_t FgdReconTemplate::GetNextHit(ReconHit* previous, ReconHit* current, ReconHit*& next, std::vector<ReconHit>& hits)
{
    Bool_t nextFound(false);

    if(current==nullptr)
    {
        // cout   << endl;
        // cout << " previous==nullptr " << (previous==nullptr) << " current->fLocalHits.size() " << current->fLocalHits.size() << endl;
        // cout << "======================"  << endl;
        throw "Invalid condition";
    }

    if(current->fIsLeaf)
    {
        // 1. If it has only one near hit, it is the next one
        if(current->fLocalHits.size()==1)
        {
            next = &hits[current->fLocalHits[0]];
            nextFound = true;
        }
        // 2. If more than 1 hit is a neighbour - choose the nearest one (for a leaf)
        else
        {
            if(current->fLocalHits.empty())
            {
                return nextFound;
            }

            size_t nearestId(0);
            Int_t min_dist = std::numeric_limits<Int_t>::max();
            for(size_t nid = 0; nid< current->fLocalHits.size(); ++nid)
            {
                ReconHit* toComp = &hits[current->fLocalHits[nid]];
                TVector3 vecPosition = current->fmppcLoc - toComp->fmppcLoc;
                Int_t dist = vecPosition.X()*vecPosition.X() + vecPosition.Y()*vecPosition.Y() + vecPosition.Z()*vecPosition.Z();

                if(dist < min_dist)
                {
                    min_dist = dist;
                    nearestId = nid;
                }
            }

            next = &hits[current->fLocalHits[nearestId]];
            nextFound = true;
        }
    }
    else
    {
        std::vector<TVector3> vecs;
        GetHitVectors(current, hits, vecs);
        for(size_t temp=0; !nextFound && temp < fGetNExtVectors.size(); ++temp)
        {
            Int_t foundPermut;
            if(fGetNExtVectors[temp].hitVectors.size() == current->fLocalHits.size())
            {
                std::vector<TVector3>& tempVecs = fGetNExtVectors[temp].hitVectors;
                
                if(AreVectorsEqual(tempVecs, vecs, foundPermut))
                {
                    TVector3 prevVec = current->fmppcLoc - previous->fmppcLoc;
                    TVector3& prevVecTem = fGetNExtVectors[temp].previousHit;
                    TVector3 tmp =  GetPermutation(prevVecTem, foundPermut);
                    nextFound = (tmp == prevVec);

                    if(current->fLocalId == 32)
                    {
                        cout << " temp " << temp << endl;
                        cout << " foundPermut " << foundPermut << endl;
                        cout << " nextFound " << nextFound << endl;
                        cout << " prevVec " << " X " << prevVec.X()<< " Y " << prevVec.Y()<< " Z " << prevVec.Z() << endl;
                        cout << " prevVecTem " << " X " << prevVecTem.X()<< " Y " << prevVecTem.Y()<< " Z " << prevVecTem.Z() << endl;
                        cout << " tmp " << " X " << tmp.X()<< " Y " << tmp.Y()<< " Z " << tmp.Z() << endl;
                        cout << endl;
                        for(size_t jj = 0; jj< tempVecs.size(); ++jj)
                        {
                            cout << " tempVecs " << " X " << tempVecs[jj].X()<< " Y " << tempVecs[jj].Y()<< " Z " << tempVecs[jj].Z() << endl;
                        }
                        cout << endl;
                        for(size_t jj = 0; jj< tempVecs.size(); ++jj)
                        {
                            TVector3 tt =  GetPermutation(tempVecs[jj], foundPermut);
                            cout << " tempVecs Permute" << " X " << tt.X()<< " Y " << tt.Y()<< " Z " << tt.Z() << endl;
                        }
                        cout << endl;
                        for(size_t jj = 0; jj< vecs.size(); ++jj)
                        {
                            cout << " vecs " << " X " << vecs[jj].X()<< " Y " << vecs[jj].Y()<< " Z " << vecs[jj].Z() << endl;
                        }
                    }
                }
            }
               

            if(nextFound)
            {
                // if(current->fLocalId == 32)

                TVector3& nextVecTem = fGetNExtVectors[temp].nextHit;
                TVector3 nextTmp =  GetPermutation(nextVecTem, foundPermut);


                
                for(size_t nid = 0; nid< current->fLocalHits.size(); ++nid)
                {
                    ReconHit* toComp = &hits[current->fLocalHits[nid]];
                    TVector3 vecPosition = current->fmppcLoc - toComp->fmppcLoc;


                    if(vecPosition == nextTmp)
                    {
                        if(current->fLocalId == 32) cout << " next = toComp; " << endl;
                        next = toComp;
                        break;
                    }
                }
            }
        }
    }
    
    return nextFound;
}


void FgdReconTemplate::LoadTemplates()
{
    TVector3 center;
    std::vector<TVector3> leaves;

    std::vector<TVector3> nextNodes;
    TVector3 nextNode;
    TVector3 previousNode;

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
        size_t nextInd = line.find(ReconTemplates::GET_NEXT);
        if( nextInd==0                       // String starts with The searched pattern
            && nextInd!=std::string::npos)   // Search found a result
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
                            case 'X':   nextNodes.emplace_back(x,y,z);
                                        break;
                            case 'N':   nextNode.SetX(x); 
                                        nextNode.SetY(y); 
                                        nextNode.SetZ(z);
                                        nextNodes.emplace_back(x,y,z);
                                        break;
                            case 'P':   previousNode.SetX(x); 
                                        previousNode.SetY(y); 
                                        previousNode.SetZ(z);
                                        nextNodes.emplace_back(x,y,z);
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

            for(size_t node=0; node<nextNodes.size(); node++)
            {
                hitTemp.hitVectors.emplace_back(center - nextNodes[node]);
            }

            if(!nextNodes.empty())
            {
                hitTemp.previousHit = center - previousNode;
                hitTemp.nextHit = center - nextNode;
                fGetNExtVectors.push_back(hitTemp);
            }
            
            nextNodes.clear();
        }
    }

    std::cout << " Leaf templates found " << fLeafVectors.size() << std::endl;
    std::cout << " GetNext templates found " << fGetNExtVectors.size() << std::endl;
}

void FgdReconTemplate::GetHitVectors(ReconHit* hit, std::vector<ReconHit>& hits, std::vector<TVector3>& vecs)
{
    for(size_t i=0; i< hit->fLocalHits.size(); ++i)
    {
        ReconHit& neightbourHit = hits[hit->fLocalHits[i]];
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