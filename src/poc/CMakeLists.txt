cmake_minimum_required(VERSION 3.15)

if(MSVC)
    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE
        /W4
    )
else()
    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE
        -Wall -Wextra -pedantic
    )
endif()

add_subdirectory(ECS)
add_subdirectory(Client)
