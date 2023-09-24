cmake_minimum_required(VERSION 3.15)

find_package(asio CONFIG REQUIRED)

target_include_directories(${PROJECT_NAME_CLIENT} PRIVATE ${asio_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME_CLIENT} PRIVATE ${asio_LIBRARIES})
