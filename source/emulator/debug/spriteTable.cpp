#include "spriteTable.hpp"

#include <base/iMemory.hpp>
#include <cartridge/cartridge.hpp>

SpriteTable::SpriteTable(IRenderer* renderer, IMemory* oam): _renderer(renderer), _oam(oam)
{
	this->loadRenderer();
}

SpriteTable::~SpriteTable()
{
	this->unloadRenderer();
	this->_cart = nullptr;
	this->_oam = nullptr;
}

void SpriteTable::draw(const bool use_right_table, const bool use_8x16_sprites)
{
	if (!this->_cart)
		return;

	for (uint16_t y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			uint8_t index = y * 8 + x;
			if (!use_8x16_sprites)
				this->draw8x8Sprite(index, x, y, use_right_table);
			else
				draw8x16Sprite(index, x, y, use_right_table);
		}
	}

	this->color(use_8x16_sprites);
}

void SpriteTable::color(const bool use_8x16_sprites)
{
	if (!this->_cart)
		return;

	SpriteTableImage image;

	for (uint8_t y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			uint8_t index = y * 8 + x;
			if (!use_8x16_sprites)
				this->color8x8Sprite(index, x, y, image);
			else
				color8x16Sprite(index, x, y, image);
		}
	}

	this->updateTexture(image);
}

void SpriteTable::loadCartridge(Cartridge* cart, const bool use_right_table,
	const bool use_8x16_sprites)
{
	this->_cart = cart;
	this->draw(use_right_table, use_8x16_sprites);
	this->color(use_8x16_sprites);
}

void SpriteTable::updatePalettes(const std::map<PaletteType, Palette>& palettes,
	const bool use_8x16_sprites)
{
	this->_palettes = palettes;
	this->color(use_8x16_sprites);
}

void SpriteTable::updateTexture(const SpriteTableImage& pixel_data)
{
	this->_renderer->updateTexture(this->_texture, pixel_data.data(), sizeof(pixel_data));
}

void SpriteTable::setRenderer(IRenderer* renderer)
{
	this->unloadRenderer();
	this->_renderer = renderer;
	this->loadRenderer();
}

void SpriteTable::loadRenderer()
{
	this->_texture = this->_renderer->createTexture(this->_width, this->_height);

	SpriteTableImage image;
	image.fill({0x00, 0x00, 0x00});
	this->updateTexture(image);
}

void SpriteTable::unloadRenderer()
{
	if (this->_texture)
		this->_renderer->destroyTexture(this->_texture);
	this->_texture = nullptr;

	this->_renderer = nullptr;
}

std::array<uint8_t, 8> SpriteTable::compilePatternTableBytes(const uint8_t low_byte,
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

void SpriteTable::draw8x8Sprite(const uint8_t sprite_index, const uint8_t x, const uint8_t y,
	const bool use_right_table)
{
	if (!this->_cart)
		return;

	uint16_t pt_index = this->_oam->read(sprite_index * 4 + 1) << 4;
	uint8_t attribute_info = this->_oam->read(sprite_index * 4 + 2);
	// TODO: handle flipping

	uint16_t pt_offset = use_right_table ? 0b0001'0000'0000'0000 : 0;
	uint16_t plane1_offset = 0b0000'0000'0000'1000;

	for (uint16_t fine_y = 0; fine_y < 8; ++fine_y)
	{
		uint16_t address = pt_offset | pt_index | fine_y;
		uint8_t plane0 = this->_cart->read_character(address);
		uint8_t plane1 = this->_cart->read_character(address + plane1_offset);
		auto chunk = this->compilePatternTableBytes(plane0, plane1);

		for (int fine_x = 0; fine_x < 8; ++fine_x)
		{
			int indexed_image_index = (((y * 8) + fine_y) * st_width_in_pixels) + (x * 8) + fine_x;
			this->_indexed_image[indexed_image_index] = chunk[fine_x];
		}
	}
}

void SpriteTable::draw8x16Sprite(const uint8_t sprite_index, const uint8_t x, const uint8_t y,
	const bool use_right_table)
{
	if (!this->_cart)
		return;

	uint16_t pt_index = this->_oam->read(sprite_index * 4 + 1) << 4;
	uint8_t attribute_info = this->_oam->read(sprite_index * 4 + 2);
	// TODO: handle flipping

	// uint16_t pt_offset = use_right_table ? 0b0001'0000'0000'0000 : 0;
	// uint16_t plane1_offset = 0b0000'0000'0000'1000;

	// for (uint16_t fine_y = 0; fine_y < 8; ++fine_y)
	// {
	// 	uint16_t address = pt_offset | pt_index | fine_y;
	// 	uint8_t plane0 = this->_cart->read_character(address);
	// 	uint8_t plane1 = this->_cart->read_character(address + plane1_offset);
	// 	auto chunk = this->compilePatternTableBytes(plane0, plane1);

	// 	for (int fine_x = 0; fine_x < 8; ++fine_x)
	// 	{
	// 		int indexed_image_index = (((y * 8) + fine_y) * st_width_in_pixels) + (x * 8) + fine_x;
	// 		this->_indexed_image[indexed_image_index] = chunk[fine_x];
	// 	}
	// }
}

void SpriteTable::color8x8Sprite(const uint8_t sprite_index, const uint8_t x, const uint8_t y,
	SpriteTableImage& image)
{
	if (!this->_cart)
		return;

	uint8_t attribute_info = this->_oam->read(sprite_index * 4 + 2);
	uint8_t palette_index = attribute_info & 0b0000'0011;
	Palette palette = this->_palettes[PaletteType::Grayscale];
	if (palette_index == 0)
		palette = this->_palettes[PaletteType::Sprite0];
	else if (palette_index == 1)
		palette = this->_palettes[PaletteType::Sprite1];
	else if (palette_index == 2)
		palette = this->_palettes[PaletteType::Sprite2];
	else if (palette_index == 3)
		palette = this->_palettes[PaletteType::Sprite3];

	for (int fine_y = 0; fine_y < 8; ++fine_y)
	{
		for (int fine_x = 0; fine_x < 8; ++fine_x)
		{
			int image_index = (((y * 8) + fine_y) * st_width_in_pixels) + (x * 8) + fine_x;
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
				image[image_index] = {0x00, 0x00, 0x00};
				break;
			}
		}
	}
}

void SpriteTable::color8x16Sprite(const uint8_t sprite_index, const uint8_t x, const uint8_t y,
	SpriteTableImage& image)
{
	if (!this->_cart)
		return;

	uint8_t attribute_info = this->_oam->read(sprite_index * 4 + 2);
	uint8_t palette_index = attribute_info & 0b0000'0011;
	Palette palette = this->_palettes[PaletteType::Grayscale];
	if (palette_index == 0)
		palette = this->_palettes[PaletteType::Sprite0];
	else if (palette_index == 1)
		palette = this->_palettes[PaletteType::Sprite1];
	else if (palette_index == 2)
		palette = this->_palettes[PaletteType::Sprite2];
	else if (palette_index == 3)
		palette = this->_palettes[PaletteType::Sprite3];

	for (int fine_y = 0; fine_y < 16; ++fine_y)
	{
		for (int fine_x = 0; fine_x < 8; ++fine_x)
		{
			int image_index = (((y * 8) + fine_y) * st_width_in_pixels) + (x * 8) + fine_x;
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
				image[image_index] = {0x00, 0x00, 0x00};
				break;
			}
		}
	}
}
