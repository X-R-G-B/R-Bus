cmake_minimum_required(VERSION 3.15)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

add_subdirectory(boost)
add_subdirectory(json)
add_subdirectory(zstd)
add_subdirectory(bluga)
add_subdirectory(bluga-physics)
add_subdirectory(bluga-graphics)
