#pragma once

#include <string>

#include "iMapper.hpp"

class Mapper0: public IMapper
{
public:
	Mapper0(bool has_second_bank);
	virtual ~Mapper0() = default;

	uint16_t translate_address(uint16_t address) const override;

private:
	bool _has_second_bank {false};
};
