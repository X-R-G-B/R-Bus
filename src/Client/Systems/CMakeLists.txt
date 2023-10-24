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
    ${CMAKE_CURRENT_SOURCE_DIR}/ClientSystems.cpp
)

add_subdirectory(Graphic)
add_subdirectory(Events)
add_subdirectory(Network)
add_subdirectory(CustomTypes)
add_subdirectory(Menu)
