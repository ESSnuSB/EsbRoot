 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             # 
 #              GNU Lesser General Public Licence (LGPL) version 3,             #  
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# Create a library called "libEsbEsb" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
	${BASE_INCLUDE_DIRECTORIES}
	#put here all directories where header files are located
	${CMAKE_SOURCE_DIR}
	#${CMAKE_SOURCE_DIR}/EsbGeometry
	#${CMAKE_SOURCE_DIR}/EsbData
	#${CMAKE_SOURCE_DIR}/esb
	${CLHEP_INCLUDE_DIR}
    $ENV{GENIE}/src
)

include_directories(${INCLUDE_DIRECTORIES})
include_directories(${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
	${ROOT_LIBRARY_DIR}
	${FAIRROOT_LIBRARY_DIR}
	${CLHEP_LIB_DIR}
    $ENV{GENIE}/lib
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
	#Put here your sourcefiles
	WCDetector.cxx
	Cave.cxx
	GeoCave.cxx
	PMTube.cxx
	FgdDetector.cxx
	EsbSuperFGD/EsbCubeParametrisation.cxx
	EsbSuperFGD/EsbCubeScintilatorConstructor.cxx
	EsbSuperFGD/EsbDetectorConstruction.cxx
	EsbSuperFGD/EsbSuperFGDConstructor.cxx
	EsbSuperFGD/EsbSuperFGDDetectorConstruction.cxx
	EsbSuperFGD/EsbSlabParametrization.cxx
)

Set(HEADERS 
	${CMAKE_SOURCE_DIR}/EsbGeometry/WCDetector.h
	${CMAKE_SOURCE_DIR}/EsbGeometry/Cave.h
	${CMAKE_SOURCE_DIR}/EsbGeometry/GeoCave.h
	${CMAKE_SOURCE_DIR}/EsbGeometry/PMTube.h
	${CMAKE_SOURCE_DIR}/EsbGeometry/FgdDetector.h
	${CMAKE_SOURCE_DIR}/EsbGeometry/EsbSuperFGD/EsbCubeParametrisation.h
	${CMAKE_SOURCE_DIR}/EsbGeometry/EsbSuperFGD/EsbCubeScintilatorConstructor.h
	${CMAKE_SOURCE_DIR}/EsbGeometry/EsbSuperFGD/EsbDetectorConstruction.h
	${CMAKE_SOURCE_DIR}/EsbGeometry/EsbSuperFGD/EsbSuperFGDConstructor.h
	${CMAKE_SOURCE_DIR}/EsbGeometry/EsbSuperFGD/EsbSuperFGDDetectorConstruction.h
	${CMAKE_SOURCE_DIR}/EsbGeometry/EsbSuperFGD/EsbSlabParametrization.h
)

Set(LINKDEF ${CMAKE_SOURCE_DIR}/EsbGeometry/GeometryLinkDef.h)
Set(LIBRARY_NAME EsbGeometry)
Set(DEPENDENCIES
    Base EsbData
)

GENERATE_LIBRARY()
