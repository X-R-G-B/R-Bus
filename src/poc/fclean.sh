#!/bin/bash

[ -f Makefile ] && echo "Running make clean..." && make clean

echo "Finding files..."
FILES=$(find . -name "*-prefix" -o -name "CMakeFiles" -o -name "CMakeCache.txt" -o -name "cmake_install.cmake" -o -name "Makefile" -o -name "_deps" -o -name "poc")
echo "Files to clean:"
echo "$FILES"

if [[ "$1" != "-y" ]]; then
    echo "Do you want to rm? (y/n)"
    read -r ANSWER
else
    ANSWER="y"
fi

if [ "$ANSWER" == "y" ]; then
    echo "Removing files..."
    rm -rf $FILES
    echo "Done!"
else
    echo "Canceled!"
fi