cmake_minimum_required(VERSION 3.3)
project(cgss)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(TARGET_ARCH x64)
#set(TARGET_ARCH x86)

set(POLYFILL_SOURCE_FILES
        src/polyfill/io.h src/polyfill/io.cpp src/polyfill/dl.h src/polyfill/dl.cpp src/polyfill/polyfill.h)
set(LIBCGSS_SOURCE_FILES
        # kawashima
        src/lib/kawashima/kstypedef.h src/lib/kawashima/hca/CHcaDecoder.h src/lib/kawashima/hca/CHcaDecoder.cpp src/lib/kawashima/hca/HcaNative.h
        src/lib/kawashima/hca/internal/CHcaCipher.cpp src/lib/kawashima/hca/internal/CHcaCipher.h src/lib/kawashima/hca/internal/CHcaAth.cpp
        src/lib/kawashima/hca/internal/CHcaAth.h src/lib/kawashima/hca/internal/CHcaData.cpp src/lib/kawashima/hca/internal/CHcaData.h
        src/lib/kawashima/hca/internal/HcaChannel.cpp src/lib/kawashima/hca/internal/HcaChannel.h src/lib/kawashima/hca/Magic.cpp
        src/lib/kawashima/hca/Magic.h src/lib/kawashima/hca/WaveGen.h src/lib/kawashima/hca/WaveGen.cpp
        src/lib/kawashima/wave/WaveNative.h src/lib/kawashima/api/ksapi.cpp src/lib/kawashima/api/hca_info.cpp
        src/lib/kawashima/ksapi.h src/lib/kawashima/api/ks_decode.h src/lib/kawashima/hca_info.h
        ${POLYFILL_SOURCE_FILES})
set(HCACC_SOURCE_FILES
        src/apps/hcacc/hcacc.cpp src/apps/cgssh.h ${LIBCGSS_SOURCE_FILES})
set(HCAENC_SOURCE_FILES
        src/apps/hcaenc/hcaenc.cpp ${POLYFILL_SOURCE_FILES})
set(HCA2WAV_SOURCE_FILES
        src/apps/hca2wav/hca2wav.cpp ${LIBCGSS_SOURCE_FILES})

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_TWEAK 0)
set(PROJECT_VERSION_PATCH 0)

if ("${CYGWIN}" OR "${MINGW}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
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
endif ("${CYGWIN}" OR "${MINGW}")
# http://stackoverflow.com/questions/10046114/in-cmake-how-can-i-test-if-the-compiler-is-clang
if (${CMAKE_COMPILER_IS_GNUC_PLUS_PLUS} OR ${CMAKE_COMPILER_IS_GNUC})
    # using GCC
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,--kill-at")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--kill-at")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -s")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -s")
endif (${CMAKE_COMPILER_IS_GNUC_PLUS_PLUS} OR ${CMAKE_COMPILER_IS_GNUC})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${TARGET_ARCH}/${CMAKE_BUILD_TYPE}")
add_executable(hcacc ${HCACC_SOURCE_FILES})
add_executable(hcaenc ${HCAENC_SOURCE_FILES})
add_executable(hca2wav ${HCA2WAV_SOURCE_FILES})
add_library(cgss SHARED ${LIBCGSS_SOURCE_FILES})

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
add_custom_command(TARGET cgss
        POST_BUILD
        COMMAND ${CMAKE_STRIP} -s $<TARGET_FILE:cgss>
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
