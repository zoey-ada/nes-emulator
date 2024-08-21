#pragma once

#include <string>

#include "iMapper.hpp"

class Mapper0: public IMapper
{
public:
	Mapper0(bool has_second_bank, bool has_vertical_nametable_mirroring);
	virtual ~Mapper0() = default;

	MemoryLocation translate_program_address(uint16_t address) const override;
	MemoryLocation translate_character_address(uint16_t address) const override;

private:
	bool _has_second_bank {false};
	bool _has_vertical_nametable_mirroring {false};
};
