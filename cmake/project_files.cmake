file(GLOB LIBCGSS_API_FILES "src/lib/*.h")
file(GLOB_RECURSE LIBCGSS_COMMON_HEADER_FILES "src/lib/common/*.h" "src/lib/cdata/*.h")
file(GLOB_RECURSE LIBCGSS_COMMON_SOURCE_FILES "src/lib/common/*.cpp" "src/lib/capi/*.cpp")
file(GLOB_RECURSE LIBCGSS_KS_SOURCE_FILES "src/lib/kawashima/*.h" "src/lib/kawashima/*.cpp")
file(GLOB_RECURSE LIBCGSS_TM_SOURCE_FILES "src/lib/takamori/*.h" "src/lib/takamori/*.cpp")
file(GLOB_RECURSE LIBCGSS_INS_SOURCE_FILES "src/lib/ichinose/*.h" "src/lib/ichinose/*.cpp")

set(LIBCGSS_SOURCE_FILES
        ${LIBCGSS_API_FILES} ${LIBCGSS_COMMON_HEADER_FILES} ${LIBCGSS_COMMON_SOURCE_FILES} ${LIBCGSS_KS_SOURCE_FILES} ${LIBCGSS_TM_SOURCE_FILES} ${LIBCGSS_INS_SOURCE_FILES})
set(HCACC_SOURCE_FILES
        src/apps/hcacc/hcacc.cpp src/apps/cgssh.h ${LIBCGSS_API_FILES})
set(HCAENC_SOURCE_FILES
        src/apps/hcaenc/hcaenc.cpp ${LIBCGSS_TM_API_FILES})
set(HCA2WAV_SOURCE_FILES
        src/apps/hca2wav/hca2wav.cpp src/apps/cgssh.h ${LIBCGSS_API_FILES})
set(HCAINFO_SOURCE_FILES
        src/apps/hcainfo/hcainfo.cpp ${LIBCGSS_API_FILES})
set(UTFTABLE_SOURCE_FILES
        src/apps/utftable/utftable.c ${LIBCGSS_API_FILES})
set(ACBUNPACK_SOURCE_FILES
        src/apps/acbunpack/acbunpack.cpp ${LIBCGSS_API_FILES})

set(LIBCGSS_DEF_FILE src/cgss.def)
set(LIBCGSS_SOURCE_FILES ${LIBCGSS_SOURCE_FILES} ${LIBCGSS_DEF_FILE})
