#pragma once

#include <array>
#include <memory>

#include <SDL.h>
#include <base/pixel.hpp>
#include <cpu/cpu.hpp>
#include <cpu/debugCpu.hpp>
#include <memory/cartridge/cartridgeLoader.hpp>
#include <memory/debugging/patternTable.hpp>
#include <memory/memoryMapper.hpp>
#include <memory/ppuMemoryMapper.hpp>
#include <ppu/ppu.hpp>

#include "cpuRenderer.hpp"

class Cartridge;

const uint64_t even_cycles = 89342;
const uint64_t odd_cycles = 89341;
const uint8_t cpu_cycle_delay = 2;

const uint16_t nes_width_in_pixels = 341;
const uint16_t nes_height_in_pixels = 262;
const uint64_t nes_total_pixels = nes_width_in_pixels * nes_height_in_pixels;

using NesFrame = std::array<Pixel, nes_total_pixels>;

class Nes
{
public:
	Nes(SDL_Renderer* renderer);

	void produceFrame();
	void produceNesFrame();
	NesFrame getFrame() { return this->_frame; }
	PtFrame getLeftPatternTableFrame() { return this->_left->getFrame(); }
	PtFrame getRightPatternTableFrame() { return this->_right->getFrame(); }
	SDL_Texture* getCpuDebugTexture() { return this->_cpu_renderer->getTexture(); }

	void loadFile(const std::string& filepath);
	inline bool isGameRunning() const { return this->_game_loaded; }

	void cycle();
	void cycle(uint8_t num)
	{
		for (int i = 0; i < num; ++i)
			this->cycle();
	}

	void step();
	void leap();
	void bound();

private:
	std::unique_ptr<MemoryMapper> _memory;
	std::unique_ptr<PpuMemoryMapper> _ppu_memory;
	std::unique_ptr<CartridgeLoader> _cart_loader;
	std::unique_ptr<Cpu> _cpu;
	std::unique_ptr<DebugCpu> _debug_cpu;
	std::unique_ptr<PictureProcessingUnit> _ppu;
	std::unique_ptr<Cartridge> _cart;

	NesFrame _frame;
	bool _game_loaded {false};

	uint8_t _cpu_cycle_delay {cpu_cycle_delay};
	uint64_t _current_cycle {0};
	bool _is_even_frame {false};

	// debug
	std::unique_ptr<PatternTable> _left;
	std::unique_ptr<PatternTable> _right;
	std::unique_ptr<CpuRenderer> _cpu_renderer;

	void blankFrame();
	void resetCurrentCycle();
};
