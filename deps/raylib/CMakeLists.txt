cmake_minimum_required(VERSION 3.15)

find_package(raylib CONFIG REQUIRED)

target_include_directories(${PROJECT_NAME_CLIENT} PRIVATE ${raylib_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME_CLIENT} PRIVATE ${raylib_LIBRARIES})

install(FILES ${raylib_LIBRARIES} DESTINATION ${CMAKE_INSTALL_BINDIR})