#pragma once

#include <optional>

#include "inesHeader.hpp"

struct VsSystemInfo
{
	VsSystemHardwareType hardware_type;
	VsSystemPpuType ppu_type;
};

class InesHeader_2_0
{
public:
	InesHeader_2_0(InesHeader header_data);
	virtual ~InesHeader_2_0() = default;

	inline uint64_t program_rom_size() const { return this->_program_rom_size; }
	inline uint64_t character_rom_size() const { return this->_character_rom_size; }
	inline uint64_t trainer_data_size() const { return this->_trainer_data_size; }
	inline uint8_t misc_rom_count() const { return this->_misc_rom_count; }

	inline uint64_t volatile_program_ram_size() const { return this->_volatile_program_ram_size; }
	inline uint64_t volatile_character_ram_size() const
	{
		return this->_volatile_character_ram_size;
	}
	inline uint64_t non_volatile_program_ram_size() const
	{
		return this->_non_volatile_program_ram_size;
	}
	inline uint64_t non_volatile_character_ram_size() const
	{
		return this->_non_volatile_character_ram_size;
	}
	inline bool has_battery_backed_ram() const { return this->_has_battery_backed_rom; }

	inline NameTableArrangement nametable_arrangement() const
	{
		return this->_nametable_arrangement;
	}

	inline uint16_t mapper_number() const { return this->_mapper_number; }
	inline uint8_t submapper_number() const { return this->_submapper_number; }

	inline TimingMode timing_mode() const { return this->_timing_mode; }
	inline ConsoleType console_type() const { return this->_console_type; }
	inline std::optional<VsSystemInfo> vs_system_info() const { return this->_vs_system_info; }
	inline uint8_t default_expansion_device() const { return this->_default_expansion_device; }

private:
	InesHeader _header_data;

	uint64_t _program_rom_size {0};
	uint64_t _character_rom_size {0};
	uint64_t _trainer_data_size {0};
	uint8_t _misc_rom_count {0};

	uint64_t _volatile_program_ram_size {0};
	uint64_t _volatile_character_ram_size {0};
	uint64_t _non_volatile_program_ram_size {0};
	uint64_t _non_volatile_character_ram_size {0};
	bool _has_battery_backed_rom {false};

	NameTableArrangement _nametable_arrangement;

	uint16_t _mapper_number;
	uint8_t _submapper_number;

	TimingMode _timing_mode;
	ConsoleType _console_type;
	std::optional<VsSystemInfo> _vs_system_info;
	uint8_t _default_expansion_device {0};

	uint64_t translate_rom_size(uint16_t rom_size_data, uint16_t chunk_size) const;
	void parse_roms();
	void parse_rams();
	void parse_nametable_arrangement();
	void parse_mapper_info();
	void parse_system_info();
};
