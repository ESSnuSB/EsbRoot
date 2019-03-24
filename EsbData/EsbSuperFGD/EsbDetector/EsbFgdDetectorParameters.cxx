#include "EsbData/EsbSuperFGD/EsbDetector/EsbFgdDetectorParameters.h"
#include "EsbData/EsbSuperFGD/EsbDetector/Materials.h"
#include <exception>
#include <iostream>
#include <fstream>

#include <sstream>
using namespace std;

namespace esbroot {

namespace data {

namespace superfgd {

namespace detector {

// -----   Default constructor   -------------------------------------------
FgdDetectorParameters::FgdDetectorParameters(void):frootGeometry(nullptr)
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
FgdDetectorParameters::~FgdDetectorParameters(void)
{
    // frootGeometry probably is freed by ROOTGeomAnalyzer
    if(frootGeometry)
        delete frootGeometry;
}
// -------------------------------------------------------------------------

TGeoManager* FgdDetectorParameters::GetRootGeometry() const 
{
    if (frootGeometry==nullptr)
        CreateRootGeometry();

    return frootGeometry;
}

/*
    Construct the ROOT geometry for the SuperFGD 
    it is assumed that the geometry is based on cubes
    with optic fibers through them
*/
void FgdDetectorParameters::CreateRootGeometry() const 
{
    // Create root geometry in CLHEP unit system
    // NOTE: use pointers, ROOT handles and cleans the used geometry
    using namespace esbroot::data::superfgd::detector; // for MATERIALS

    TGeoManager *geom = new TGeoManager("geometry", "Simple Geometry");

    // Basic elements
    TGeoElement* elH = new TGeoElement("H", "hydrogen", materials::H_Z, materials::H1_ATOMIC_MASS/(g/mole));
    TGeoElement* elC = new TGeoElement("C", "carbon",   materials::C_Z, materials::C12_ATOMIC_MASS/(g/mole));
    //TGeoElement* elN = new TGeoElement("N", "nitrogen", (int)MATERIALS::N_Z, MATERIALS::N_ATOMIC_MASS/(g/mole));
    //TGeoElement* elO = new TGeoElement("O", "oxygen",   (int)MATERIALS::O_Z, MATERIALS::O_ATOMIC_MASS/(g/mole));

    // Scintillator (polystyrene)
    TGeoMixture* mixSci = new TGeoMixture("sciMixture", materials::POLYSTYRENE_NCOMPONENTS, materials::POLYSTYRENE_DENSITY);
    mixSci->AddElement(elC, materials::POLYSTYRENE_NATOMS_C);
    mixSci->AddElement(elH, materials::POLYSTYRENE_NATOMS_H);
    //mixSci->AddElement(elN, materials::POLYSTYRENE_NATOMS_H);
    //mixSci->Print();
    TGeoMedium *scintillator = new TGeoMedium("scintillator", 1, mixSci);

    Double_t lenghtX = ParamAsDouble(DP::length_X) * ParamAsDouble(DP::number_cubes_X) * GetLenghtUnit();
    Double_t lenghtY = ParamAsDouble(DP::length_Y) * ParamAsDouble(DP::number_cubes_Y) * GetLenghtUnit();
    Double_t lenghtZ = ParamAsDouble(DP::length_Z) * ParamAsDouble(DP::number_cubes_Z) * GetLenghtUnit();

    //double detectorOrigin[3] = {0., 0., (lenghtZ/2)};// Z axis is the central axis used
    double detectorOrigin[3] = {0., 0., 0.};// Z axis is the central axis used
    TGeoBBox* detectorBox = new TGeoBBox("detector", 0.5*lenghtX, 0.5*lenghtY, 0.5*lenghtZ, detectorOrigin);
    TGeoVolume* detector = new TGeoVolume("detector", detectorBox, scintillator);
    geom->SetTopVolume(detector);

    geom->CloseGeometry();
    frootGeometry = geom;
}

/*
    Add all values from the file in format:
    key = value; This will be easier for maintenance
    if more values are required.
*/
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
    Validate(paramName);
    return fparamsMap[paramName];
}

Int_t       FgdDetectorParameters::ParamAsInt(const string& paramName) const
{
    Validate(paramName);
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
    Validate(paramName);
    std::string val = fparamsMap[paramName];
    return std::stod(val);
}

FgdDetectorParameters& FgdDetectorParameters::operator=(FgdDetectorParameters& newDp) 
{
    SetParamsMap(newDp.GetParamsMap());
};

void    FgdDetectorParameters::Validate(const string& paramName) const
{
    if(fparamsMap.find(paramName) == fparamsMap.end())
    {
        THROW("Parameter not found in map " + paramName);
    }
}


Bool_t   FgdDetectorParameters::ExistsParam(const string& paramName) const
{
    bool exists(false);
    exists = (fparamsMap.find(paramName) != fparamsMap.end());
    return exists;
}

}   // detector
}   // superfgd
}   // data
}   // esbroot