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

	int _nes_base_height {262};
	int _nes_base_width {341};
	int _nes_scale {3};
	int _nes_height {_nes_base_height * _nes_scale};
	int _nes_width {_nes_base_width * _nes_scale};
	Texture _nes_texture {nullptr};

	int _pattern_table_base_height {128};
	int _pattern_table_base_width {128};
	int _pattern_table_scale {2};
	int _pattern_table_height {_pattern_table_base_height * _pattern_table_scale};
	int _pattern_table_width {_pattern_table_base_width * _pattern_table_scale};

	int _sprite_table_scale {2};
	int _palette_table_scale {2};

	int _height {_nes_height};
	int _width {_nes_width};

	bool _is_paused {true};

	uint64_t _ppu_cycles_per_update {512};

	void update(Milliseconds now, Milliseconds delta_ms);
	void render(Milliseconds now, Milliseconds delta_ms);
	void handleEvent(const SDL_Event& e);

	void step();
	void leap();
	void bound();
	void renderFrame(NesFrame frame);
};
