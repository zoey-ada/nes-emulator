#include "nametable.hpp"

#include <format>
#include <stdexcept>

#include <cartridge/cartridge.hpp>

Nametable::Nametable(IRenderer* renderer): _renderer(renderer)
{
	this->loadRenderer();
}

Nametable::~Nametable()
{
	this->unloadRenderer();
}

void Nametable::loadCartridge(Cartridge* cart, const bool use_right_table)
{
	this->_cart = cart;
	this->draw(use_right_table);
}

void Nametable::draw(const bool use_right_table)
{
	if (!this->_cart)
		return;

	uint64_t nametable_x_offset = 0;
	uint64_t nametable_y_offset = 0;

	for (uint16_t nametable_offset = 0x2000; nametable_offset <= 0x2c00; nametable_offset += 0x400)
	{
		if (nametable_offset == 0x2400)
		{
			nametable_x_offset = nt_width;
			nametable_y_offset = 0;
		}
		else if (nametable_offset == 0x2800)
		{
			nametable_x_offset = 0;
			nametable_y_offset = nt_height;
		}
		else if (nametable_offset == 0x2c00)
		{
			nametable_x_offset = nt_width;
			nametable_y_offset = nt_height;
		}

		for (uint16_t y = 0; y < num_tiles_tall; ++y)
		{
			for (uint16_t x = 0; x < num_tiles_wide; ++x)
			{
				uint16_t index = y * num_tiles_wide + x;
				uint64_t x_offset = x * tile_width + nametable_x_offset;
				uint64_t y_offset = y * tile_height + nametable_y_offset;

				if (this->_display_mode == DisplayMode::PaletteIndices)
					this->drawPatternIndex(x_offset, y_offset, nametable_offset, index);
				else
					this->drawTile(x_offset, y_offset, nametable_offset, index, use_right_table);
			}
		}
	}

	this->color();
}

void Nametable::color()
{
	if (!this->_cart || this->_display_mode == DisplayMode::PaletteIndices)
		return;

	NametableImage image;
	uint64_t nametable_x_offset = 0;
	uint64_t nametable_y_offset = 0;

	for (uint16_t nametable_offset = 0x2000; nametable_offset <= 0x2c00; nametable_offset += 0x400)
	{
		if (nametable_offset == 0x2400)
		{
			nametable_x_offset = nt_width;
			nametable_y_offset = 0;
		}
		else if (nametable_offset == 0x2800)
		{
			nametable_x_offset = 0;
			nametable_y_offset = nt_height;
		}
		else if (nametable_offset == 0x2c00)
		{
			nametable_x_offset = nt_width;
			nametable_y_offset = nt_height;
		}

		for (uint16_t y = 0; y < num_tiles_tall; ++y)
		{
			for (uint16_t x = 0; x < num_tiles_wide; ++x)
			{
				uint16_t index = y * num_tiles_wide + x;
				uint64_t x_offset = x * tile_width + nametable_x_offset;
				uint64_t y_offset = y * tile_height + nametable_y_offset;

				if (this->_display_mode == DisplayMode::GreyscalePatternTiles)
					this->colorTile(x_offset, y_offset, PaletteType::Grayscale, image);
				else if (this->_display_mode == DisplayMode::ColoredPatternTiles)
					this->colorTile(x_offset, y_offset, nametable_offset, index, image);
			}
		}
	}

	this->updateTexture(image);
}

void Nametable::updatePalettes(const std::map<PaletteType, Palette>& palettes)
{
	this->_palettes = palettes;
	this->color();
}

void Nametable::updateTexture(const NametableImage& pixel_data)
{
	this->_renderer->updateTexture(this->_texture, pixel_data.data(), sizeof(pixel_data));
}

void Nametable::setRenderer(IRenderer* renderer)
{
	this->unloadRenderer();
	this->_renderer = renderer;
	this->loadRenderer();
}

void Nametable::setDisplayMode(const DisplayMode mode)
{
	this->_display_mode = mode;

	switch (mode)
	{
	case DisplayMode::GreyscalePatternTiles:
		this->color();
		break;
	}
}

