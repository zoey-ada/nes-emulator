#include "memoryMapper.hpp"

#include <cassert>
#include <iostream>
#include <new>

#include <ppu/ppu.hpp>

#include "cartridge/cartridge.hpp"
#include "randomAccessMemory.hpp"

MemoryMapper::MemoryMapper(PictureProcessingUnit* ppu): _ppu(ppu)
{
	assert(this->_ppu);
	this->_ram = new (std::nothrow) RandomAccessMemory(0x0800);
}

MemoryMapper::~MemoryMapper()
{
	if (this->_ram != nullptr)
		delete this->_ram;
	this->_ram = nullptr;
	this->_cartridge = nullptr;
	this->_ppu = nullptr;
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
		auto ppu_register = ((address - 0x2000) % 8) + 0x2000;
		switch (ppu_register)
		{
		case 0x2000:
			// PPUCTRL
			std::cout << "PPUCTRL is a write only register!!";
			break;
		case 0x2001:
			// PPUMASK
			std::cout << "PPUMASK is a write only register!!";
			break;
		case 0x2002:
			// PPUSTATUS
			return this->_ppu->ppu_status();
		case 0x2003:
			// OAMADDR
			std::cout << "OAMADDR is a write only register!!";
			break;
		case 0x2004:
			// OAMDATA
			return this->_ppu->oam_data();
		case 0x2005:
			// PPUSCROLL
			std::cout << "PPUSCROLL is a write only register!!";
			break;
		case 0x2006:
			// PPUADDR
			std::cout << "PPUADDR is a write only register!!";
			break;
		case 0x2007:
			// PPUDATA
			return this->_ppu->ppu_data();
		}
		return 0;
	}
	else if (address < 0x4018)
	{
		switch (address)
		{
		case 0x4000:
			// SQ1_VOL
			break;
		case 0x4001:
			// SQ1_SWEEP
			break;
		case 0x4002:
			// SQ1_LO
			break;
		case 0x4003:
			// SQ1_HI
			break;
		case 0x4004:
			// SQ2_VOL
			break;
		case 0x4005:
			// SQ2_SWEEP
			break;
		case 0x4006:
			// SQ2_LO
			break;
		case 0x4007:
			// SQ2_HI
			break;
		case 0x4008:
			// TRI_LINEAR
			break;
		case 0x4009:
			// -
			break;
		case 0x400A:
			// TRI_LO
			break;
		case 0x400B:
			// TRI_HI
			break;
		case 0x400C:
			// NOISE_VOL
			break;
		case 0x400D:
			// -
			break;
		case 0x400E:
			// NOISE_LO
			break;
		case 0x400F:
			// NOISE_HI
			break;
		case 0x4010:
			// DMC_FREQ
			break;
		case 0x4011:
			// DMC_RAW
			break;
		case 0x4012:
			// DMC_START
			break;
		case 0x4013:
			// DMC_LEN
			break;
		case 0x4014:
			// OAMDMA
			break;
		case 0x4015:
			// SND_CHN
			break;
		case 0x4016:
			// JOY1
			break;
		case 0x4017:
			// JOY2
			break;
		}
		return 0;
	}
	else if (address < 0x4020)
	{
		auto cpu_test_address = address - 0x4018;
		return 0;
	}
	else
	{
		return this->_cartridge->read(address);
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
		auto ppu_register = ((address - 0x2000) % 8) + 0x2000;
		switch (ppu_register)
		{
		case 0x2000:
			// PPUCTRL
			this->_ppu->ppu_ctrl(data);
			break;
		case 0x2001:
			// PPUMASK
			this->_ppu->ppu_mask(data);
			break;
		case 0x2002:
			// PPUSTATUS
			std::cout << "PPUSTATUS is a read only register!!";
			break;
		case 0x2003:
			// OAMADDR
			this->_ppu->oam_addr(data);
			break;
		case 0x2004:
			// OAMDATA
			this->_ppu->oam_data(data);
			break;
		case 0x2005:
			// PPUSCROLL
			this->_ppu->ppu_scroll(data);
			break;
		case 0x2006:
			// PPUADDR
			this->_ppu->ppu_addr(data);
			break;
		case 0x2007:
			// PPUDATA
			this->_ppu->ppu_data(data);
			break;
		}
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
		return this->_cartridge->write(address, data);
	}
}

void MemoryMapper::load_cartridge(Cartridge* cartridge)
{
	this->_cartridge = cartridge;
}

void MemoryMapper::unload_cartridge()
{
	this->_cartridge = nullptr;
}
