###################################################################
# CMake extensions
###################################################################

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

# Print sample banner.
function(sample_print_banner)
    message(STATUS "==================================================
    Sample: ${PROJECT_NAME}
   ==================================================")
endfunction()

# Finalize a sample by setting up properties and other things to
# make it easier to run using 'Tasks' (with Zed/Visual Studio Code).
macro(sample_finalize)
    set(_output_dir ${CMAKE_BINARY_DIR})
    set_target_properties(${PROJECT_NAME} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY ${_output_dir}
      RUNTIME_OUTPUT_DIRECTORY_DEBUG ${_output_dir}
      RUNTIME_OUTPUT_DIRECTORY_RELEASE ${_output_dir}
      RUNTIME_OUTPUT_DIRECtORY_RELWITHDEBINFO ${_output_dir}
      RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${_output_dir}
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE
      SAMPLE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    )

    if (CMAKE_CXX_COMPILER_LOADED)
        target_include_directories(${PROJECT_NAME} PRIVATE $ENV{LIB_CPP_DIR})
        add_subdirectory($ENV{LIB_CPP_DIR}/core ${CMAKE_BINARY_DIR}/core_build)
        target_link_libraries(${PROJECT_NAME} PRIVATE
            cpp_core
        )
    endif()

    if (CMAKE_C_COMPILER_LOADED)
        target_include_directories(${PROJECT_NAME} PRIVATE $ENV{LIB_DIR}/c)
        if (PROJECT_NAME MATCHES "^c_sample_net")
            add_subdirectory($ENV{LIB_DIR}/c/net ${CMAKE_BINARY_DIR}/net_build)
            target_link_libraries(${PROJECT_NAME} PRIVATE c_net)
        endif()
    endif()

    # Create link to target so run.sh can be used in tasks.
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_hardlink
            $<TARGET_FILE:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/target
    )

    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            $ENV{REPO_ROOT}/build
        COMMAND ${CMAKE_COMMAND} -E create_hardlink
            $<TARGET_FILE:${PROJECT_NAME}>
            $ENV{REPO_ROOT}/build/${PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -E create_hardlink
            $<TARGET_FILE:${PROJECT_NAME}>
            $ENV{REPO_ROOT}/build/target
    )
endmacro()

# Print experimental project banner.
function(experimental_print_banner)
    message(STATUS "==================================================
    ${PROJECT_NAME}
   ==================================================")
endfunction()

macro(experimental_finalize)
    set(_output_dir ${CMAKE_BINARY_DIR})
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${_output_dir}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${_output_dir}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${_output_dir}
        RUNTIME_OUTPUT_DIRECtORY_RELWITHDEBINFO ${_output_dir}
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${_output_dir}
    )

    add_subdirectory($ENV{LIB_CPP_DIR}/core ${CMAKE_BINARY_DIR}/core_build)
    target_include_directories(${PROJECT_NAME} PRIVATE
        $ENV{LIB_CPP_DIR}
        $ENV{EXPERIMENTAL_CPP_DIR}
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE
        cpp_core
    )

    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_hardlink
                $<TARGET_FILE:${PROJECT_NAME}>
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/target
    )
endmacro()
