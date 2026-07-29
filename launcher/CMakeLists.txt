cmake_minimum_required(VERSION 3.10)
project(casiolauncher VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(glad)
include(imgui)
if(NOT NINTENDO_SWITCH)
    include(whereami)
endif()

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)



# Define the executable
add_executable(casiolauncher
    src/casiolauncher.cpp
)

# Link libraries
target_link_libraries(casiolauncher
    Threads::Threads
    imgui
    ${SDL2_LIBRARIES}
    ${GLAD_LIBRARIES}
)

if(NOT NINTENDO_SWITCH)
    target_link_libraries(casiolauncher
        whereami
    )
endif()

# Include directories
target_include_directories(casiolauncher PRIVATE
    ${SDL2_INCLUDE_DIRS}
)

# Add compile flags if needed
target_compile_options(casiolauncher PRIVATE
    ${SDL2_CFLAGS_OTHER}
)

include(GNUInstallDirs)
install(TARGETS casiolauncher
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

include(casiolauncher_switch)