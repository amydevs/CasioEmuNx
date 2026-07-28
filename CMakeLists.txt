cmake_minimum_required(VERSION 3.10)
project(casiobundle VERSION 1.0.0)

add_subdirectory(emulator)
add_subdirectory(launcher)

add_custom_target(casiobundle-models ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/models"
        "${CMAKE_BINARY_DIR}/models"
    COMMENT "Copying models folder to binary directory"
)

if(NINTENDO_SWITCH)
    add_custom_target(casiobundle-nro
        DEPENDS casioemu-nro casiolauncher-nro
    )
endif()