#pragma once

#include <array>
#include <cstdint>
#include <memory>

#include "../cartridge/cartridge.hpp"

struct PtPixel
{
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t padding;
};

const uint16_t pt_width_in_pixels = 128;
const uint16_t pt_height_in_pixels = 128;
const uint64_t pt_total_pixels = pt_width_in_pixels * pt_height_in_pixels;

using PtFrame = std::array<PtPixel, pt_total_pixels>;

class PatternTable
{
public:
	PatternTable(bool right_table);
	virtual ~PatternTable();

	void produceFrame();
	void loadCartridge(Cartridge* cart);
	PtFrame getFrame() { return this->_frame; }

private:
	std::array<uint32_t, 8 * 8> getTile(const uint8_t tile_num);
	uint32_t indexToPixel(uint8_t index);

	Cartridge* _cart {nullptr};

	bool _is_right_table {0};

	PtFrame _frame;
};
