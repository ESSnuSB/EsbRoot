#ifndef ESBROOT_ESBDRECONSTRUCTION_FGD_RECON_TEMPLATE_H
#define ESBROOT_ESBDRECONSTRUCTION_FGD_RECON_TEMPLATE_H


#include "EsbReconstruction/EsbSuperFGD/FgdReconHit.h"

#include "TObject.h"
#include <TVector3.h>

#include <algorithm>
#include <vector>

namespace esbroot {
namespace reconstruction {
namespace superfgd {

class FgdReconTemplate : public TObject
{

public:

    /** Default constructor **/  
    FgdReconTemplate();

    ~FgdReconTemplate();

    Bool_t IsLeaf(ReconHit* hit);

    Bool_t GetNextHit(ReconHit* previous, ReconHit* current, ReconHit*& next);

private:

    /** Class Containing the vectors for each template found**/ 
    class HitTemplate
    {
    public:
        HitTemplate(){}

        ~HitTemplate(){}

        HitTemplate(const HitTemplate& c)
        {
            this->previousHit = c.previousHit;
            this->nextHit = c.nextHit;
            this->hitVectors = c.hitVectors;
        }

        HitTemplate& operator=(const HitTemplate& c)
        {
            this->previousHit = c.previousHit;
            this->nextHit = c.nextHit;
            this->hitVectors = c.hitVectors;
            return *this;
        }

        size_t Length(){return hitVectors.size();}
    
        TVector3 nextHit;
        TVector3 previousHit;
        std::vector<TVector3> hitVectors;
    };

    void LoadTemplates();
    void GetHitVectors(ReconHit* hit, std::vector<TVector3>& vecs);

    Bool_t AreVectorsEqual(const std::vector<TVector3>& tempVecs, const std::vector<TVector3>& vecs, Int_t& foundPermutation );
    TVector3 GetPermutation(TVector3 vec, Int_t numPermutation);

    std::vector<FgdReconTemplate::HitTemplate> fLeafVectors;//!<!  

    ClassDef(FgdReconTemplate, 2);
};

} //superfgd
} //reconstruction
} //esbroot

#endif