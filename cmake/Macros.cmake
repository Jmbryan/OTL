include(CMakeParseArguments)


# add a new target which is a OTL library
# ex: otl_add_library(otl-core
#                     SOURCES Orbit.cpp Orbit.h ...
#                     DEPENDS cspice ...
#                     EXTERNAL_LIBS ...
#                     FOLDER "OTL"
#                     COVERAGE true)
macro(otl_add_library target)

    # parse the arguments
    cmake_parse_arguments(THIS "" "" "SOURCES;DEPENDS;EXTERNAL_LIBS;FOLDER;COVERAGE" ${ARGN})

    # create the target
    add_library(${target} ${THIS_SOURCES})
    if(THIS_COVERAGE AND OTL_ENABLE_CODE_COVERAGE)
      add_coverage_target(${target})
    endif()

    # define the export symbol of the module
    string(REPLACE "-" "_" NAME_UPPER "${target}")
    string(TOUPPER "${NAME_UPPER}" NAME_UPPER)
    set_target_properties(${target} PROPERTIES DEFINE_SYMBOL ${NAME_UPPER}_EXPORTS)

    # adjust the output file prefix/suffix to match our conventions
    if(BUILD_SHARED_LIBS)
        if(WINDOWS)
            # include the major version number in Windows shared library names (but not import library names)
            set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -d)
            set_target_properties(${target} PROPERTIES SUFFIX "-${VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX}")
        else()
            set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -d)
        endif()
        if (WINDOWS AND COMPILER_GCC)
            # on Windows/gcc get rid of "lib" prefix for shared libraries,
            # and transform the ".dll.a" suffix into ".a" for import libraries
            set_target_properties(${target} PROPERTIES PREFIX "")
            set_target_properties(${target} PROPERTIES IMPORT_SUFFIX ".a")
        endif()
    else()
        set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -s-d)
        set_target_properties(${target} PROPERTIES RELEASE_POSTFIX -s)
        set_target_properties(${target} PROPERTIES MINSIZEREL_POSTFIX -s)
    endif()

    # set the version and soversion of the target (for compatible systems -- mostly Linuxes)
    set_target_properties(${target} PROPERTIES SOVERSION ${VERSION_MAJOR})
    set_target_properties(${target} PROPERTIES VERSION ${VERSION_MAJOR}.${VERSION_MINOR})

    # set the target's folder (for IDEs that support it, e.g. Visual Studio)
    set_target_properties(${target} PROPERTIES FOLDER ${THIS_FOLDER})

    # for gcc >= 4.0 on Windows, apply the OTL_USE_STATIC_STD_LIBS option if it is enabled
    if(WINDOWS AND COMPILER_GCC AND NOT GCC_VERSION VERSION_LESS "4")
        if(OTL_USE_STATIC_STD_LIBS)
            set_target_properties(${target} PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
        endif()
    endif()

    # if using gcc >= 4.0 or clang >= 3.0 on a non-Windows platform, we must hide public symbols by default
    # (exported ones are explicitly marked)
    if(NOT WINDOWS AND ((COMPILER_GCC AND NOT GCC_VERSION VERSION_LESS "4") OR (COMPILER_CLANG AND NOT CLANG_VERSION VERSION_LESS "3")))
        set_target_properties(${target} PROPERTIES COMPILE_FLAGS -fvisibility=hidden)
    endif()

    # link the target to its OTL dependencies
    if(THIS_DEPENDS)
        target_link_libraries(${target} ${THIS_DEPENDS})
    endif()

    # build frameworks or dylibs
    if(MACOSX AND BUILD_SHARED_LIBS)
        if(OTL_BUILD_FRAMEWORKS)
            # adapt target to build frameworks instead of dylibs
            set_target_properties(${target} PROPERTIES
                                  FRAMEWORK TRUE
                                  FRAMEWORK_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
                                  MACOSX_FRAMEWORK_IDENTIFIER org.otl-dev.${target}
                                  MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
                                  MACOSX_FRAMEWORK_BUNDLE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
        endif()

        # adapt install directory to allow distributing dylibs/frameworks in user’s frameworks/application bundle
        set_target_properties(${target} PROPERTIES
                              BUILD_WITH_INSTALL_RPATH 1
                              INSTALL_NAME_DIR "@rpath")
    endif()

    # link the target to its external dependencies
    if(THIS_EXTERNAL_LIBS)
        target_link_libraries(${target} ${THIS_EXTERNAL_LIBS})
    endif()

    # add the install rule
    install(TARGETS ${target}
            RUNTIME DESTINATION bin COMPONENT bin
            LIBRARY DESTINATION lib${LIB_SUFFIX} COMPONENT bin
            ARCHIVE DESTINATION lib${LIB_SUFFIX} COMPONENT devel
            FRAMEWORK DESTINATION ${CMAKE_INSTALL_FRAMEWORK_PREFIX} COMPONENT bin)
endmacro()

# add a new target which is a OTL project
# ex: otl_add_project(unit-tests
#                     SOURCES main.cpp ...
#                     DEPENDS otl-core
#                     FOLDER "Tests"
#                     COVERAGE true)
macro(otl_add_project target)

    # parse the arguments
    cmake_parse_arguments(THIS "" "" "SOURCES;DEPENDS;FOLDER;COVERAGE" ${ARGN})

    # set a source group for the source files
    source_group("" FILES ${THIS_SOURCES})

    # create the target
    add_executable(${target} ${THIS_SOURCES})
    if(THIS_COVERAGE AND OTL_ENABLE_CODE_COVERAGE)
      add_coverage_target(${target})
    endif()

    # set the debug suffix
    set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -d)

    # set the target's folder (for IDEs that support it, e.g. Visual Studio)
    set_target_properties(${target} PROPERTIES FOLDER ${THIS_FOLDER})

    # for gcc >= 4.0 on Windows, apply the OTL_USE_STATIC_STD_LIBS option if it is enabled
    if(WINDOWS AND COMPILER_GCC AND NOT GCC_VERSION VERSION_LESS "4")
        if(OTL_USE_STATIC_STD_LIBS)
            set_target_properties(${target} PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
        endif()
    endif()

    # link the target to its OTL dependencies
    if(THIS_DEPENDS)
        target_link_libraries(${target} ${THIS_DEPENDS})
    endif()

    # add the install rule
    install(TARGETS ${target}
            RUNTIME DESTINATION ${INSTALL_MISC_DIR}/examples/${target} COMPONENT examples
            BUNDLE DESTINATION ${INSTALL_MISC_DIR}/examples/${target} COMPONENT examples)

    # install the example's source code
    #install(FILES ${THIS_SOURCES}
    #        DESTINATION ${INSTALL_MISC_DIR}/examples/${target} COMPONENT examples)

  #install(FILES ${THIS_SOURCES}
  #    DESTINATION bin COMPONENT bin)
endmacro()

# global gcov target
if (NOT TARGET gcov)
   add_custom_target(gcov)
endif()

# add a target for code coverage
function(add_coverage_target target)
  message("Adding coverage target ${target}")

  # find the gcov binary
  if(NOT GCOV_EXE)
    get_filename_component(COMPILER_PATH "${CMAKE_CXX_COMPILER}" PATH)
    string(REGEX MATCH "^[0-9]+" GCC_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
    find_program(GCOV_EXE NAMES gcov-${GCC_VERSION} gcov HINTS ${COMPILER_PATH})
  endif()
  if(NOT GCOV_EXE)
    message(WARNING "Could not find gcov binary for compiler ${CMAKE_CXX_COMPILER}")
    return()
  endif()

  # get all files for this target
  get_target_property(target_files ${target} SOURCES)

  set(SOURCES "")
  set(ADDITIONAL_CLEAN_FILES "")
  foreach(FILE ${target_files})
    # extract the (lowercase) extension e.g. "cpp"
    get_filename_component(FILE_EXT "${FILE}" EXT)
    string(TOLOWER "${FILE_EXT}" FILE_EXT)
    string(SUBSTRING "${FILE_EXT}" 1 -1 FILE_EXT)

    # Filter out any non-source files e.g. header files
    list(FIND CMAKE_CXX_SOURCE_FILE_EXTENSIONS "${FILE_EXT}" TEMP)
    if (NOT ${TEMP} EQUAL -1)
      # Change the path to be relative to the object file directory
      if(IS_ABSOLUTE ${FILE})
        file(RELATIVE_PATH FILE ${CMAKE_CURRENT_SOURCE_DIR} ${FILE})
      endif()
      string(REPLACE ".." "__" FILE "${FILE}")
      string(CONCAT FILE "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${target}.dir/" "${FILE}")

      list(APPEND SOURCES "${FILE}")
      list(APPEND ADDITIONAL_CLEAN_FILES "${FILE}.gcno")
      list(APPEND ADDITIONAL_CLEAN_FILES "${FILE}.gcda")
    endif()
  endforeach()

  # add the intermediate coverage files to clean target
  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${ADDITIONAL_CLEAN_FILES}")

  # redirect the output to null depending on platform
  set(GCOV_NUL /dev/null)
  if(WIN32)
    set(GCOV_NUL NUL)
  endif()

  # Call gcov on the generated .gcno file for each source file
  set(target_depends "")
  foreach(FILE ${SOURCES})
    get_filename_component(FILE_PATH "${FILE}" PATH)

    # call gcov command
    add_custom_command(OUTPUT ${FILE}.gcov
      COMMAND ${GCOV_EXE} ${FILE}.gcno > ${GCOV_NUL}
      DEPENDS ${target} ${FILE}.gcno
      WORKING_DIRECTORY ${FILE_PATH})

    list(APPEND target_depends ${FILE}.gcov)
  endforeach()

  # add this coverage target and add to global gcov target
  add_custom_target(${target}-gcov DEPENDS ${target_depends})
  add_dependencies(gcov ${target}-gcov)
endfunction()
