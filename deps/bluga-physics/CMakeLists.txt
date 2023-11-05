cmake_minimum_required(VERSION 3.15)

include(FetchContent)

FetchContent_Declare(
    b-luga-physics
    URL "https://github.com/X-R-G-B/B-luga/releases/latest/download/b-luga-physics.tar"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS FALSE
)

FetchContent_MakeAvailable(b-luga-physics)

target_link_libraries(${PROJECT_NAME_CLIENT} PRIVATE b-luga-physics)

target_link_libraries(
    ${PROJECT_NAME_CLIENT}
    INTERFACE
    b-luga-physics
)

target_link_libraries(${PROJECT_NAME_SERVER} PRIVATE b-luga-physics)

target_link_libraries(
    ${PROJECT_NAME_SERVER}
    INTERFACE
    b-luga-physics
)
