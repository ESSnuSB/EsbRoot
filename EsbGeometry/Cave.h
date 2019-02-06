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


#ifndef ESBROOT_ESBGEOMETRY_CAVE_H
#define ESBROOT_ESBGEOMETRY_CAVE_H

#include "FairModule.h"                 // for FairModule

#include "Rtypes.h"                     // for EsbCave::Class, ClassDef, etc

namespace esbroot {
namespace geometry {

class Cave : public FairModule
{
  public:
    Cave(const char* name, const char* Title="Exp Cave");
    Cave();
    virtual ~Cave();
    virtual void ConstructGeometry();


  private:
    Double_t world[3];
    ClassDef(Cave,2) //PNDCaveSD
};

}
}

#endif //ESBROOT_ESBGEOMETRY_GEOCAVE_H

