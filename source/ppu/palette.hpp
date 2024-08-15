#pragma once

#include <cstdint>
#include <map>

class Palette
{
public:
	Palette();
	virtual ~Palette() = default;

	uint32_t getColor(uint8_t palette_index) const;

private:
	std::map<uint8_t, uint32_t> _palette_map;
};
