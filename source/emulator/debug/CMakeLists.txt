target_sources(emulator
	PRIVATE
		cpuRecorder.cpp
		cpuRenderer.cpp
		debugCpu.cpp
		debugDma.cpp
		debugPpu.cpp
		debugWindow.cpp
		decompiler.cpp
		nametable.cpp
		operations.cpp
		palette.cpp
		paletteRenderer.cpp
		patternTable.cpp
		spriteTable.cpp
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
		debugDma.hpp
		debugPpu.hpp
		debugWindow.hpp
		decompiler.hpp
		nametable.hpp
		operations.hpp
		palette.hpp
		paletteRenderer.hpp
		patternTable.hpp
		spriteTable.hpp
)
