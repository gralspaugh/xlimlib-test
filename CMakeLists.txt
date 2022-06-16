cmake_minimum_required(VERSION 3.0.0)
project(xlimlib-test VERSION 0.1.0)

set(CMAKE_DEBUG_POSTFIX d)

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_11)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

if(APPLE)
    set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
elseif(UNIX)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif()

# configure a header file to pass the version number only
configure_file(Config.h.in Config.h)

add_subdirectory(XlimlibUtils)

include(CTest)

add_executable(xlimlib-test main.cpp)

set_target_properties(xlimlib-test PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries(xlimlib-test PUBLIC XlimlibUtils)

# add the binary tree to the search path for include files
# so that we will find Config.h
target_include_directories(xlimlib-test PUBLIC
    "${PROJECT_BINARY_DIR}"
)

# add the install targets
install(TARGETS xlimlib-test DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/Config.h"
    DESTINATION include
)

# Testing: does the application run (all that's needed)
add_test(NAME Runs COMMAND xlimlib-test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${XlimlibTest_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${XlimlibTest_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)


# install the configuration targets
install(EXPORT XlimlibUtilsTargets
    FILE XlimlibUtilsTargets.cmake
    DESTINATION lib/cmake/XlimlibUtils
)

include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/XlimlibUtilsConfig.cmake"
    INSTALL_DESTINATION "lib/cmake/example"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
# generate the version file for the config file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/XlimlibUtilsConfigVersion.cmake"
    VERSION "${XlimlibTest_VERSION_MAJOR}.${XlimlibTest_VERSION_MINOR}"
    COMPATIBILITY AnyNewerVersion
)

# install the configuration file
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/XlimlibUtilsConfig.cmake
  DESTINATION lib/cmake/XlimlibUtils
  )

# generate the export targets for the build tree
# needs to be after the install(TARGETS ) command
export(EXPORT XlimlibUtilsTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/XlimlibUtilsTargets.cmake"
)