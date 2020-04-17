# https://cmake.org/cmake/help/v3.0/command/add_custom_command.html
# http://stackoverflow.com/questions/9994045/copy-target-file-to-another-location-in-a-post-build-step-in-cmake
if (${GNU_COMPILER})
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
    add_custom_command(TARGET hcainfo
            POST_BUILD
            COMMAND ${CMAKE_STRIP} -s $<TARGET_FILE:hcainfo>
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    add_custom_command(TARGET utftable
            POST_BUILD
            COMMAND ${CMAKE_STRIP} -s $<TARGET_FILE:utftable>
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    add_custom_command(TARGET acbunpack
            POST_BUILD
            COMMAND ${CMAKE_STRIP} -s $<TARGET_FILE:acbunpack>
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    add_custom_command(TARGET acb2wavs
            POST_BUILD
            COMMAND ${CMAKE_STRIP} -s $<TARGET_FILE:acb2wavs>
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    add_custom_command(TARGET acb2hcas
            POST_BUILD
            COMMAND ${CMAKE_STRIP} -s $<TARGET_FILE:acb2hcas>
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    add_custom_command(TARGET cgss
            POST_BUILD
            COMMAND ${CMAKE_STRIP} -s $<TARGET_FILE:cgss>
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif ()
