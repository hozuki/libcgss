if ($ENV{CI})
    string(LENGTH LIBCGSS_LIBRARY_VERSION VERSION_INFO_LENGTH)

    if (VERSION_INFO_LENGTH GREATER 0)
        string(REPLACE "." ";" VERSION_COMPONENTS ${LIBCGSS_LIBRARY_VERSION})

        # If we are in a CI environment, load version info from environment variables (see configure-cmake.ps1)
        list(GET VERSION_COMPONENTS 0 PROJECT_VERSION_MAJOR)
        list(GET VERSION_COMPONENTS 1 PROJECT_VERSION_MINOR)
        list(GET VERSION_COMPONENTS 2 PROJECT_VERSION_PATCH)
        # Always 0
        set(PROJECT_VERSION_TWEAK 0)

        set_target_properties(cgss
                PROPERTIES
                VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}"
                SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
                )
    else ()
        message(WARNING "Warning: CI environment detected but libcgss version is empty.")
    endif ()
endif ()
