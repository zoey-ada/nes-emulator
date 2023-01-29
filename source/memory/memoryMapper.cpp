#include "memoryMapper.hpp"

#include <new>

MemoryMapper::MemoryMapper()
{
	this->_ram = new (std::nothrow) RandomAccessMemory(0x07ff);
}

MemoryMapper::~MemoryMapper()
{
	if (this->_ram != nullptr)
		delete this->_ram;
	this->_ram = nullptr;
}

// $0000–$07FF	$0800	2 KB internal RAM
// $0800–$0FFF	$0800	Mirrors of $0000–$07FF
// $1000–$17FF	$0800
// $1800–$1FFF	$0800
// $2000–$2007	$0008	NES PPU registers
// $2008–$3FFF	$1FF8	Mirrors of $2000–$2007 (repeats every 8 bytes)
// $4000–$4017	$0018	NES APU and I/O registers
// $4018–$401F	$0008	APU and I/O functionality that is normally disabled. See CPU Test Mode.
// $4020–$FFFF	$BFE0	Cartridge space: PRG ROM, PRG RAM, and mapper registers (see note)

uint8_t MemoryMapper::read(uint16_t address) const
{
	if (address < 0x2000)
	{
		auto ram_address = address % 0x0800;
		return this->_ram->read(ram_address);
	}
	else if (address < 0x4000)
	{
		auto ppu_register = (address - 0x1fff) % 8;
		return 0;
	}
	else if (address < 0x4018)
	{
		auto register_index = address - 0x3fff;
		return 0;
	}
	else if (address < 0x4020)
	{
		auto cpu_test_address = address - 0x4017;
		return 0;
	}
	else
	{
		// cartridge data
		return 0;
	}
}

void MemoryMapper::write(uint16_t address, const uint8_t data)
{
	if (address < 0x2000)
	{
		auto ram_address = address % 0x0800;
		this->_ram->write(ram_address, data);
	}
	else if (address < 0x4000)
	{
		auto ppu_register = (address - 0x1fff) % 8;
	}
	else if (address < 0x4018)
	{
		auto register_index = address - 0x3fff;
	}
	else if (address < 0x4020)
	{
		auto cpu_test_address = address - 0x4017;
	}
	else
	{
		// cartridge data
	}
}
