#pragma once

#include <array>
#include <cstdint>
#include <map>
#include <memory>

#include <base/pixel.hpp>
#include <platform/iRenderer.hpp>

#include "palette.hpp"
#include "patternTable.hpp"

class Cartridge;
class IMemory;

const uint16_t st_width_in_pixels = 64;
const uint16_t st_height_in_pixels = 64;
const uint64_t st_total_pixels = st_width_in_pixels * st_height_in_pixels;

using SpriteTableIndexedImage = std::array<uint8_t, st_total_pixels>;
using SpriteTableImage = std::array<Pixel, st_total_pixels>;

class SpriteTable
{
public:
	SpriteTable(std::shared_ptr<IRenderer> renderer, IMemory* oam);
	virtual ~SpriteTable();

	void draw(const bool use_right_table, const bool use_8x16_sprites);
	void color(const bool use_8x16_sprites);

	void loadCartridge(Cartridge* cart, const bool use_right_table, const bool use_8x16_sprites);
	void updatePalettes(const std::map<PaletteType, Palette>& palettes,
		const bool use_8x16_sprites);

	Texture getTexture() const { return this->_texture; }

private:
	Cartridge* _cart {nullptr};
	IMemory* _oam {nullptr};
	std::map<PaletteType, Palette> _palettes;

	SpriteTableIndexedImage _indexed_image;
	std::shared_ptr<IRenderer> _renderer {nullptr};
	Texture _texture {nullptr};

	int _height {st_height_in_pixels};
	int _width {st_width_in_pixels};

	void updateTexture(const SpriteTableImage& pixel_data);
	std::array<uint8_t, 8> compilePatternTableBytes(const uint8_t high_byte,
		const uint8_t low_byte);

	void draw8x8Sprite(const uint8_t sprite_index, const uint8_t x, const uint8_t y,
		const bool use_right_table);
	void color8x8Sprite(const uint8_t sprite_index, const uint8_t x, const uint8_t y,
		SpriteTableImage& image);
};
