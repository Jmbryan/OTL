cmake_minimum_required(VERSION 2.8)

# define a macro that helps defining an option
macro(otl_set_option var default type docstring)
	if(NOT DEFINED ${var})
		set(${var} ${default})
	endif()
	set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

# project name
project(OTL)

# setup version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

# enable c++11 (GCC compilers only)
if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
    #add_definitions(-std=c++11)
endif()
if (MACOSX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=lib++")
endif()

# required to compile spdlog on mingw
if (CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-D_WIN32_WINNT=0x600)
endif()

# include the configuration file
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake)

# set a default build type if none was provided
# this has to be done before the project() instruction!
otl_set_option(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")

# add an option for choosing the build type
otl_set_option(BUILD_SHARED_LIBS FALSE BOOL "TRUE to build OTL as shared libraries, FALSE to build it as static libraries")

# add an option for building the unit tests
otl_set_option(OTL_BUILD_UNIT_TESTS TRUE BOOL "TRUE to build the OTL unit tests, FALSE to ignore them")

# add an option for building the functional tests
otl_set_option(OTL_BUILD_FUNCTIONAL_TESTS FALSE BOOL "TRUE to build the OTL functional tests, FALSE to ignore them")

# add an option for building the examples
otl_set_option(OTL_BUILD_EXAMPLES TRUE BOOL "TRUE to build the OTL examples, FALSE to ignore them")

# add an option for building the API documentation
otl_set_option(OTL_BUILD_DOC FALSE BOOL "TRUE to generate the API documentation, FALSE to ignore it")

# add an option for building the SPICE library
otl_set_option(OTL_BUILD_SPICE FALSE BOOL "TRUE to build spice (experimental), FALSE to ignore it or link externally")

# add an option for building the SGP4 library
otl_set_option(OTL_BUILD_SGP4 FALSE BOOL "TRUE to build sgp4 (experimental), FALSE to ignore it or link externally")

# set install directories
set(CMAKE_INSTALL_PREFIX ./install)
set(INSTALL_MISC_DIR ./)

# Mac OS X specific options
if(MACOSX)
	# add an option to build frameworks instead of dylybs (release only)
	otl_set_option(OTL_BUILD_FRAMEWORKS FALSE BOOL "TRUE to build SFML as frameworks libraries (release only), FALSE to build according to BUILD_SHARED_LIBS")

	# add an option to let the user specify a custom directory for frameworks installation (OTL, SFML, ...)
	otl_set_option(CMAKE_INSTALL_FRAMEWORK_PREFIX "/Library/Frameworks" STRING "Frameworks installation directory")
endif()

# define OTL_STATIC if build type is not set to 'shared'
if(NOT BUILD_SHARED_LIBS)
	add_definitions(-DOTL_STATIC)
endif()

# remove SL security warnings with Visual C++
if(COMPILER_MSVC)
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif()

# set the output directory for OTL libraries
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

# Create outputs in just bin instead of bin/Debug, bin/Release, etc. (Windows only)
if(WINDOWS)
	foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
			string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
			set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin )
			set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib )
			set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib )
	endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
endif()

# define an option for choosing between static and dynamic C runtime (Windows only)
if(WINDOWS)
	otl_set_option(OTL_USE_STATIC_STD_LIBS TRUE BOOL "TRUE to statically link to the standard libraries, FALSE to use them as DLLs")

	# the following combination of flags is not valid
	if(BUILD_SHARED_LIBS AND OTL_USE_STATIC_STD_LIBS)
		message(FATAL_ERROR "BUILD_SHARED_LIBS and OTL_USE_STATIC_STD_LIBS cannot be used together")
	endif()

	# for VC++, we can apply it globally by modifying the compiler flags
	if(COMPILER_MSVC AND OTL_USE_STATIC_STD_LIBS)
		foreach(flag
		        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
				CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
			if(${flag} MATCHES "/MD")
				string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
			endif()
		endforeach()
	endif()
endif()

# Spice options
if (OTL_BUILD_SPICE AND BUILD_SHARED_LIBS)
	message(FATAL_ERROR "OTL_BUILD_SPICE and BUILD_SHARED_LIBS cannot be used together")
elseif(OTL_BUILD_SPICE)
	add_definitions(-DOTL_SPICE)
endif()

# setup Mac OS X stuff
if(MACOSX)
    # OTL_BUILD_FRAMEWORKS needs two things :
    # first, it's available only for release
    #    (because cmake currently doesn't allow specifying a custom framework name so XXX-d is not possible)
    # secondly, it works only with BUILD_SHARED_LIBS enabled
    if(OTL_BUILD_FRAMEWORKS)
        # requirement #1
        if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
            message(FATAL_ERROR "CMAKE_BUILD_TYPE should be \"Release\" when OTL_BUILD_FRAMEWORKS is TRUE")
            return()
        endif()

        # requirement #2
        if(NOT BUILD_SHARED_LIBS)
            message(FATAL_ERROR "BUILD_SHARED_LIBS should be TRUE when OTL_BUILD_FRAMEWORKS is TRUE")
            return()
        endif()
    endif()
endif()

if(LINUX)
	set(PKGCONFIG_DIR lib${LIB_SUFFIX}/pkgconfig)
    if(BUILD_SHARED_LIBS)
        otl_set_option(OTL_INSTALL_PKGCONFIG_FILES FALSE BOOL "TRUE to automatically install pkg-config files so other projects can find OTL")
        if(OTL_INSTALL_PKGCONFIG_FILES)
            foreach(otl_module IN ITEMS otl-core)
                CONFIGURE_FILE(
                    "tools/pkg-config/otl-${otl_module}.pc.in"
                    "tools/pkg-config/otl-${otl_module}.pc"
                    @ONLY)
				INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/tools/pkg-config/otl-${otl_module}.pc"
                    DESTINATION "${CMAKE_INSTALL_PREFIX}/${PKGCONFIG_DIR}")
            endforeach()
        endif()
    else()
        if(OTL_INSTALL_PKGCONFIG_FILES)
            message(WARNING "No pkg-config files are provided for the static SFML libraries (SFML_INSTALL_PKGCONFIG_FILES will be ignored).")
        endif()
    endif()
