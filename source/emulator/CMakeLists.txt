add_executable(emulator)

target_sources(emulator
	PRIVATE
		app.cpp
		main.cpp
		nes.cpp
		sdlController.cpp
)

target_sources(emulator
	INTERFACE
	FILE_SET HEADERS
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
	FILES
		app.hpp
		nes.hpp
		sdlController.hpp
)

add_subdirectory(debug)

target_link_libraries(emulator
	PUBLIC
		base
		cartridge
		cpu
		dma
		input
		platform
		ppu
)

target_include_directories(emulator
	PRIVATE
		${CMAKE_SOURCE_DIR}/source
		${CMAKE_SOURCE_DIR}/libraries/sdl/include
		${CMAKE_SOURCE_DIR}/libraries/sdl_ttf/include
)
