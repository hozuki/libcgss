cmake_minimum_required(VERSION 3.2)
project(cgss)

include(cmake/common_defines.cmake)

# Override target architecture (auto-detected in common_defines.cmake)
#set(TARGET_ARCH x86)

include(cmake/project_files.cmake)
include(cmake/project_targets.cmake)
include(cmake/project_properties.cmake)

# Override JNI build (disabled by default in jni_support.cmake)
# set(BUILD_JNI_INTERFACE 0)

include(cmake/jni_support.cmake)

include(cmake/project_prebuild.cmake)
include(cmake/project_compile.cmake)
include(cmake/project_link.cmake)
include(cmake/project_postbuild.cmake)
