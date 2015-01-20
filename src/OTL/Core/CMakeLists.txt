
# set the include directory
set(INCROOT ${PROJECT_SOURCE_DIR}/include/OTL/Core)

# set the source directory
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/OTL/Core)

# set the Catch include directory
#set(CATCHROOT ${PROJECT_SOURCE_DIR}/extlibs/include/Catch)

# set the FakeIt include directory
#set(FAKEITROOT ${PROJECT_SOURCE_DIR}/extlibs/include/FakeIt)

# set the Niek-ephem include direction
#set (EPHEM_INCROOT ${PROJECT_SOURCE_DIR}/extlibs/include/niek-ephem)
set (EPHEM_SRCROOT ${PROJECT_SOURCE_DIR}/extlibs/src/niek-ephem)

# all source files
set(SRC
	${INCROOT}/Base.h
	${SRCROOT}/Conversion.cpp
	${INCROOT}/Conversion.h
	${SRCROOT}/Ephemeris.cpp
	${INCROOT}/Ephemeris.h
	${SRCROOT}/Epoch.cpp
	${INCROOT}/Epoch.h
	${INCROOT}/Epoch.inl
	${INCROOT}/Flyby.h
	${SRCROOT}/FlybyUnpowered.cpp
	${INCROOT}/FlybyUnpowered.h
	${SRCROOT}/ForceModel.cpp
	${INCROOT}/ForceModel.h
	${SRCROOT}/JplApproximateEphemeris.cpp
	${INCROOT}/JplApproximateEphemeris.h
	${SRCROOT}/JplEphemeris.cpp
	${INCROOT}/JplEphemeris.h
	${SRCROOT}/KeplersEquations.cpp
	${INCROOT}/KeplersEquations.h
	${INCROOT}/KeplersEquations.inl
	${INCROOT}/Lambert.h
	${SRCROOT}/LambertExponentialSinusoid.cpp
	${INCROOT}/LambertExponentialSinusoid.h
	${INCROOT}/Matrix3.h
	${INCROOT}/Matrix3.inl
	${INCROOT}/MGADSMNodes.h
	${INCROOT}/MGADSMNodes.inl
	${SRCROOT}/MGADSMTrajectory.cpp
	${INCROOT}/MGADSMTrajectory.h
	${SRCROOT}/NaturalBody.cpp
	${INCROOT}/NaturalBody.h
	${SRCROOT}/Orbit.cpp
	${INCROOT}/Orbit.h
	${SRCROOT}/OrbitalBody.cpp	
	${INCROOT}/OrbitalBody.h
	${SRCROOT}/Planet.cpp
	${INCROOT}/Planet.h
	${INCROOT}/Propagate.h
	${SRCROOT}/KeplerianPropagator.cpp
	${INCROOT}/KeplerianPropagator.h
	${SRCROOT}/Time.cpp
	${INCROOT}/Time.h
	${SRCROOT}/Transformation.cpp
	${INCROOT}/Transformation.h
	${INCROOT}/Vector3.h
	${INCROOT}/Vector3.inl
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