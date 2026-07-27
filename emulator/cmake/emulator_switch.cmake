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

set(EMULATOR_SWITCH_INTERMEDIATES_DIR "${CMAKE_BINARY_DIR}/intermediates")
set(EMULATOR_SWITCH_NACP
    "${EMULATOR_SWITCH_INTERMEDIATES_DIR}/emulator.nacp")

add_custom_command(
    OUTPUT "${EMULATOR_SWITCH_NACP}"
    COMMAND "${NACPTOOL_BIN}"
    --create
    "Casio Calculator Emulator"
    "amydevs"
    "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.0"
    "${EMULATOR_SWITCH_NACP}"
    VERBATIM
)

add_custom_target(emulator-nacp
    DEPENDS "${EMULATOR_SWITCH_NACP}")

add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/emulator.nro"
    COMMAND "${ELF2NRO_BIN}"
    "$<TARGET_FILE:emulator>"
    "${CMAKE_BINARY_DIR}/emulator.nro"
    "--nacp=${EMULATOR_SWITCH_NACP}"
    DEPENDS emulator "${EMULATOR_SWITCH_NACP}"
    VERBATIM
)

add_custom_target(emulator-nro
    DEPENDS "${CMAKE_BINARY_DIR}/emulator.nro")
