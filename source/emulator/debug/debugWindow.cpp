#include "debugWindow.hpp"

#include <functional>

#include <platform/iRenderer.hpp>
#include <platform/window/iSubWindow.hpp>
#include <platform/window/iWindow.hpp>

#include "../nes.hpp"

namespace ph = std::placeholders;

DebugWindow::~DebugWindow()
{
	this->_window = nullptr;
	this->_renderer = nullptr;
	this->_nes = nullptr;
}

void DebugWindow::open(IWindow* window, Nes* nes)
{
	this->_nes = nes;

	RenderFuncDelegate render_func = std::bind(&DebugWindow::render, this, ph::_1, ph::_2);
	SubWindowCreateInfo sub_window_info = {this->_width, this->_height, render_func};
	this->_window = window->openSubWindow(sub_window_info);
	this->_renderer = this->_window->getRenderer();
	this->_renderer->setBackgroundColor({0x22, 0x22, 0x22, 0xff});
	this->_nes->setDebugRenderer(this->_renderer);
}

void DebugWindow::render(Milliseconds now, Milliseconds delta_ms)
{
	this->_renderer->preRender();
	this->renderFrame();
	this->_renderer->postRender();
}

void DebugWindow::renderFrame()
{
	// nametable
	Rect dest_rect = {
		0,
		0,
		this->_nametable_width,
		this->_nametable_height,
	};
	this->_renderer->drawTexture(this->_nes->getNametableTexture(), dest_rect);

	// cpu debug
	Rect cpu_debug_size = this->_renderer->measureTexture(this->_nes->getCpuDebugTexture());
	dest_rect = {
		this->_nametable_width + 1,
		0,
		cpu_debug_size.width,
		cpu_debug_size.height,
	};
	this->_renderer->drawTexture(this->_nes->getCpuDebugTexture(), dest_rect);

	// left pattern table
	auto left_pt_size = this->_renderer->measureTexture(this->_nes->getLeftPtTexture());
	dest_rect = {
		this->_nametable_width + 1,
		this->_height - left_pt_size.height * this->_pattern_table_scale,
		left_pt_size.width * this->_pattern_table_scale,
		left_pt_size.height * this->_pattern_table_scale,
	};
	this->_renderer->drawTexture(this->_nes->getLeftPtTexture(), dest_rect);

	// right pattern table
	auto right_pt_size = this->_renderer->measureTexture(this->_nes->getRightPtTexture());
	dest_rect = {
		this->_nametable_width + 1 + left_pt_size.width * this->_pattern_table_scale + 1,
		this->_height - right_pt_size.height * this->_pattern_table_scale,
		right_pt_size.width * this->_pattern_table_scale,
		right_pt_size.height * this->_pattern_table_scale,
	};
	this->_renderer->drawTexture(this->_nes->getRightPtTexture(), dest_rect);

	// palette table
	Rect palette_table_size = this->_renderer->measureTexture(this->_nes->getPaletteTableTexture());
	dest_rect = {
		this->_nametable_width + 1,
		this->_height - left_pt_size.height * this->_pattern_table_scale - 1 -
			(palette_table_size.height * this->_palette_table_scale),
		palette_table_size.width * this->_palette_table_scale,
		palette_table_size.height * this->_palette_table_scale,
	};
	this->_renderer->drawTexture(this->_nes->getPaletteTableTexture(), dest_rect);

	// sprite table
	Rect sprite_table_size = this->_renderer->measureTexture(this->_nes->getSpriteTableTexture());
	dest_rect = {
		this->_nametable_width + 1,
		this->_height - (left_pt_size.height * this->_pattern_table_scale) - 1 -
			(palette_table_size.height * this->_palette_table_scale) - 1 -
			(sprite_table_size.height * this->_sprite_table_scale),
		sprite_table_size.width * this->_sprite_table_scale,
		sprite_table_size.height * this->_sprite_table_scale,
	};
	this->_renderer->drawTexture(this->_nes->getSpriteTableTexture(), dest_rect);
}
