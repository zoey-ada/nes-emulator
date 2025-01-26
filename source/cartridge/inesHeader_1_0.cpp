#include "inesHeader_1_0.hpp"

const uint16_t program_rom_chunk_size = 0x4000;
const uint16_t character_rom_chunk_size = 0x2000;
const uint16_t trainer_data_byte_size = 0x0200;

InesHeader_1_0::InesHeader_1_0(InesHeader header_data): _header_data(header_data)
{
	this->parse_roms();
	this->parse_flag_info();
}

void InesHeader_1_0::parse_roms()
{
	// program ROM
	uint8_t program_rom_chunk_count = this->_header_data.prg_rom_size;
	this->_program_rom_size =
		static_cast<uint64_t>(program_rom_chunk_size) * program_rom_chunk_count;

	// character ROM
	uint8_t character_rom_chunk_count = this->_header_data.chr_rom_size;
	this->_character_rom_size =
		static_cast<uint64_t>(character_rom_chunk_size) * character_rom_chunk_count;

	// 512 byte trainer data
	if ((this->_header_data.flags_6 & 0b00000100) > 0)
		this->_trainer_data_size = trainer_data_byte_size;
	else
		this->_trainer_data_size = 0;
}

void InesHeader_1_0::parse_flag_info()
{
	// nametables
	if ((this->_header_data.flags_6 & 0b00001000) > 0)
		this->_nametable_arrangement = NameTableArrangement::alternative;
	else if ((this->_header_data.flags_6 & 0b00000001) > 0)
		this->_nametable_arrangement = NameTableArrangement::vertical;
	else
		this->_nametable_arrangement = NameTableArrangement::horizontal;

	// battery-backed RAM
	if ((this->_header_data.flags_6 & 0b00000010) > 0)
		this->_has_battery_backed_rom = true;
	else
		this->_has_battery_backed_rom = false;

	// console type
	if ((this->_header_data.flags_7 & 0b00000001) > 0)
		this->_console_type = ConsoleType::nintento_vs_system;
	else
		this->_console_type = ConsoleType::nintendo_entertainment_system;

	// mapper number
	this->_mapper_number = (this->_header_data.flags_7 & 0b11110000) |
		((this->_header_data.flags_6 & 0b11110000) >> 4);
}
