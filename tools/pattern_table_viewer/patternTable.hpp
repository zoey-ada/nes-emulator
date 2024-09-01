#pragma once

#include <array>
#include <cstdint>
#include <memory>

#include <base/pixel.hpp>
#include <cartridge/cartridgeLoader.hpp>
#include <platform/iRenderer.hpp>

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
	PatternTable(PatternTableType pt_type, std::shared_ptr<IRenderer> renderer);
	virtual ~PatternTable();

	void loadFile(const std::string& filepath);
	void draw();
	void color();

	Texture getTexture() const { return this->_texture; }

private:
	std::unique_ptr<CartridgeLoader> _cart_loader;
	std::unique_ptr<Cartridge> _cart;

	PatternTableIndexedImage _indexed_image;
	std::shared_ptr<IRenderer> _renderer {nullptr};
	Texture _texture {nullptr};

	PatternTableType _pt_type;
	int _width {128};
	int _height {128};

	void updateTexture(const PatternTableImage& pixel_data);
	std::array<uint8_t, 8> compilePatternTableBytes(uint8_t high_byte, uint8_t low_byte);
};
