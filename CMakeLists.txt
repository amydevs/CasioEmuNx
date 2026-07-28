cmake_minimum_required(VERSION 3.10)
project(casiobundle VERSION 1.0.0)

add_subdirectory(emulator)
add_subdirectory(launcher)

include(GNUInstallDirs)
install(DIRECTORY models DESTINATION ${CMAKE_INSTALL_DATADIR}/casioemu)

if(NINTENDO_SWITCH)
    add_custom_target(casiobundle-nro
        DEPENDS casioemu-nro casiolauncher-nro
    )

    add_custom_command(TARGET casiobundle-nro POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/out"
        COMMENT "Creating output directory for sdmc:/switch/.../"
        VERBATIM
    )

    add_custom_command(TARGET casiobundle-nro POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/emulator/casioemu.nro"
            "${CMAKE_BINARY_DIR}/out/"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/launcher/casiolauncher.nro"
            "${CMAKE_BINARY_DIR}/out/"
        COMMENT "Copying NRO files for sdmc:/switch/.../"
        VERBATIM
    )
    add_custom_command(TARGET casiobundle-nro POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/models"
            "${CMAKE_BINARY_DIR}/out/models"
        COMMENT "Copying model files for sdmc:/switch/.../"
        VERBATIM
    )
endif()