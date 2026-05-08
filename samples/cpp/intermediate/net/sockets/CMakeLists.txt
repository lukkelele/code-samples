cmake_minimum_required(VERSION 3.20...3.28)
project(cpp_sample_intermediate_net_sockets LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT DEFINED ENV{REPO_ROOT})
    message(FATAL_ERROR "[${PROJECT_NAME}] Environment not sourced")
endif()
list(APPEND CMAKE_MODULE_PATH "$ENV{REPO_ROOT}/cmake")
include(extensions)

sample_print_banner()

if (MSVC)
	add_compile_options(
		/Zc:__cplusplus
		/Zc:preprocessor
		/utf-8
	)
endif()

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
    socket_utils.cpp
    tcp_socket.cpp
)

sample_finalize()
