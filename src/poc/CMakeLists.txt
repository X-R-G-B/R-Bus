cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME poc)

project(
    ${PROJECT_NAME}
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "4.5.0"
)

FetchContent_GetProperties(raylib)

if(NOT raylib_POPULATED)
    FetchContent_Populate(raylib)
    add_subdirectory(
        ${raylib_SOURCE_DIR}
        ${raylib_BINARY_DIR}
    )
endif()

add_executable(
    ${PROJECT_NAME}
)

# Set compiler flags
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

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    raylib
)

add_subdirectory(src)

# Copy the executable to the root directory
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
