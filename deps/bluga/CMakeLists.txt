include(FetchContent)

FetchContent_Declare(
        b-luga
        URL "https://github.com/X-R-G-B/B-luga/releases/latest/download/b-luga.tar"
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(b-luga)

target_link_libraries(
        ${PROJECT_NAME_CLIENT}
        PRIVATE
        b-luga
)

target_link_libraries(
        ${PROJECT_NAME_SERVER}
        PRIVATE
        b-luga
)
