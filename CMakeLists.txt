cmake_minimum_required(VERSION 3.21)

project(
	NES_Emulator
	VERSION 0.1
	DESCRIPTION "NES emulator"
	LANGUAGES CXX
)

include(cmake/config.cmake)

add_subdirectory(libraries)
add_subdirectory(source/cpu)
add_subdirectory(tests)

add_subdirectory(source/emulator)
