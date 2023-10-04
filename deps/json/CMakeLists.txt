cmake_minimum_required(VERSION 3.15)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(${PROJECT_NAME_CLIENT} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME_SERVER} PRIVATE nlohmann_json::nlohmann_json)