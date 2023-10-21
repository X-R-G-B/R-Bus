cmake_minimum_required(VERSION 3.15)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

add_subdirectory(raylib)
add_subdirectory(boost)
add_subdirectory(json)
add_subdirectory(date)
add_subdirectory(zstd)
