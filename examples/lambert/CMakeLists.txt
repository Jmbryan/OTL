
# set the source directory
set(EXAMPLE_LAMBERT_SRCROOT ${PROJECT_SOURCE_DIR}/examples/lambert)

# all source files
set(SRC
	${EXAMPLE_LAMBERT_SRCROOT}/lambert.cpp
)
source_group("" FILES ${SRC})

# add the OTL include paths
include_directories(${PROJECT_SOURCE_DIR}/include)

# for testing. should be removed
include_directories(${OTL_INCLUDE_EXT_LIB_TYPE} ${PROJECT_SOURCE_DIR}/extlibs/include)

# define the otl-test target
otl_add_project(lambert
				SOURCES ${SRC}
				DEPENDS otl-core
				FOLDER "Examples"
				COVERAGE false)
