cmake_minimum_required(VERSION 3.2)
project(cgss)

include(cmake/common_defines.cmake)

# Override target architecture (auto-detected in common_defines.cmake)
#set(TARGET_ARCH x86)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${TARGET_ARCH}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

include(cmake/project_options.cmake)

include(cmake/project_files.cmake)
include(cmake/project_targets.cmake)
include(cmake/project_properties.cmake)

include(cmake/project_prebuild.cmake)
include(cmake/project_compile.cmake)
include(cmake/project_link.cmake)
include(cmake/project_postbuild.cmake)

# Override JNI build (disabled by default in jni_support.cmake)
#set(CGSS_BUILD_JNI_LIBRARY ON)

include(cmake/jni_support.cmake)

add_subdirectory(lib/vgaudio-cpp)
