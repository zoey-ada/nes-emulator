set(root_srcs
	iMemory.hpp
	memoryMapper.hpp
	memoryMapper.cpp
	randomAccessMemory.hpp
	randomAccessMemory.cpp
	slowMemory.hpp
	slowMemory.cpp
)
source_group(source FILES ${root_srcs})

set(all_srcs
	${root_srcs}
)

add_library(memory STATIC ${all_srcs})

target_include_directories(memory
	PRIVATE
		${CMAKE_SOURCE_DIR}/source
)
