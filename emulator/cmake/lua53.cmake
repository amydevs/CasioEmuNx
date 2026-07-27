include(FetchContent)

FetchContent_Declare(
    lua53
    GIT_REPOSITORY https://github.com/walterschell/Lua.git
    GIT_TAG v5.3.3
)

FetchContent_MakeAvailable(lua53)

if(NINTENDO_SWITCH)
    # clear properties to get rid of LUA_USE_POSIX
    set_property(TARGET lua_static PROPERTY COMPILE_DEFINITIONS)
    target_compile_definitions(lua_static PUBLIC LUA_C89_NUMBERS)
endif()