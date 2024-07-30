#pragma once

#include <array>
#include <memory>

#include <cpu/cpu.hpp>
#include <memory/memoryMapper.hpp>
#include <ppu/ppu.hpp>

struct Pixel
{
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t padding;
};

const uint16_t width_in_pixels = 341;
const uint16_t height_in_pixels = 262;
const uint64_t total_pixels = width_in_pixels * height_in_pixels;

using Frame = std::array<Pixel, total_pixels>;

class Nes
{
public:
	Nes();

	void produceFrame();
	Frame getFrame() { return this->_frame; }

private:
	std::unique_ptr<IMemory> _memory;
	std::unique_ptr<Cpu> _cpu;
	std::unique_ptr<PictureProcessingUnit> _ppu;

	Frame _frame;
};
