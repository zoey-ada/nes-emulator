#pragma once

#include <cstdint>

#include "pixel.hpp"

struct Color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint32_t get()
	{
		return static_cast<uint32_t>(r) << 16 | static_cast<uint32_t>(g) << 8 |
			static_cast<uint32_t>(b);
	}
	Pixel pixel() { return {b, g, r}; }
};
