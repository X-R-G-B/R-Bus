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
    ${CMAKE_CURRENT_SOURCE_DIR}/GameSystems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameCustomTypes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DeathSystems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CreateMissiles.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WaveSystemsAll.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PhysicSystems.cpp
)

target_sources(
    ${PROJECT_NAME_SERVER}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/GameSystems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GameCustomTypes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DeathSystems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CreateMissiles.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WaveSystemsAll.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PhysicSystems.cpp
)
