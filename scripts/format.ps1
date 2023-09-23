#!powershell
# Format c++ files and exit with error if files are not formatted

$files = Get-Childitem –Path C:\ -Include *.cpp,*.hpp -Recurse -ErrorAction SilentlyContinue

clang-format --style=file -i $files
