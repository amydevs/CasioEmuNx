cmake_minimum_required(VERSION 3.10)
project(casioemu VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(lua53)
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_image REQUIRED SDL2_image)
if(NOT NINTENDO_SWITCH)
    pkg_check_modules(READLINE REQUIRED readline)
endif()

# Define the executable
add_executable(casioemu
    src/Logger.cpp
    src/Peripheral/ROMWindow.cpp
    src/Peripheral/Timer.cpp
    src/Peripheral/Screen.cpp
    src/Peripheral/StandbyControl.cpp
    src/Peripheral/Keyboard.cpp
    src/Peripheral/Miscellaneous.cpp
    src/Peripheral/Peripheral.cpp
    src/Peripheral/BatteryBackedRAM.cpp
    src/Data/ModelInfo.cpp
    src/casioemu.cpp
    src/Emulator.cpp
    src/Chipset/CPUPushPop.cpp
    src/Chipset/CPU.cpp
    src/Chipset/InterruptSource.cpp
    src/Chipset/Chipset.cpp
    src/Chipset/CPUArithmetic.cpp
    src/Chipset/MMU.cpp
    src/Chipset/CPUControl.cpp
    src/Chipset/MMURegion.cpp
    src/Chipset/CPULoadStore.cpp
)

# Link libraries
target_link_libraries(casioemu
    Threads::Threads
    lua_static
    ${SDL2_LIBRARIES}
    ${SDL2_image_LIBRARIES}
    ${READLINE_LIBRARIES}
)

# Include directories
target_include_directories(casioemu PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_image_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIRS}
    ${READLINE_INCLUDE_DIRS}
)

# Add compile flags if needed
target_compile_options(casioemu PRIVATE
    ${SDL2_CFLAGS_OTHER}
    ${SDL2_image_CFLAGS_OTHER}
    ${LUA_CFLAGS_OTHER}
    ${READLINE_CFLAGS_OTHER}
)

include(GNUInstallDirs)
install(TARGETS casioemu
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

include(casioemu_switch)