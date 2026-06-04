cmake_minimum_required(VERSION 3.20...3.28)
project(lvgl-phone-dial LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

if (NOT DEFINED ENV{REPO_ROOT})
    message(FATAL_ERROR "[${PROJECT_NAME}] Environment not sourced")
endif()
list(APPEND CMAKE_MODULE_PATH "$ENV{REPO_ROOT}/cmake")
include(extensions)

find_package(SDL2 REQUIRED)

sample_print_banner()

if (MSVC)
	add_compile_options(
		/Zc:__cplusplus
		/Zc:preprocessor
		/utf-8
	)
endif()

add_executable(${PROJECT_NAME})

set(LV_BUILD_CONF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../core ${CMAKE_BINARY_DIR}/core_lvgl_build)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../core
)
target_link_libraries(${PROJECT_NAME} PRIVATE
    lvgl-core
)

target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
    contacts_view.cpp
    dial_view.cpp
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wno-deprecated-enum-enum-conversion
)

sample_finalize()
