###################################################################
# CMake extensions
###################################################################

# Print sample banner.
function(sample_print_banner)
    message(STATUS "==================================================
    Sample: ${PROJECT_NAME}
   ==================================================")
endfunction()

# Finalize a sample by setting up properties and other things to
# make it easier to run using 'Tasks' (with Zed/Visual Studio Code).
macro(sample_finalize)
    set(_output_dir ${CMAKE_BINARY_DIR}/app)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${_output_dir}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${_output_dir}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${_output_dir}
        RUNTIME_OUTPUT_DIRECtORY_RELWITHDEBINFO ${_output_dir}
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${_output_dir}
    )

    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_hardlink
                $<TARGET_FILE:${PROJECT_NAME}>
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/app
    )
endmacro()

# Print experimental project banner.
function(experimental_print_banner)
    message(STATUS "==================================================
    ${PROJECT_NAME}
   ==================================================")
endfunction()

macro(experimental_finalize)
    set(_output_dir ${CMAKE_BINARY_DIR}/app)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${_output_dir}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${_output_dir}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${_output_dir}
        RUNTIME_OUTPUT_DIRECtORY_RELWITHDEBINFO ${_output_dir}
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${_output_dir}
    )

    target_include_directories(${PROJECT_NAME} PRIVATE
        $ENV{EXPERIMENTAL_CPP_DIR}
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE
        experimental_core
    )

    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_hardlink
                $<TARGET_FILE:${PROJECT_NAME}>
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/app
    )
endmacro()

function(add_subdirectory_ifdef feature_toggle source_dir)
    if (${${feature_toggle}})
	    add_subdirectory(${source_dir} ${ARGN})
    endif()
endfunction()

function(add_subdirectory_ifndef feature_toggle source_dir)
    if (NOT ${feature_toggle})
        add_subdirectory(${source_dir} ${ARGN})
    endif()
endfunction()
