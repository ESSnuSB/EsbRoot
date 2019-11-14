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
    : feventData(eventData), fIsInit(false)
{
    Init();
}

FgdMCEventRecord::FgdMCEventRecord() 
    : feventData(""), fIsInit(false)
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
    fIsInit = false;
    feventData = data;
    Init();
}

Int_t FgdMCEventRecord::GetNuPdg(void)
{
    return std::stoi(fDataTokens[Data::NEUTRINO_PDG]);
}

Double_t FgdMCEventRecord::GetNuE(void)
{
    return std::stod(fDataTokens[Data::NEUTRINO_ENERGY]);
}

TVector3 FgdMCEventRecord::GetVertex(void)
{
    Double_t x = std::stod(fDataTokens[Data::VERTEX_POSTION_X]);
    Double_t y = std::stod(fDataTokens[Data::VERTEX_POSTION_Y]);
    Double_t z = std::stod(fDataTokens[Data::VERTEX_POSTION_Z]);

    return TVector3(x,y,z);
}

Bool_t FgdMCEventRecord::IsWeakCC(void)
{
    Int_t val = std::stoi(fDataTokens[Data::IS_WEAK_CC]);
    return (val==1);
}

Bool_t FgdMCEventRecord::IsWeakNC(void)
{
    Int_t val = std::stoi(fDataTokens[Data::IS_WEACK_NC]);
    return (val==1);
}

Bool_t FgdMCEventRecord::IsQuasiElastic(void)
{
    Int_t val = std::stoi(fDataTokens[Data::IS_QUASI_ELASTIC]);
    return (val==1);
}

FgdMCEventRecord& FgdMCEventRecord::operator=(const FgdMCEventRecord& c)
{
    this->feventData = c.feventData;
    this->Init();
    return *this;
}

std::vector<std::pair<Int_t, TVector3>> FgdMCEventRecord::GetPrimaryParticles()
{
    size_t particleDataSize = 4;

    size_t particlePdg = 0;
    size_t particleMomX = 1;
    size_t particleMomY = 2;
    size_t particleMomZ = 3;

    std::vector<std::pair<Int_t, TVector3>> particles;
    for(size_t i = Data::PRIMARY_PARTICLES; i < fDataTokens.size(); i+=particleDataSize)
    {
        Int_t pdg = std::stoi(fDataTokens[i + particlePdg]);
        Double_t momX = std::stod(fDataTokens[i + particleMomX]);
        Double_t momY = std::stod(fDataTokens[i + particleMomY]);
        Double_t momZ = std::stod(fDataTokens[i + particleMomZ]);

        particles.push_back(
                            std::pair<Int_t, TVector3>( pdg,    TVector3(momX , momY, momZ) )
                            );
    }
    return particles;
}

void FgdMCEventRecord::PrintData(std::ostream & stream)
{
    stream << feventData;
}

// -------------------------------------------------------------------------


// -----   Protected methods   ---------------------------------------------
void FgdMCEventRecord::Init()
{
    if(fIsInit) return;

    fIsInit = true;
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
    }
    catch(const std::exception& e)
    {
        LOG(fatal) << e.what();
        fIsInit = false;
    }
}

// -------------------------------------------------------------------------

}// namespace superfgd
}// namespace reconstruction
}// namespace esbroot