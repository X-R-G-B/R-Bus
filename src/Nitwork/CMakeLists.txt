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
    ${CMAKE_CURRENT_SOURCE_DIR}/ANitwork.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NitworkClient.cpp
)

target_sources(
    ${PROJECT_NAME_SERVER}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ANitwork.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NitworkServer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NitworkMainServer.cpp
)
