target_sources(platform
	PRIVATE
		sdlSubWindow.cpp
		sdlWindow.cpp
)

target_sources(platform
	INTERFACE
	FILE_SET HEADERS
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../..
	FILES
		iSubWindow.hpp
		iWindow.hpp
		sdlSubWindow.hpp
		sdlWindow.hpp
)
