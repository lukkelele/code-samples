cmake_minimum_required(VERSION 3.20...3.28)
project(cpp_gui LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT DEFINED ENV{REPO_ROOT})
    message(FATAL_ERROR "[${PROJECT_NAME}] Environment not sourced")
endif()
list(APPEND CMAKE_MODULE_PATH "$ENV{REPO_ROOT}/cmake")
include(extensions)

if (NOT TARGET cpp_core)
    add_subdirectory($ENV{LIB_CPP_DIR}/core ${CMAKE_BINARY_DIR}/cpp_core_build)
endif()
build_glad()
build_glfw()
build_imgui()
build_glm()

add_library(${PROJECT_NAME} STATIC
    rendercontext.h
    rendercontext.cpp
    window.h
    window.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $ENV{LIB_CPP_DIR}
    $ENV{EXTERNAL_DIR}
    $ENV{EXTERNAL_DIR}/glfw/include
    $ENV{EXTERNAL_DIR}/glm
    $ENV{EXTERNAL_DIR}/imgui
    $ENV{MODULES_DIR}/glad/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    cpp_core
    glad
    glfw
    glm
    imgui
)
