# This script locates the SPICE c library
# -----------------------------------
#
# Output
# ------
#
# This script defines the following variables:
# SPICE_FOUND				true if SPICE is found
# SPICE_INCLUDE_DIR			SPICE include directories (the directory containing the SpiceUsr.h file)
# SPICE_LIBRARY				SPICE static/dynamic libs (includes both debug and release names if necessary)
# SPICE_LIBRARY_DEBUG		SPICE static/dynamic libs (debug only)
# SPICE_LIBRARY_RELEASE		SPICE static/dynamic libs (release only)
# SPICE_INCLUDE_DIRS		list of include directories for SPICE and all dependencies
# SPICE_LIBRARIES			list of lib directories for SPICE and all dependencies
#
# example:
# add_executable(myapp ...)
# find_package(cspice REQUIRED)
# include_directories(${SPICE_INCLUDE_DIRS})
# target_link_libraries(myapp ${SPICE_LIBRARIES})

# define the SPICE_STATIC macro if static build was chosen
if(OTL_STATIC_LIBRARIES)
	add_definitions(-DOTL_STATIC)
endif()

# define the list of search paths for headers and libraries
set(FIND_OTL_PATHS
    ${OTL_ROOT}/extlibs/include
    $ENV{OTL_ROOT}
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt)

# deduce the libraries suffix from the options
set(FIND_SPICE_LIB_SUFFIX "")
if(OTL_STATIC_LIBRARIES)
    set(FIND_SPICE_LIB_SUFFIX "${FIND_SPICE_LIB_SUFFIX}-s")
endif()

# find the SPICE include directory
find_path(SPICE_INCLUDE_DIR cspice/SpiceUsr.h
		  PATH_SUFFIXES include
		  PATHS ${FIND_OTL_PATHS})

# find the requested modules
set(SPICE_FOUND TRUE) # will be set to false if not found
set(FIND_SPICE_NAME cspice-${FIND_SPICE_LIB_SUFFIX})
set(FIND_SPICE_LIB_PATHS
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
find_library(SPICE_LIBRARY_DEBUG
			 NAMES ${FIND_SPICE_NAME}-d
			 PATH_SUFFIXES lib64 lib64
			 PATHES ${FIND_SPICE_LIB_PATHS})

# find the release libraries
find_library(SPICE_LIBRARY_RELEASE
			 NAMES ${FIND_SPICE_NAME}
			 PATH_SUFFIXES lib64 lib64
			 PATHES ${FIND_SPICE_LIB_PATHS})

if (SPICE_LIBRARY_DEBUG OR SPICE_LIBRARY_RELEASE)
	# library found
	set(SPICE_FOUND TRUE)
	
	# if both are found, set SPICE_LIBRARY to contain both
	if (SPICE_LIBRARY_DEBUG AND SPICE_LIBRARY_RELEASE)
		set(SPICE_LIBRARY debug ${SPICE_LIBRARY_DEBUG} release ${SPICE_LIBRARY_RELEASE})
	endif()
	
	#if only one debug/release variant is found, set the other to be equal to the found one
	if (SPICE_LIBRARY_DEBUG AND NOT SPICE_LIBRARY_RELEASE)
		set(SPICE_LIBRARY_RELEASE ${SPICE_LIBRARY_DEBUG})
		set(SPICE_LIBRARY ${SPICE_LIBRARY_DEBUG})
	endif()
	if (SPICE_LIBRARY_RELEASE AND NOT SPICE_LIBRARY_DEBUG)
		set(SPICE_LIBRARY_DEBUG ${SPICE_LIBRARY_RELEASE})
		set(SPICE_LIBRARY ${SPICE_LIBRARY_RELEASE})
	endif()
else()	
	#library not found
	set(SPICE_FOUND FALSE)
	set(SPICE_LIBRARY "")
endif()

# mark as advanced
mark_as_advanced(SPICE_INCLUDE_DIR
				 SPICE_LIBRARY
				 SPICE_LIBRARY_RELEASE
				 SPICE_LIBRARY_DEBUG)
				 
# add to the global list of libraries
set(SPICE_INCLUDE_DIRS ${SPICE_INCLUDE_DIR})
set(SPICE_LIBRARIES ${SPICE_LIBRARY}) 

# handle success or failure
if(SPICE_FOUND)
	message(STATUS "Found SPICE in ${SPICE_INCLUDE_DIR}")
    #message(STATUS "Found SPICE ${SPICE_VERSION_MAJOR}.${SPICE_VERSION_MINOR} in ${SPICE_INCLUDE_DIR}")	
elseif(NOT SPICE_FOUND)
	# include directory or library not found
	set(FIND_SPICE_ERROR "Could NOT find SPICE")
	if (SPICE_FIND_REQUIRED)
		# fatal error
		message(FATAL_ERROR ${FIND_SPICE_ERROR})
	elseif(NOT SPICE_FIND_QUIETLY)
		# error but continue
		message("${FIND_SPICE_ERROR}")
	endif()
endif()
