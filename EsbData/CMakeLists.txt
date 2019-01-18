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
EsbStack.cxx
EsbMCTrack.cxx
EsbWCDetectorPoint.cxx
)

Set(HEADERS )
Set(LINKDEF EsbDataLinkDef.h)
Set(LIBRARY_NAME EsbData)
Set(DEPENDENCIES Base EG Physics Core)

GENERATE_LIBRARY()
