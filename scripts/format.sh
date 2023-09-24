#!/bin/bash
# Format c++ files and exit with error if files are not formatted

set -ex

ARGS=

if [ "$1" == "--dry-run" ]; then
    ARGS="--dry-run --Werror"
fi

find src/ \( -name '*.cpp' -o -name '*.hpp' \) -print0 | xargs -0 clang-format --style=file $ARGS -i

# found=$(find src/ \( -name '*.cpp' -o -name '*.hpp' \) -print0 | xargs -0 grep -e '"raylib.h"' -e '"raylib.hpp"')
# if [ -n "$found" ]; then
#     echo "$found"
#     exit 1
# fi
