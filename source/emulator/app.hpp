#pragma once

#include <cstdint>
#include <memory>

#include <platform/window/iWindow.hpp>

#include "debug/debugWindow.hpp"
#include "nes.hpp"

class App
{
public:
	App() = default;
	virtual ~App();
	void run();

private:
	std::unique_ptr<IWindow> _window {nullptr};
	std::shared_ptr<IRenderer> _renderer {nullptr};
	std::unique_ptr<Nes> _nes;

	DebugWindow _debug_window;

	uint64_t _nes_base_height {262};
	uint64_t _nes_base_width {341};
	uint8_t _nes_scale {3};
	uint64_t _nes_height {_nes_base_height * _nes_scale};
	uint64_t _nes_width {_nes_base_width * _nes_scale};
	Texture _nes_texture {nullptr};

	uint64_t _pattern_table_base_height {128};
	uint64_t _pattern_table_base_width {128};
	uint8_t _pattern_table_scale {2};
	uint64_t _pattern_table_height {_pattern_table_base_height * _pattern_table_scale};
	uint64_t _pattern_table_width {_pattern_table_base_width * _pattern_table_scale};

	uint8_t _sprite_table_scale {2};
	uint8_t _palette_table_scale {2};

	uint64_t _height {_nes_height};
	uint64_t _width {_nes_width};
	uint8_t _frame_rate {30};

	bool _is_paused {true};

	uint64_t _ppu_cycles_per_update {512};

	void createWindow();
	void createNes();

	void update(Milliseconds now, Milliseconds delta_ms);
	void render(Milliseconds now, Milliseconds delta_ms);
	void handleEvent(const SDL_Event& e);

	void step();
	void leap();
	void bound();
	void renderFrame(NesFrame frame);
};
