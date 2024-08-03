#include "cartridge.hpp"

#include "mappers/iMapper.hpp"

Cartridge::Cartridge(uint8_t* rom, uint32_t rom_size, std::unique_ptr<IMapper> mapper)
	: _rom(rom), _rom_size(rom_size), _mapper(std::move(mapper))
{}

Cartridge::~Cartridge()
{
	if (this->_rom)
		delete[] this->_rom;
	this->_rom = nullptr;
}

uint8_t Cartridge::read(uint16_t address) const
{
	uint16_t rom_address = this->_mapper->translate_address(address);
	return this->_rom[rom_address];
}

void Cartridge::write(uint16_t address, const uint8_t data)
{
	uint16_t rom_address = this->_mapper->translate_address(address);
	// this->_rom[rom_address] = data;
}
