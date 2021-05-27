 # Create a library called "libEsbTools" 

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
#${CMAKE_SOURCE_DIR}/EsbTools
${CMAKE_SOURCE_DIR}
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
#${CMAKE_SOURCE_DIR}/EsbTools
)
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
    PMTubeGeoCreator.cxx
    THStackPlus.cxx
    GenieAnaEventData.cxx
    fiTQunAnaEventData.cxx
)

Set(HEADERS 
    ${CMAKE_SOURCE_DIR}/EsbTools/PMTubeGeoCreator.h
    ${CMAKE_SOURCE_DIR}/EsbTools/THStackPlus.h
    ${CMAKE_SOURCE_DIR}/EsbTools/GenieAnaEventData.h
    ${CMAKE_SOURCE_DIR}/EsbTools/fiTQunAnaEventData.h
)

Set(LINKDEF ${CMAKE_SOURCE_DIR}/EsbTools/ToolsLinkDef.h)
Set(LIBRARY_NAME EsbTools)
Set(DEPENDENCIES EsbGeometry)

GENERATE_LIBRARY()
