cmake_minimum_required(VERSION 2.8)

# define a macro that helps defining an option
macro(otl_set_option var default type docstring)
	if(NOT DEFINED ${var})
		set(${var} ${default})
	endif()
	set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

# set a default build type if none was provided
# this has to be done before the project() instruction!
otl_set_option(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")

# enable c++11
set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")

# project name
project(OTL)

# include the configuration file

# setup version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

# add the OTL header path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# add an option for choosing the build type
otl_set_option(BUILD_SHARED_LIBS TRUE BOOL "TRUE to build OTL as shared libraries, FALSE to build it as static libraries")

# add an option for building the examples
otl_set_option(OTL_BUILD_EXAMPLES FALSE BOOL "TRUE to build the OTL examples, FALSE to ignore them")

# add an option fo rbuilding the API documentation
otl_set_option(OTL_BUILD_DOC FALSE BOOL "TRUE to generate the API documentation, FALSE to ignore it")

# Mac OS X specific options
#if(MACOSX)
#endif()

#if(WINDOWS)
#endif()

#if(LINUX)
#endif()

# enable project folders
# set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# set_property(GLBOAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

# add the subdirectories
add_subdirectory(src)
if(OTL_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()
if(OTL_BUILD_DOC)
	add_subdirectory(doc)
endif()

# setup the install rules
#if(NOT OTL_BUILD_FRAMEWORKS)
#	install(DIRECTORY include
#			  DESTINATION .
#			  COMPONENT devel
#			  PATTERN ".svn" EXCLUDE)
#else()
#endif()