#include "patternTable.hpp"

#include <SDL.h>
#include <cartridge/cartridge.hpp>

PatternTable::PatternTable(PatternTableType pt_type, std::shared_ptr<IRenderer> renderer)
	: _pt_type(pt_type), _renderer(renderer)
{
	this->_texture = this->_renderer->createTexture(this->_width, this->_height);

	PatternTableImage image;
	image.fill({0x00, 0x00, 0x00});
	this->updateTexture(image);
}

PatternTable::~PatternTable()
{
	if (this->_texture)
		this->_renderer->destroyTexture(this->_texture);
	this->_texture = nullptr;

	this->_cart = nullptr;
}

void PatternTable::draw()
{
	if (!this->_cart)
		return;

	uint16_t pt_offset = this->_pt_type == PatternTableType::Left ? 0 : 0b0001'0000'0000'0000;
	uint16_t plane1_offset = 0b0000'0000'0000'1000;

	for (uint16_t y = 0; y < 16; ++y)
	{
		uint16_t coarse_y_offset = y << 8;

		for (int x = 0; x < 16; ++x)
		{
			uint16_t coarse_x_offset = x << 4;

			for (uint16_t fine_y = 0; fine_y < 8; ++fine_y)
			{
				uint16_t address = pt_offset | coarse_y_offset | coarse_x_offset | fine_y;
				uint8_t plane0 = this->_cart->read_character(address);
				uint8_t plane1 = this->_cart->read_character(address + plane1_offset);
				auto chunk = this->compilePatternTableBytes(plane0, plane1);

				for (int fine_x = 0; fine_x < 8; ++fine_x)
				{
					int indexed_image_index = (((y * 8) + fine_y) * 128) + (x * 8) + fine_x;
					this->_indexed_image[indexed_image_index] = chunk[fine_x];
				}
			}
		}
	}

	this->color();
}

void PatternTable::color()
{
	if (!this->_cart)
		return;

	PatternTableImage image;

	for (uint64_t i = 0; i < this->_indexed_image.size(); ++i)
	{
		const auto& index = this->_indexed_image[i];
		switch (index)
		{
		case 0:
			image[i] = this->_palette.index0.pixel();
			break;
		case 1:
			image[i] = this->_palette.index1.pixel();
			break;
		case 2:
			image[i] = this->_palette.index2.pixel();
			break;
		case 3:
			image[i] = this->_palette.index3.pixel();
			break;
		default:
			image[i] = {0xff, 0x00, 0xff};
			break;
		}
	}

	this->updateTexture(image);
}

void PatternTable::loadCartridge(Cartridge* cart)
{
	this->_cart = cart;
	this->draw();
	this->color();
}

void PatternTable::loadPalette(Palette palette)
{
	this->_palette = palette;
	this->color();
}

void PatternTable::updateTexture(const PatternTableImage& pixel_data)
{
	this->_renderer->updateTexture(this->_texture, pixel_data.data(), sizeof(pixel_data));
}

std::array<uint8_t, 8> PatternTable::compilePatternTableBytes(uint8_t low_byte, uint8_t high_byte)
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
