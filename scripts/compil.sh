#!/bin/bash
# Compil using cmake

set -ex

cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

if [[ "$1" == "--dry-run" ]]; then
    find src/ \( -name '*.cpp' -o -name '*.hpp' \) -print0 \
        | xargs -0 \
            clang-tidy -format-style=file -p build --fix --fix-notes --fix-errors
fi

cmake --build build
