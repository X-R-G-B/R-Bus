cmake_minimum_required(VERSION 3.15)

include(FetchContent)

FetchContent_Declare(
    raylib
    URL "https://github.com/X-R-G-B/R-Bus/releases/latest/download/raylib.tar"
)

FetchContent_GetProperties(raylib)

if(NOT raylib_POPULATED)
    FetchContent_Populate(raylib)
    add_subdirectory(
        ${raylib_SOURCE_DIR}
        ${raylib_BINARY_DIR}
    )

    target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        raylib
    )
endif()
