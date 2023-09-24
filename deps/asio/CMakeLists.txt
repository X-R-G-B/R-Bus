cmake_minimum_required(VERSION 3.15)

# -- Fetch asio
include(FetchContent)
FetchContent_Declare(
    asio
    URL "https://github.com/X-R-G-B/R-Bus/releases/latest/download/asio.tar"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_GetProperties(asio)

if(NOT asio_POPULATED)
    FetchContent_Populate(asio)

    target_include_directories(${PROJECT_NAME_CLIENT} PRIVATE ${asio_SOURCE_DIR}/include)
    target_include_directories(${PROJECT_NAME_SERVER} PRIVATE ${asio_SOURCE_DIR}/include)

    find_package(Threads)
    target_link_libraries(${PROJECT_NAME_CLIENT} INTERFACE Threads::Threads)
    target_link_libraries(${PROJECT_NAME_SERVER} INTERFACE Threads::Threads)
endif()
