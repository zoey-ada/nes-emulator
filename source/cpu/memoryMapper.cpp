#include "memoryMapper.hpp"

#include <cassert>
#include <iostream>

#include <base/iDma.hpp>
#include <base/iPpu.hpp>
#include <base/randomAccessMemory.hpp>
#include <cartridge/cartridge.hpp>
#include <input/iInput.hpp>

MemoryMapper::MemoryMapper(IPpu* ppu, IDma* dma): _ppu(ppu), _dma(dma)
{
	assert(this->_dma);
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
		this->_ppu->cpu_address_bus(static_cast<uint8_t>(address));
		this->_ppu->cpu_read();
		return this->_ppu->cpu_data_bus();
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
			if (this->_controller_1)
			{
				auto data = this->_controller_1->read();
				return 0x00 | static_cast<uint8_t>(data);
			}
			else
			{
				return 0x00;
			}
			break;
		case 0x4017:  // JOY2
			if (this->_controller_2)
			{
				auto data = this->_controller_2->read();
				return 0x00 | static_cast<uint8_t>(data);
			}
			else
			{
				return 0x00;
			}
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
		this->_ppu->cpu_address_bus(static_cast<uint8_t>(address));
		this->_ppu->cpu_data_bus(data);
		this->_ppu->cpu_write();
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
		{
			uint16_t start_address = static_cast<uint16_t>(data) << 8;
			this->_dma->copyToOam(start_address);
			break;
		}
		case 0x4015:  // SND_CHN
			break;
		case 0x4016:  // JOY1
			if (this->_controller_1)
				this->_controller_1->write((data & 0b0000'0001) > 0);
			break;
		case 0x4017:  // JOY2
			if (this->_controller_2)
				this->_controller_2->write((data & 0b0000'0001) > 0);
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

void MemoryMapper::connect_controller(ControllerPort port, IInput* controller)
{
	if (port == ControllerPort::Port1)
		this->_controller_1 = controller;
	else if (port == ControllerPort::Port2)
		this->_controller_2 = controller;
}

void MemoryMapper::disconnect_controller(ControllerPort port)
{
	if (port == ControllerPort::Port1)
		this->_controller_1 = nullptr;
	else if (port == ControllerPort::Port2)
		this->_controller_2 = nullptr;
}
