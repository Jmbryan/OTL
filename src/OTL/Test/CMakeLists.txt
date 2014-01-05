
# set the include directory
set(INCROOT ${PROJECT_SOURCE_DIR}/include/OTL/Test)

# set the source directory
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/OTL/Test)

# set the Catch include directory
set(CATCHROOT ${PROJECT_SOURCE_DIR}/extlibs/include/Catch)

# set the Niek-ephem include direction
set (EPHEM_INCROOT ${PROJECT_SOURCE_DIR}/extlibs/include/niek-ephem)
set (EPHEM_SRCROOT ${PROJECT_SOURCE_DIR}/extlibs/src/niek-ephem)

# all source files
set(SRC
	${CATCHROOT}/Catch.hpp
)
source_group("" FILES ${SRC})

# let CMake know about our additional library paths (on Windows and OSX)
if(WINDOWS OR MACOSX)
	set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/include/SFML")
endif()
if(MACOSX)
	set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-osx/Frameworks")
endif()

# find external libraries
find_package(SFML REQUIRED)

# add include paths of external libraries
include_directories(${SFML_INCLUDE_DIR})

# build the list of libraries to link
set(CORE_EXT_LIBS ${SFML_LIBRARY})

# define the otl-test target
otl_add_library(otl-test
                SOURCES ${SRC}
					 EXTERNAL_LIBS ${CORE_EXT_LIBS})