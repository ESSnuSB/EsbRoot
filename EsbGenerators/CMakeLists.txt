# Create a library called "libGen" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
    ${PYTHIA8_INCLUDE_DIR}
    ${SIMPATH}/include
    ${BASE_INCLUDE_DIRECTORIES} 
    ${CMAKE_SOURCE_DIR}
    $ENV{GENIE}/src
)

include_directories(${INCLUDE_DIRECTORIES})
include_directories(${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
    ${ROOT_LIBRARY_DIR}
    ${FAIRROOT_LIBRARY_DIR}
    ${SIMPATH}/lib
    $ENV{GENIE}/lib
)
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
    #Pythia6Generator.cxx
    #Pythia8Generator.cxx
    GenieGenerator.cxx
)

Set(HEADERS 
    ${CMAKE_SOURCE_DIR}/EsbGenerators/GenieGenerator.h
)

Set(LINKDEF ${CMAKE_SOURCE_DIR}/EsbGenerators/GenLinkDef.h)
set(LIBRARY_NAME EsbGen)

#~ set(DEPENDENCIES  Base EsbData pythia8 Pythia6)
set(DEPENDENCIES  Base EsbData Genie)

GENERATE_LIBRARY()
