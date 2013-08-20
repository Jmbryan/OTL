
# include the OTL specific macros
#include(${PROJECT_SOURCE_DIR}/cmake/Macros.cmake)

# let CMake know about our additional library paths (on Windows and OS X)

# add the OTL sources path
#include_directories(${PROJECT_SOURCE_DIR}/src)

# define the path o four additional CMake modules

# set the output directory for OTL libraries
#set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# add the modules subdirectories

#########################################################################

set(INCROOT ${PROJECT_SOURCE_DIR}/include)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

# all source files
set(SRC
	${SRCROOT}/Orbit.cpp
)
source_group("" FILES ${SRC})

add_library(otl STATIC ${SRC})

# set the target's folder (for IDEs that support it, e.g. Visual Studio)
set_target_properties(otl PROPERTIES FOLDER "OTL")