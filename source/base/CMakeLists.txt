add_library(base INTERFACE ${all_srcs})

target_sources(base
	INTERFACE
	FILE_SET HEADERS
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
	FILES
		pixel.hpp
)
