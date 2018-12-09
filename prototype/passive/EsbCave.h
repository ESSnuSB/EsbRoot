/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

// -------------------------------------------------------------------------
// -----                    EsbCave  file                               -----
// -----                Created 26/03/14  by M. Al-Turany              -----
// -------------------------------------------------------------------------


#ifndef Cave_H
#define Cave_H

#include "FairModule.h"                 // for FairModule

#include "Rtypes.h"                     // for EsbCave::Class, ClassDef, etc

class EsbCave : public FairModule
{
  public:
    EsbCave(const char* name, const char* Title="Exp Cave");
    EsbCave();
    virtual ~EsbCave();
    virtual void ConstructGeometry();


  private:
    Double_t world[3];
    ClassDef(EsbCave,1) //PNDCaveSD
};

#endif //Cave_H

