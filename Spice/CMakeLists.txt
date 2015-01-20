
if(COMPILER_MSVC)
	add_definitions(-D_COMPLEX_DEFINED -DMSDOS -DOMIT_BLANK_CC -DNON_ANSI_STDIO)
endif()

foreach(flag
		CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
		  CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
		string(REGEX REPLACE "/RTC(su|[1su])" "" ${flag} "${${flag}}")
		if(${flag} MATCHES "/MD")
			string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
		endif()
endforeach()

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-O2")
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS} "-O2")
set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS} "-O2")

# set the include directory
set(SPICE_INCROOT ${PROJECT_SOURCE_DIR}/extlibs/include/cspice)

# set the source directory
set(SPICE_SRCROOT ${PROJECT_SOURCE_DIR}/extlibs/src/cspice)

# all source files
file(GLOB SRC ${SPICE_SRCROOT}/*.c ${SPICE_INCROOT}/*.h)	
source_group("" FILES ${SRC})

include_directories(${SPICE_INCROOT})

# define the cspice target
otl_add_library(cspice
                SOURCES ${SRC}
				FOLDER "Spice")				