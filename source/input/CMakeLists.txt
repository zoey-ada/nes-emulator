add_library(input STATIC)

target_sources(input
	PRIVATE
		controller.cpp
)

target_sources(input
	INTERFACE
	FILE_SET HEADERS
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
	FILES
		controller.hpp
		iInput.hpp
)

target_link_libraries(input
	base
)

target_include_directories(input
	PRIVATE
		${CMAKE_SOURCE_DIR}/source
)
