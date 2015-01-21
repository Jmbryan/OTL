
set(SPICE_INCROOT ${PROJECT_SOURCE_DIR}/extlibs/include/cspice)
set(SPICE_SRCROOT ${PROJECT_SOURCE_DIR}/extlibs/src/cspice)

# all source files
file(GLOB SRC ${SPICE_SRCROOT}/*.c ${SPICE_INCROOT}/*.h)	
source_group("" FILES ${SRC})

# all header files
include_directories(${SPICE_INCROOT})

# add the compiler definitions
if(COMPILER_MSVC)
	add_definitions(-D_COMPLEX_DEFINED -DMSDOS -DOMIT_BLANK_CC -DNON_ANSI_STDIO)
endif()

# SPICE must be compiled as a static library
# disable runtime checks, set optimization level to O2 (max speed),
# and disable warnings

#foreach(flag
#		CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
#		CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
#	if(${flag} MATCHES "/MD")
#		string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
#	endif()
#	if(${flag} MATCHES "/RTC(su|[1su])")
#		string(REGEX REPLACE "/RTC(su|[1su])" "" ${flag} "${${flag}}")
#	endif()
#	set(${flag} "${flag} /O2")
#	set(${flag} "${flag} /W0")
#endforeach()

# regex replace for /MD doesn't work for some reason,
# so set the flags manually. Should be safe to overwrite
# existing flags.
set(CMAKE_C_FLAGS_DEBUG 			"/MTd /O2 /W0")
set(CMAKE_C_FLAGS_RELEASE 			"/MT /O2 /W0")
set(CMAKE_C_FLAGS_MINSIZEREL 		"/MT /O2 /W0")
set(CMAKE_C_FLAGS_RELWITHDEBINFO 	"/MT /O2 /W0")

# according to the SPICE documentation:
# "the optimization algorithm has a very tough time with zzsecptr.c,
# so exempt this routine from optimization."
set_source_files_properties(${SPICE_SRCROOT}/zzsecprt.c PROPERTIES COMPILE_FLAGS /Od)

# define the cspice target
otl_add_library(cspice
                SOURCES ${SRC}
				FOLDER "Spice")				