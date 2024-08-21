target_sources(emulator
	PRIVATE
		cpuRenderer.cpp
		debugCpu.cpp
		debugPpu.cpp
		operations.cpp
		palette.cpp
		patternTable.cpp
)

target_sources(emulator
	INTERFACE
	FILE_SET HEADERS
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../..
	FILES
		cpuRenderer.hpp
		debugCpu.hpp
		debugPpu.hpp
		operations.hpp
		palette.hpp
		patternTable.hpp
)
