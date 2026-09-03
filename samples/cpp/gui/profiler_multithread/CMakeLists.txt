cmake_minimum_required(VERSION 3.20...3.28)
project(cpp_sample_gui_profiler_multithread LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT DEFINED ENV{REPO_ROOT})
    message(FATAL_ERROR "[${PROJECT_NAME}] Environment not sourced")
endif()
list(APPEND CMAKE_MODULE_PATH "$ENV{REPO_ROOT}/cmake")
include(extensions)

set(PROFILER_SCOPED_TIMERS OFF CACHE BOOL "" FORCE)

sample_print_banner()

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
)

cpp_gui_linkage()
sample_finalize()
