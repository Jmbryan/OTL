
set(SGP4_INCROOT ${PROJECT_SOURCE_DIR}/extlibs/include/libsgp4)
set(SGP4_SRCROOT ${PROJECT_SOURCE_DIR}/extlibs/src/libsgp4)

# all source files
set(SRC
	#${SGP4_SRCROOT}/CoordGeodetic.cpp
	#${SGP4_SRCROOT}/CoordTopocentric.cpp
	${SGP4_SRCROOT}/DateTime.cpp
	${SGP4_SRCROOT}/Eci.cpp
	#${SGP4_SRCROOT}/Globals.cpp
	${SGP4_SRCROOT}/Observer.cpp
	${SGP4_SRCROOT}/OrbitalElements.cpp
	${SGP4_SRCROOT}/SGP4.cpp
	${SGP4_SRCROOT}/SolarPosition.cpp
	#${SGP4_SRCROOT}/TimeSpan.cpp
	${SGP4_SRCROOT}/Tle.cpp
	${SGP4_SRCROOT}/Util.cpp
	#${SGP4_SRCROOT}/Vector.cpp	
)
source_group("" FILES ${SRC})

# all header files
include_directories(${SGP4_INCROOT})

# define the sgp4 target
otl_add_library(sgp4
                SOURCES ${SRC}
				FOLDER "SGP4")		