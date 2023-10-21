cmake_minimum_required(VERSION 3.15)

target_include_directories(
    ${PROJECT_NAME_CLIENT}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(
    ${PROJECT_NAME_CLIENT}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main_client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/init.cpp
)

target_sources(
    ${PROJECT_NAME_SERVER}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main_server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/init.cpp
)

add_subdirectory(Client)
add_subdirectory(ECS)
add_subdirectory(Game)
add_subdirectory(Nitwork)
add_subdirectory(Server)
