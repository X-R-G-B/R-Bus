cmake_minimum_required(VERSION 3.15)

include(FetchContent)

FetchContent_Declare(
    date
    CMAKE_ARGS "-DCMAKE_BUILD_TYPE=Release" "-DCMAKE_CONFIGURATION_TYPES=\"Release;Release\"" "-DCONFIG=Release"
    URL "https://github.com/X-R-G-B/R-Bus/releases/latest/download/date.tar"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(date)

target_include_directories(
    ${PROJECT_NAME_CLIENT}
    PRIVATE
    ${date_SOURCE_DIR}/include
)

target_include_directories(
    ${PROJECT_NAME_SERVER}
    PRIVATE
    ${date_SOURCE_DIR}/include
)
