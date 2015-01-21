
# set the source directory
set(CONSOLE_SRCROOT ${PROJECT_SOURCE_DIR}/Console)

# all source files
set(SRC
	${CONSOLE_SRCROOT}/Main.cpp
)
source_group("" FILES ${SRC})

# add the OTL include paths
include_directories(${PROJECT_SOURCE_DIR}/include)

# for testing. should be removed
include_directories(${PROJECT_SOURCE_DIR}/extlibs/include)

# define the otl-test target
otl_add_project(otl-console
				SOURCES ${SRC}
				DEPENDS otl-core
				FOLDER "Console")