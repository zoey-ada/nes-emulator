#pragma once

#include <cstdint>

#include "pixel.hpp"

struct Color
{
	Color(uint32_t byte)
	{
		r = (byte & 0x00'ff'00'00) >> 16;
		g = (byte & 0x00'00'ff'00) >> 8;
		b = (byte & 0x00'00'00'ff);
	}

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
