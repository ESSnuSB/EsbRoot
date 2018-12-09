/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

// -------------------------------------------------------------------------
// -----                    EsbMagnet  file                               -----
// -----                Created 26/03/14  by M. Al-Turany              -----
// -------------------------------------------------------------------------


#ifndef MAGNET_H
#define MAGNET_H

#include "FairModule.h"                 // for FairModule
#include "Rtypes.h"                     // for EsbMagnet::Class, Bool_t, etc
#include <string>                       // for string

class EsbMagnet : public FairModule
{
  public:
    EsbMagnet(const char* name, const char* Title="MY Magnet");
    EsbMagnet();
    virtual ~EsbMagnet();
    void ConstructGeometry();
    ClassDef(EsbMagnet,1)

};

#endif //MAGNET_H

