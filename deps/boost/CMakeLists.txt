cmake_minimum_required(VERSION 3.15)

# -- Fetch Boost
include(FetchContent)
FetchContent_Declare(
    boost
    URL "https://github.com/X-R-G-B/R-Bus/releases/latest/download/boost.tar"
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_GetProperties(boost)

if(NOT boost_POPULATED)
    FetchContent_Populate(boost)

    message(WARNING "${boost_SOURCE_DIR}")

    add_subdirectory(
        ${boost_SOURCE_DIR}
        ${boost_BINARY_DIR}
    )


    find_package(Threads)
    target_link_libraries(${PROJECT_NAME_CLIENT} INTERFACE Threads::Threads)
    target_link_libraries(${PROJECT_NAME_SERVER} INTERFACE Threads::Threads)
endif()
