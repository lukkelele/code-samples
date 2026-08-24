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

    target_include_directories(${PROJECT_NAME} PRIVATE
        $ENV{EXTERNAL_DIR}
        $ENV{MODULES_DIR}
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE
      SAMPLE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    )

    if (CMAKE_CXX_COMPILER_LOADED)
        target_include_directories(${PROJECT_NAME} PRIVATE $ENV{LIB_CPP_DIR})
        if (NOT TARGET cpp_core)
            add_subdirectory($ENV{LIB_CPP_DIR}/core ${CMAKE_BINARY_DIR}/cpp_core_build)
        endif()
        target_link_libraries(${PROJECT_NAME} PRIVATE
            cpp_core
        )
    endif()

    if (CMAKE_C_COMPILER_LOADED)
        target_include_directories(${PROJECT_NAME} PRIVATE $ENV{LIB_DIR}/c)
        if (PROJECT_NAME MATCHES "^c_sample_net")
            add_subdirectory($ENV{LIB_DIR}/c/net ${CMAKE_BINARY_DIR}/c_net_build)
            target_link_libraries(${PROJECT_NAME} PRIVATE c_net)
        endif()
    endif()

    # Create link to target so run.sh can be used in tasks.
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_hardlink
            $<TARGET_FILE:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/target
    )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
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

macro(build_glfw)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_LIBRARY_TYPE STATIC CACHE STRING "" FORCE)
    add_subdirectory($ENV{EXTERNAL_DIR}/glfw ${CMAKE_BINARY_DIR}/glfw_build)
endmacro()

macro(build_imgui)
    add_library(imgui STATIC
        $ENV{EXTERNAL_DIR}/imgui/imgui_internal.h
        $ENV{EXTERNAL_DIR}/imgui/imgui.h
        $ENV{EXTERNAL_DIR}/imgui/imgui.cpp
        $ENV{EXTERNAL_DIR}/imgui/imgui_demo.cpp
        $ENV{EXTERNAL_DIR}/imgui/imgui_draw.cpp
        $ENV{EXTERNAL_DIR}/imgui/imgui_tables.cpp
        $ENV{EXTERNAL_DIR}/imgui/imgui_tables.cpp
        $ENV{EXTERNAL_DIR}/imgui/imgui_widgets.cpp
        $ENV{EXTERNAL_DIR}/imgui/backends/imgui_impl_glfw.cpp
        $ENV{EXTERNAL_DIR}/imgui/backends/imgui_impl_opengl3.cpp
    )
    target_include_directories(imgui PUBLIC
        $ENV{EXTERNAL_DIR}/imgui
        $ENV{EXTERNAL_DIR}/imgui/backends
    )
    target_include_directories(imgui PRIVATE $ENV{EXTERNAL_DIR}/glfw/include)

    if (CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang)$")
        find_package(X11 REQUIRED)
        target_link_libraries(imgui PRIVATE X11)
    endif()
endmacro()

macro(build_glad)
    add_subdirectory($ENV{MODULES_DIR}/glad ${CMAKE_BINARY_DIR}/glad_build)
endmacro()

macro(build_glm)
    add_subdirectory($ENV{EXTERNAL_DIR}/glm ${CMAKE_BINARY_DIR}/glm_build)
endmacro()

# Build the cpp_gui library and link the current project to it.
macro(cpp_gui_linkage)
    add_subdirectory($ENV{LIB_CPP_DIR}/gui ${CMAKE_BINARY_DIR}/cpp_gui_build)
    target_link_libraries(${PROJECT_NAME} PRIVATE cpp_gui)
endmacro()
