#pragma once

#include <array>
#include <memory>

#include <cpu/cpu.hpp>
#include <memory/cartridge/cartridgeLoader.hpp>
#include <memory/debugging/patternTable.hpp>
#include <memory/memoryMapper.hpp>
#include <memory/ppuMemoryMapper.hpp>
#include <ppu/ppu.hpp>

class Cartridge;

struct NesPixel
{
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t padding;
};

const uint16_t nes_width_in_pixels = 341;
const uint16_t nes_height_in_pixels = 262;
const uint64_t nes_total_pixels = nes_width_in_pixels * nes_height_in_pixels;

using NesFrame = std::array<NesPixel, nes_total_pixels>;

class Nes
{
public:
	Nes();

	void produceFrame();
	void produceNesFrame();
	NesFrame getFrame() { return this->_frame; }
	PtFrame getLeftPatternTableFrame() { return this->_left->getFrame(); }
	PtFrame getRightPatternTableFrame() { return this->_right->getFrame(); }

	void loadFile(const std::string& filepath);

private:
	std::unique_ptr<MemoryMapper> _memory;
	std::unique_ptr<PpuMemoryMapper> _ppu_memory;
	std::unique_ptr<CartridgeLoader> _cart_loader;
	std::unique_ptr<Cpu> _cpu;
	std::unique_ptr<PictureProcessingUnit> _ppu;
	std::unique_ptr<Cartridge> _cart;

	NesFrame _frame;

	// debug
	std::unique_ptr<PatternTable> _left;
	std::unique_ptr<PatternTable> _right;
};
