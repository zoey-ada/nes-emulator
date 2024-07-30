# catch
add_subdirectory(catch)
include(catch/extras/Catch.cmake)

add_subdirectory(sdl)

set(SDL2TTF_INSTALL off)
set(BUILD_SHARED_LIBS off)
set(SDL2TTF_VENDORED on)
add_subdirectory(sdl_ttf)
