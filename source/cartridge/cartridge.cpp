#include "cartridge.hpp"

#include <base/iMemory.hpp>

#include "mappers/iMapper.hpp"

Cartridge::Cartridge(CartridgeData_1_0 cart_data, std::unique_ptr<IMapper> mapper)
	: _program_rom(std::move(cart_data.program_rom_data)),
	  _character_rom(std::move(cart_data.character_rom_data)),
	  _mapper(std::move(mapper))
{
	this->_program_ram.resize(0x2000);

	if (this->_character_rom.size() == 0)
		this->_character_ram.resize(0x2000);
}

Cartridge::Cartridge(CartridgeData_2_0 cart_data, std::unique_ptr<IMapper> mapper)
	: _program_rom(std::move(cart_data.program_rom_data)),
	  _character_rom(std::move(cart_data.character_rom_data)),
	  _misc_rom(std::move(cart_data.misc_rom_data)),
	  _mapper(std::move(mapper))
{}

Cartridge::~Cartridge()
{
	this->_console_video_ram = nullptr;
}

uint8_t Cartridge::read_program(uint16_t address) const
{
	auto location = this->_mapper->translate_program_address(address);
	switch (location.device)
	{
	case MemoryDevice::CartridgeRom:
		return this->_program_rom[location.address];
	case MemoryDevice::CartridgeRam:
		return this->_program_ram[location.address];
	case MemoryDevice::ConsoleRam:
		throw std::exception("Console RAM not accessible for program reads");
	default:
		throw std::exception("unreachable");
	}
}

void Cartridge::write_program(uint16_t address, const uint8_t data)
{
	auto location = this->_mapper->translate_program_address(address);
	switch (location.device)
	{
	case MemoryDevice::CartridgeRom:
		// ROM cannot be written to
		break;
	case MemoryDevice::CartridgeRam:
		this->_program_ram[location.address] = data;
		break;
	case MemoryDevice::ConsoleRam:
		throw std::exception("Console RAM not accessible for program writes");
	default:
		throw std::exception("unreachable");
	}
}

uint8_t Cartridge::read_character(uint16_t address) const
{
	auto location = this->_mapper->translate_character_address(address);
	switch (location.device)
	{
	case MemoryDevice::CartridgeRom:
		return this->_character_rom[location.address];
	case MemoryDevice::CartridgeRam:
		return this->_character_ram[location.address];
	case MemoryDevice::ConsoleRam:
		return this->_console_video_ram->read(location.address);
	default:
		throw std::exception("unreachable");
	}
}

void Cartridge::write_character(uint16_t address, const uint8_t data)
{
	auto location = this->_mapper->translate_character_address(address);
	switch (location.device)
	{
	case MemoryDevice::CartridgeRom:
		// ROM cannot be written to
		break;
	case MemoryDevice::CartridgeRam:
		this->_character_ram[location.address] = data;
		break;
	case MemoryDevice::ConsoleRam:
		this->_console_video_ram->write(location.address, data);
		break;
	default:
		throw std::exception("unreachable");
	}
}
