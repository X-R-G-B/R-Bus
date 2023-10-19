cmake_minimum_required(VERSION 3.15)

include(FetchContent)

set(ZSTD_BUILD_STATIC ON)
set(ZSTD_BUILD_SHARED OFF)

FetchContent_Declare(
    zstd
    CMAKE_ARGS "-DCMAKE_BUILD_TYPE=Release" "-DCMAKE_CONFIGURATION_TYPES=\"Release;Release\"" "-DCONFIG=Release"
    URL "https://github.com/X-R-G-B/R-Bus/releases/latest/download/zstd.tar"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    SOURCE_SUBDIR build/cmake
)

FetchContent_MakeAvailable(zstd)

target_link_libraries(
    ${PROJECT_NAME_CLIENT}
    PRIVATE
    libzstd_static
)

target_link_libraries(
    ${PROJECT_NAME_SERVER}
    PRIVATE
    libzstd_static
)
