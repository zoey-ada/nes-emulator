#include "mapper0.hpp"

#include <cassert>

Mapper0::Mapper0(const Mapper0Options& options)
	: _has_second_bank(options.has_two_banks),
	  _has_vertical_nametable_mirroring(options.has_vertical_nametable_mirroring),
	  _uses_character_ram(options.uses_character_ram)
{}

// CPU $6000-$7FFF: Family Basic only: PRG RAM, mirrored as necessary to fill entire 8 KiB window,
// 	write protectable with an external switch
// CPU $8000-$BFFF: First 16 KB of ROM.
// CPU $C000-$FFFF: Last 16 KB of ROM (NROM-256) or mirror of $8000-$BFFF (NROM-128).
MemoryLocation Mapper0::translate_program_address(uint16_t address) const
{
	if (address < 0x6000)
	{
		throw std::exception("Invalid program address received");
	}
	if (address < 0x8000)
	{
		uint16_t mapped_addr = address - 0x6000;
		return {MemoryDevice::CartridgeRam, mapped_addr};
	}
	if (this->_has_second_bank)
	{
		uint16_t mapped_addr = address - 0x8000;
		return {MemoryDevice::CartridgeRom, mapped_addr};
	}
	else
	{
		uint16_t mapped_addr = (address - 0x8000) % 0x4000;
		return {MemoryDevice::CartridgeRom, mapped_addr};
	}
}

MemoryLocation Mapper0::translate_character_address(uint16_t address) const
{
	if (address < 0x2000)
	{
		if (this->_uses_character_ram)
			return {MemoryDevice::CartridgeRam, address};
		else
			return {MemoryDevice::CartridgeRom, address};
	}
	else if (address > 0x3eff)
	{
		throw std::exception("Invalid character address received");
	}
	else if (this->_has_vertical_nametable_mirroring)
	{
		uint16_t mapped_addr = (address - 0x2000) % 0x0800;
		return {MemoryDevice::ConsoleRam, mapped_addr};
	}
	else
	{
		if (address < 0x2800)
		{
			uint16_t mapped_addr = (address - 0x2000) % 0x0400;
			return {MemoryDevice::ConsoleRam, mapped_addr};
		}
		else if (address < 0x3000)
		{
			uint16_t mapped_addr = ((address - 0x2000) % 0x0400) + 0x0400;
			return {MemoryDevice::ConsoleRam, mapped_addr};
		}
		if (address < 0x3800)
		{
			uint16_t mapped_addr = (address - 0x3000) % 0x0400;
			return {MemoryDevice::ConsoleRam, mapped_addr};
		}
		else
		{
			uint16_t mapped_addr = ((address - 0x3000) % 0x0400) + 0x0400;
			return {MemoryDevice::ConsoleRam, mapped_addr};
		}
	}
}
