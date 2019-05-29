# The name of our project is “ESBROOT”.  CMakeLists files in this project can
# refer to the root source directory of the project as ${ESBROOT_SOURCE_DIR} 
# or as ${CMAKE_SOURCE_DIR} and to the root binary directory of the project as 
# ${ESBROOT_BINARY_DIR} or ${CMAKE_BINARY_DIR}.
# This difference is important for the base classes which are in FAIRROOT 
# and the experiment part.

#Make Doxygen documentation
#This is at the beginning of this file because we might want to build only
#documentation.

option (BUILD_DOC "Build documentation using Doxygen" OFF)
option (BUILD_ONLY_DOC "Build only the documentation" OFF)

option (WITH_GENIE "Build Genie generator interface" OFF)

if(BUILD_DOC)
	find_package(Doxygen)
	if (DOXYGEN_FOUND)
		#Input and output files
		set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/Doxyfile.in)
		set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/Doxyfile)
		
		#Configure file
		configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
		
		#Create doxygen target
		add_custom_target( doc_doxygen
			COMMAND rm -rf ${CMAKE_BINARY_DIR}/doxygen
			COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
			COMMENT "Generating documentation with Doxygen"
			VERBATIM
		)
	else (DOXYGEN_FOUND)
		message("Doxygen not found" FATAL_ERROR)
	endif (DOXYGEN_FOUND)
	
	if(BUILD_ONLY_DOC)
		message("Will only generate documentation using doxygen.")
		return()
	endif(BUILD_ONLY_DOC)
	
endif(BUILD_DOC)

# Check if cmake has the required version
CMAKE_MINIMUM_REQUIRED(VERSION 3.7.2 FATAL_ERROR)

### CMP0025   Compiler id for Apple Clang is now AppleClang.
### CMP0042   MACOSX_RPATH is enabled by default.

foreach(p
  CMP0025 # CMake 3.0
  CMP0028 # double colon for imported and alias targets
  CMP0042 # CMake 3.0
  CMP0054 # Only interpret ``if()`` arguments as variables or keywords when unquoted.
  )
  if(POLICY ${p})
  cmake_policy(SET ${p} NEW)
  endif()
endforeach()



# Set name of our project to “ESBROOT". Has to be done
# after check of cmake version since this is a new feature                  
project(ESBROOT)

#In case you need Fortran 
#ENABLE_LANGUAGE(Fortran)

# Check for needed environment variables
IF(NOT DEFINED ENV{FAIRROOTPATH})
  MESSAGE(FATAL_ERROR "You did not define the environment variable FAIRROOTPATH which is needed to find FairRoot. Please set this variable and execute cmake again.")
ENDIF(NOT DEFINED ENV{FAIRROOTPATH})

IF(NOT DEFINED ENV{SIMPATH})
   MESSAGE(FATAL_ERROR "You did not define the environment variable SIMPATH which is nedded to find the external packages. Please set this variable and execute cmake again.") 
ENDIF(NOT DEFINED ENV{SIMPATH})

SET(SIMPATH $ENV{SIMPATH})
SET(FAIRROOTPATH $ENV{FAIRROOTPATH})

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ 
# is checked
 set(CMAKE_MODULE_PATH "${FAIRROOTPATH}/share/fairbase/cmake/modules"  ${CMAKE_MODULE_PATH})
  set(CMAKE_MODULE_PATH "${FAIRROOTPATH}/share/fairbase/cmake/modules_old"  ${CMAKE_MODULE_PATH})
  set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

Set(CheckSrcDir "${FAIRROOTPATH}/share/fairbase/cmake/checks")

find_package(FairRoot)

# Load some basic macros which are needed later on
include(FairMacros)
include(WriteConfigFile)
include(CTest)
include(CheckCompiler)
#include(CheckFortran)

#Check the compiler and set the compile and link flags
If(NOT CMAKE_BUILD_TYPE)
  Message("Set BuildType DEBUG")
  set(CMAKE_BUILD_TYPE Debug)
EndIf(NOT CMAKE_BUILD_TYPE)
Check_Compiler()


