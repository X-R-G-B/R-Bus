#!/bin/bash
# Format c++ files and exit with error if files are not formatted

ARGS=

if [ "$1" == "--dry-run" ]; then
    ARGS="--dry-run --Werror"
fi

files=$(find src/ -name '*.cpp' -o -name '*.hpp')

clang-format --style=file $ARGS -i $files
