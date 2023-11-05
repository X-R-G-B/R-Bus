cmake_minimum_required(VERSION 3.15)

include(FetchContent)

FetchContent_Declare(
    b-luga-graphics
    URL "https://github.com/X-R-G-B/B-luga/releases/latest/download/b-luga-graphics.tar"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS FALSE
)

FetchContent_Declare(
    raylib
    CMAKE_ARGS "-DCMAKE_BUILD_TYPE=Release" "-DCMAKE_CONFIGURATION_TYPES=\"Release;Release\"" "-DCONFIG=Release"
    URL "https://github.com/X-R-G-B/B-luga/releases/latest/download/raylib.tar"
    DOWNLOAD_NO_PROGRESS FALSE
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(b-luga-graphics raylib)

target_link_libraries(
    ${PROJECT_NAME_CLIENT}
    PRIVATE
    b-luga-graphics
)

target_link_libraries(
    ${PROJECT_NAME_CLIENT}
    PRIVATE
    raylib
)
