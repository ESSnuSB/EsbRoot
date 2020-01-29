# Create a library called "libGen" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
    ${PYTHIA8_INCLUDE_DIR}
    ${SIMPATH}/include
    ${BASE_INCLUDE_DIRECTORIES} 
    ${CMAKE_SOURCE_DIR}
)

set(LINK_DIRECTORIES
    ${ROOT_LIBRARY_DIR}
    ${FAIRROOT_LIBRARY_DIR}
    ${SIMPATH}/lib
)
 
set(SRCS
)

set(HEADERS 
)

set(DEPENDENCIES Base EsbData EsbGeometry)

if(WITH_GENIE)
	list(APPEND SRCS
	  GenieGenerator.cxx
		SimpleGenieGenerator.cxx
		GenieNtpFluxV1.cxx
	)	

	list(APPEND HEADERS
    ${CMAKE_SOURCE_DIR}/EsbGenerators/GenieGenerator.h
    ${CMAKE_SOURCE_DIR}/EsbGenerators/SimpleGenieGenerator.h	
    ${CMAKE_SOURCE_DIR}/EsbGenerators/GenieNtpFluxV1.h
	)
	
	#~ message(FATAL_ERROR "Genie include directory "${GENIE_INC_DIR})

	list(APPEND INCLUDE_DIRECTORIES ${GENIE_INC_DIR})
	list(APPEND LINK_DIRECTORIES ${GENIE_LIB_DIR})

	list(APPEND DEPENDENCIES
    Genie
	)
endif(WITH_GENIE)

set(LINKDEF ${CMAKE_SOURCE_DIR}/EsbGenerators/GenLinkDef.h)
set(LIBRARY_NAME EsbGen)

include_directories(${INCLUDE_DIRECTORIES})
include_directories(${SYSTEM_INCLUDE_DIRECTORIES})

link_directories( ${LINK_DIRECTORIES})

GENERATE_LIBRARY()
