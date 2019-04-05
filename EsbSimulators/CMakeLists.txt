 # Create a library called "libEsbData" 

message(STATUS "Using Genie : $ENV{GENIE}")

set(INCLUDE_DIRECTORIES
    ${BASE_INCLUDE_DIRECTORIES}
    #${CMAKE_SOURCE_DIR}/EsbData
    ${CMAKE_SOURCE_DIR}
    ${CLHEP_INCLUDE_DIR}
    $ENV{GENIE}/src
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
    ${ROOT_LIBRARY_DIR}
    ${FAIRROOT_LIBRARY_DIR}
    #${CMAKE_SOURCE_DIR}/EsbData
    ${CLHEP_LIB_DIR}
    $ENV{GENIE}/lib
)
 
link_directories( ${LINK_DIRECTORIES})

file(GLOB sourceFiles ${CMAKE_SOURCE_DIR}/EsbSimulators/*.cxx)
file(GLOB fgdSourceFiles ${CMAKE_SOURCE_DIR}/EsbSimulators/EsbSuperFGD/*.cxx)

set(SRCS
    ${sourceFiles}
    ${fgdSourceFiles}
)

file(GLOB headerFiles ${CMAKE_SOURCE_DIR}/EsbSimulators/EsbSuperFGD/*.h)
file(GLOB fgdHeaderFiles ${CMAKE_SOURCE_DIR}/EsbSimulators/EsbSuperFGD/*.h)

Set(HEADERS 
    ${headerFiles}
    ${fgdHeaderFiles}
)

Set(LIBRARY_NAME EsbSimulators)
Set(DEPENDENCIES Base EG Physics Core EsbTools)

GENERATE_LIBRARY()
