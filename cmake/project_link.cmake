if (WIN32)
    set_target_properties(cgss
            PROPERTIES
            LINK_DEF_FILE_FLAG ${LIBCGSS_DEF_FILE})
    if (TARGET cgss_jni)
        set_target_properties(cgss_jni
                PROPERTIES
                LINK_DEF_FILE_FLAG ${LIBCGSS_DEF_FILE})
    endif()
endif ()

if (UNIX AND NOT (MINGW OR CYGWIN))
    target_link_libraries(cgss dl)
endif ()

if (TARGET cgss_jni)
    target_link_libraries(cgss_jni jvm)
endif ()

target_link_libraries(hcacc cgss)
target_link_libraries(hcaenc cgss)
target_link_libraries(hca2wav cgss)
target_link_libraries(hcainfo cgss)
target_link_libraries(utftable cgss)
