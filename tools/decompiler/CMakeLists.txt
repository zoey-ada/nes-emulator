add_executable(decompiler)

target_sources(decompiler
	PRIVATE
		app.cpp
		main.cpp
		codeViewer.cpp
		decompiler.cpp
		operations.cpp
)

target_sources(decompiler
	INTERFACE
	FILE_SET HEADERS
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
	FILES
		app.hpp
		codeViewer.hpp
		decompiler.hpp
		memoryWrapper.hpp
		operations.hpp
)

target_link_libraries(decompiler
	PUBLIC
		base
		cartridge
		cpu
		platform
)

target_include_directories(decompiler
	PRIVATE
		${CMAKE_SOURCE_DIR}/source
		${CMAKE_SOURCE_DIR}/libraries/sdl/include
		${CMAKE_SOURCE_DIR}/libraries/sdl_ttf/include
)
