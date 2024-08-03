#include "ppuMemoryMapper.hpp"

#include <new>

#include "cartridge/cartridge.hpp"
#include "randomAccessMemory.hpp"

PpuMemoryMapper::PpuMemoryMapper()
{
	this->_ram = new (std::nothrow) RandomAccessMemory(0x00ff);
}

PpuMemoryMapper::~PpuMemoryMapper()
{
	if (this->_ram != nullptr)
		delete this->_ram;
	this->_ram = nullptr;
	this->_cartridge = nullptr;
}

// $0000-$0FFF	$1000	Pattern table 0			Cartridge
// $1000-$1FFF	$1000	Pattern table 1			Cartridge
// $2000-$23FF	$0400	Nametable 0				Cartridge
// $2400-$27FF	$0400	Nametable 1				Cartridge
// $2800-$2BFF	$0400	Nametable 2				Cartridge
// $2C00-$2FFF	$0400	Nametable 3				Cartridge
// $3000-$3EFF	$0F00	Mirrors of $2000-$2EFF	Cartridge
// $3F00-$3F1F	$0020	Palette RAM indexes		Internal
// $3F20-$3FFF	$00E0	Mirrors of $3F00-$3F1F	Internal

uint8_t PpuMemoryMapper::read(uint16_t address) const
{
	if (address < 0x3F00)
	{
		return this->_cartridge->read(address);
	}
	else
	{
		auto ram_address = address % 0x0800;
		return this->_ram->read(ram_address);
	}
}

void PpuMemoryMapper::write(uint16_t address, const uint8_t data)
{
	if (address < 0x2000)
	{
		return this->_cartridge->write(address, data);
	}
	else
	{
		auto ram_address = address % 0x0800;
		this->_ram->write(ram_address, data);
	}
}

void PpuMemoryMapper::load_cartridge(Cartridge* cartridge)
{
	this->_cartridge = cartridge;
}

void PpuMemoryMapper::unload_cartridge()
{
	this->_cartridge = nullptr;
}
