# This script locates the OTL library
# -----------------------------------
#
# Output
# ------
#
# This script defines the following variables:
# OTL_FOUND				true if OTL is found
# OTL_INCLUDE_DIR		OTL include directories (the directory containing the OTL/Core/Base.hpp file)
# OTL_LIBRARY			OTL static/dynamic libs (includes both debug and release names if necessary)
# OTL_LIBRARY_DEBUG		OTL static/dynamic libs (debug only)
# OTL_LIBRARY_RELEASE	OTL static/dynamic libs (release only)
# OTL_INCLUDE_DIRS		list of include directories for OTL and all dependencies
# OTL_LIBRARIES			list of lib directories for OTL and all dependencies
#
# example:
# add_executable(myapp ...)
# find_package(otl-core REQUIRED)
# include_directories(${OTL_INCLUDE_DIRS})
# target_link_libraries(myapp ${OTL_LIBRARIES})

# define the OTL_STATIC macro if static build was chosen
if(OTL_STATIC_LIBRARIES)
	add_definitions(-DOTL_STATIC)
endif()

# deduce the libraries suffix from the options
set(FIND_OTL_LIB_SUFFIX "")
if(OTL_STATIC_LIBRARIES)
    set(FIND_OTL_LIB_SUFFIX "${FIND_OTL_LIB_SUFFIX}-s")
endif()

# find the OTL include directory
find_path(OTL_INCLUDE_DIR OTL/Core/Base.hpp
		  PATH_SUFFIXES include
		  PATHS
		  ${OTL_ROOT}
		  $ENV{OTL_ROOT}
		  ~/Library/Frameworks
		  /Library/Frameworks
		  /usr/local/
		  /usr/
		  /sw			# Fink
		  /opt/local/	# DarwinPorts
		  /opt/csw/		# Blastwave
		  /opt/)

# find the requested modules
set(OTL_FOUND TRUE) # will be set to false if not found
set(FIND_OTL_NAME otl-core-${FIND_OTL_LIB_SUFFIX})
set(FIND_OTL_LIB_PATHS
    ${OTL_ROOT}
    $ENV{OTL_ROOT}
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt)

# find the debug libraries
find_library(OTL_LIBRARY_DEBUG
			 NAMES ${FIND_OTL_NAME}-d
			 PATH_SUFFIXES lib64 lib64
			 PATHES ${FIND_OTL_LIB_PATHS})

# find the release libraries
find_library(OTL_LIBRARY_RELEASE
			 NAMES ${FIND_OTL_NAME}
			 PATH_SUFFIXES lib64 lib64
			 PATHES ${FIND_OTL_LIB_PATHS})

if (OTL_LIBRARY_DEBUG OR OTL_LIBRARY_RELEASE)
	# library found
	set(OTL_FOUND TRUE)
	
	# if both are found, set OTL_LIBRARY to contain both
	if (OTL_LIBRARY_DEBUG AND OTL_LIBRARY_RELEASE)
		set(OTL_LIBRARY debug ${OTL_LIBRARY_DEBUG} release ${OTL_LIBRARY_RELEASE})
	endif()
	
	#if only one debug/release variant is found, set the other to be equal to the found one
	if (OTL_LIBRARY_DEBUG AND NOT OTL_LIBRARY_RELEASE)
		set(OTL_LIBRARY_RELEASE ${OTL_LIBRARY_DEBUG})
		set(OTL_LIBRARY ${OTL_LIBRARY_DEBUG})
	endif()
	if (OTL_LIBRARY_RELEASE AND NOT OTL_LIBRARY_DEBUG)
		set(OTL_LIBRARY_DEBUG ${OTL_LIBRARY_RELEASE})
		set(OTL_LIBRARY ${OTL_LIBRARY_RELEASE})
	endif()
else()	
	#library not found
	set(OTL_FOUND FALSE)
	set(OTL_LIBRARY "")
endif()

# mark as advanced
mark_as_advanced(OTL_INCLUDE_DIR
				 OTL_LIBRARY
				 OTL_LIBRARY_RELEASE
				 OTL_LIBRARY_DEBUG)
				 
# add to the global list of libraries
set(OTL_INCLUDE_DIRS ${OTL_INCLUDE_DIR})
set(OTL_LIBRARIES ${OTL_LIBRARY}) 

# handle success or failure
if(OTL_FOUND)
	message(STATUS "Found OTL in ${OTL_INCLUDE_DIR}")
    #message(STATUS "Found OTL ${OTL_VERSION_MAJOR}.${OTL_VERSION_MINOR} in ${OTL_INCLUDE_DIR}")	
elseif(NOT OTL_FOUND)
	# include directory or library not found
	set(FIND_OTL_ERROR "Could NOT find OTL")
	if (OTL_FIND_REQUIRED)
		# fatal error
		message(FATAL_ERROR ${FIND_OTL_ERROR})
	elseif(NOT OTL_FIND_QUIETLY)
		# error but continue
		message("${FIND_OTL_ERROR}")
	endif()
endif()
