if(NOT NINTENDO_SWITCH)
    return()
endif()

find_program(NACPTOOL_BIN
    NAMES nacptool)
find_program(ELF2NRO_BIN
    NAMES elf2nro)

foreach(tool NACPTOOL_BIN ELF2NRO_BIN)
    if(NOT ${tool} OR NOT EXISTS "${${tool}}")
        message(FATAL_ERROR "Required Switch packaging tool not found: ${tool}")
    endif()
endforeach()

set(CASIOLAUNCHER_SWITCH_INTERMEDIATES_DIR "${CMAKE_BINARY_DIR}/intermediates")
set(CASIOLAUNCHER_SWITCH_NACP
    "${CASIOLAUNCHER_SWITCH_INTERMEDIATES_DIR}/casiolauncher.nacp")

add_custom_command(
    OUTPUT "${CASIOLAUNCHER_SWITCH_NACP}"
    COMMAND "${NACPTOOL_BIN}"
    --create
    "Casio Calculator Launcher"
    "amydevs"
    "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.0"
    "${CASIOLAUNCHER_SWITCH_NACP}"
    VERBATIM
)

add_custom_target(casiolauncher-nacp
    DEPENDS "${CASIOLAUNCHER_SWITCH_NACP}")

add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/casiolauncher.nro"
    COMMAND "${ELF2NRO_BIN}"
    "$<TARGET_FILE:casiolauncher>"
    "${CMAKE_BINARY_DIR}/casiolauncher.nro"
    "--nacp=${CASIOLAUNCHER_SWITCH_NACP}"
    DEPENDS casiolauncher "${CASIOLAUNCHER_SWITCH_NACP}"
    VERBATIM
)

add_custom_target(casiolauncher-nro
    DEPENDS "${CMAKE_BINARY_DIR}/casiolauncher.nro")
