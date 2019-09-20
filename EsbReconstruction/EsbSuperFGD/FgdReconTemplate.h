#ifndef ESBROOT_ESBDRECONSTRUCTION_FGD_RECON_TEMPLATE_H
#define ESBROOT_ESBDRECONSTRUCTION_FGD_RECON_TEMPLATE_H

#include "TObject.h"
#include <TVector3.h>
#include <vector>
#include <algorithm>
#include "EsbReconstruction/EsbSuperFGD/FgdReconHit.h"

namespace esbroot {
namespace reconstruction {
namespace superfgd {

namespace ReconTemplates
{
    static const std::string LEAF = "Leaf";
    static const std::string GET_NEXT = "GetNext";

    static const char SEPARATOR = ' ';
    static const char NO_HIT = 'O';
    static const char HIT = 'X';
    static const char PREVIOUS_HIT = 'P';
    static const char NEXT_HI = 'N';
}

class FgdReconTemplate : public TObject
{

public:

    /** Constructor with argument
        *@param templateConfigFile - path to the template configuration file
    **/ 
    FgdReconTemplate(const char* templateConfigFile);

    ~FgdReconTemplate();

    Bool_t IsLeaf(Int_t ind, std::vector<ReconHit>& hits);

    Bool_t GetNextHit(Int_t previousId, Int_t currentId, Int_t& nextId, std::vector<ReconHit>& hits);

    void AddLeafVector(TVector3& v){fLeafVectors.push_back(v);}
    void AddLeafVector(Int_t x, Int_t y, Int_t z){fLeafVectors.emplace_back(x,y,z);}
    void ClearLeavesVector(){fLeafVectors.clear();}

    void AddNextVector(TVector3& v){fGetNExtVectors.push_back(v);}
    void AddNextVector(Int_t x, Int_t y, Int_t z){fGetNExtVectors.emplace_back(x,y,z);}
    void ClearNextVector(){fGetNExtVectors.clear();}

private:

    std::vector<TVector3> fLeafVectors;//!<!  
    std::vector<TVector3> fGetNExtVectors;//!<!  

    ClassDef(FgdReconTemplate, 2);
};

} //superfgd
} //reconstruction
} //esbroot

#endif