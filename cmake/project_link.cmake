if (WIN32)
    set_target_properties(cgss
            PROPERTIES
            LINK_DEF_FILE_FLAG ${LIBCGSS_DEF_FILE})
endif ()

if (UNIX AND NOT (MINGW OR CYGWIN))
    target_link_libraries(cgss dl)
endif ()

target_link_libraries(hcacc cgss)
target_link_libraries(hcaenc cgss)
target_link_libraries(hca2wav cgss)
target_link_libraries(hcainfo cgss)
target_link_libraries(utftable cgss)
target_link_libraries(acbunpack cgss)
target_link_libraries(acb2wavs cgss)
target_link_libraries(acb2hcas cgss)
