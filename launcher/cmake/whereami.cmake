include(FetchContent)

FetchContent_Declare(
    whereami
    GIT_REPOSITORY https://github.com/gpakosz/whereami.git
    GIT_TAG master
)

FetchContent_MakeAvailable(whereami)

add_library(whereami)

target_include_directories(whereami
    PUBLIC
    ${whereami_SOURCE_DIR}/src
)

target_sources(
    whereami
    PUBLIC
    ${whereami_SOURCE_DIR}/src/whereami.c
)
