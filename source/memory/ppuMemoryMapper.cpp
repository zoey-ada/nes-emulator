#include "ppuMemoryMapper.hpp"

#include <new>

#include "cartridge/cartridge.hpp"
#include "randomAccessMemory.hpp"
#include "slowMemory.hpp"

PpuMemoryMapper::PpuMemoryMapper()
{
	this->_palette_ram = new (std::nothrow) RandomAccessMemory(0x0100);
	// this->_video_ram = new (std::nothrow) RandomAccessMemory(0x0800);
	this->_video_ram = new (std::nothrow) SlowMemory(0x0800);
}

PpuMemoryMapper::~PpuMemoryMapper()
{
	if (this->_cartridge)
		this->unload_cartridge();
	this->_cartridge = nullptr;

	if (this->_palette_ram != nullptr)
		delete this->_palette_ram;
	this->_palette_ram = nullptr;

	if (this->_video_ram != nullptr)
		delete this->_video_ram;
	this->_video_ram = nullptr;
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
	if (address < 0x3f00)
	{
		return this->_cartridge->read_character(address);
	}
	else
	{
		auto ram_address = address % 0x0800;
		return this->_palette_ram->read(ram_address);
	}
}

void PpuMemoryMapper::write(uint16_t address, const uint8_t data)
{
	if (address < 0x3f00)
	{
		this->_cartridge->write_character(address, data);
	}
	else
	{
		auto ram_address = address % 0x0800;
		this->_palette_ram->write(ram_address, data);
	}
}

void PpuMemoryMapper::load_cartridge(Cartridge* cartridge)
{
	this->_cartridge = cartridge;
	this->_cartridge->setConsoleVideoRam(this->_video_ram);
}

void PpuMemoryMapper::unload_cartridge()
{
	this->_cartridge->setConsoleVideoRam(nullptr);
	this->_cartridge = nullptr;
}
