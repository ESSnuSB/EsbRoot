/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// -------------------------------------------------------------------------
// -----                    EsbConstField header file                  -----
// -----                Created 25/03/14  by M. Al-Turany              -----
// -------------------------------------------------------------------------

#include "EsbFieldCreator.h"

#include "EsbFieldPar.h"
#include "EsbConstField.h"

#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairField.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

static EsbFieldCreator gEsbFieldCreator;

EsbFieldCreator::EsbFieldCreator()
  :FairFieldFactory(),
   fFieldPar(NULL)
{
	fCreator=this;
}

EsbFieldCreator::~EsbFieldCreator()
{
}

void EsbFieldCreator::SetParm()
{
  FairRunAna *Run = FairRunAna::Instance();
  FairRuntimeDb *RunDB = Run->GetRuntimeDb();
  fFieldPar = (EsbFieldPar*) RunDB->getContainer("EsbFieldPar");

}

FairField* EsbFieldCreator::createFairField()
{ 
  FairField *fMagneticField=0;

  if ( ! fFieldPar ) {
    cerr << "-E-  No field parameters available!"
	 << endl;
  }else{
	// Instantiate correct field type
	Int_t fType = fFieldPar->GetType();
	if      ( fType == 0 ) fMagneticField = new EsbConstField(fFieldPar);
    else cerr << "-W- FairRunAna::GetField: Unknown field type " << fType
		<< endl;
	cout << "New field at " << fMagneticField << ", type " << fType << endl;
	// Initialise field
	if ( fMagneticField ) {
		fMagneticField->Init();
		fMagneticField->Print("");
	}
  }
  return fMagneticField;
}


ClassImp(EsbFieldCreator)
