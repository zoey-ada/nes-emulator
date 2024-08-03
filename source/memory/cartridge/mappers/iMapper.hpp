#pragma once

#include <string>

class IMapper
{
public:
	virtual ~IMapper() = default;

	virtual uint16_t translate_address(uint16_t address) const = 0;
};
