cmake_minimum_required(VERSION 3.15)

target_include_directories(
    ${PROJECT_NAME_CLIENT}
    PRIVATE
        .
)

target_include_directories(
    ${PROJECT_NAME_SERVER}
    PRIVATE
        .
)

target_sources(
    ${PROJECT_NAME_CLIENT}
    PRIVATE
        .
    ${CMAKE_CURRENT_SOURCE_DIR}/SystemManagersDirector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SystemManager.cpp
)

target_sources(
    ${PROJECT_NAME_SERVER}
    PRIVATE
        .
    ${CMAKE_CURRENT_SOURCE_DIR}/SystemManagersDirector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SystemManager.cpp
)
