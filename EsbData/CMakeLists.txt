 # Create a library called "libEsbData" 

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${CMAKE_SOURCE_DIR}/EsbData
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
${CMAKE_SOURCE_DIR}/EsbData
)
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
    Stack.cxx
    MCTrack.cxx
    WCDetectorPoint.cxx
)

Set(HEADERS 
    ${CMAKE_SOURCE_DIR}/Stack.h
    ${CMAKE_SOURCE_DIR}/MCTrack.h
    ${CMAKE_SOURCE_DIR}/WCDetectorPoint.h
)

Set(LINKDEF DataLinkDef.h)
Set(LIBRARY_NAME EsbData)
Set(DEPENDENCIES Base EG Physics Core)

GENERATE_LIBRARY()
