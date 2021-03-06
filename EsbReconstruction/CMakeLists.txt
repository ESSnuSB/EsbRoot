 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             # 
 #              GNU Lesser General Public Licence (LGPL) version 3,             #  
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# Create a library called "libEsbRecon" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

FIND_PACKAGE(Eigen3 REQUIRED)

set(INCLUDE_DIRECTORIES
	${BASE_INCLUDE_DIRECTORIES}
	#put here all directories where header files are located
	${CMAKE_SOURCE_DIR}
	${CLHEP_INCLUDE_DIR}
    $ENV{GENIE}/src
	${GENFIT_INCLUDE}
	${GENFIT_INCLUDE}/include
	${PATHFINDER_INC}
	SYSTEM ${EIGEN3_INCLUDE_DIR}
)

set( $ENV{ROOT_INCLUDE_PATH} ${EIGEN3_INCLUDE_DIR} )

include_directories(${INCLUDE_DIRECTORIES})
include_directories(${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
	${ROOT_LIBRARY_DIR}
	${FAIRROOT_LIBRARY_DIR}
	${CLHEP_LIB_DIR}
    $ENV{GENIE}/lib
	${GENFIT_LIBRARY}
	${GENFIT_LIBRARY}/lib
	${PATHFINDER_LIB}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
	EsbSuperFGD/FgdGenFitRecon.cxx
	EsbSuperFGD/FgdMCGenFitRecon.cxx
	EsbSuperFGD/FgdMCLeptonStats.cxx
	EsbSuperFGD/FgdMCEventRecord.cxx
	EsbSuperFGD/FgdGraphStats.cxx
	EsbSuperFGD/FgdReconHit.cxx
	EsbSuperFGD/FgdReconTemplate.cxx
	EsbSuperFGD/PdgFromMomentumLoss.cxx
	EsbSuperFGD/FgdMuonRecon.cxx
)

Set(HEADERS 
	${CMAKE_SOURCE_DIR}/EsbReconstruction/EsbSuperFGD/FgdGenFitRecon.h
	${CMAKE_SOURCE_DIR}/EsbReconstruction/EsbSuperFGD/FgdMCGenFitRecon.h
	${CMAKE_SOURCE_DIR}/EsbReconstruction/EsbSuperFGD/FgdMCLeptonStats.h
	${CMAKE_SOURCE_DIR}/EsbReconstruction/EsbSuperFGD/FgdMCEventRecord.h
	${CMAKE_SOURCE_DIR}/EsbReconstruction/EsbSuperFGD/FgdGraphStats.h
	${CMAKE_SOURCE_DIR}/EsbReconstruction/EsbSuperFGD/FgdReconHit.h
	${CMAKE_SOURCE_DIR}/EsbReconstruction/EsbSuperFGD/FgdReconTemplate.h
	${CMAKE_SOURCE_DIR}/EsbReconstruction/EsbSuperFGD/PdgFromMomentumLoss.h
	${CMAKE_SOURCE_DIR}/EsbReconstruction/EsbSuperFGD/FgdMuonRecon.h
)

Set(LINKDEF ${CMAKE_SOURCE_DIR}/EsbReconstruction/ReconLinkDef.h)
Set(LIBRARY_NAME EsbReconstruction)
Set(DEPENDENCIES
    Base EsbData EsbGeometry EsbDigitizer
)

file(GLOB genfit_libs ${GENFIT_LIBRARY}/lib/*.so)

#Add shared library to be run in root interpreter (macro files)
FOREACH(glib ${genfit_libs})
	#message("Adding genfit libs as dependency: ${glib}")
	list(APPEND DEPENDENCIES
    	${glib}
	)
ENDFOREACH()

file(GLOB pathfind_libs ${PATHFINDER_LIB}/*)

#Add shared library to be run in root interpreter (macro files)
FOREACH(pflib ${pathfind_libs})
	#message("Adding pathfinder libs as dependency: ${pflib}")
	list(APPEND DEPENDENCIES
    	${pflib}
	)
ENDFOREACH()

GENERATE_LIBRARY()
