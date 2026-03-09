cmake_minimum_required(VERSION 3.20...3.28)
project(lvgl-core LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT DEFINED ENV{REPO_ROOT})
    message(FATAL_ERROR "[${PROJECT_NAME}] Environment not sourced")
endif()
list(APPEND CMAKE_MODULE_PATH "$ENV{REPO_ROOT}/cmake")
include(extensions)

find_package(SDL2 REQUIRED)

if (MSVC)
	add_compile_options(
		/Zc:__cplusplus
		/Zc:preprocessor
		/utf-8
	)
endif()

add_subdirectory($ENV{REPO_ROOT}/external/lvgl external_lvgl)

add_library(${PROJECT_NAME} STATIC
    lvgl_core.h
    lvgl_core.cpp
    demo_shop.h
)

target_link_libraries(lvgl PRIVATE ${SDL2_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE
    ${SDL2_INCLUDE_DIRS}
)
target_include_directories(${PROJECT_NAME} PUBLIC
    $ENV{REPO_ROOT}/external/lvgl
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${SDL2_LIBRARIES}
    lvgl
    lvgl::demos
    lvgl::examples
)
