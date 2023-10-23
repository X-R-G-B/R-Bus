cmake_minimum_required(VERSION 3.15)

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
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Systems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BulletsSystems.cpp
)

target_sources(
        ${PROJECT_NAME_SERVER}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Systems.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/BulletsSystems.cpp
)

add_subdirectory(Managers)
