
# set the source directory
set(CONSOLE_SRCROOT ${PROJECT_SOURCE_DIR}/Console)

# all source files
set(SRC
	${CONSOLE_SRCROOT}/Main.cpp
)
source_group("" FILES ${SRC})

# define the otl-test target
otl_add_project(otl-console
				SOURCES ${SRC}
				DEPENDS otl-core
				FOLDER "Console")