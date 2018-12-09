/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef NEWDETECTORCONTFACT_H
#define NEWDETECTORCONTFACT_H

#include "FairContFact.h"

class FairContainer;

class EsbEsbContFact : public FairContFact
{
  private:
    void setAllContainers();
  public:
    EsbEsbContFact();
    ~EsbEsbContFact() {}
    FairParSet* createContainer(FairContainer*);
    ClassDef( EsbEsbContFact,0) // Factory for all EsbEsb parameter containers
};

#endif
