add_library(cpu STATIC)

target_sources(cpu
	PRIVATE
		alu.cpp
		cpu.cpp
		memoryMapper.cpp
)

target_sources(cpu
	INTERFACE
	FILE_SET HEADERS
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
	FILES
		addressing_modes.hpp
		alu.hpp
		cpu.hpp
		memoryMapper.hpp
)

target_link_libraries(cpu
	PUBLIC
		base
		cartridge
		input
)

target_include_directories(cpu
	PRIVATE
		${CMAKE_SOURCE_DIR}/source
)
