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
