
# let CMake know about our additional library paths (on Windows and OS X)
#if(WINDOWS)
#	set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/include")
#elseif(MACOSX)
#	set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/include")
#endif()

# add the OTL sources path
#include_directories(${PROJECT_SOURCE_DIR}/src)

# set the output directory for OTL libraries
#set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

#########################################################################

# set the include directory
set(INCROOT ${PROJECT_SOURCE_DIR}/include)

# set the source directory
set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

# set the catch include directory
set(CATCHROOT ${PROJECT_SOURCE_DIR}/extlibs/include/Catch)

# all source files
set(SRC
	${SRCROOT}/Orbit.cpp
	${INCROOT}/Orbit.hpp
	${INCROOT}/Vector3.hpp
	${CATCHROOT}/Catch.hpp
)
source_group("" FILES ${SRC})

add_library(otl STATIC ${SRC})

# set the target's folder (for IDEs that support it, e.g. Visual Studio)
set_target_properties(otl PROPERTIES FOLDER "OTL")