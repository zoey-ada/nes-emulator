set(root_srcs
	ppu.hpp
	ppu.cpp
)
source_group(source FILES ${root_srcs})

set(all_srcs
	${root_srcs}
)

add_library(ppu STATIC ${all_srcs})

target_link_libraries(ppu
	memory
)

target_include_directories(ppu
	PRIVATE
	${CMAKE_SOURCE_DIR}/source
)
