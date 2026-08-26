cmake_minimum_required(VERSION 3.20...3.28)
project(cpp_core LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(PROFILER_ENABLED "Profiler enabled" ON)
option(PROFILER_SCOPED_TIMERS "Use scoped timers instead of using tracy" OFF)

if (NOT DEFINED ENV{REPO_ROOT})
    message(FATAL_ERROR "[${PROJECT_NAME}] Environment not sourced")
endif()
list(APPEND CMAKE_MODULE_PATH "$ENV{REPO_ROOT}/cmake")
include(extensions)

set(LKLOG_BUILD_TESTS OFF)
add_subdirectory($ENV{EXTERNAL_DIR}/lklog ${CMAKE_BINARY_DIR}/lklog_build)

build_tracy()

add_library(${PROJECT_NAME} STATIC
    core.h
    core.cpp
    print.cpp
)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:-g3>
    $<$<CONFIG:Debug>:-Og>
    $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    lklog
    Tracy::TracyClient
)
target_include_directories(${PROJECT_NAME} PUBLIC
    $ENV{EXTERNAL_DIR}
    $ENV{EXTERNAL_DIR}/tracy
)

if (PROFILER_ENABLED)
    target_compile_definitions(${PROJECT_NAME} PUBLIC PROFILER_ENABLED)
    if (PROFILER_SCOPED_TIMERS)
        target_compile_definitions(${PROJECT_NAME} PUBLIC PROFILER_SCOPED_TIMERS)
    endif()
endif()
