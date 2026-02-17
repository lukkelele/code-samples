cmake_minimum_required(VERSION 3.20...3.28)
project(class LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
	add_compile_options(
		/Zc:__cplusplus
		/Zc:preprocessor
		/utf-8
	)

    # Disable memory stack fill pattern (for showing uninitialized values easier).
    string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    string(REPLACE "/RTC1" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
endif()

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
)

set(output_dir ${CMAKE_BINARY_DIR}/app)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${output_dir}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${output_dir}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${output_dir}
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${output_dir}
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${output_dir}
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_hardlink
            $<TARGET_FILE:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/app$<TARGET_FILE_SUFFIX:${PROJECT_NAME}>
)