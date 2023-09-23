#!/bin/bash
# Compil using cmake

set -ex

cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

if [[ "$1" == "--dry-run" ]]; then
    find src/ \( -name '*.cpp' -o -name '*.hpp' \) -print0 \
        | xargs -0 \
            clang-tidy \
            -format-style=file \
            -header-filter=. \
            -p build \
            -checks=-*,clang-analyzer-*,concurrency-*,cppcoreguidelines-*,modernize-*,portability-*,readability-*
fi

cmake --build build
