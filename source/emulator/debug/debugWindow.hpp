#pragma once

#include <memory>

#include <platform/platformBase.hpp>

class IRenderer;
class ISubWindow;
class IWindow;
class Nes;

const int sprite_table_height = 128;
const int palette_table_height = 4;
const int pattern_table_height = 128;
const int pattern_table_width = 128;

class DebugWindow
{
public:
	DebugWindow() = default;
	virtual ~DebugWindow();

	void open(IWindow* window, Nes* nes);
	IRenderer* getRenderer() { return this->_renderer; }

private:
	ISubWindow* _window {nullptr};
	IRenderer* _renderer {nullptr};
	Nes* _nes;

	uint8_t _pattern_table_scale {2};
	int _pattern_table_height {pattern_table_height * _pattern_table_scale};
	int _pattern_table_width {pattern_table_width * _pattern_table_scale};

	uint8_t _sprite_table_scale {2};
	uint8_t _palette_table_scale {2};

	int _height {262 * 3};
	int _width {_pattern_table_width + 1 + _pattern_table_width};

	void render(Milliseconds now, Milliseconds delta_ms);
	void renderFrame();
};
