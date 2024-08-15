set(root_srcs
	addressing_modes.hpp
	alu.hpp
	alu.cpp
	cpu.hpp
	cpu.cpp
	debugCpu.hpp
	debugCpu.cpp
	operations.hpp
	operations.cpp
)
source_group(source FILES ${root_srcs})

set(all_srcs
	${root_srcs}
)

add_library(cpu STATIC ${all_srcs})

target_link_libraries(cpu
	base
	ppu
	memory
)

target_include_directories(cpu
	PRIVATE
		${CMAKE_SOURCE_DIR}/source
)
