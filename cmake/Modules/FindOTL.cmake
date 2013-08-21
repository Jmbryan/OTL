
if(OTL_STATIC_LIBRARIES)
	add_definitions(-DOTL_STATIC)
endif()

# find the OTL include directory
find_path(OTL_INCLUDE_DIR OTL include/Orbit.hpp)

# find the requested modules
set(OTL_FOUND TRUE) # will be set to false if one of the required modules is not found
set(FIND_OTL_LIB_PATHS
    ${OTL_ROOT}
    $ENV{OTL_ROOT}
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt)
    