set(root_srcs
	iMemory.hpp
	memoryMapper.hpp
	memoryMapper.cpp
	ppuMemoryMapper.hpp
	ppuMemoryMapper.cpp
	randomAccessMemory.hpp
	randomAccessMemory.cpp
	slowMemory.hpp
	slowMemory.cpp
)
source_group(source FILES ${root_srcs})

set(cartridge_srcs
	cartridge/cartridge.hpp
	cartridge/cartridge.cpp
	cartridge/cartridgeLoader.hpp
	cartridge/cartridgeLoader.cpp
	cartridge/inesHeader.hpp
	cartridge/inesHeader.cpp
	cartridge/inesHeader_1_0.hpp
	cartridge/inesHeader_1_0.cpp
	cartridge/inesHeader_2_0.hpp
	cartridge/inesHeader_2_0.cpp
)
source_group(source\\cartridge FILES ${cartridge_srcs})

set(debugging_src
	debugging/patternTable.hpp
	debugging/patternTable.cpp
)
source_group(source\\debugging FILES ${debugging_src})

set(mappers_srcs
	cartridge/mappers/iMapper.hpp
	cartridge/mappers/mapper0.hpp
	cartridge/mappers/mapper0.cpp
)
source_group(source\\cartridge\\mappers FILES ${mappers_srcs})

set(all_srcs
	${root_srcs}
	${cartridge_srcs}
	${debugging_src}
	${mappers_srcs}
)

add_library(memory STATIC ${all_srcs})

target_link_libraries(memory
	PRIVATE
		base
)
