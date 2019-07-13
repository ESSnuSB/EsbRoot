 # Create a library called "libEsbDigitizer" 

set(INCLUDE_DIRECTORIES
    ${BASE_INCLUDE_DIRECTORIES}
    #${CMAKE_SOURCE_DIR}/EsbDigitizer
    ${CMAKE_SOURCE_DIR}
    ${CLHEP_INCLUDE_DIR}
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
    ${ROOT_LIBRARY_DIR}
    ${FAIRROOT_LIBRARY_DIR}
    #${CMAKE_SOURCE_DIR}/EsbDigitizer
    ${CLHEP_LIB_DIR}
)
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
    WCDigitizer.cxx
    EsbSuperFGD/FgdDigitizer.cxx
    EsbSuperFGD/FgdMppcDisplay.cxx
)

Set(HEADERS 
    ${CMAKE_SOURCE_DIR}/EsbDigitizer/WCDigitizer.h
    ${CMAKE_SOURCE_DIR}/EsbDigitizer/EsbSuperFGD/FgdDigitizer.h
    ${CMAKE_SOURCE_DIR}/EsbDigitizer/EsbSuperFGD/FgdMppcDisplay.h
)

Set(LINKDEF ${CMAKE_SOURCE_DIR}/EsbDigitizer/DigitizerLinkDef.h)
Set(LIBRARY_NAME EsbDigitizer)	
Set(DEPENDENCIES EsbData EsbGeometry)

GENERATE_LIBRARY()
