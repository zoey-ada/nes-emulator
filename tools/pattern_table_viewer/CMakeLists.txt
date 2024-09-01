add_executable(pattern_table_viewer)

target_sources(pattern_table_viewer
	PRIVATE
		app.cpp
		main.cpp
		patternTable.cpp
)

target_sources(pattern_table_viewer
	INTERFACE
	FILE_SET HEADERS
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
	FILES
		app.hpp
		patternTable.hpp
)

target_link_libraries(pattern_table_viewer
	PUBLIC
		base
		cartridge
		platform
)

target_include_directories(pattern_table_viewer
	PRIVATE
		${CMAKE_SOURCE_DIR}/source
		${CMAKE_SOURCE_DIR}/libraries/sdl/include
		${CMAKE_SOURCE_DIR}/libraries/sdl_ttf/include
)
