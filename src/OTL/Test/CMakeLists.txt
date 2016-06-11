
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

# add the built-in external libraries paths
include_directories(${PROJECT_SOURCE_DIR}/extlibs/include/FakeIt)

# define the otl-test target
otl_add_project(otl-unit-test
				SOURCES ${SRC}
				DEPENDS otl-core -pthread
				FOLDER "Tests")
