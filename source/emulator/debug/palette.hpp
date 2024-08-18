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
	Color index0 {0x00ff00ff};
	Color index1 {0x00ff00ff};
	Color index2 {0x00ff00ff};
	Color index3 {0x00ff00ff};
};

extern Palette greyscale_palette;
