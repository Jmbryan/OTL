

# find the ephemeris include directory
find_path(EPHEMERIS_INCLUDE_DIR DE405Ephemeris.h)

set(EPHEMERIS_FOUND TRUE) # will be set to false if one of the required modules is not found