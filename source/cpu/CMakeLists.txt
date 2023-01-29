set(root_srcs
	addressing_modes.hpp
	alu.hpp
	alu.cpp
	cpu.hpp
	cpu.cpp
	iMemory.hpp
	randomAccessMemory.hpp
	randomAccessMemory.cpp
)
source_group(source FILES ${root_srcs})

set(all_srcs
	${root_srcs}
)

add_library(cpu STATIC ${all_srcs})
