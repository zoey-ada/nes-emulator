#pragma once

#include <array>
#include <cstdint>
#include <memory>

#include <memory/cartridge/cartridgeLoader.hpp>

struct Pixel
{
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t padding;
};

const uint16_t width_in_pixels = 128;
const uint16_t height_in_pixels = 128;
const uint64_t total_pixels = width_in_pixels * height_in_pixels;

using Frame = std::array<Pixel, total_pixels>;

class PatternTable
{
public:
	PatternTable(bool right_table);
	virtual ~PatternTable() = default;

	void produceFrame();
	Frame getFrame() { return this->_frame; }

	std::array<uint32_t, 8 * 8> getTile(const uint8_t tile_num);
	uint32_t indexToPixel(uint8_t index);

private:
	std::unique_ptr<CartridgeLoader> _cart_loader;
	std::unique_ptr<Cartridge> _cart;

	bool _is_right_table {0};

	Frame _frame;
};
