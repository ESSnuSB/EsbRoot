/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// -------------------------------------------------------------------------
// -----                  M. Al-Turany   June 2014                     -----
// -------------------------------------------------------------------------


#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedef;

#pragma link C++ namespace esbroot::generators;

#ifdef WITH_GENIE
	#pragma link C++ class  esbroot::generators::GenieGenerator+;
	#pragma link C++ class  esbroot::generators::GenieGenerator::GlobalState_t+;
	#pragma link C++ class  esbroot::generators::SimpleGenieGenerator+;

	#pragma link C++ class  esbroot::generators::superfgd::GenieFluxDriver+;
	#pragma link C++ class  esbroot::generators::superfgd::FgdGeomAnalyzer+;
	#pragma link C++ class  esbroot::generators::superfgd::FgdGenieGenerator+;
#endif

#endif
