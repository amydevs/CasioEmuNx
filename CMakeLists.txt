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
endif()