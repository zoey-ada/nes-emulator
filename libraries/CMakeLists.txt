# catch
add_subdirectory(catch)
include(catch/extras/Catch.cmake)

add_subdirectory(sdl)

set(SDL2TTF_INSTALL off)
set(BUILD_SHARED_LIBS off)
add_subdirectory(sdl_ttf)
