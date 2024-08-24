#pragma once

#include <string>

#include "iMapper.hpp"

struct Mapper0Options
{
	bool has_two_banks;
	bool has_vertical_nametable_mirroring;
	bool uses_character_ram;
};

class Mapper0: public IMapper
{
public:
	Mapper0(const Mapper0Options& options);
	virtual ~Mapper0() = default;

	MemoryLocation translate_program_address(uint16_t address) const override;
	MemoryLocation translate_character_address(uint16_t address) const override;

private:
	bool _has_second_bank {false};
	bool _has_vertical_nametable_mirroring {false};
	bool _uses_character_ram {false};
};
