target_sources(emulator
	PRIVATE
		cpuRecorder.cpp
		cpuRenderer.cpp
		debugCpu.cpp
		debugPpu.cpp
		decompiler.cpp
		operations.cpp
		palette.cpp
		paletteRenderer.cpp
		patternTable.cpp
)

target_sources(emulator
	INTERFACE
	FILE_SET HEADERS
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../..
	FILES
		cpuRecorder.hpp
		cpuRenderer.hpp
		cpuStackFrame.hpp
		debugCpu.hpp
		debugPpu.hpp
		decompiler.hpp
		operations.hpp
		palette.hpp
		paletteRenderer.hpp
		patternTable.hpp
)
