
# set the include directory
set(INCROOT ${PROJECT_SOURCE_DIR}/include/OTL/Core)

# set the source directory
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/OTL/Core)

# set the Catch include directory
set(CATCHROOT ${PROJECT_SOURCE_DIR}/extlibs/include/Catch)

# set the Jpl_eph include direction
set (JPLEPHROOT ${PROJECT_SOURCE_DIR}/extlibs/include/Jpl_eph)

# all source files
set(SRC
	${INCROOT}/Base.hpp
	${SRCROOT}/Conversion.cpp
	${INCROOT}/Conversion.hpp
	${INCROOT}/Matrix3.hpp
	${INCROOT}/Matrix3.inl
	${SRCROOT}/Orbit.cpp	
	${INCROOT}/Orbit.hpp
	${INCROOT}/Orbit.inl
	${INCROOT}/OrbitalBody.hpp
	${INCROOT}/OrbitalBody.inl
	${SRCROOT}/Planet.cpp	
	${INCROOT}/Planet.hpp
	${INCROOT}/Planet.inl
	${SRCROOT}/Transformation.cpp	
	${INCROOT}/Transformation.hpp
	${INCROOT}/Vector3.hpp
	${INCROOT}/Vector3.inl
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