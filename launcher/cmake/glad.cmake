if(NINTENDO_SWITCH)
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(GLAD REQUIRED libglad)
else()
    include(FetchContent)

    set(GLAD_API "gl=4.3" CACHE STRING "API type/version pairs" FORCE)

    FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG        v0.1.27
    )

    FetchContent_MakeAvailable(glad)
endif()


