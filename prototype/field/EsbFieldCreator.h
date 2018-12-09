/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

// -------------------------------------------------------------------------
// -----                    EsbFieldCreator header file                  -----
// -----                Created 26/03/14  by M. Al-Turany              -----
// -------------------------------------------------------------------------


#ifndef EsbFieldCreator_H
#define EsbFieldCreator_H

#include "FairFieldFactory.h"

class EsbFieldPar;

class FairField;

class EsbFieldCreator : public FairFieldFactory 
{

 public:
  EsbFieldCreator();
  virtual ~EsbFieldCreator();
  virtual FairField* createFairField();
  virtual void SetParm();
  ClassDef(EsbFieldCreator,1);
  
 protected:
  EsbFieldPar* fFieldPar;
  
 private:
  EsbFieldCreator(const EsbFieldCreator&);
  EsbFieldCreator& operator=(const EsbFieldCreator&);

};
#endif //EsbFieldCreator_H
