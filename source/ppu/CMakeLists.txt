add_library(ppu STATIC)

target_sources(ppu
	PRIVATE
		systemPalette.cpp
		ppu.cpp
)

target_sources(ppu
	INTERFACE
	FILE_SET HEADERS
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
	FILES
		systemPalette.hpp
		ppu.hpp
)

target_link_libraries(ppu
	base
	memory
)

target_include_directories(ppu
	PRIVATE
		${CMAKE_SOURCE_DIR}/source
)
