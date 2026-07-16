cmake_minimum_required(VERSION 3.20...3.28)
project(c_net LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT DEFINED ENV{REPO_ROOT})
    message(FATAL_ERROR "[${PROJECT_NAME}] Environment not sourced")
endif()
list(APPEND CMAKE_MODULE_PATH "$ENV{REPO_ROOT}/cmake")
include(extensions)

add_library(${PROJECT_NAME} STATIC
    net.h
    net.c
    wrappers_sockets.c
    wrappers_unix.c
)
