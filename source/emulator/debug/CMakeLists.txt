target_sources(emulator
	PRIVATE
		cpuRenderer.cpp
		debugPpu.cpp
		palette.cpp
		patternTable.cpp
)

target_sources(emulator
	INTERFACE
	FILE_SET HEADERS
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../..
	FILES
		cpuRenderer.hpp
		debugPpu.hpp
		palette.hpp
		patternTable.hpp
)
