cmake_minimum_required(VERSION 3.15)

include(FetchContent)

FetchContent_Declare(
    json
    CMAKE_ARGS "-DCMAKE_BUILD_TYPE=Release" "-DCMAKE_CONFIGURATION_TYPES=\"Release;Release\"" "-DCONFIG=Release"
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS FALSE
)
FetchContent_MakeAvailable(json)

target_link_libraries(${PROJECT_NAME_CLIENT} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME_SERVER} PRIVATE nlohmann_json::nlohmann_json)
