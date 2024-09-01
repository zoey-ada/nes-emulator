#pragma once

#include <cstdint>

class IMemory
{
public:
	virtual ~IMemory() = default;

	virtual uint8_t read(const uint16_t address) const = 0;
	virtual void write(const uint16_t address, const uint8_t data) = 0;
};