endif()

# enable project folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

# add the subdirectories
add_subdirectory(src/OTL)
if(OTL_BUILD_SPICE)
	add_subdirectory(spice)
endif()
if(OTL_BUILD_SGP4)
	add_subdirectory(sgp4)
endif()
if(OTL_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()
if(OTL_BUILD_DOC)
	add_subdirectory(doc)
endif()

# setup the install rules
if(NOT OTL_BUILD_FRAMEWORKS)
	install(DIRECTORY include
			DESTINATION .
			COMPONENT devel
			PATTERN ".svn" EXCLUDE)
else()
    # find only "root" headers
    file(GLOB OTL_HEADERS RELATIVE ${PROJECT_SOURCE_DIR} "include/OTL/*")

    # in fact we have to fool cmake to copy all the headers in subdirectories
    # to do that we have to add the "root" headers to the PUBLIC_HEADER
    # then we can run a post script to copy the remaining headers

    # we need a dummy file in order to compile the framework
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp
                       COMMAND touch ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)

    set(OTL_SOURCES ${OTL_HEADERS})
    list(APPEND OTL_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)

    # create OTL.framework
    add_library(OTL ${OTL_SOURCES})

    # edit target properties
    set_target_properties(OTL PROPERTIES
                          FRAMEWORK TRUE
                          FRAMEWORK_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
                          MACOSX_FRAMEWORK_IDENTIFIER org.otl-dev.OTL
                          MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
                          MACOSX_FRAMEWORK_BUNDLE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
                          PUBLIC_HEADER "${OTL_HEADERS}")

    # add the remaining headers
    add_custom_command(TARGET OTL
                       POST_BUILD
                       COMMAND cp -r ${PROJECT_SOURCE_DIR}/include/OTL/* $<TARGET_FILE_DIR:OTL>/Headers)

    # adapt install directory to allow distributing dylibs/frameworks in user�s frameworks/application bundle
    # NOTE : it's not required to link against OTL.framework
    set_target_properties(OTL PROPERTIES
                          BUILD_WITH_INSTALL_RPATH 1
                          INSTALL_NAME_DIR "@rpath")

    # install rule
    install(TARGETS OTL
            FRAMEWORK DESTINATION ${CMAKE_INSTALL_FRAMEWORK_PREFIX}
            COMPONENT devel)
endif()

install(FILES license.txt DESTINATION ${INSTALL_MISC_DIR})
install(FILES README.md DESTINATION ${INSTALL_MISC_DIR})
install(FILES cmake/Modules/FindOTL.cmake DESTINATION ${INSTALL_MISC_DIR}/cmake/Modules)
install(FILES cmake/Modules/FindSPICE.cmake DESTINATION ${INSTALL_MISC_DIR}/cmake/Modules)

# install 3rd-party libraries and tools
if(WINDOWS)

	# install the binaries of SFML dependencies
    if(ARCH_32BITS)
        install(DIRECTORY extlibs/bin/x86/ DESTINATION bin)
        if(COMPILER_MSVC)
            install(DIRECTORY extlibs/libs-msvc/x86/ DESTINATION lib)
        else()
            install(DIRECTORY extlibs/libs-mingw/x86/ DESTINATION lib)
        endif()
    elseif(ARCH_64BITS)
        install(DIRECTORY extlibs/bin/x64/ DESTINATION bin)
        if(COMPILER_MSVC)
            install(DIRECTORY extlibs/libs-msvc/x64/ DESTINATION lib)
        else()
            install(DIRECTORY extlibs/libs-mingw/x64/ DESTINATION lib)
        endif()
    endif()

elseif(MACOSX)

	# install the non-standard frameworks SFML depends on
    #install(DIRECTORY extlibs/libs-osx/Frameworks/sndfile.framework DESTINATION ${CMAKE_INSTALL_FRAMEWORK_PREFIX})
    #install(DIRECTORY extlibs/libs-osx/Frameworks/freetype.framework DESTINATION ${CMAKE_INSTALL_FRAMEWORK_PREFIX})

endif()
