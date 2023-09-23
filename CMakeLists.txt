cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ----------------------------------------------------------------------------
# CLIENT

set(PROJECT_NAME_CLIENT r-type_client)
set(PROJECT_NAME_SERVER r-type_server)

project(
    ${PROJECT_NAME_CLIENT}
    VERSION 1.0.0
    LANGUAGES CXX
)
project(
    ${PROJECT_NAME_SERVER}
    VERSION 1.0.0
    LANGUAGES CXX
)

add_executable(
    ${PROJECT_NAME_CLIENT}
)
add_executable(
    ${PROJECT_NAME_SERVER}
)

add_subdirectory(deps)

if(MSVC)
    target_compile_options(
        ${PROJECT_NAME_CLIENT}
        PRIVATE
        /W4
    )
else()
    target_compile_options(
        ${PROJECT_NAME_CLIENT}
        PRIVATE
        -Wall -Wextra -pedantic
    )
endif()
if(MSVC)
    target_compile_options(
        ${PROJECT_NAME_SERVER}
        PRIVATE
        /W4
    )
else()
    target_compile_options(
        ${PROJECT_NAME_SERVER}
        PRIVATE
        -Wall -Wextra -pedantic
    )
endif()

add_subdirectory(src)

if (WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME_CLIENT} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME_CLIENT}>
        ${CMAKE_SOURCE_DIR}/${PROJECT_NAME_CLIENT}.exe
    )
else()
    add_custom_command(
        TARGET ${PROJECT_NAME_CLIENT} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME_CLIENT}>
        ${CMAKE_SOURCE_DIR}/${PROJECT_NAME_CLIENT}
    )
endif()
if (WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME_SERVER} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME_SERVER}>
        ${CMAKE_SOURCE_DIR}/${PROJECT_NAME_SERVER}.exe
    )
else()
    add_custom_command(
        TARGET ${PROJECT_NAME_SERVER} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME_SERVER}>
        ${CMAKE_SOURCE_DIR}/${PROJECT_NAME_SERVER}
    )
endif()
