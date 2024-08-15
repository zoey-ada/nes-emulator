set(root_srcs
	main.cpp

	cpuRenderer.hpp
	cpuRenderer.cpp
	nes.hpp
	nes.cpp
	window.hpp
	window.cpp
)
source_group(source FILES ${root_srcs})

set(all_srcs
	${root_srcs}
)

add_executable(emulator ${all_srcs})
target_link_libraries(emulator
	PUBLIC
		base
		cpu
		memory
		ppu

		nfd
		SDL2main
		SDL2-static
		SDL2_ttf
)

target_include_directories(emulator
	PRIVATE
		${CMAKE_SOURCE_DIR}/source
		${CMAKE_SOURCE_DIR}/libraries/sdl/include
		${CMAKE_SOURCE_DIR}/libraries/sdl_ttf/include
)
