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

set(CASIOEMU_SWITCH_INTERMEDIATES_DIR "${CMAKE_BINARY_DIR}/intermediates")
set(CASIOEMU_SWITCH_NACP
    "${CASIOEMU_SWITCH_INTERMEDIATES_DIR}/casioemu.nacp")

add_custom_command(
    OUTPUT "${CASIOEMU_SWITCH_NACP}"
    COMMAND "${NACPTOOL_BIN}"
    --create
    "Casio Calculator Emulator"
    "amydevs"
    "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.0"
    "${CASIOEMU_SWITCH_NACP}"
    VERBATIM
)

add_custom_target(casioemu-nacp
    DEPENDS "${CASIOEMU_SWITCH_NACP}")

add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/casioemu.nro"
    COMMAND "${ELF2NRO_BIN}"
    "$<TARGET_FILE:casioemu>"
    "${CMAKE_BINARY_DIR}/casioemu.nro"
    "--nacp=${CASIOEMU_SWITCH_NACP}"
    DEPENDS casioemu "${CASIOEMU_SWITCH_NACP}"
    VERBATIM
)

add_custom_target(casioemu-nro
    DEPENDS "${CMAKE_BINARY_DIR}/casioemu.nro")
