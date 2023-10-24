cmake_minimum_required(VERSION 3.15)

target_include_directories(
    ${PROJECT_NAME_CLIENT}
    PRIVATE
        .
)

add_subdirectory(Geometry)
add_subdirectory(Graphics)
add_subdirectory(Audio)
add_subdirectory(Events)
