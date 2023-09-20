cmake_minimum_required(VERSION 3.27)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Collison.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Movement.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ASystem.cpp
)

add_subdirectory(managers)
