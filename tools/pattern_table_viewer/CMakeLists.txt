add_executable(pattern_table_viewer)

target_sources(pattern_table_viewer
	PRIVATE
		main.cpp
		patternTable.cpp
		window.cpp
)

target_sources(pattern_table_viewer
	INTERFACE
	FILE_SET HEADERS
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
	FILES
		patternTable.hpp
		window.hpp
)

target_link_libraries(pattern_table_viewer
	PUBLIC
		base
		cartridge

		nfd
		SDL2main
		SDL2-static
)

target_include_directories(pattern_table_viewer
	PRIVATE
		${CMAKE_SOURCE_DIR}/source
		${CMAKE_SOURCE_DIR}/libraries/sdl/include
		${CMAKE_SOURCE_DIR}/libraries/sdl_ttf/include
)
