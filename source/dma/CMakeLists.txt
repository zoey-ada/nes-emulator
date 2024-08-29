add_library(dma STATIC)

target_sources(dma
	PRIVATE
		dma.cpp
)

target_sources(dma
	INTERFACE
	FILE_SET HEADERS
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
	FILES
		dma.hpp
)

target_link_libraries(dma
	PUBLIC
		base
		cpu
		ppu
)

target_include_directories(dma
	PRIVATE
		${CMAKE_SOURCE_DIR}/source
)
