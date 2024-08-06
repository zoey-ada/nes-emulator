#include "patternTable.hpp"

#include "../cartridge/cartridge.hpp"

PatternTable::PatternTable(bool right_table): _is_right_table(right_table)
{}

PatternTable::~PatternTable()
{
	this->_cart = nullptr;
}

void PatternTable::produceFrame()
{
	if (!this->_cart)
	{
		for (auto& pixel : this->_frame)
		{
			pixel.r = 0x00;
			pixel.g = 0x00;
			pixel.b = 0x00;
		}

		return;
	}

	for (int y = 0; y < 16; ++y)
	{
		for (int x = 0; x < 16; ++x)
		{
			int tile_index = (y * 16) + x;
			auto tile = this->getTile(tile_index);

			// render tile
			int coarse_y = y * 8;
			int coarse_x = x * 8;

			for (int i = 0; i < 8; ++i)
			{
				int fine_y = i;
				for (int j = 0; j < 8; ++j)
				{
					int fine_x = j;

					auto tile_pixel_index = (i * 8) + j;
					auto wanted_pixel = tile[tile_pixel_index];

					int frame_index = ((coarse_y + fine_y) * 128) + coarse_x + fine_x;
					auto pixel = ((PtPixel*)&this->_frame[frame_index]);
					*((uint32_t*)pixel) = wanted_pixel;
				}
			}
		}
	}
}

void PatternTable::loadCartridge(Cartridge* cart)
{
	this->_cart = cart;
}

std::array<uint32_t, 8 * 8> PatternTable::getTile(const uint8_t tile_num)
{
	uint16_t pattern_table_offset = static_cast<uint16_t>(this->_is_right_table) << 12;
	uint16_t tile_offset = static_cast<uint16_t>(tile_num) << 4;
	uint16_t plane1_offset = static_cast<uint16_t>(0b00001000);

	std::array<uint32_t, 8 * 8> tile;

	for (int y = 0; y < 8; ++y)
	{
		uint16_t address = static_cast<uint16_t>(y) + tile_offset + pattern_table_offset;
		uint8_t plane0 = this->_cart->read_character(address);
		uint8_t plane1 = this->_cart->read_character(address + plane1_offset);

		for (int i = 0; i < 8; ++i)
		{
			uint8_t mask = 0b00000001 << i;
			uint16_t pixel_index = (y * 8) + (7 - i);
			uint8_t index = ((plane0 & mask) >> i) + (((plane1 & mask) >> i) << 1);
			tile[pixel_index] = this->indexToPixel(index);
		}
	}

	return tile;
}

uint32_t PatternTable::indexToPixel(uint8_t index)
{
	switch (index)
	{
	case 0:
		return 0x00ffffff;
	case 1:
		return 0x00aaaaaa;
	case 2:
		return 0x00555555;
	case 3:
		return 0x00000000;
	default:
		return 0x00ff00ff;
	}
}
