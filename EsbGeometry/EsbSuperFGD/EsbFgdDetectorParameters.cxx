#include "EsbGeometry/EsbSuperFGD/EsbFgdDetectorParameters.h"
#include "EsbGeometry/EsbSuperFGD/Materials.h"
#include <exception>

#include "FairLogger.h"                 // for FairLogger, etc

using namespace std;

namespace esbroot {

namespace geometry {

namespace superfgd {


#define CHECK_PARAM(param) \
    if(!ExistsParam(paramName))\
    {\
        LOG(fatal) << "Parameter not found in map " << paramName;\
        throw paramName;\
    }\

// -----   Default constructor   -------------------------------------------
FgdDetectorParameters::FgdDetectorParameters(void)
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
FgdDetectorParameters::~FgdDetectorParameters(void)
{
}
// -------------------------------------------------------------------------


/*
    Add all values from the file in format:
    key = value; This will be easier for maintenance
    if more values are required.
*/
#include <fstream>
void FgdDetectorParameters::LoadPartParams(string fullfilePathName)
{
    std::ifstream file(fullfilePathName);
    std::string line;
    while(std::getline(file,line))
    {
        size_t indSep = line.find(DP::SEPERATOR);
        if(indSep!=-1)
        {
            string key = line.substr(0,indSep);
            indSep++; // the value is after the seperator
            string value = line.substr(indSep,std::string::npos);
            fparamsMap.emplace(key,value);
        }
    }
}


string FgdDetectorParameters::ParamAsString(const string& paramName) const
{
    CHECK_PARAM(paramName);
    return fparamsMap[paramName];
}

Int_t       FgdDetectorParameters::ParamAsInt(const string& paramName) const
{
    CHECK_PARAM(paramName);
    std::string val = fparamsMap[paramName];
    return std::stoi(val);
}

Bool_t       FgdDetectorParameters::ParamAsBool(const string& paramName) const
{
    Int_t val = ParamAsInt(paramName);
    return (val>0);
}

Double_t    FgdDetectorParameters::ParamAsDouble(const string& paramName) const
{
    CHECK_PARAM(paramName);
    std::string val = fparamsMap[paramName];
    return std::stod(val);
}

FgdDetectorParameters& FgdDetectorParameters::operator=(FgdDetectorParameters& newDp) 
{
    SetParamsMap(newDp.GetParamsMap());
};

Bool_t   FgdDetectorParameters::ExistsParam(const string& paramName) const
{
    bool exists(false);
    exists = (fparamsMap.find(paramName) != fparamsMap.end());
    return exists;
}

}   // superfgd
}   // geometry
}   // esbroot