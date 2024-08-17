#pragma once

#include <memory>

#include <memory/debugging/patternTable.hpp>

#include "nes.hpp"

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Window;

using Milliseconds = uint64_t;

class Window
{
public:
	Window() = default;
	virtual ~Window() { close(); }

	bool open();
	void close();

	void run();
	void renderFrame(NesFrame frame);

	void openFileDialog();
	void step();
	void leap();
	void bound();

private:
	// int _height {262};
	// int _width {341};
	// int _scale_factor {3};

	int _nes_base_height {262};
	int _nes_base_width {341};
	int _nes_scale {3};
	int _nes_height {_nes_base_height * _nes_scale};
	int _nes_width {_nes_base_width * _nes_scale};
	SDL_Texture* _nes_texture {nullptr};
	std::unique_ptr<Nes> _nes;

	int _pattern_table_base_height {128};
	int _pattern_table_base_width {128};
	int _pattern_table_scale {2};
	int _pattern_table_height {_pattern_table_base_height * _pattern_table_scale};
	int _pattern_table_width {_pattern_table_base_width * _pattern_table_scale};
	SDL_Texture* _left_pattern_table_texture {nullptr};
	SDL_Texture* _right_pattern_table_texture {nullptr};

	int _height {_nes_height};
	int _width {_nes_width + 1 + 2 * _pattern_table_width + 1};

	SDL_Window* _window {nullptr};
	SDL_Renderer* _renderer {nullptr};

	bool _run {true};
	bool _debugging {true};

	uint8_t _ppu_cycles_per_update {150};
	Milliseconds _frametime {33};
	Milliseconds _last_draw {0};
	Milliseconds _prev_time {0};

	Milliseconds getTime() const;
	void update(Milliseconds now, Milliseconds delta_ms);
	void render(Milliseconds now, Milliseconds delta_ms);

	void prepareScene();
	void presentScene();

	void handleEvent(const SDL_Event& e);
};
