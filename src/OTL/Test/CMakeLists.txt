
set(TEST_INCROOT ${PROJECT_SOURCE_DIR}/include/OTL/Test)
set(TEST_SRCROOT ${PROJECT_SOURCE_DIR}/src/OTL/Test)

# all source files
set(SRC
	${TEST_INCROOT}/BaseTest.h
	${TEST_INCROOT}/fakeit_instance.h
	${TEST_SRCROOT}/ConversionTest.cpp
	${TEST_SRCROOT}/LambertTest.cpp
	${TEST_SRCROOT}/Main.cpp
	${TEST_SRCROOT}/PropagatorTest.cpp
	${TEST_SRCROOT}/TimeTest.cpp
)
source_group("" FILES ${SRC})

# add the OTL include paths
include_directories(${PROJECT_SOURCE_DIR}/include)

# add the built-in external libraries paths
include_directories(${OTL_INCLUDE_EXT_LIB_TYPE} ${PROJECT_SOURCE_DIR}/extlibs/include)
include_directories(${OTL_INCLUDE_EXT_LIB_TYPE} ${PROJECT_SOURCE_DIR}/extlibs/include/FakeIt)

# define the otl-test target
otl_add_project(otl-unit-test
				SOURCES ${SRC}
				DEPENDS otl-core
				FOLDER "Tests"
        COVERAGE false)