void Nametable::nextDisplayMode()
{
	auto display_mode_num = static_cast<int>(this->_display_mode);
	display_mode_num = (display_mode_num + 1) % numberDisplayModes;
	this->_display_mode = DisplayMode(display_mode_num);
	this->recreateTexture();
}

void Nametable::prevDisplayMode()
{
	auto display_mode_num = static_cast<int>(this->_display_mode);
	display_mode_num = (display_mode_num - 1 + numberDisplayModes) % numberDisplayModes;
	this->_display_mode = DisplayMode(display_mode_num);
	this->recreateTexture();
}

void Nametable::loadRenderer()
{
	this->_font = this->_renderer->createFont(this->_font_name, this->_font_size);
	this->_texture = this->_renderer->createTexture(this->_width, this->_height, true);

	NametableImage image;
	image.fill({nt_background_color.r, nt_background_color.g, nt_background_color.b});
	this->updateTexture(image);

	this->createDigitTextures();
}

void Nametable::unloadRenderer()
{
	this->destroyDigitTextures();

	if (this->_texture)
		this->_renderer->destroyTexture(this->_texture);
	this->_texture = nullptr;

	if (this->_font)
		this->_renderer->destroyFont(this->_font);
	this->_font = nullptr;

	this->_renderer = nullptr;
}

void Nametable::recreateTexture()
{
	if (this->_texture)
		this->_renderer->destroyTexture(this->_texture);

	if (this->_display_mode == DisplayMode::PaletteIndices)
	{
		this->_texture = this->_renderer->createTexture(this->_width * 2, this->_height * 2, false);
	}
	else
	{
		this->_texture = this->_renderer->createTexture(this->_width, this->_height, true);
	}
}

void Nametable::createDigitTextures()
{
	Color white {0xff, 0xff, 0xff};
	Color black {0x00, 0x00, 0x00};
	TextRenderOptions text_render_options {this->_small_font, white, black};

	for (uint8_t i = 0; i < 16; ++i)
	{
		std::string index_str = std::format("{:01X}", i);
		auto index_texture = this->_renderer->renderText(index_str, text_render_options);

		this->_digit_textures[i] = index_texture;
	}
}

void Nametable::destroyDigitTextures()
{
	while (!this->_digit_textures.empty())
	{
		auto texture = this->_digit_textures.begin();
		this->_renderer->destroyTexture(texture->second);
		this->_digit_textures.erase(texture);
	}
}

std::array<uint8_t, 8> Nametable::compilePatternTableBytes(const uint8_t low_byte,
	const uint8_t high_byte)
{
	std::array<uint8_t, 8> indices;

	for (int i = 0; i < 8; ++i)
	{
		uint8_t mask = 0b1000'0000 >> i;
		uint8_t index = ((low_byte & mask) >> (7 - i)) + (((high_byte & mask) >> (7 - i)) << 1);
		indices[i] = index;
	}

	return indices;
}

void Nametable::drawPatternIndex(const uint64_t x_offset, const uint64_t y_offset,
	const uint16_t nametable_offset, const uint16_t tile_index)
{
	uint16_t nt_address = nametable_offset | tile_index;
	uint8_t pt_offset = this->_cart->read_character(nt_address);

	auto swapped_target = this->_renderer->swapRenderTarget(this->_texture);

	uint8_t left_digit = pt_offset >> 4;
	uint8_t right_digit = pt_offset & 0b0000'1111;

	Rect source_rect = this->_renderer->measureTexture(this->_digit_textures[left_digit]);
	Rect dest_rect = {
		x_offset * 2,
		y_offset * 2,
		tile_width,
		tile_height * 2,
	};
	this->_renderer->drawTexture(this->_digit_textures[left_digit], dest_rect);

	dest_rect.x += tile_width;
	this->_renderer->drawTexture(this->_digit_textures[right_digit], dest_rect);
}

