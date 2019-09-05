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
	$ENV{GENFIT_INCLUDE}
	$ENV{PATHFINDER_INC}
	SYSTEM ${EIGEN3_INCLUDE_DIR}
)

set( $ENV{ROOT_INCLUDE_PATH} ${EIGEN3_INCLUDE_DIR} )

include_directories(${INCLUDE_DIRECTORIES})
include_directories(${SYSTEM_INCLUDE_DIRECTORIES})

FILE(GLOB inc_dirs $ENV{GENFIT_INCLUDE}/*)
FOREACH(child ${inc_dirs})
    	IF(IS_DIRECTORY ${child}/include)
      		include_directories(${child}/include)
    	ENDIF()
ENDFOREACH()

set(LINK_DIRECTORIES
	${ROOT_LIBRARY_DIR}
	${FAIRROOT_LIBRARY_DIR}
	${CLHEP_LIB_DIR}
    $ENV{GENIE}/lib
	$ENV{GENFIT_LIBRARY}
	$ENV{GENFIT_LIBRARY}/lib
	$ENV{PATHFINDER_LIB}
) 

link_directories( ${LINK_DIRECTORIES})

set(SRCS
	EsbSuperFGD/FgdGenFitRecon.cxx
	EsbSuperFGD/FgdMuonOnlyGenFitRecon.cxx
)

Set(HEADERS 
	${CMAKE_SOURCE_DIR}/EsbReconstruction/EsbSuperFGD/FgdGenFitRecon.h
	${CMAKE_SOURCE_DIR}/EsbReconstruction/EsbSuperFGD/FgdMuonOnlyGenFitRecon.h
)

Set(LINKDEF ${CMAKE_SOURCE_DIR}/EsbReconstruction/ReconLinkDef.h)
Set(LIBRARY_NAME EsbReconstruction)
Set(DEPENDENCIES
    Base EsbData EsbGeometry
)

file(GLOB genfit_libs $ENV{GENFIT_LIBRARY}/lib/*)

#Add shared library to be run in root interpreter (macro files)
FOREACH(glib ${genfit_libs})
	message("Adding genfit libs as dependency: ${glib}")
	list(APPEND DEPENDENCIES
    	${glib}
	)
ENDFOREACH()

file(GLOB pathfind_libs $ENV{PATHFINDER_LIB}/*)

#Add shared library to be run in root interpreter (macro files)
FOREACH(pflib ${pathfind_libs})
	message("Adding pathfinder libs as dependency: ${pflib}")
	list(APPEND DEPENDENCIES
    	${pflib}
	)
ENDFOREACH()

GENERATE_LIBRARY()
