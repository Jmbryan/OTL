
# include the OTL specific macros
include(${PROJECT_SOURCE_DIR}/cmake/Macros.cmake)

# let CMake know about our additional library paths (on Windows and OS X)
if(WINDOWS)
	set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/include")
	if(COMPILER_GCC)
		if(ARCH_32BITS)
			set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-mingw/x86")
			set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/bin-mingw/x86")
		elseif(ARCH_64BITS)
			set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-mingw/x64")
			set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/bin-mingw/x64")
		endif()
	elseif(COMPILER_MSVC)
		if(ARCH_32BITS)
			set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-msvc/x86")
		elseif(ARCH_64BITS)
			set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-msvc/x64")
		endif()
	endif()
elseif(MACOSX)
	set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/include")
	set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-osx/lib")
endif()

# add the OTL include paths
include_directories(${PROJECT_SOURCE_DIR}/include)

# add the built-in external libraries paths
include_directories(${PROJECT_SOURCE_DIR}/extlibs/include)

# define the path of our additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules/")

# set the output directory for OTL libraries
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# add the modules subdirectories
add_subdirectory(Core)
if (OTL_BUILD_UNIT_TESTS OR OTL_BUILD_FUNCTIONAL_TESTS)
	add_subdirectory(Test)
endif()