set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(INCLUDE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/include")
Set(VMCWORKDIR ${CMAKE_SOURCE_DIR})
Option(USE_PATH_INFO "Information from PATH and LD_LIBRARY_PATH are used." OFF)
If(USE_PATH_INFO)
  Set(PATH "$PATH")
  If (APPLE)
    Set(LD_LIBRARY_PATH $ENV{DYLD_LIBRARY_PATH})
  Else (APPLE)
    Set(LD_LIBRARY_PATH $ENV{LD_LIBRARY_PATH})
  EndIf (APPLE)
Else(USE_PATH_INFO)
  STRING(REGEX MATCHALL "[^:]+" PATH $ENV{PATH})
EndIf(USE_PATH_INFO)


# Check if the user wants to build the project in the source
# directory
CHECK_OUT_OF_SOURCE_BUILD()

# Check if we are on an UNIX system. If not stop with an error
# message
IF(NOT UNIX)
  MESSAGE(FATAL_ERROR "You're not on an UNIX system. The project was up to now only tested on UNIX systems, so we break here. If you want to go on please edit the CMakeLists.txt in the source directory.")
ENDIF(NOT UNIX)  

# Check if the external packages are installed into a separate install
# directory
CHECK_EXTERNAL_PACKAGE_INSTALL_DIR()

# searches for needed packages
# REQUIRED means that cmake will stop if this packages are not found
# For example the framework can run without GEANT4, but ROOT is
# mandatory

find_package(ROOT 5.32.00 REQUIRED)
find_package(Pythia8)
find_package(Pythia6 REQUIRED)
find_package(Geant3 REQUIRED)
find_package(Geant4)
find_package(GEANT4)
find_package(GEANT4DATA)
find_package(GEANT4VMC)
find_package(CLHEP)
find_package(HEPMC)

Set(Boost_NO_SYSTEM_PATHS TRUE)
Set(Boost_NO_BOOST_CMAKE TRUE)
If(${ROOT_LIBRARY_DIR} MATCHES /lib/root)
  set(BOOST_ROOT ${SIMPATH})
  set(GSL_DIR ${SIMPATH})
Else(${ROOT_LIBRARY_DIR} MATCHES /lib/root)
  set(BOOST_ROOT ${SIMPATH}/basics/boost)
  set(GSL_DIR ${SIMPATH}/basics/gsl)
EndIf(${ROOT_LIBRARY_DIR} MATCHES /lib/root)
Message("-- Looking for Boost ...")
# If an older version of boost is found both of the variables below are
# cached and in a second cmake run, a good boost version is found even 
# if the version is to old. 
# To overcome this problem both variables are cleared before checking
# for boost.
Unset(Boost_INCLUDE_DIR CACHE)
Unset(Boost_LIBRARY_DIRS CACHE)
find_package(Boost 1.41)
If (Boost_FOUND)
  Set(Boost_Avail 1)
Else (Boost_FOUND)
  Set(Boost_Avail 0)
EndIf (Boost_FOUND)

# set a variable which should be used in all CMakeLists.txt
# Defines all basic include directories from fairbase
SetBasicVariables()

# Set the library version in the main CMakeLists.txt
SET(FAIRROOT_MAJOR_VERSION 0)
SET(FAIRROOT_MINOR_VERSION 0)
SET(FAIRROOT_PATCH_VERSION 0)
SET(FAIRROOT_VERSION "${FAIRROOT_MAJOR_VERSION}.${FAIRROOT_MINOR_VERSION}.${FAIRROOT_PATCH_VERSION}")
SET(FAIRROOT_LIBRARY_PROPERTIES ${FAIRROOT_LIBRARY_PROPERTIES}
    VERSION "${FAIRROOT_VERSION}"
    SOVERSION "${FAIRROOT_MAJOR_VERSION}"
    SUFFIX ".so"
)

#Generate_Version_Info()

SET(_LIBDIR ${CMAKE_BINARY_DIR}/lib)
SET(LD_LIBRARY_PATH  ${_LIBDIR} ${LD_LIBRARY_PATH})

# Check if the compiler support specific C++11 features
# Up to now this is only a check since the code does not use
# any of the features of the new standard
include(CheckCXX11Features)

IF(HAS_CXX11_SHAREDPOINTER)
  Add_Definitions(-DHAS_SHAREDPOINTER)
ENDIF(HAS_CXX11_SHAREDPOINTER)

if(WITH_GENIE)
	message("Searching for Genie")
	
	if(NOT DEFINED ENV{GENIE})
		message(FATAL_ERROR "GENIE environment variable not found")
	endif()
	set(GENIE_DIR $ENV{GENIE})
	
	if(NOT EXISTS ${GENIE_DIR}/lib/libGenie.so)
		message(FATAL_ERROR ${GENIE_DIR}/lib/libGenie.so" not found.")
	endif()
	set(GENIE_LIB_DIR ${GENIE_DIR}/lib/)
	
	if(NOT EXISTS ${GENIE_DIR}/src)
		message(FATAL_ERROR ${GENIE_DIR}/src" not found.")
	endif()
	set(GENIE_INC_DIR ${GENIE_DIR}/src/)
	
	list(APPEND LD_LIBRARY_PATH ${GENIE_LIB_DIR})

	#Definition for rootcling
	list(APPEND DEFINITIONS -DWITH_GENIE)
	
	#Definition for gcc	
	add_definitions(-DWITH_GENIE)
endif(WITH_GENIE)

# Recurse into the given subdirectories.  This does not actually
# cause another cmake executable to run.  The same process will walk through
# the project's entire directory structure.
add_subdirectory (EsbData)
add_subdirectory (EsbGeometry)
add_subdirectory (EsbGenerators)
add_subdirectory (EsbDigitizer)
add_subdirectory (EsbTools)

SET(ROOT_INCLUDE_PATH "${ROOT_INCLUDE_PATH}:${CMAKE_SOURCE_DIR}")
SET(GEOPATH "${CMAKE_SOURCE_DIR}/EsbGeometry")

WRITE_CONFIG_FILE(fairroot_config.sh)
 
configure_file(${CMAKE_SOURCE_DIR}/config.sh.in
               ${CMAKE_BINARY_DIR}/config.sh
              )

