
# set the include directory
set(INCROOT ${PROJECT_SOURCE_DIR}/include/OTL)

# set the source directory
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/OTL/Core)

# set the catch include directory
set(CATCHROOT ${PROJECT_SOURCE_DIR}/extlibs/include/Catch)

# all source files
set(SRC
	${SRCROOT}/Orbit.cpp
	${INCROOT}/Orbit.hpp
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

# define the otl-core target
otl_add_library(otl-core
                SOURCES ${SRC}
					 EXTERNAL_LIBS ${CORE_EXT_LIBS})