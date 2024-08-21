target_sources(cartridge
	PRIVATE
		mapper0.cpp
)

target_sources(cartridge
	INTERFACE
	FILE_SET HEADERS
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../..
	FILES
		iMapper.hpp
		mapper0.hpp
)
