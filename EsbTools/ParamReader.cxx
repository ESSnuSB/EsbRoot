#include "EsbTools/ParamReader.h"
#include "EsbTools/EsbDefines.h"


namespace esbroot {
namespace tools {


ParamReader::ParamReader()
{
}

ParamReader::~ParamReader()
{
}

void    ParamReader::LoadParams(const string& fullfilePathName)
{
    std::ifstream file(fullfilePathName);
    std::string line;
    while(std::getline(file,line))
    {
        size_t indSep = line.find(PR::SEPERATOR);
        if(indSep!=-1)
        {
            string key = line.substr(0,indSep);
            indSep++; // the value is after the seperator
            string value = line.substr(indSep,std::string::npos);
            fparamsMap.emplace(key,value);
        }
    }
}


string  ParamReader::ParamAsString(const string& paramName) const
{
    Validate(paramName);
    return fparamsMap[paramName];
}

Int_t   ParamReader::ParamAsInt(const string& paramName) const
{
    Validate(paramName);
    std::string val = fparamsMap[paramName];
    return std::stoi(val);
}

bool    ParamReader::ParamAsBool(const string& paramName) const
{
    Int_t val = ParamAsInt(paramName);
    return (val>0);
}

Double_t    ParamReader::ParamAsDouble(const string& paramName) const
{
    Validate(paramName);
    std::string val = fparamsMap[paramName];
    return std::stod(val);
}

void    ParamReader::Validate(const string& paramName) const
{
    if(fparamsMap.find(paramName) == fparamsMap.end())
    {
        THROW("Parameter not found in map " + paramName);
    }
}

bool   ParamReader::ExistsParam(const string& paramName) const
{
    bool exists(false);
    exists = (fparamsMap.find(paramName) != fparamsMap.end());
    return exists;
}

}
}