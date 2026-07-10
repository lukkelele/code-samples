cmake_minimum_required(VERSION 3.20...3.28)
project(core LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT DEFINED ENV{REPO_ROOT})
    message(FATAL_ERROR "[${PROJECT_NAME}] Environment not sourced")
endif()
list(APPEND CMAKE_MODULE_PATH "$ENV{REPO_ROOT}/cmake")
include(extensions)

add_library(${PROJECT_NAME} STATIC
    core.h
    core.cpp
    print.cpp
)

if (MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE
		/Zc:__cplusplus
		/Zc:preprocessor
		/utf-8
	)
endif()
