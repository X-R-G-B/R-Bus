cmake_minimum_required(VERSION 3.15)

add_library(asio INTERFACE)
target_compile_options(asio INTERFACE ASIO_STANDALONE)

# -- Fetch asio
include(FetchContent)
FetchContent_Declare(
    asio
    URL "https://github.com/X-R-G-B/R-Bus/releases/latest/download/asio.tar"
)

FetchContent_GetProperties(asio)

if(NOT asio_POPULATED)
    FetchContent_Populate(asio)

    target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/include)

    find_package(Threads)
    target_link_libraries(asio PRIVATE Threads::Threads)
endif()
