cmake_minimum_required(VERSION 3.15)

include(FetchContent)

#FetchContent_Declare(
#        zstd
#        URL "https://github.com/facebook/zstd/releases/download/v1.5.5/zstd-1.5.5.tar.gz"
#        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
#)

#FetchContent_MakeAvailable(zstd)
#


target_link_libraries(
        ${PROJECT_NAME_CLIENT}
        PRIVATE

)

target_include_directories(
        ${PROJECT_NAME_SERVER}
        PRIVATE
        zstd
)
