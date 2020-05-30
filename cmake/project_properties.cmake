if ($ENV{CI})
    if (DEFINED ENV{LIBCGSS_VERSION_MAJOR)
        string(LENGTH $ENV{LIBCGSS_VERSION_MAJOR} VERSION_INFO_LENGTH)

        if (VERSION_INFO_LENGTH GREATER 0)
            # If we are in a CI environment, load version info from environment variables (see configure-cmake.ps1)
            set(PROJECT_VERSION_MAJOR $ENV{LIBCGSS_VERSION_MAJOR})
            set(PROJECT_VERSION_MINOR $ENV{LIBCGSS_VERSION_MINOR})
            set(PROJECT_VERSION_PATCH $ENV{LIBCGSS_VERSION_PATCH})
            set(PROJECT_VERSION_TWEAK $ENV{LIBCGSS_VERSION_TWEAK})

            set_target_properties(cgss
                    PROPERTIES
                    VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}"
                    SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
                    )
        else ()
            message(WARNING "Warning: CI environment detected but libcgss version is empty.")
        endif ()
    else ()
        message(WARNING "Warning: CI environment detected but libcgss version is not defined.")
    endif ()
endif ()
