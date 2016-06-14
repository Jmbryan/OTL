
set(INCROOT ${PROJECT_SOURCE_DIR}/include/OTL/Core)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/OTL/Core)

# all source files
set(SRC
	${INCROOT}/Base.h
	${INCROOT}/Config.h
	${INCROOT}/Constants.h
	${SRCROOT}/Conversion.cpp
	${INCROOT}/Conversion.h
	${SRCROOT}/Ephemeris.cpp
	${INCROOT}/Ephemeris.h
	#${SRCROOT}/EphemerisBody.cpp
	#${INCROOT}/EphemerisBody.h
	${SRCROOT}/Epoch.cpp
	${INCROOT}/Epoch.h
	${SRCROOT}/Exceptions.cpp
	${INCROOT}/Exceptions.h
	${INCROOT}/Export.h
	${INCROOT}/Flyby.h
	#${SRCROOT}/ForceModel.cpp
	#${INCROOT}/ForceModel.h
	${SRCROOT}/JplApproximateBody.cpp
	${INCROOT}/JplApproximateBody.h
	${SRCROOT}/JplApproximateEphemeris.cpp
	${INCROOT}/JplApproximateEphemeris.h
	#${SRCROOT}/JplEphemeris.cpp
	#${INCROOT}/JplEphemeris.h
	${SRCROOT}/KeplerianPropagator.cpp
	${INCROOT}/KeplerianPropagator.h
	${SRCROOT}/KeplersEquations.cpp
	${INCROOT}/KeplersEquations.h
	${SRCROOT}/LagrangianPropagator.cpp
	${INCROOT}/LagrangianPropagator.h
	${INCROOT}/Lambert.h
	${SRCROOT}/LambertExponentialSinusoid.cpp
	${INCROOT}/LambertExponentialSinusoid.h
	${SRCROOT}/Logger.cpp
	${INCROOT}/Logger.h
	${INCROOT}/Matrix.h
	${INCROOT}/MGADSMNodes.h
	${INCROOT}/MGADSMNodes.inl
	${SRCROOT}/MGADSMTrajectory.cpp
	${INCROOT}/MGADSMTrajectory.h
	${SRCROOT}/MpcorbBody.cpp
	${INCROOT}/MpcorbBody.h
	${SRCROOT}/MpcorbEphemeris.cpp
	${INCROOT}/MpcorbEphemeris.h
	${SRCROOT}/Orbit.cpp
	${INCROOT}/Orbit.h
	${SRCROOT}/OrbitalBody.cpp
	${INCROOT}/OrbitalBody.h
	${SRCROOT}/OrbitalElements.cpp
	${INCROOT}/OrbitalElements.h
	${SRCROOT}/PhysicalProperties.cpp
	${INCROOT}/PhysicalProperties.h
	${SRCROOT}/Planet.cpp
	${INCROOT}/Planet.h
	#${SRCROOT}/Propagator.cpp
	#${INCROOT}/Propagator.h
	#${SRCROOT}/Rotation.cpp
	#${INCROOT}/Rotation.h
	${SRCROOT}/StateVector.cpp
	${INCROOT}/StateVector.h
	${SRCROOT}/System.cpp
	${INCROOT}/System.h
	${SRCROOT}/Time.cpp
	${INCROOT}/Time.h
	${SRCROOT}/Transformation.cpp
	${INCROOT}/Transformation.h
	${SRCROOT}/UnpoweredFlyby.cpp
	${INCROOT}/UnpoweredFlyby.h
	${SRCROOT}/UserDefinedBody.cpp
	${INCROOT}/UserDefinedBody.h
	${INCROOT}/Vector.h
)
source_group("" FILES ${SRC})

# add internal files
set(INTERNAL_SRC
	${SRCROOT}/Jpl/JplEphemerisConverter.cpp
	${SRCROOT}/Jpl/JplEphemerisConverter.h
	${SRCROOT}/Jpl/JplApproximateEphemerisIO.cpp
	${SRCROOT}/Jpl/JplApproximateEphemerisIO.h
	${SRCROOT}/Mpcorb/MpcorbEphemerisIO.cpp
	${SRCROOT}/Mpcorb/MpcorbEphemerisIO.h
	${SRCROOT}/Spdlog/LoggerImpl.cpp
	${SRCROOT}/Spdlog/LoggerImpl.h
	${PROJECT_SOURCE_DIR}/extlibs/src/niek-ephem/convert.cpp
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

# add SPICE files
if(OTL_BUILD_SPICE)
	set(SPICE_SRC
		${SRCROOT}/SpiceBody.cpp
		${INCROOT}/SpiceBody.h
		${SRCROOT}/SpiceEphemeris.cpp
		${INCROOT}/SpiceEphemeris.h
	)
	source_group("spice" FILES ${SPICE_SRC})
endif()

# add SGP4 files
if(OTL_BUILD_SGP4)
	set(SGP4_SRC
		${SRCROOT}/TleBody.cpp
		${INCROOT}/TleBody.h
	)
	source_group("sgp4" FILES ${SGP4_SRC})
endif()

# add the OTL include paths
include_directories(${PROJECT_SOURCE_DIR}/include)

# add the built-in external libraries paths
include_directories(${OTL_INCLUDE_EXT_LIB_TYPE} ${PROJECT_SOURCE_DIR}/extlibs/include)
include_directories(${OTL_INCLUDE_EXT_LIB_TYPE} ${PROJECT_SOURCE_DIR}/extlibs/include/niek-ephem)
if(OTL_BUILD_SPICE)
	include_directories(${OTL_INCLUDE_EXT_LIB_TYPE} ${PROJECT_SOURCE_DIR}/extlibs/include/cspice)
endif()
if(OTL_BUILD_SGP4)
	include_directories(${OTL_INCLUDE_EXT_LIB_TYPE} ${PROJECT_SOURCE_DIR}/extlibs/include/libsgp4)
endif()

# add the OTL sources path
include_directories(${PROJECT_SOURCE_DIR}/src)

if(OTL_BUILD_SPICE)
	set(CORE_DEPENDS cspice)
endif()
if(OTL_BUILD_SGP4)
	set(CORE_DEPENDS "${CORE_DEPENDS} sgp4")
endif()

# define the otl-core target
otl_add_library(otl-core
                SOURCES ${SRC} ${INTERNAL_SRC} ${PLATFORM_SRC} ${SPICE_SRC} ${SGP4_SRC}
								FOLDER "OTL"
								DEPENDS ${CORE_DEPENDS}
								COVERAGE true)