void Nametable::drawTile(const uint64_t x_offset, const uint64_t y_offset,
	const uint16_t nametable_offset, const uint16_t tile_index, const bool use_right_pt)
{
	uint16_t nt_address = nametable_offset | tile_index;
	uint8_t pt_offset = this->_cart->read_character(nt_address);
	uint16_t tile_offset = static_cast<uint16_t>(pt_offset) << 4;

	uint16_t right_pattern_table_offset = 0b0001'0000'0000'0000;
	uint16_t plane1_offset = 0b0000'00000'0000'1000;

	uint16_t base_address = tile_offset;
	if (use_right_pt)
		base_address |= right_pattern_table_offset;

	for (int y = 0; y < 8; ++y)
	{
		uint16_t address = base_address | y;

		uint8_t low_byte = this->_cart->read_character(address);
		address |= plane1_offset;
		uint8_t high_byte = this->_cart->read_character(address);

		auto chunk = this->compilePatternTableBytes(low_byte, high_byte);
		for (int x = 0; x < 8; ++x)
		{
			uint64_t indexed_image_index = ((y_offset + y) * nt_width * 2) + x_offset + x;
			this->_indexed_image[indexed_image_index] = chunk[x];
		}
	}
}

void Nametable::colorTile(const uint64_t x_offset, const uint64_t y_offset,
	const PaletteType palette_type, NametableImage& image)
{
	Palette palette = this->_palettes[palette_type];

	for (int fine_y = 0; fine_y < 8; ++fine_y)
	{
		for (int fine_x = 0; fine_x < 8; ++fine_x)
		{
			uint64_t image_index = ((y_offset + fine_y) * nt_width * 2) + x_offset + fine_x;
			auto image_index_value = this->_indexed_image[image_index];

			switch (image_index_value)
			{
			case 0:
				image[image_index] = palette.index0.pixel();
				break;
			case 1:
				image[image_index] = palette.index1.pixel();
				break;
			case 2:
				image[image_index] = palette.index2.pixel();
				break;
			case 3:
				image[image_index] = palette.index3.pixel();
				break;
			default:
				throw std::runtime_error("Invalid palette index");
				break;
			}
		}
	}
}

void Nametable::colorTile(const uint64_t x_offset, const uint64_t y_offset,
	const uint16_t nametable_offset, const uint16_t tile_index, NametableImage& image)
{
	uint8_t palette_index = this->getPaletteIndex(nametable_offset, tile_index);
	if (palette_index != 0)
	{
		auto here = 0;
	}
	PaletteType palette_type = this->getBackgroundPalette(palette_index);
	this->colorTile(x_offset, y_offset, palette_type, image);
}

PaletteType Nametable::getBackgroundPalette(const uint8_t palette_index) const
{
	switch (palette_index)
	{
	case 0:
		return PaletteType::Background0;
	case 1:
		return PaletteType::Background1;
	case 2:
		return PaletteType::Background2;
	case 3:
		return PaletteType::Background3;
	}

	throw std::runtime_error("Invalid palette index");
}

uint8_t Nametable::getPaletteIndex(const uint16_t nametable_offset, const uint16_t tile_index) const
{
	uint16_t y_offset = tile_index / num_tiles_wide;
	uint16_t coarse_y_offset = (y_offset & 0b0001'1100) << 1;
	uint16_t x_offset = tile_index % num_tiles_wide;
	uint16_t coarse_x_offset = (x_offset & 0b0001'1100) >> 2;

	uint16_t attribute_table_offset = 0b0011'1100'0000;
	uint16_t attribute_address =
		nametable_offset | attribute_table_offset | coarse_x_offset | coarse_y_offset;
	uint8_t attribute_data = this->_cart->read_character(attribute_address);

	// will be 0, 2, 4, or 6
	uint8_t shift_ammount =
		static_cast<uint8_t>((x_offset & 0b0000'0010) | ((y_offset & 0b0000'0010) << 1));
	uint8_t palette_index = (attribute_data >> shift_ammount) & 0b0000'0011;
	return palette_index;
}
