
# set the source directory
set(EXAMPLE_DEMO_SRCROOT ${PROJECT_SOURCE_DIR}/examples/demo)

# all source files
set(SRC
	${EXAMPLE_DEMO_SRCROOT}/Main.cpp
)
source_group("" FILES ${SRC})

# add the OTL include paths
include_directories(${PROJECT_SOURCE_DIR}/include)

# for testing. should be removed
include_directories(${PROJECT_SOURCE_DIR}/extlibs/include)

# define the otl-test target
otl_add_project(demo
				SOURCES ${SRC}
				DEPENDS otl-core
				FOLDER "Examples")