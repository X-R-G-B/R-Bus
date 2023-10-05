cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME_CLIENT r-type_client)
set(PROJECT_NAME_SERVER r-type_server)
set(PROJECT_NAME r-type)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
SET(CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE} CACHE STRING "" FORCE)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(WITH_PIC "Build with position independent code" ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(
    ${PROJECT_NAME}
    VERSION 1.0.0
    DESCRIPTION "r-type"
    LANGUAGES CXX
)

include(GNUInstallDirs)

# if(MSVC)
#     set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
# else()
#     set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
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

install(TARGETS ${PROJECT_NAME_CLIENT}
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
install(TARGETS ${PROJECT_NAME_SERVER}
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)

add_subdirectory(deps)

# Clang-Tidy

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")

if(CLANG_TIDY_EXE)
    if (MSVC)
	if (NOT DEFINED NO_USE_CLANG_TIDY)
        add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/.clang-tidy
            COMMAND ${CMAKE_COMMAND} -E copy
	    ${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy
	    ${CMAKE_BINARY_DIR}/.clang-tidy
        )
        set_target_properties(${PROJECT_NAME_CLIENT} PROPERTIES
            VS_GLOBAL_RunCodeAnalysis true
            VS_GLOBAL_EnableClangTidyCodeAnalysis true
            VS_GLOBAL_ClangTidyChecks "-checks=-*,clang-diagnostic-*,clang-analyzer-*,-*,clang-analyzer-*,concurrency-*,cppcoreguidelines-*,modernize-*,portability-*,readability-*,-readability-identifier-length,-modernize-use-trailing-return-type"
        )
        set_target_properties(${PROJECT_NAME_SERVER} PROPERTIES
            VS_GLOBAL_RunCodeAnalysis true
            VS_GLOBAL_EnableClangTidyCodeAnalysis true
            VS_GLOBAL_ClangTidyChecks "-checks=-*,clang-diagnostic-*,clang-analyzer-*,-*,clang-analyzer-*,concurrency-*,cppcoreguidelines-*,modernize-*,portability-*,readability-*,-readability-identifier-length,-modernize-use-trailing-return-type"
        )
        endif()
    else()
        set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "--fix" "--fix-notes" "--fix-errors")
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
    endif()
endif()

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
    add_custom_command(
        TARGET ${PROJECT_NAME_CLIENT} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME_CLIENT}>
        "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME_CLIENT}.exe"
    )
    add_custom_command(
        TARGET ${PROJECT_NAME_CLIENT} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME_CLIENT}>
        "${CMAKE_BINARY_DIR}/Release/${PROJECT_NAME_CLIENT}.exe"
    )
else()
    add_custom_command(
        TARGET ${PROJECT_NAME_CLIENT} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME_CLIENT}>
        ${CMAKE_SOURCE_DIR}/${PROJECT_NAME_CLIENT}
    )
    add_custom_command(
        TARGET ${PROJECT_NAME_CLIENT} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME_CLIENT}>
        "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME_CLIENT}"
    )
endif()
if (WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME_SERVER} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME_SERVER}>
        ${CMAKE_SOURCE_DIR}/${PROJECT_NAME_SERVER}.exe
    )
    add_custom_command(
        TARGET ${PROJECT_NAME_SERVER} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME_SERVER}>
        "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME_SERVER}.exe"
    )
    add_custom_command(
        TARGET ${PROJECT_NAME_CLIENT} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME_CLIENT}>
        "${CMAKE_BINARY_DIR}/Release/${PROJECT_NAME_SERVER}.exe"
    )
else()
    add_custom_command(
        TARGET ${PROJECT_NAME_SERVER} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME_SERVER}>
        ${CMAKE_SOURCE_DIR}/${PROJECT_NAME_SERVER}
    )
    add_custom_command(
        TARGET ${PROJECT_NAME_SERVER} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME_SERVER}>
        "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME_SERVER}"
    )
endif()

# ----------------------------------------------------------------------------
# CPACK

set(CPACK_PACKAGE_VENDOR "X-L-R-G-B")

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "R-Type")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CMAKE_SYSTEM_NAME}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CMAKE_SYSTEM_NAME}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

# NSIS
set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/assets/icons/favicon.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/assets/icons/favicon.ico")
set(CPACK_NSIS_PACKAGE_NAME "R-Type")

set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
if(NOT WIN32)
    set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/assets/icons/favicon.bmp")
endif()
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "R-Type")
set(CPACK_CREATE_DESKTOP_LINKS ${PROJECT_NAME_CLIENT} ${PROJECT_NAME_SERVER})
set(CPACK_PACKAGE_EXECUTABLES ${PROJECT_NAME_CLIENT} "RType Client" ${PROJECT_NAME_SERVER} "RType Server")

list(APPEND CPACK_SOURCE_IGNORE_FILES "/\.git/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/CMakeFiles/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/_CPack_Packages/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/bin(ary)?/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/build/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/dist/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/libs?/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/logs?/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/objs?/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/[tT]e?mp/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/___pycache__")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/\.cache")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/\.gitignore.*")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/CMakeCache.txt")
list(APPEND CPACK_SOURCE_IGNORE_FILES "\.pyc$")
list(APPEND CPACK_SOURCE_IGNORE_FILES "\.swp$")
list(APPEND CPACK_SOURCE_IGNORE_FILES "~$")


include(CPack)
