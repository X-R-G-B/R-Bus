cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_NAME_CLIENT r-type_client)
set(PROJECT_NAME_SERVER r-type_server)
set(PROJECT_NAME r-type)


find_program(CLANG_TIDY_EXE NAMES "clang-tidy")

if(CLANG_TIDY_EXE)
    set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "--fix" "--fix-notes" "--fix-errors" "-header-filter=(src/ECS/)|(src/Client/)|(src/Server/)" "-checks=-*,clang-diagnostic-*,clang-analyzer-*,-*,clang-analyzer-*,concurrency-*,cppcoreguidelines-*,modernize-*,portability-*,readability-*,-readability-identifier-length,-modernize-use-trailing-return-type")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(WITH_PIC "Build with position independent code" ON)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/deps/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(
    ${PROJECT_NAME}
    VERSION 1.0.0
    DESCRIPTION "r-type"
    LANGUAGES CXX
)

# if(MSVC)
#     set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
#     set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
# else()
#     set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#     set(BUILD_SHARED_LIBS OFF)
#     set(CMAKE_EXE_LINKER_FLAGS "-static")
# endif()

# ----------------------------------------------------------------------------

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets/" DESTINATION "share/${PROJECT_NAME_CLIENT}")


add_executable(
    ${PROJECT_NAME_CLIENT}
)
add_executable(
    ${PROJECT_NAME_SERVER}
)

add_subdirectory(deps)

if(MSVC)
    target_compile_options(
        ${PROJECT_NAME_CLIENT}
        PRIVATE
        /W4
    )
else()
    target_compile_options(
        ${PROJECT_NAME_CLIENT}
        PRIVATE
        -Wall -Wextra -pedantic
    )
endif()
if(MSVC)
    target_compile_options(
        ${PROJECT_NAME_SERVER}
        PRIVATE
        /W4
    )
else()
    target_compile_options(
        ${PROJECT_NAME_SERVER}
        PRIVATE
        -Wall -Wextra -pedantic
    )
endif()

add_subdirectory(src)

if (WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME_CLIENT} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME_CLIENT}>
        ${CMAKE_SOURCE_DIR}/${PROJECT_NAME_CLIENT}.exe
    )
else()
    add_custom_command(
        TARGET ${PROJECT_NAME_CLIENT} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME_CLIENT}>
        ${CMAKE_SOURCE_DIR}/${PROJECT_NAME_CLIENT}
    )
endif()
if (WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME_SERVER} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME_SERVER}>
        ${CMAKE_SOURCE_DIR}/${PROJECT_NAME_SERVER}.exe
    )
else()
    add_custom_command(
        TARGET ${PROJECT_NAME_SERVER} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME_SERVER}>
        ${CMAKE_SOURCE_DIR}/${PROJECT_NAME_SERVER}
    )
endif()

# ----------------------------------------------------------------------------
# CPACK

set(CPACK_PACKAGE_VENDOR "X-L-R-G-B")

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "R-Type")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

# NSIS
set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/assets/icons/favicon.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/assets/icons/favicon.ico")
set(CPACK_NSIS_PACKAGE_NAME "R-Type")

# DEBIAN
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR}")

set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
if(NOT WIN32)
    set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/assets/icons/favicon.bmp")
endif()
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "R-Type")
set(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME_CLIENT}" "${PROJECT_NAME_SERVER}")
set(CPACK_CREATE_DESKTOP_LINKS "${PROJECT_NAME_CLIENT}" "${PROJECT_NAME_SERVER}")


include(CPack)
