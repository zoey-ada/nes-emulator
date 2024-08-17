#pragma once

#include <array>
#include <cstdint>
#include <memory>

#include <base/pixel.hpp>

#include "palette.hpp"

class Cartridge;
struct SDL_Renderer;
struct SDL_Texture;

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
	PatternTable(PatternTableType pt_type, SDL_Renderer* renderer);
	virtual ~PatternTable();

	void draw();
	void color();

	void loadCartridge(Cartridge* cart);
	void loadPalette(Palette palette);

	SDL_Texture* getTexture() const { return this->_texture; }

private:
	PatternTableType _pt_type;
	Cartridge* _cart {nullptr};
	Palette _palette {greyscale_palette};

	PatternTableIndexedImage _indexed_image;
	SDL_Renderer* _renderer {nullptr};

	SDL_Texture* _texture {nullptr};
	int _texture_height {pt_width_in_pixels};
	int _texture_width {pt_height_in_pixels};

	void updateTexture(const PatternTableImage& pixel_data);
	std::array<uint8_t, 8> compilePatternTableBytes(uint8_t high_byte, uint8_t low_byte);
};
