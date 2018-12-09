/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

// -------------------------------------------------------------------------
// -----                    EsbPipe file                                -----
// -----                Created by M. Al-Turany  June 2014             -----
// -------------------------------------------------------------------------

#ifndef PIPE_H
#define PIPE_H

#include "FairModule.h"

class EsbPipe : public FairModule {
  public:
    EsbPipe(const char * name, const char *Title="Esb Pipe");
    EsbPipe();

    virtual ~EsbPipe();
    virtual void ConstructGeometry();
   
  ClassDef(EsbPipe,1) //EsbPIPE

};

#endif //PIPE_H

