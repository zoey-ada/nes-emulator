#include "inesHeader_2_0.hpp"

const uint16_t program_rom_chunk_size = 0x4000;
const uint16_t character_rom_chunk_size = 0x2000;
const uint16_t trainer_data_byte_size = 0x0200;
const uint8_t ram_base_size = 0x40;

InesHeader_2_0::InesHeader_2_0(InesHeader header_data): _header_data(header_data)
{
	this->parse_roms();
	this->parse_rams();
	this->parse_mapper_info();
	this->parse_nametable_arrangement();
	this->parse_system_info();
}

uint64_t InesHeader_2_0::translate_rom_size(uint16_t rom_size_data, uint16_t chunk_size) const
{
	uint8_t high_byte = (rom_size_data & 0xff00) >> 8;
	uint8_t low_byte = (rom_size_data & 0x00ff);

	// if most significant nibble is less than 0xF, the size indicates the number of 16KB chunks
	if ((high_byte & 0b00001111) < 0x0F)
	{
		return static_cast<uint64_t>(rom_size_data) * chunk_size;
	}
	// if most significant nibble is 0xF, the size is in exponent-multiplier notation
	else
	{
		uint8_t exponent = (low_byte & 0b11111100) >> 2;
		uint8_t multiplier = ((low_byte & 0b00000011) * 2) + 1;
		return static_cast<uint64_t>(multiplier) << exponent;
	}
}

void InesHeader_2_0::parse_roms()
{
	// program ROM
	uint16_t program_rom_size_data =
		(static_cast<uint16_t>(this->_header_data.flags_9 & 0b00001111) << 8) |
		static_cast<uint16_t>(this->_header_data.prg_rom_size);

	this->_program_rom_size =
		this->translate_rom_size(program_rom_size_data, program_rom_chunk_size);

	// character ROM
	uint16_t character_rom_size_data =
		(static_cast<uint16_t>(this->_header_data.flags_9 & 0b11110000) << 8) |
		static_cast<uint16_t>(this->_header_data.chr_rom_size);

	this->_character_rom_size =
		this->translate_rom_size(character_rom_size_data, character_rom_chunk_size);

	// 512 byte trainer data
	if ((this->_header_data.flags_6 & 0b00000100) > 0)
		this->_trainer_data_size = trainer_data_byte_size;
	else
		this->_trainer_data_size = 0;

	// misc ROM
	this->_misc_rom_count = this->_header_data.flags_14 & 0b00000011;
}

void InesHeader_2_0::parse_rams()
{
	// volatile RAM
	uint8_t program_ram_shift = this->_header_data.flags_10 & 0b00001111;
	this->_volatile_program_ram_size = static_cast<uint64_t>(ram_base_size) << program_ram_shift;

	uint8_t character_ram_shift = this->_header_data.flags_11 & 0b00001111;
	this->_volatile_character_ram_size = static_cast<uint64_t>(ram_base_size)
		<< character_ram_shift;

	// non-volatile RAM
	program_ram_shift = this->_header_data.flags_10 & 0b11110000;
	this->_non_volatile_program_ram_size = static_cast<uint64_t>(ram_base_size)
		<< program_ram_shift;

	character_ram_shift = this->_header_data.flags_11 & 0b11110000;
	this->_non_volatile_character_ram_size = static_cast<uint64_t>(ram_base_size)
		<< character_ram_shift;

	// battery-backed RAM
	if ((this->_header_data.flags_6 & 0b00000010) > 0)
		this->_has_battery_backed_rom = true;
	else
		this->_has_battery_backed_rom = false;
}

void InesHeader_2_0::parse_nametable_arrangement()
{
	// nametables
	if ((this->_header_data.flags_6 & 0b00001000) > 0)
		this->_nametable_arrangement = NameTableArrangement::alternative;
	else if ((this->_header_data.flags_6 & 0b00000001) > 0)
		this->_nametable_arrangement = NameTableArrangement::horizontal;
	else
		this->_nametable_arrangement = NameTableArrangement::vertical;
}

void InesHeader_2_0::parse_mapper_info()
{
	// mapper number
	this->_mapper_number = (static_cast<uint16_t>(this->_header_data.flags_8 & 0b00001111) << 8) |
		static_cast<uint16_t>(this->_header_data.flags_7 & 0b11110000) |
		(static_cast<uint16_t>(this->_header_data.flags_6 & 0b11110000) >> 4);

	// submapper number
	this->_submapper_number = (this->_header_data.flags_8 & 0b11110000) >> 4;
}

void InesHeader_2_0::parse_system_info()
{
	// timing mode
	this->_timing_mode = static_cast<TimingMode>(this->_header_data.flags_12 & 0b00000011);

	// console type
	uint8_t console_type = (this->_header_data.flags_7 & 0b00000011);
	if (console_type < 3)
		this->_console_type = static_cast<ConsoleType>(console_type);
	else
	{
		uint8_t extended_console_type = this->_header_data.flags_13 & 0b00001111;
		this->_console_type = static_cast<ConsoleType>(extended_console_type);
	}

	// VS system info
	if (this->_console_type == ConsoleType::nintento_vs_system)
	{
		VsSystemHardwareType hardware_type =
			static_cast<VsSystemHardwareType>((this->_header_data.flags_13 & 0b11110000) >> 4);

		VsSystemPpuType ppu_type =
			static_cast<VsSystemPpuType>(this->_header_data.flags_13 & 0b00001111);

		this->_vs_system_info = {hardware_type, ppu_type};
	}

	// default expansion device
	this->_default_expansion_device = this->_header_data.flags_15 & 0b00111111;
}
