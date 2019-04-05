#ifndef ESBROOT_ESBTOOLS_PARAM_READER_H
#define ESBROOT_ESBTOOLS_PARAM_READER_H 1

#include "TObject.h"

#include <sstream>
#include <exception>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

namespace esbroot {
namespace tools {

namespace PR
{
    static const char SEPERATOR='=';
}

class ParamReader
{
public:
    /** Constructor **/
    ParamReader();

    /** Destructor **/
    ~ParamReader();

    /** Read the paramters and stores them in a map member
     *@param fullfilePathName - full path of the file 
     **/
    void LoadParams(const string& fullfilePathName);

    /** Returns the parater in a string format
     *@param paramName - name of parameter 
     **/
    string ParamAsString(const string& paramName) const;

    /** Returns the parater as an integer
     *@param paramName - name of parameter 
     **/
    Int_t       ParamAsInt(const string& paramName) const;

    /** Returns the parater as a bool
     *@param paramName - name of parameter 
     **/
    bool        ParamAsBool(const string& paramName) const;

    /** Returns the parater as a double
     *@param paramName - name of parameter 
     **/
    Double_t    ParamAsDouble(const string& paramName) const;

    /** Checks if the paramters exists in the map
     *@param paramName - name of parameter 
     **/
    bool    ExistsParam(const string& paramName) const;

private:

    /** Checks if the paramters exists in the map, thows exception if
     * parameters is not in the map [internal use only]
     *@param paramName - name of parameter 
     **/
    void    Validate(const string& paramName) const;

    /** Map to hold the key-value pairs of the parameters **/
    mutable map<string, string> fparamsMap;
};

}
}

#endif