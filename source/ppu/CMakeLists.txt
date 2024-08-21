add_library(ppu STATIC)

target_sources(ppu
	PRIVATE
		ppu.cpp
		ppuMemoryMapper.cpp
		systemPalette.cpp
)

target_sources(ppu
	INTERFACE
	FILE_SET HEADERS
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
	FILES
		ppu.hpp
		ppuMemoryMapper.hpp
		systemPalette.hpp
)

target_link_libraries(ppu
	PUBLIC
		base
		cartridge
)

target_include_directories(ppu
	PRIVATE
		${CMAKE_SOURCE_DIR}/source
)
