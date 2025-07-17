#pragma once

#include <array>
#include <cstdint>
#include <map>

#include <base/pixel.hpp>
#include <platform/iRenderer.hpp>

#include "palette.hpp"

class Cartridge;
class IMemory;

const uint8_t tile_width = 8;
const uint8_t tile_height = 8;

const uint16_t num_tiles_wide = 32;
const uint16_t num_tiles_tall = 30;

const uint16_t nt_width = tile_width * num_tiles_wide;
const uint16_t nt_height = tile_height * num_tiles_tall;

const uint64_t nt_total_pixels = nt_width * 2 * nt_height * 2;
const uint16_t nt_attribute_bytes = 64;

const SDL_Color nt_background_color = {0x22, 0x22, 0x22, 0xff};

using NametableIndexedImage = std::array<uint8_t, nt_total_pixels>;
using NametableImage = std::array<Pixel, nt_total_pixels>;

enum class DisplayMode
{
	GreyscalePatternTiles,
	PaletteIndices,
	ColoredPatternTiles,
};

const int numberDisplayModes = 3;

class Nametable
{
public:
	Nametable(IRenderer* renderer);
	virtual ~Nametable();

	void draw(const bool use_right_table);
	void color();

	void loadCartridge(Cartridge* cart, const bool use_right_table);
	void updatePalettes(const std::map<PaletteType, Palette>& palettes);

	Texture getTexture() const { return this->_texture; }

	void setRenderer(IRenderer* renderer);

	void setDisplayMode(const DisplayMode mode);
	void nextDisplayMode();
	void prevDisplayMode();

private:
	Cartridge* _cart {nullptr};
	std::map<PaletteType, Palette> _palettes;

	NametableIndexedImage _indexed_image;
	IRenderer* _renderer {nullptr};
	Texture _texture {nullptr};

	DisplayMode _display_mode {DisplayMode::ColoredPatternTiles};

	int _height {nt_height * 2};
	int _width {nt_width * 2};
	int _attribute_padding {nt_attribute_bytes};

	void loadRenderer();
	void unloadRenderer();

	void updateTexture(const NametableImage& pixel_data);
	std::array<uint8_t, 8> compilePatternTableBytes(const uint8_t high_byte,
		const uint8_t low_byte);

	void drawTile(const uint64_t x_offset, const uint64_t y_offset, const uint16_t nametable_offset,
		const uint16_t tile_index, const bool use_right_pt);

	void colorTile(const uint64_t x_offset, const uint64_t y_offset, const PaletteType palette_type,
		NametableImage& image);
	void colorTile(const uint64_t x_offset, const uint64_t y_offset,
		const uint16_t nametable_offset, const uint16_t tile_index, NametableImage& image);

	PaletteType getBackgroundPalette(const uint8_t palette_index) const;
	uint8_t getPaletteIndex(const uint16_t nametable_offset, const uint16_t tile_index) const;
};
