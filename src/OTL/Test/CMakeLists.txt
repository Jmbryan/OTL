
# set the include directory
set(TEST_INCROOT ${PROJECT_SOURCE_DIR}/include/OTL/Test)

# set the source directory
set(TEST_SRCROOT ${PROJECT_SOURCE_DIR}/src/OTL/Test)

# set the Catch include directory
#set(CATCHROOT ${PROJECT_SOURCE_DIR}/extlibs/include/Catch)

# set the FakeIt include directory
#set(FAKEITROOT ${PROJECT_SOURCE_DIR}/extlibs/include/FakeIt)

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

# define the otl-test target
otl_add_project(otl-unit-test
				SOURCES ${SRC}
				DEPENDS otl-core
				FOLDER "Tests")