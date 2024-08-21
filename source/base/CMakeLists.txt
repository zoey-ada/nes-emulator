add_library(base STATIC)

target_sources(base
	PRIVATE
		randomAccessMemory.cpp
		slowMemory.cpp
)

target_sources(base
	INTERFACE
	FILE_SET HEADERS
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
	FILES
		color.hpp
		iCpu.hpp
		iMemory.hpp
		iPpu.hpp
		pixel.hpp
		randomAccessMemory.hpp
		register.hpp
		slowMemory.hpp
)
