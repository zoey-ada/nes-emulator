#pragma once

#include <array>
#include <memory>

#include <base/pixel.hpp>
#include <cartridge/cartridgeLoader.hpp>
#include <cpu/cpu.hpp>
#include <cpu/memoryMapper.hpp>
#include <dma/dma.hpp>
#include <ppu/ppu.hpp>
#include <ppu/ppuMemoryMapper.hpp>

#include "debug/cpuRenderer.hpp"
#include "debug/debugCpu.hpp"
#include "debug/debugPpu.hpp"
#include "sdlController.hpp"

class Cartridge;
struct SDL_Renderer;
struct SDL_Texture;

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

	void produceNesFrame();
	NesFrame getFrame() { return this->_frame; }
	SDL_Texture* getLeftPtTexture() const;
	SDL_Texture* getRightPtTexture() const;
	SDL_Texture* getCpuDebugTexture() const;

	void loadFile(const std::string& filepath);
	inline bool isGameRunning() const { return this->_game_loaded; }

	void cycle();
	void cycle(uint64_t num)
	{
		for (int i = 0; i < num; ++i)
			this->cycle();
	}

	void step();
	void leap();
	void bound();

	void nextFrame();

	void nextPalette();
	void prevPalette();

	void dumpMemory();

private:
	std::unique_ptr<MemoryMapper> _cpu_memory;
	std::unique_ptr<PpuMemoryMapper> _ppu_memory;
	std::unique_ptr<IMemory> _oam;
	std::unique_ptr<DirectMemoryAccess> _dma;

	std::unique_ptr<Cpu> _cpu;
	std::unique_ptr<DebugCpu> _debug_cpu;

	std::unique_ptr<PictureProcessingUnit> _ppu;
	std::unique_ptr<DebugPpu> _debug_ppu;

	std::unique_ptr<CartridgeLoader> _cart_loader;
	std::unique_ptr<Cartridge> _cart;

	std::unique_ptr<SdlController> _p1_controller;

	NesFrame _frame;
	bool _game_loaded {false};

	uint8_t _cpu_cycle_delay {cpu_cycle_delay};
	uint64_t _current_cycle {0};
	bool _is_even_frame {false};

	bool _debug_mode {true};

	void blankFrame();
	void resetCurrentCycle();

	void setupDebugNes(SDL_Renderer* renderer);
	void setupNes();
};
