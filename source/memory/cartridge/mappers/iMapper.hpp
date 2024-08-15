#pragma once

#include <cstdint>
#include <string>

enum class MemoryDevice
{
	ConsoleRam,
	CartridgeRam,
	CartridgeRom
};

struct MemoryLocation
{
	MemoryDevice device;
	uint16_t address;
};

class IMapper
{
public:
	virtual ~IMapper() = default;

	virtual MemoryLocation translate_program_address(uint16_t address) const = 0;
	virtual MemoryLocation translate_character_address(uint16_t address) const = 0;
};
