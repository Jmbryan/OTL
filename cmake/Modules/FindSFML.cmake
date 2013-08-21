# - Find SFML
# Find the native SFML includes and libraries
#
#  SFML_INCLUDE_DIR - Where to find System.hpp, etc.
#  SFML_LIBRARIES   - List of libraries when using SFML.
#  SFML_FOUND       - True if SFML found.

find_path(SFML_INCLUDE_DIR SFML/System.hpp)

find_library(SFML_LIBRARY NAMES sfml-system sfml-system-d sfml-system-s sfml-system-s-d)

set(SFML_FOUND "NO")
if(SFML_INCLUDE_DIR AND SFML_LIBRARY)
	set(SFML_FOUND "YES")
	set(SFML_LIBRARIES ${SFML_LIBRARY})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SFML DEFAULT_MSG SFML_LIBRARY SFML_INCLUDE_DIR)

#mark_as_advanced(SFML_INCLUDE_DIR SFML_LIBRARY)