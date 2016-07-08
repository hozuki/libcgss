cmake_minimum_required(VERSION 3.3)
project(hcatools)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#set(COMPILE_WITH_CGSS 1)

set(HCA_SOURCE_FILES hca/CHCA.cpp hca/CHCA.h hca/hca_info.h hca/HcaNative.h hca/kstypedef.h hca/Magic.cpp hca/Magic.h hca/internal/CHcaCipher.cpp hca/internal/CHcaCipher.h hca/internal/CHcaData.cpp hca/internal/CHcaData.h)
set(HCACC_SOURCE_FILES hcacc.cpp ${HCA_SOURCE_FILES})
if (COMPILE_WITH_CGSS)
set(HCACC_SOURCE_FILES ${HCACC_SOURCE_FILES} cgssh.h)
endif ()
set(HCAENC_SOURCE_FILES hcaenc.cpp)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_TWEAK 0)
set(PROJECT_VERSION_PATCH 0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${TARGET_ARCH}/${CMAKE_BUILD_TYPE}")
add_executable(hcacc ${HCACC_SOURCE_FILES})
add_executable(hcaenc ${HCAENC_SOURCE_FILES})