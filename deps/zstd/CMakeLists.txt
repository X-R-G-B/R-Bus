cmake_minimum_required(VERSION 3.15)

include(FetchContent)

set(ZSTD_BUILD_STATIC ON)
set(ZSTD_BUILD_SHARED OFF)

FetchContent_Declare(
    zstd
    URL "https://github.com/facebook/zstd/releases/download/v1.5.5/zstd-1.5.5.tar.gz"
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
