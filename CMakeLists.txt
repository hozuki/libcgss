cmake_minimum_required(VERSION 3.3)
project(cgss)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(TARGET_ARCH x64)
set(TARGET_ARCH x86)

if ("${CMAKE_COMPILER_IS_GNUC_PLUS_PLUS}" OR "${CMAKE_COMPILER_IS_GNUC}" OR "${CYGWIN}" OR "${MINGW}")
    set(GNU_COMPILER 1)
else ()
    set(GNU_COMPILER 0)
endif ()

set(LIBCGSS_TM_API_FILES
        # takamori
        src/lib/tm_io.h src/lib/tm_dylib.h src/lib/tm_api.h)
set(LIBCGSS_KS_API_FILES
        #kawashima
        src/lib/ks_api.h src/lib/cgss_typedef.h src/lib/hca_info.hpp src/lib/kawashima/api/ks_decode_magic.h)
set(LIBCGSS_SOURCE_FILES
        # kawashima
        ${LIBCGSS_KS_API_FILES}
        src/lib/kawashima/hca/CHcaDecoder.h src/lib/kawashima/hca/CHcaDecoder.cpp src/lib/kawashima/hca/HcaNative.h
        src/lib/kawashima/hca/internal/CHcaCipher.cpp src/lib/kawashima/hca/internal/CHcaCipher.h src/lib/kawashima/hca/internal/CHcaAth.cpp
        src/lib/kawashima/hca/internal/CHcaAth.h src/lib/kawashima/hca/internal/CHcaData.cpp src/lib/kawashima/hca/internal/CHcaData.h
        src/lib/kawashima/hca/internal/HcaChannel.cpp src/lib/kawashima/hca/internal/HcaChannel.h src/lib/kawashima/hca/Magic.cpp
        src/lib/kawashima/hca/Magic.h src/lib/kawashima/hca/WaveGen.h src/lib/kawashima/hca/WaveGen.cpp
        src/lib/kawashima/wave/WaveNative.h src/lib/kawashima/api/ks_api.cpp
        src/lib/kawashima/api/ks_decode.h src/lib/common/libcgss_building_dll.h
        # kawashima features
        src/lib/kawashima/features/ks_features.h src/lib/kawashima/features/ks_features.cpp
        src/lib/kawashima/features/ks_feat_streaming.cpp
        # takamori
        ${LIBCGSS_TM_API_FILES}
        src/lib/takamori/polyfill/tm_io.cpp src/lib/takamori/polyfill/tm_dylib.cpp)
set(HCACC_SOURCE_FILES
        src/apps/hcacc/hcacc.cpp src/apps/cgssh.h ${LIBCGSS_SOURCE_FILES})
set(HCAENC_SOURCE_FILES
        src/apps/hcaenc/hcaenc.cpp ${LIBCGSS_TM_API_FILES})
set(HCA2WAV_SOURCE_FILES
        src/apps/hca2wav/hca2wav.cpp ${LIBCGSS_KS_API_FILES} ${LIBCGSS_TM_API_FILES})

set(DEF_FILE src/libcgss.def)
if (${GNU_COMPILER})
    set(LIBCGSS_SOURCE_FILES ${LIBCGSS_SOURCE_FILES} ${DEF_FILE})
endif ()

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_TWEAK 0)
set(PROJECT_VERSION_PATCH 0)

if ("${CYGWIN}" OR "${MINGW}")
    # http://stackoverflow.com/questions/12921911/mingw-libgcc-s-sjlj-1-dll-is-missing
    # However, these options will only work if putting them here, not in 'compiler is gnu c' section.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
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
        if (${CMAKE_SIZEOF_VOID_P} EQUAL 4)
            set(TARGET_ARCH x86)
        elseif (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
            set(TARGET_ARCH x64)
        else ()
            message(FATAL_ERROR "Unknown pointer size: ${CMAKE_SIZEOF_VOID_P}")
        endif ()
    endif ()
endif ()
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
    set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} /DEF:${DEF_FILE})
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${TARGET_ARCH}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_executable(hcacc ${HCACC_SOURCE_FILES})
add_executable(hcaenc ${HCAENC_SOURCE_FILES})
add_executable(hca2wav ${HCA2WAV_SOURCE_FILES})
add_library(cgss SHARED ${LIBCGSS_SOURCE_FILES})
if ("${WIN32}")
    # http://stackoverflow.com/questions/31038963/how-do-you-rename-a-library-filename-in-cmake
    set_target_properties(cgss PROPERTIES PREFIX "")
endif ()
target_link_libraries(hca2wav cgss)
target_link_libraries(hcaenc cgss)

# https://cmake.org/cmake/help/v3.0/command/add_custom_command.html
# http://stackoverflow.com/questions/9994045/copy-target-file-to-another-location-in-a-post-build-step-in-cmake
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
