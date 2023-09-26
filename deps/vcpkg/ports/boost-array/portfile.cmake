# Automatically generated by scripts/boost/generate-ports.ps1

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO boostorg/array
    REF boost-1.83.0
    SHA512 3f1ec923d440139f57a5b7ec652501dad2cb0b6317cf7f064db2fddcc126fcad63e8b6c95b44b50b3e0c0b457245e386540e14d5e881cc611d6f2ce6fe395d00
    HEAD_REF master
)

include(${CURRENT_INSTALLED_DIR}/share/boost-vcpkg-helpers/boost-modular-headers.cmake)
boost_modular_headers(SOURCE_PATH ${SOURCE_PATH})