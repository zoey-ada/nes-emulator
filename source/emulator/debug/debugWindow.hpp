#pragma once

#include <memory>

#include <platform/platformBase.hpp>

class IRenderer;
class ISubWindow;
class IWindow;
class Nes;

const uint16_t nametable_height = 240 * 2;
const uint16_t nametable_width = 256 * 2;
const uint16_t sprite_table_height = 128;
const uint16_t palette_table_height = 4;
const uint16_t pattern_table_height = 128;
const uint16_t pattern_table_width = 128;

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
	uint32_t _pattern_table_height {
		static_cast<uint32_t>(pattern_table_height * _pattern_table_scale)};
	uint32_t _pattern_table_width {
		static_cast<uint32_t>(pattern_table_width * _pattern_table_scale)};

	uint8_t _nametable_scale {2};
	uint32_t _nametable_height {static_cast<uint32_t>(nametable_height * _nametable_scale)};
	uint32_t _nametable_width {static_cast<uint32_t>(nametable_width * _nametable_scale)};

	uint8_t _sprite_table_scale {2};
	uint8_t _palette_table_scale {2};

	uint32_t _height {_nametable_height};
	uint32_t _width {_nametable_width + 1 + _pattern_table_width + 1 + _pattern_table_width};

	void render(Milliseconds now, Milliseconds delta_ms);
	void renderFrame();
};
