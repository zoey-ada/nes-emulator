add_library(platform STATIC)

target_sources(platform
	PRIVATE
		sdlAlternateRenderTarget.cpp
		sdlRenderer.cpp
)

target_sources(platform
	INTERFACE
	FILE_SET HEADERS
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
	FILES
		iAlternateRenderTarget.hpp
		iRenderer.hpp
		sdlAlternateRenderTarget.hpp
		sdlRenderer.hpp
)

add_subdirectory(window)

target_link_libraries(platform
	PUBLIC
		base

		nfd
		SDL2-static
		SDL2_ttf

	INTERFACE
		SDL2main
)

target_include_directories(platform
	PRIVATE
		${CMAKE_SOURCE_DIR}/source
		# ${CMAKE_SOURCE_DIR}/libraries/sdl/include
		# ${CMAKE_SOURCE_DIR}/libraries/sdl_ttf/include
)
