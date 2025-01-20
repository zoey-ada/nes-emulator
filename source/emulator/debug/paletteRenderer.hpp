#pragma once

#include <map>
#include <memory>

#include <platform/iRenderer.hpp>

#include "palette.hpp"

const uint16_t palette_index_height = 4;
const uint16_t palette_index_width = 4;
const uint16_t palette_width = 4 * palette_index_width;
const uint16_t palette_table_spacing = 11;

const uint16_t outline_height = palette_index_height + 2;
const uint16_t outline_width = palette_width + 2;
const SDL_Color unselected_outline_color = {0x00, 0x00, 0x00, 0xff};
const SDL_Color selected_outline_color = {0xff, 0x00, 0x00, 0xff};

const uint16_t palette_table_padding = 3;
const uint16_t palette_table_width =
	palette_table_padding * 2 + outline_width * 9 + palette_table_spacing * 8;

class PaletteRenderer
{
public:
	PaletteRenderer(std::shared_ptr<IRenderer> renderer);
	virtual ~PaletteRenderer();

	void renderPalettes(const std::map<PaletteType, Palette>& palettes,
		const PaletteType selected_palette);

	Texture getTexture() const { return this->_texture; }

private:
	std::shared_ptr<IRenderer> _renderer {nullptr};
	Texture _texture {nullptr};

	int _height {outline_height};
	int _width {palette_table_width};

	void renderPalette(const Palette& palette, const int x_offset, const bool is_selected);
};
