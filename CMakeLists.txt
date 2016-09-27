cmake_minimum_required(VERSION 3.3)
project(libcgss)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(HCA_SOURCE_FILES
        src/apps/hca/CHCA.cpp src/apps/hca/CHCA.h src/apps/hca/hca_info.h src/apps/hca/HcaNative.h src/apps/hca/kstypedef.h
        src/apps/hca/Magic.cpp src/apps/hca/Magic.h src/apps/hca/internal/CHcaCipher.cpp src/apps/hca/internal/CHcaCipher.h
        src/apps/hca/internal/CHcaData.cpp src/apps/hca/internal/CHcaData.h)
set(HCACC_SOURCE_FILES src/apps/hcacc/hcacc.cpp src/apps/hcacc/cgssh.h ${HCA_SOURCE_FILES})
set(HCAENC_SOURCE_FILES src/apps/hcaenc/hcaenc.cpp src/win_header.h)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_TWEAK 0)
set(PROJECT_VERSION_PATCH 0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${TARGET_ARCH}/${CMAKE_BUILD_TYPE}")
add_executable(hcacc ${HCACC_SOURCE_FILES})
add_executable(hcaenc ${HCAENC_SOURCE_FILES})
