cmake_minimum_required(VERSION 3.15)

target_include_directories(
    ${PROJECT_NAME_CLIENT}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(
    ${PROJECT_NAME_CLIENT}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/GraphicsSystems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AudioSystems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SpriteSystems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TextSystems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DeathSystems.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ParallaxSystems.cpp
)
