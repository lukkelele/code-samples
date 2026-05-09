cmake_minimum_required(VERSION 3.20...3.28)
project(cpp_sample_advanced_net_tls_http_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT DEFINED ENV{REPO_ROOT})
    message(FATAL_ERROR "[${PROJECT_NAME}] Environment not sourced")
endif()
list(APPEND CMAKE_MODULE_PATH "$ENV{REPO_ROOT}/cmake")
include(extensions)

sample_print_banner()

find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
    http.cpp
    router.cpp
    server.cpp
    ssl.cpp
    socket.cpp
    transport.cpp
    users.cpp
)

find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Threads::Threads
    OpenSSL::SSL
    OpenSSL::Crypto
)

sample_finalize()
