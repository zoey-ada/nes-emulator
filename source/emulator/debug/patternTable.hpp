#pragma once

#include <array>
#include <cstdint>
#include <memory>

#include <base/pixel.hpp>
#include <platform/iRenderer.hpp>

#include "palette.hpp"

class Cartridge;
class IRenderer;

const uint16_t pt_width_in_pixels = 128;
const uint16_t pt_height_in_pixels = 128;
const uint64_t pt_total_pixels = pt_width_in_pixels * pt_height_in_pixels;

using PatternTableIndexedImage = std::array<uint8_t, pt_total_pixels>;
using PatternTableImage = std::array<Pixel, pt_total_pixels>;

enum class PatternTableType
{
	Left,
	Right
};

class PatternTable
{
public:
	PatternTable(PatternTableType pt_type, IRenderer* renderer);
	virtual ~PatternTable();

	void draw();
	void color();

	void loadCartridge(Cartridge* cart);
	void loadPalette(Palette palette);

	Texture getTexture() const { return this->_texture; }

	void setRenderer(IRenderer* renderer);

private:
	Cartridge* _cart {nullptr};
	Palette _palette {greyscale_palette};

	PatternTableIndexedImage _indexed_image;
	IRenderer* _renderer {nullptr};
	Texture _texture {nullptr};

	PatternTableType _pt_type;
	int _height {pt_height_in_pixels};
	int _width {pt_width_in_pixels};

	void loadRenderer();
	void unloadRenderer();

	void updateTexture(const PatternTableImage& pixel_data);
	std::array<uint8_t, 8> compilePatternTableBytes(uint8_t high_byte, uint8_t low_byte);
};
