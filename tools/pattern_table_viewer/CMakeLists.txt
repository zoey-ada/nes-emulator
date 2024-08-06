set(root_srcs
	main.cpp

	patternTable.hpp
	patternTable.cpp

	window.hpp
	window.cpp
)
source_group(source FILES ${root_srcs})

set(all_srcs
	${root_srcs}
)

add_executable(pattern_table_viewer ${all_srcs})
target_link_libraries(pattern_table_viewer
	PUBLIC
		memory

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
