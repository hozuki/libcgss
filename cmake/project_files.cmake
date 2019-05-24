file(GLOB LIBCGSS_API_FILES "src/lib/*.h")
file(GLOB_RECURSE LIBCGSS_COMMON_HEADER_FILES "src/lib/common/*.h" "src/lib/cdata/*.h")
file(GLOB_RECURSE LIBCGSS_COMMON_SOURCE_FILES "src/lib/common/*.cpp" "src/lib/capi/*.cpp")
file(GLOB_RECURSE LIBCGSS_KS_SOURCE_FILES "src/lib/kawashima/*.h" "src/lib/kawashima/*.cpp")
file(GLOB_RECURSE LIBCGSS_TM_SOURCE_FILES "src/lib/takamori/*.h" "src/lib/takamori/*.cpp")
file(GLOB_RECURSE LIBCGSS_INS_SOURCE_FILES "src/lib/ichinose/*.h" "src/lib/ichinose/*.cpp")

set(LIBCGSS_SOURCE_FILES
        ${LIBCGSS_API_FILES} ${LIBCGSS_COMMON_HEADER_FILES} ${LIBCGSS_COMMON_SOURCE_FILES} ${LIBCGSS_KS_SOURCE_FILES} ${LIBCGSS_TM_SOURCE_FILES} ${LIBCGSS_INS_SOURCE_FILES})

set(CGSS_APP_SOURCE_FILES
        src/apps/cgssh.h)

set(APP_SOURCE_FILES
        src/apps/common/common.h src/apps/common/atoh.hpp)

set(HCACC_SOURCE_FILES
        src/apps/hcacc/hcacc.cpp ${CGSS_APP_SOURCE_FILES} ${APP_SOURCE_FILES} ${LIBCGSS_API_FILES})
set(HCAENC_SOURCE_FILES
        src/apps/hcaenc/hcaenc.cpp ${CGSS_APP_SOURCE_FILES} ${APP_SOURCE_FILES} ${LIBCGSS_TM_API_FILES})
set(HCA2WAV_SOURCE_FILES
        src/apps/hca2wav/hca2wav.cpp ${CGSS_APP_SOURCE_FILES} ${APP_SOURCE_FILES} ${LIBCGSS_API_FILES})
set(HCAINFO_SOURCE_FILES
        src/apps/hcainfo/hcainfo.cpp ${APP_SOURCE_FILES} ${LIBCGSS_API_FILES})
set(UTFTABLE_SOURCE_FILES
        src/apps/utftable/utftable.c ${APP_SOURCE_FILES} ${LIBCGSS_API_FILES})
set(ACBUNPACK_SOURCE_FILES
        src/apps/acbunpack/acbunpack.cpp ${APP_SOURCE_FILES} ${LIBCGSS_API_FILES})
set(ACB2WAVS_SOURCE_FILES
        src/apps/acb2wavs/acb2wavs.cpp ${CGSS_APP_SOURCE_FILES} ${APP_SOURCE_FILES} ${LIBCGSS_API_FILES})
set(ACB2HCAS_SOURCE_FILES
        src/apps/acb2hcas/acb2hcas.cpp ${APP_SOURCE_FILES} ${LIBCGSS_API_FILES})

set(LIBCGSS_DEF_FILE src/cgss.def)
set(LIBCGSS_SOURCE_FILES ${LIBCGSS_SOURCE_FILES} ${LIBCGSS_DEF_FILE})
