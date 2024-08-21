add_library(cartridge STATIC)

target_sources(cartridge
	PRIVATE
		cartridge.cpp
		cartridgeLoader.cpp
		inesHeader.cpp
		inesHeader_1_0.cpp
		inesHeader_2_0.cpp
)

target_sources(cartridge
	INTERFACE
	FILE_SET HEADERS
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
	FILES
		cartridgeLoader.hpp
		cartridge.hpp
		inesHeader.hpp
		inesHeader_1_0.hpp
		inesHeader_2_0.hpp
)

add_subdirectory(mappers)

target_link_libraries(cartridge
	PUBLIC
		base
)
