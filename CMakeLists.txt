cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME client)

set(CMAKE_CXX_STANDARD 20)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

project(
    ${PROJECT_NAME}
    VERSION 1.0.0
    LANGUAGES CXX
)

add_executable(
    ${PROJECT_NAME}
)

add_subdirectory(deps)

if(MSVC)
    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE
        /W4
    )
else()
    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE
        -Wall -Wextra -pedantic
    )
endif()

add_subdirectory(src)

if (WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME}>
        ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.exe
    )
else()
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME}>
        ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}
    )
endif()
