#include "cartridge.hpp"

#include "mappers/iMapper.hpp"

Cartridge::Cartridge(CartridgeData_1_0 cart_data, std::unique_ptr<IMapper> mapper)
	: _program_rom(std::move(cart_data.program_rom_data)),
	  _character_rom(std::move(cart_data.character_rom_data)),
	  _mapper(std::move(mapper))
{}

Cartridge::Cartridge(CartridgeData_2_0 cart_data, std::unique_ptr<IMapper> mapper)
	: _program_rom(std::move(cart_data.program_rom_data)),
	  _character_rom(std::move(cart_data.character_rom_data)),
	  _misc_rom(std::move(cart_data.misc_rom_data)),
	  _mapper(std::move(mapper))
{}

Cartridge::~Cartridge()
{}

uint8_t Cartridge::read_program(uint16_t address) const
{
	uint16_t rom_address = this->_mapper->translate_address(address);
	return this->_program_rom[rom_address];
}

void Cartridge::write_program(uint16_t address, const uint8_t data)
{
	uint16_t rom_address = this->_mapper->translate_address(address);
	// this->_program_rom[rom_address] = data;
}

uint8_t Cartridge::read_character(uint16_t address) const
{
	uint16_t rom_address = this->_mapper->translate_address(address);
	return this->_character_rom[rom_address];
}

void Cartridge::write_character(uint16_t address, const uint8_t data)
{
	uint16_t rom_address = this->_mapper->translate_address(address);
	// this->_character_rom[rom_address] = data;
}
