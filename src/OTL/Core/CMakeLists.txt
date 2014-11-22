
# set the include directory
set(INCROOT ${PROJECT_SOURCE_DIR}/include/OTL/Core)

# set the source directory
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/OTL/Core)

# set the Catch include directory
#set(CATCHROOT ${PROJECT_SOURCE_DIR}/extlibs/include/Catch)

# set the FakeIt include directory
#set(FAKEITROOT ${PROJECT_SOURCE_DIR}/extlibs/include/FakeIt)

# set the Niek-ephem include direction
set (EPHEM_INCROOT ${PROJECT_SOURCE_DIR}/extlibs/include/niek-ephem)
set (EPHEM_SRCROOT ${PROJECT_SOURCE_DIR}/extlibs/src/niek-ephem)

# all source files
set(SRC
	#${INCROOT}/Base.hpp
	${SRCROOT}/Conversion.cpp
	#${INCROOT}/Conversion.hpp
	${SRCROOT}/Epoch.cpp
	#${INCROOT}/Epoch.hpp
	#${INCROOT}/Epoch.inl
	#${INCROOT}/Flyby.hpp
	${SRCROOT}/FlybyUnpowered.cpp
	#${INCROOT}/FlybyUnpowered.hpp
	${SRCROOT}/KeplersEquations.cpp
	#${INCROOT}/KeplersEquations.hpp
	#${INCROOT}/KeplersEquations.inl
	#${INCROOT}/Lambert.hpp
	${SRCROOT}/LambertExponentialSinusoid.cpp
	#${INCROOT}/LambertExponentialSinusoid.hpp
	#${INCROOT}/Matrix3.hpp
	#${INCROOT}/Matrix3.inl
	#${INCROOT}/MGADSMNodes.hpp
	#${INCROOT}/MGADSMNodes.inl
	${SRCROOT}/MGADSMTrajectory.cpp
	#${INCROOT}/MGADSMTrajectory.hpp
	${SRCROOT}/Orbit.cpp
	#${INCROOT}/Orbit.hpp
	#${INCROOT}/Orbit.inl
	#${INCROOT}/OrbitalBody.hpp
	#${INCROOT}/OrbitalBody.inl
	${SRCROOT}/Planet.cpp
	#${INCROOT}/Planet.hpp
	#${INCROOT}/Planet.inl
	#${INCROOT}/Propagate.hpp
	${SRCROOT}/PropagateLagrangian.cpp
	#${INCROOT}/PropagateLagrangian.hpp
	${SRCROOT}/Time.cpp
	#${INCROOT}/Time.hpp
	${SRCROOT}/Transformation.cpp
	#${INCROOT}/Transformation.hpp
	#${INCROOT}/Vector3.hpp
	#${INCROOT}/Vector3.inl
	${EPHEM_SRCROOT}/DE405Ephemeris.cpp
	#${EPHEM_INCROOT}/DE405Ephemeris.h
	${EPHEM_SRCROOT}/DE405EphemerisIO.cpp
	#${EPHEM_INCROOT}/DE405EphemerisIO.h
)
source_group("" FILES ${SRC})

# let CMake know about our additional library paths (on Windows and OSX)
#if(WINDOWS OR MACOSX)
#	set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/include/SFML")
#endif()
#if(MACOSX)
#	set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-osx/Frameworks")
#endif()

# find external libraries
#find_package(SFML REQUIRED)
#find_package(EPHEMERIS REQUIRED)

# add include paths of external libraries
#include_directories(${EXTERNAL_INCLUDE_DIR})
#include_directories(${EPHEMERIS_INCLUDE_DIR})

# build the list of libraries to link
#set(CORE_EXT_LIBS ${SFML_LIBRARY})

# define the otl-core target
otl_add_library(otl-core
                SOURCES ${SRC})
                #EXTERNAL_LIBS ${CORE_EXT_LIBS})