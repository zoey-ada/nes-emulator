#pragma once

#include <cstdint>

class IDma
{
public:
	virtual ~IDma() = default;

	virtual void cycle() = 0;

	virtual void copyToOam(const uint16_t address) = 0;
};
