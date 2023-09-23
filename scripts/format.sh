#!/bin/bash
# Format c++ files and exit with error if files are not formatted

ARGS=

if [ "$1" == "--dry-run" ]; then
    ARGS="--dry-run --Werror"
fi

find src/ \( -name '*.cpp' \) -print0 | xargs -0 clang-format --style=file $ARGS -i
