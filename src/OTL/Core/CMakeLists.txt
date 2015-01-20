
# set the include directory
set(INCROOT ${PROJECT_SOURCE_DIR}/include/OTL/Core)

# set the source directory
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/OTL/Core)

# set the Catch include directory
#set(CATCHROOT ${PROJECT_SOURCE_DIR}/extlibs/include/Catch)

# set the FakeIt include directory
#set(FAKEITROOT ${PROJECT_SOURCE_DIR}/extlibs/include/FakeIt)

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
	${SRCROOT}/Exceptions.cpp	
	${INCROOT}/Exceptions.h
	${INCROOT}/Flyby.h
	${SRCROOT}/FlybyUnpowered.cpp
	${INCROOT}/FlybyUnpowered.h
	${SRCROOT}/ForceModel.cpp
	${INCROOT}/ForceModel.h
	${SRCROOT}/JplApproximateEphemeris.cpp
	${INCROOT}/JplApproximateEphemeris.h
	${SRCROOT}/JplEphemeris.cpp
	${INCROOT}/JplEphemeris.h
	${SRCROOT}/KeplerianPropagator.cpp
	${INCROOT}/KeplerianPropagator.h
	${SRCROOT}/KeplersEquations.cpp
	${INCROOT}/KeplersEquations.h
	${INCROOT}/KeplersEquations.inl
	${INCROOT}/Lambert.h
	${SRCROOT}/LambertExponentialSinusoid.cpp
	${INCROOT}/LambertExponentialSinusoid.h
	${SRCROOT}/Logger.cpp
	${INCROOT}/Logger.h
	${INCROOT}/Matrix.h
	${INCROOT}/Matrix.inl
	${INCROOT}/MGADSMNodes.h
	${INCROOT}/MGADSMNodes.inl
	${SRCROOT}/MGADSMTrajectory.cpp
	${INCROOT}/MGADSMTrajectory.h
	${SRCROOT}/MpcorbEphemeris.cpp
	${INCROOT}/MpcorbEphemeris.h
	${SRCROOT}/NaturalBody.cpp
	${INCROOT}/NaturalBody.h
	${SRCROOT}/Orbit.cpp
	${INCROOT}/Orbit.h
	${SRCROOT}/OrbitalBody.cpp	
	${INCROOT}/OrbitalBody.h
	${SRCROOT}/OrbitalElements.cpp
	${INCROOT}/OrbitalElements.h
	${SRCROOT}/Planet.cpp
	${INCROOT}/Planet.h
	${SRCROOT}/Propagator.cpp
	${INCROOT}/Propagator.h
	${SRCROOT}/SpiceEphemeris.cpp
	${INCROOT}/SpiceEphemeris.h
	${SRCROOT}/StateVector.cpp
	${INCROOT}/StateVector.h
	${SRCROOT}/System.cpp
	${INCROOT}/System.h
	${SRCROOT}/Time.cpp
	${INCROOT}/Time.h
	${SRCROOT}/Transformation.cpp
	${INCROOT}/Transformation.h
)
source_group("" FILES ${SRC})

# add internal files
set(INTERNAL_SRC
	${SRCROOT}/Jpl/JplApproximateEphemerisIO.cpp
	${SRCROOT}/Jpl/JplApproximateEphemerisIO.h
	${SRCROOT}/Mpcorb/MpcorbEphemerisIO.cpp
	${SRCROOT}/Mpcorb/MpcorbEphemerisIO.h
	${SRCROOT}/Spdlog/LoggerImpl.cpp
	${SRCROOT}/Spdlog/LoggerImpl.h
	${PROJECT_SOURCE_DIR}/extlibs/src/niek-ephem/DE405Ephemeris.cpp
	${PROJECT_SOURCE_DIR}/extlibs/include/niek-ephem/DE405Ephemeris.h
	${PROJECT_SOURCE_DIR}/extlibs/src/niek-ephem/DE405EphemerisIO.cpp
	${PROJECT_SOURCE_DIR}/extlibs/include/niek-ephem/DE405EphemerisIO.h
)
source_group("internal" FILES ${INTERNAL_SRC})

# add platform specific sources
if(WINDOWS)
	set (PLATFORM_SRC
		${SRCROOT}/Win32/SystemImpl.cpp
		${SRCROOT}/Win32/SystemImpl.h
	)		
	source_group("windows" FILES ${PLATFORM_SRC})
else()
	set (PLATFORM_SRC
		${SRCROOT}/Unix/SystemImpl.cpp
		${SRCROOT}/Unix/SystemImpl.h
	)
	source_group("unix" FILES ${PLATFORM_SRC})
endif()


	
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
                SOURCES ${SRC} ${INTERNAL_SRC} ${PLATFORM_SRC}
				FOLDER "OTL")
                #EXTERNAL_LIBS ${CORE_EXT_LIBS})