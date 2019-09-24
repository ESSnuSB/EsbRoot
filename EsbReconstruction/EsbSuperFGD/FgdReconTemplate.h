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

namespace ReconTemplates
{
    static const std::string LEAF = "Leaf";
    static const std::string GET_NEXT = "GetNextNode";

    static const char SEPARATOR = ' ';
    static const char NO_HIT = 'O';
    static const char HIT = 'X';
    static const char PREVIOUS_HIT = 'P';
    static const char NEXT_HIT = 'N';
    static const char CENTER = 'C';
}

class FgdReconTemplate : public TObject
{

public:

    /** Default constructor **/  
    FgdReconTemplate();

    /** Constructor with argument
        *@param templateConfigFile - path to the template configuration file
    **/ 
    FgdReconTemplate(const char* templateConfigFile);

    ~FgdReconTemplate();

    Bool_t IsLeaf(ReconHit* hit, std::vector<ReconHit>& hits);

    Bool_t GetNextHit(ReconHit* previous, ReconHit* current, ReconHit*& next, std::vector<ReconHit>& hits);

    void LoadTemplates();

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
    
        TVector3 previousHit;
        TVector3 nextHit;
        std::vector<TVector3> hitVectors;
    };

    void GetHitVectors(ReconHit* hit, std::vector<ReconHit>& hits, std::vector<TVector3>& vecs);

    Bool_t AreVectorsEqual(const std::vector<TVector3>& tempVecs, const std::vector<TVector3>& vecs, Int_t& foundPermutation );
    TVector3 GetPermutation(TVector3 vec, Int_t numPermutation);

    std::vector<FgdReconTemplate::HitTemplate> fLeafVectors;//!<!  
    std::vector<FgdReconTemplate::HitTemplate> fGetNExtVectors;//!<!  

    std::string freconFile;//!<!  

    ClassDef(FgdReconTemplate, 2);
};

} //superfgd
} //reconstruction
} //esbroot

#endif