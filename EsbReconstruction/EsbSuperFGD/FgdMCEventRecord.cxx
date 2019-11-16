#include "EsbReconstruction/EsbSuperFGD/FgdMCEventRecord.h"


#include "FairLogger.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include <math.h>
#include <bits/stdc++.h>

namespace esbroot {
namespace reconstruction {
namespace superfgd {


// -----   Constructors and Destructors  -----------------------------------

FgdMCEventRecord::FgdMCEventRecord(std::string eventData) 
    : TObject(), feventData(eventData), fnuPdg(0)
        , fNuEnergy(0.), fvertex (TVector3(0,0,0)), fIsWeakCC(false)
        , fIsWeakNC(false), fIsQuasiElastic(false)
{
    Init();
}

FgdMCEventRecord::FgdMCEventRecord() 
    : feventData("")
{
}

FgdMCEventRecord::FgdMCEventRecord(const FgdMCEventRecord& c)
{
    this->feventData = c.feventData;
    this->Init();
}

FgdMCEventRecord::~FgdMCEventRecord()
{
}
// -------------------------------------------------------------------------

// -----   Public methods   ------------------------------------------------

void FgdMCEventRecord::SetEventData(std::string data)
{
    feventData = data;
    Init();
}

Int_t FgdMCEventRecord::GetNuPdg(void)
{
    return fnuPdg;
}

Double_t FgdMCEventRecord::GetNuE(void)
{
    return fNuEnergy;
}

TVector3 FgdMCEventRecord::GetVertex(void)
{
    return fvertex;
}

Bool_t FgdMCEventRecord::IsWeakCC(void)
{
    return fIsWeakCC;
}

Bool_t FgdMCEventRecord::IsWeakNC(void)
{
    return fIsWeakNC;
}

Bool_t FgdMCEventRecord::IsQuasiElastic(void)
{
    return fIsQuasiElastic;
}

FgdMCEventRecord& FgdMCEventRecord::operator=(const FgdMCEventRecord& c)
{
    this->feventData = c.feventData;
    this->Init();
    return *this;
}

const std::vector<std::pair<Int_t, TVector3>>& FgdMCEventRecord::GetPrimaryParticles()
{
    if(fPrimaryParticles.empty()) // primary particles is a transient value
    {
        Init();
    }

    return fPrimaryParticles;
}

void FgdMCEventRecord::PrintData(std::ostream & stream)
{
    stream << feventData;
}

// -------------------------------------------------------------------------


// -----   Protected methods   ---------------------------------------------
void FgdMCEventRecord::Init()
{
    fDataTokens.clear();

    try
    {        
        const char spaceChar(' ');

        std::istringstream ss(feventData);
        std::string token;
        while(std::getline(ss, token, spaceChar))
        {
            if(!token.empty())
            {
                fDataTokens.push_back(token);
            }
        }
        InitMembers();
    }
    catch(const std::exception& e)
    {
        LOG(fatal) << e.what();
    }
}

void FgdMCEventRecord::InitMembers()
{
    fnuPdg = std::stoi(fDataTokens[Data::NEUTRINO_PDG]);
    fNuEnergy = std::stod(fDataTokens[Data::NEUTRINO_ENERGY]);

    Double_t x = std::stod(fDataTokens[Data::VERTEX_POSTION_X]);
    Double_t y = std::stod(fDataTokens[Data::VERTEX_POSTION_Y]);
    Double_t z = std::stod(fDataTokens[Data::VERTEX_POSTION_Z]);

    fvertex = TVector3(x,y,z);

    Int_t isWeakCC = std::stoi(fDataTokens[Data::IS_WEAK_CC]);
    fIsWeakCC = (isWeakCC==1);

    Int_t isWeakNC = std::stoi(fDataTokens[Data::IS_WEACK_NC]);
    fIsWeakNC = (isWeakNC==1);

    Int_t isQuasiElastic = std::stoi(fDataTokens[Data::IS_QUASI_ELASTIC]);
    fIsQuasiElastic = (isQuasiElastic==1);


    // Initalize primary particles
    size_t particleDataSize = 4;

    size_t particlePdg = 0;
    size_t particleMomX = 1;
    size_t particleMomY = 2;
    size_t particleMomZ = 3;

    fPrimaryParticles.clear();
    for(size_t i = Data::PRIMARY_PARTICLES; i < fDataTokens.size(); i+=particleDataSize)
    {
        Int_t pdg = std::stoi(fDataTokens[i + particlePdg]);
        Double_t momX = std::stod(fDataTokens[i + particleMomX]);
        Double_t momY = std::stod(fDataTokens[i + particleMomY]);
        Double_t momZ = std::stod(fDataTokens[i + particleMomZ]);

        fPrimaryParticles.push_back(
                            std::pair<Int_t, TVector3>( pdg,    TVector3(momX , momY, momZ) )
                            );
    }
}

// -------------------------------------------------------------------------

}// namespace superfgd
}// namespace reconstruction
}// namespace esbroot