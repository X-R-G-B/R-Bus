cmake_minimum_required(VERSION 3.15)

add_subdirectory(Systems)

target_include_directories(
    ${PROJECT_NAME_CLIENT}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_include_directories(
    ${PROJECT_NAME_SERVER}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(
    ${PROJECT_NAME_CLIENT}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Registry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Clock.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Json.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ECSCustomTypes.cpp
)

target_sources(
    ${PROJECT_NAME_SERVER}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Registry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Clock.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Json.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ECSCustomTypes.cpp
)
