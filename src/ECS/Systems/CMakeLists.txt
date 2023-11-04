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
    ${CMAKE_CURRENT_SOURCE_DIR}/PhysicSystems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WaveSystems.cpp
)

target_sources(
        ${PROJECT_NAME_SERVER}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Systems.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/BulletsSystems.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/PhysicSystems.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/WaveSystems.cpp
)

add_subdirectory(Managers)
