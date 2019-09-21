#include "EsbReconstruction/EsbSuperFGD/FgdReconTemplate.h"

#include <fstream>
#include <iostream>

namespace esbroot {
namespace reconstruction {
namespace superfgd {

using namespace std;

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
        // If it has only one local hit, there is not another option to check
        // cout << "Leaf " << "X " << hit->fmppcLoc.X() << " Y " << hit->fmppcLoc.Y()<< " Z " << hit->fmppcLoc.Z() << endl;
        isHitLeaf = true;
    }
    else
    {
        for(size_t temp=0; !isHitLeaf && temp < fLeafVectors.size(); ++temp)
        {
            if(fLeafVectors[temp].hitVectors.size() == hit->fLocalHits.size())
            {
                std::vector<TVector3> vecs;
                GetHitVectors(hit, hits, vecs);
                bool doesMatchTemplate(true);
                std::vector<TVector3>& tempVecs = fLeafVectors[temp].hitVectors;

                for(size_t i=0; doesMatchTemplate && i<vecs.size(); ++i)
                {
                    doesMatchTemplate = (   
                                            std::find(tempVecs.begin(), tempVecs.end(),vecs[i])     !=  tempVecs.end()
                                        );
                }
                isHitLeaf = doesMatchTemplate && !vecs.empty();
            }
        }
    }
    
    return isHitLeaf;
}

Bool_t FgdReconTemplate::GetNextHit(ReconHit* previous, ReconHit* current, ReconHit* next, std::vector<ReconHit>& hits)
{
    // TODO Implement
    return false;
}


void FgdReconTemplate::LoadTemplates()
{
    TVector3 center;
    std::vector<TVector3> leaves;
    std::vector<TVector3> next;

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
            for(Int_t leaf=0; leaf<leaves.size(); leaf++)
            {
                hitTemp.hitVectors.emplace_back(center - leaves[leaf]);
            }

            if(!leaves.empty())
            {
                fLeafVectors.push_back(hitTemp);
            }
            
            leaves.clear();
        }

        // size_t leafInd = line.find(ReconTemplates::GET_NEXT);
        // if( leafInd==0                       // String starts with The searched pattern
        //     && leafInd!=std::string::npos)   // Search found a result
        // {
        //     for(Int_t row =0; row < 3; ++row)
        //     {
        //         if(std::getline(file,line))
        //         {
        //             for(size_t cube=0; cube<line.size(); ++cube)
        //             {
                        
        //             }
        //         }
        //     }
        //     next.clear();
        // }
    }
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

} //superfgd
} //reconstruction
} //esbroot