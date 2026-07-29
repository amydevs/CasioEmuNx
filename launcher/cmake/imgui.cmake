include(FetchContent)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.9
)

FetchContent_MakeAvailable(imgui)

add_library(imgui)

target_include_directories(imgui
    PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_sources(
    imgui
    PUBLIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

set_source_files_properties(
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    PROPERTIES
        COMPILE_DEFINITIONS IMGUI_IMPL_OPENGL_LOADER_CUSTOM
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set_source_files_properties(
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        PROPERTIES
            COMPILE_OPTIONS "-include;glad/glad.h"
    )
elseif(MSVC)
    set_source_files_properties(
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        PROPERTIES
            COMPILE_OPTIONS "/FIglad/glad.h"
    )
endif()

target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_CUSTOM)

if(NINTENDO_SWITCH)
    target_compile_definitions(imgui PUBLIC
        IMGUI_DISABLE_DEFAULT_SHELL_FUNCTIONS
    )
endif()

target_link_libraries(imgui PUBLIC
    ${SDL2_LIBRARIES}
    ${GLAD_LIBRARIES}
)
target_include_directories(imgui PUBLIC
    ${SDL2_INCLUDE_DIRS}
)
target_compile_options(imgui PRIVATE
    ${SDL2_CFLAGS_OTHER}
)
