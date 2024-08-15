#include "memoryMapper.hpp"

#include <cassert>
#include <iostream>

#include <ppu/ppu.hpp>

#include "cartridge/cartridge.hpp"
#include "randomAccessMemory.hpp"

MemoryMapper::MemoryMapper(PictureProcessingUnit* ppu): _ppu(ppu)
{
	assert(this->_ppu);
	this->_ram = std::make_unique<RandomAccessMemory>(0x0800);
}

MemoryMapper::~MemoryMapper()
{
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
		case 0x2002:  // PPUSTATUS
			return this->_ppu->ppu_status();
		case 0x2004:  // OAMDATA
			return this->_ppu->oam_data();
		case 0x2007:  // PPUDATA
			return this->_ppu->ppu_data();
		case 0x2000:  // PPUCTRL
		case 0x2001:  // PPUMASK
		case 0x2003:  // OAMADDR
		case 0x2005:  // PPUSCROLL
		case 0x2006:  // PPUADDR
			assert(false);
			break;
		}
		return 0;
	}
	else if (address < 0x4018)
	{
		switch (address)
		{
		case 0x4000:  // SQ1_VOL
		case 0x4001:  // SQ1_SWEEP
		case 0x4002:  // SQ1_LO
		case 0x4003:  // SQ1_HI
		case 0x4004:  // SQ2_VOL
		case 0x4005:  // SQ2_SWEEP
		case 0x4006:  // SQ2_LO
		case 0x4007:  // SQ2_HI
		case 0x4008:  // TRI_LINEAR
		case 0x4009:  // -
		case 0x400A:  // TRI_LO
		case 0x400B:  // TRI_HI
		case 0x400C:  // NOISE_VOL
		case 0x400D:  // -
		case 0x400E:  // NOISE_LO
		case 0x400F:  // NOISE_HI
		case 0x4010:  // DMC_FREQ
		case 0x4011:  // DMC_RAW
		case 0x4012:  // DMC_START
		case 0x4013:  // DMC_LEN
		case 0x4014:  // OAMDMA
			// TODO: should return previous read data
			return 0;
			assert(false);
			break;
		case 0x4015:  // SND_CHN
			break;
		case 0x4016:  // JOY1
			break;
		case 0x4017:  // JOY2
			break;
		}
		return 0;
	}
	else if (address < 0x4020)
	{
		assert(false);
		return 0;
	}
	else
	{
		return this->_cartridge->read_program(address);
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
		case 0x2000:  // PPUCTRL
			this->_ppu->ppu_ctrl(data);
			break;
		case 0x2001:  // PPUMASK
			this->_ppu->ppu_mask(data);
			break;
		case 0x2002:  // PPUSTATUS
			assert(false);
			break;
		case 0x2003:  // OAMADDR
			this->_ppu->oam_addr(data);
			break;
		case 0x2004:  // OAMDATA
			this->_ppu->oam_data(data);
			break;
		case 0x2005:  // PPUSCROLL
			this->_ppu->ppu_scroll(data);
			break;
		case 0x2006:  // PPUADDR
			this->_ppu->ppu_addr(data);
			break;
		case 0x2007:  // PPUDATA
			this->_ppu->ppu_data(data);
			break;
		}
	}
	else if (address < 0x4018)
	{
		switch (address)
		{
		case 0x4000:  // SQ1_VOL
			break;
		case 0x4001:  // SQ1_SWEEP
			break;
		case 0x4002:  // SQ1_LO
			break;
		case 0x4003:  // SQ1_HI
			break;
		case 0x4004:  // SQ2_VOL
			break;
		case 0x4005:  // SQ2_SWEEP
			break;
		case 0x4006:  // SQ2_LO
			break;
		case 0x4007:  // SQ2_HI
			break;
		case 0x4008:  // TRI_LINEAR
			break;
		case 0x4009:  // -
			break;
		case 0x400A:  // TRI_LO
			break;
		case 0x400B:  // TRI_HI
			break;
		case 0x400C:  // NOISE_VOL
			break;
		case 0x400D:  // -
			break;
		case 0x400E:  // NOISE_LO
			break;
		case 0x400F:  // NOISE_HI
			break;
		case 0x4010:  // DMC_FREQ
			break;
		case 0x4011:  // DMC_RAW
			break;
		case 0x4012:  // DMC_START
			break;
		case 0x4013:  // DMC_LEN
			break;
		case 0x4014:  // OAMDMA
			this->_ppu->oam_dma(data);
			break;
		case 0x4015:  // SND_CHN
			break;
		case 0x4016:  // JOY1
			break;
		case 0x4017:  // JOY2
			break;
		}
	}
	else if (address < 0x4020)
	{
		assert(false);
	}
	else
	{
		return this->_cartridge->write_program(address, data);
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
