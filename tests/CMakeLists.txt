add_executable(nes_emulator_tests)

target_link_libraries(nes_emulator_tests
	PRIVATE
		Catch2::Catch2WithMain
		cpu
)

target_sources(nes_emulator_tests
	PRIVATE
		mockMemory.hpp
		mockMemory.cpp

		cpu/arithmetic_operations.cpp
		cpu/branch_operations.cpp
		cpu/load_operations.cpp
		cpu/logical_operations.cpp
		cpu/shift_operations.cpp
		cpu/status_register_operations.cpp
		cpu/stack_operations.cpp
		cpu/store_operations.cpp
		cpu/subroutine_operations.cpp
		cpu/transfer_operations.cpp
)

target_include_directories(nes_emulator_tests
	PRIVATE
		${CMAKE_SOURCE_DIR}/source
)

include(cTest)
catch_discover_tests(nes_emulator_tests)
