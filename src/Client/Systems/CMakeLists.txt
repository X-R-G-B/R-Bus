cmake_minimum_required(VERSION 3.15)

target_include_directories(
    ${PROJECT_NAME_CLIENT}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Events)
add_subdirectory(Network)
add_subdirectory(Menus)
add_subdirectory(Parallax)
