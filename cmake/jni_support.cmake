if ($ENV{CGSS_BUILD_JNI_LIBRARY})
    set(CGSS_BUILD_JNI_LIBRARY ON)
elseif (NOT DEFINED CGSS_BUILD_JNI_LIBRARY)
    set(CGSS_BUILD_JNI_LIBRARY OFF)
endif ()

if (${CGSS_BUILD_JNI_LIBRARY})
    # http://public.kitware.com/pipermail/cmake/2012-June/050674.html
    macro(header_directories base_path return_list)
        file(GLOB_RECURSE new_list ${base_path}/*.h)
        set(dir_list "")
        foreach (file_path ${new_list})
            get_filename_component(dir_path ${file_path} DIRECTORY)
            set(dir_list ${dir_list} ${dir_path})
        endforeach ()
        list(REMOVE_DUPLICATES dir_list)
        set(${return_list} ${dir_list})
    endmacro()

    # http://stackoverflow.com/questions/28070810/cmake-generate-error-on-windows-as-it-uses-as-escape-seq
    set(JAVA_HOME $ENV{JAVA_HOME})
    message("Java: ${JAVA_HOME}")
    string(REPLACE "\\" "/" JAVA_HOME "${JAVA_HOME}")

    set(JAVA_BASE_INCLUDE_PATH ${JAVA_HOME}/include)

    header_directories(${JAVA_BASE_INCLUDE_PATH} JAVA_FULL_INCLUDE_PATH)

    set(JAVA_LIBRARY_PATH ${JAVA_HOME}/lib)

    file(GLOB_RECURSE LIBCGSS_JNI_HEADER_FILES bindings/java/target/headers/*.h)
    set(LIBCGSS_JNI_HELPER_FILES src/lib/jni/helpers/cgss_jni_ns.h src/lib/jni/jni_helper.h src/lib/jni/helpers/jni_helper_string.h src/lib/jni/helpers/jni_helper_string.cpp src/lib/jni/helpers/jni_helper_reflection.h src/lib/jni/helpers/jni_helper_reflection.cpp src/lib/jni/helpers/jni_helper_math.h src/lib/jni/helpers/jni_helper_math.cpp)
    set(LIBCGSS_JNI_IMPL_FILES src/lib/jni/modules/cgss_jni_class_names.cpp src/lib/jni/modules/cgss_jni_class_names.h src/lib/jni/modules/takamori/NativeStream.cpp src/lib/jni/modules/takamori/FileStream.cpp src/lib/jni/modules/takamori/MemoryStream.cpp src/lib/jni/modules/kawashima/HcaFormatReader.cpp src/lib/jni/modules/kawashima/HcaDecoder.cpp)

    set(LIBCGSS_JNI_SOURCE_FILES ${LIBCGSS_SOURCE_FILES} ${LIBCGSS_JNI_HEADER_FILES} ${LIBCGSS_JNI_HELPER_FILES} ${LIBCGSS_JNI_IMPL_FILES})

    # New target
    add_library(cgss_jni SHARED ${LIBCGSS_JNI_SOURCE_FILES})

    # Target compile options
    target_include_directories(cgss_jni
            PUBLIC ${JAVA_FULL_INCLUDE_PATH}
            PUBLIC src/lib
            )
    target_link_directories(cgss_jni
            PUBLIC ${JAVA_LIBRARY_PATH}
            )
    target_compile_definitions(cgss_jni
            PRIVATE __CGSS_BUILDING_DLL__
            PRIVATE __CGSS_WITH_JNI__
            )

    # Target properties
    set_target_properties(cgss_jni
            PROPERTIES
            VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}"
            SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
            )

    if (MSVC)
        set_target_properties(cgss_jni
                PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
                ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
                )
    endif ()

    # Target link options
    if (WIN32)
        set_target_properties(cgss_jni
                PROPERTIES
                LINK_DEF_FILE_FLAG ${LIBCGSS_DEF_FILE}
                )
    endif ()

    # Post-build
    if (${GNU_COMPILER})
        add_custom_command(TARGET cgss_jni
                POST_BUILD
                COMMAND ${CMAKE_STRIP} -s $<TARGET_FILE:cgss_jni>
                WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                )
    endif ()
endif ()

