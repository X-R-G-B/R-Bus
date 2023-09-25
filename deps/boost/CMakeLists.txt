cmake_minimum_required(VERSION 3.15)

# -- Fetch Boost
include(FetchContent)
FetchContent_Declare(
    Boost
    URL "https://github.com/X-R-G-B/R-Bus/releases/latest/download/boost.tar"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_GetProperties(Boost)

if(NOT Boost_POPULATED)
    FetchContent_Populate(Boost)

    add_subdirectory(
        ${Boost_SOURCE_DIR}
        ${Boost_BINARY_DIR}
    )

    find_package(Threads)
    target_link_libraries(${PROJECT_NAME_CLIENT} INTERFACE Threads::Threads)
    target_link_libraries(${PROJECT_NAME_SERVER} INTERFACE Threads::Threads)
endif()
