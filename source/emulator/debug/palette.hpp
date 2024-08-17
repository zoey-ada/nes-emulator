#pragma once

#include <base/color.hpp>

enum class PaletteType
{
	Background0,
	Background1,
	Background2,
	Background3,
	Sprite0,
	Sprite1,
	Sprite2,
	Sprite3,
	Grayscale
};

struct Palette
{
	Color index0;
	Color index1;
	Color index2;
	Color index3;
};

extern Palette greyscale_palette;
