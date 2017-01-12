cmake_minimum_required(VERSION 3.6)
project(cgss)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)
set(CMAKE_USE_RELATIVE_PATHS TRUE)
if (${CMAKE_SIZEOF_VOID_P} EQUAL 4)
    set(TARGET_ARCH x86)
elseif (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set(TARGET_ARCH x64)
else ()
    message(FATAL_ERROR "Unknown pointer size: ${CMAKE_SIZEOF_VOID_P}")
endif ()

# Override
#set(TARGET_ARCH x86)

if ("${CMAKE_COMPILER_IS_GNUC_PLUS_PLUS}" OR "${CMAKE_COMPILER_IS_GNUC}" OR "${CYGWIN}" OR "${MINGW}")
    set(GNU_COMPILER 1)
else ()
    set(GNU_COMPILER 0)
endif ()

file(GLOB LIBCGSS_API_FILES "src/lib/*.h")
file(GLOB_RECURSE LIBCGSS_COMMON_HEADER_FILES "src/lib/common/*.h" "src/lib/cdata/*.h")
file(GLOB_RECURSE LIBCGSS_COMMON_SOURCE_FILES "src/lib/common/*.cpp" "src/lib/capi/*.cpp")
file(GLOB_RECURSE LIBCGSS_KS_API_FILES "src/lib/kawashima/*.h")
file(GLOB_RECURSE LIBCGSS_KS_SOURCE_FILES "src/lib/kawashima/*.cpp")
file(GLOB_RECURSE LIBCGSS_TM_API_FILES "src/lib/takamori/*.h")
file(GLOB_RECURSE LIBCGSS_TM_SOURCE_FILES "src/lib/takamori/*.cpp")
set(LIBCGSS_SOURCE_FILES
        ${LIBCGSS_API_FILES} ${LIBCGSS_COMMON_HEADER_FILES} ${LIBCGSS_COMMON_SOURCE_FILES} ${LIBCGSS_KS_API_FILES} ${LIBCGSS_KS_SOURCE_FILES} ${LIBCGSS_TM_API_FILES} ${LIBCGSS_TM_SOURCE_FILES})
set(HCACC_SOURCE_FILES
        src/apps/hcacc/hcacc.cpp src/apps/cgssh.h ${LIBCGSS_API_FILES})
set(HCAENC_SOURCE_FILES
        src/apps/hcaenc/hcaenc.cpp ${LIBCGSS_TM_API_FILES})
set(HCA2WAV_SOURCE_FILES
        src/apps/hca2wav/hca2wav.cpp src/apps/cgssh.h ${LIBCGSS_API_FILES})
set(HCAINFO_SOURCE_FILES
        src/apps/hcainfo/hcainfo.cpp ${LIBCGSS_API_FILES})

set(LIBCGSS_DEF_FILE src/cgss.def)
set(LIBCGSS_SOURCE_FILES ${LIBCGSS_SOURCE_FILES} ${LIBCGSS_DEF_FILE})

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 2)
set(PROJECT_VERSION_TWEAK 0)
set(PROJECT_VERSION_PATCH 0)

if ("${CYGWIN}" OR "${MINGW}")
    # http://stackoverflow.com/questions/12921911/mingw-libgcc-s-sjlj-1-dll-is-missing
    # However, these options will only work if putting them here, not in 'compiler is gnu c' section.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -static")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")
    if ("${CYGWIN}")
        # Force using MinGW even though compiling via Cygwin (please install the packages)
        if (${TARGET_ARCH} STREQUAL "x86")
            set(CMAKE_C_COMPILER /usr/bin/i686-w64-mingw32-gcc)
            set(CMAKE_CXX_COMPILER /usr/bin/i686-w64-mingw32-g++)
        elseif (${TARGET_ARCH} STREQUAL "x64")
            set(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-gcc)
            set(CMAKE_CXX_COMPILER /usr/bin/x86_64-w64-mingw32-g++)
        else ()
            message(FATAL_ERROR "Unrecognized target architecture '${TARGET_ARCH}'.")
        endif ()
    else ()
        # MinGW
    endif ()
    # http://stackoverflow.com/questions/31038963/how-do-you-rename-a-library-filename-in-cmake
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${TARGET_ARCH}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_executable(hcacc ${HCACC_SOURCE_FILES})
add_executable(hcaenc ${HCAENC_SOURCE_FILES})
add_executable(hca2wav ${HCA2WAV_SOURCE_FILES})
add_executable(hcainfo ${HCAINFO_SOURCE_FILES})
add_library(cgss SHARED ${LIBCGSS_SOURCE_FILES})

set_target_properties(cgss
        PROPERTIES
        VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}"
        SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

target_compile_definitions(cgss PRIVATE __CGSS_BUILDING_DLL__)
target_compile_definitions(hcacc PRIVATE __COMPILE_WITH_CGSS_KEYS)
target_compile_definitions(hcaenc PRIVATE __COMPILE_WITH_CGSS_KEYS)
target_compile_definitions(hca2wav PRIVATE __COMPILE_WITH_CGSS_KEYS)

# http://stackoverflow.com/questions/10046114/in-cmake-how-can-i-test-if-the-compiler-is-clang
if (${GNU_COMPILER})
    # using GCC
    # http://stackoverflow.com/questions/10369513/dll-linking-via-windows-cgo-gcc-ld-gives-undefined-reference-to-function-e
    # http://www.willus.com/mingw/yongweiwu_stdcall.html
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,--enable-stdcall-fixup,--add-stdcall-alias")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--enable-stdcall-fixup,--add-stdcall-alias")
    # https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html
    #set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -s)
    #set(CMAKE_STATIC_LINKER_FLAGS ${CMAKE_STATIC_LINKER_FLAGS} -s)
    #set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} -s)
    #set(CMAKE_MODULE_LINKER_FLAGS ${CMAKE_MODULE_LINKER_FLAGS} -s)
elseif (${MSVC})
    # http://stackoverflow.com/questions/34659795/cmake-how-to-add-a-def-file-which-is-not-a-module-definition-file
    # http://stackoverflow.com/questions/18786690/cmake-for-def-and-nodefaultlib
    set_target_properties(cgss
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif ()

if (${WIN32})
    set_target_properties(cgss
            PROPERTIES
            LINK_DEF_FILE_FLAG ${LIBCGSS_DEF_FILE})
endif ()

target_link_libraries(hcacc cgss)
target_link_libraries(hcaenc cgss)
target_link_libraries(hca2wav cgss)
target_link_libraries(hcainfo cgss)

# https://cmake.org/cmake/help/v3.0/command/add_custom_command.html
# http://stackoverflow.com/questions/9994045/copy-target-file-to-another-location-in-a-post-build-step-in-cmake
if (${GNU_COMPILER})
    add_custom_command(TARGET hcacc
            POST_BUILD
            COMMAND ${CMAKE_STRIP} -s $<TARGET_FILE:hcacc>
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    add_custom_command(TARGET hcaenc
            POST_BUILD
            COMMAND ${CMAKE_STRIP} -s $<TARGET_FILE:hcaenc>
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    add_custom_command(TARGET hca2wav
            POST_BUILD
            COMMAND ${CMAKE_STRIP} -s $<TARGET_FILE:hca2wav>
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    add_custom_command(TARGET hcainfo
            POST_BUILD
            COMMAND ${CMAKE_STRIP} -s $<TARGET_FILE:hcainfo>
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    add_custom_command(TARGET cgss
            POST_BUILD
            COMMAND ${CMAKE_STRIP} -s $<TARGET_FILE:cgss>
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif ()
