#pragma once

#include <optional>

#include "inesHeader.hpp"

class InesHeader_1_0
{
public:
	InesHeader_1_0(InesHeader header_data);
	virtual ~InesHeader_1_0() = default;

	inline uint64_t program_rom_size() const { return this->_program_rom_size; }
	inline uint64_t character_rom_size() const { return this->_character_rom_size; }
	inline uint64_t trainer_data_size() const { return this->_trainer_data_size; }

	inline NameTableArrangement nametable_arrangement() const
	{
		return this->_nametable_arrangement;
	}
	inline bool has_battery_backed_ram() const { return this->_has_battery_backed_rom; }

	inline uint16_t mapper_number() const { return this->_mapper_number; }

	inline ConsoleType console_type() const { return this->_console_type; }

private:
	InesHeader _header_data;

	uint64_t _program_rom_size {0};
	uint64_t _character_rom_size {0};
	uint64_t _trainer_data_size {0};

	NameTableArrangement _nametable_arrangement;
	bool _has_battery_backed_rom {false};
	ConsoleType _console_type;
	uint8_t _mapper_number;

	void parse_roms();
	void parse_flag_info();
};
