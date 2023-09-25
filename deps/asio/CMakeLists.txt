cmake_minimum_required(VERSION 3.15)

find_package(asio REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED)

target_link_libraries(${PROJECT_NAME_CLIENT} PRIVATE Threads::Threads)
target_include_directories(${PROJECT_NAME_CLIENT} PRIVATE ${asio_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME_CLIENT} PRIVATE ${asio_LIBRARIES})
target_include_directories(${PROJECT_NAME_CLIENT} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME_CLIENT} PRIVATE ${Boost_LIBRARIES})

target_link_libraries(${PROJECT_NAME_SERVER} PRIVATE Threads::Threads)
target_include_directories(${PROJECT_NAME_SERVER} PRIVATE ${asio_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME_SERVER} PRIVATE ${asio_LIBRARIES})
target_include_directories(${PROJECT_NAME_SERVER} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME_SERVER} PRIVATE ${Boost_LIBRARIES})