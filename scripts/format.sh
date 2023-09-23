#!/bin/bash
# Format c++ files and exit with error if files are not formatted

files=$(find . -name '*.cpp' -o -name '*.hpp')

clang-format --style=file -i 
