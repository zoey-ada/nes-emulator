#include "paletteRenderer.hpp"

#include <array>

PaletteRenderer::PaletteRenderer(IRenderer* renderer): _renderer(renderer)
{
	this->loadRenderer();
}

PaletteRenderer::~PaletteRenderer()
{
	this->unloadRenderer();
}

void PaletteRenderer::renderPalettes(const std::map<PaletteType, Palette>& palettes,
	const PaletteType selected_palette)
{
	this->_texture = this->_renderer->createTexture(this->_width, this->_height, false);
	auto swapped_target = this->_renderer->swapRenderTarget(this->_texture);
	auto size = this->_renderer->measureTexture(this->_texture);
	this->_renderer->drawRectangle(size, palette_background_color, true);

	std::array<PaletteType, 9> types = {PaletteType::Grayscale, PaletteType::Background0,
		PaletteType::Background1, PaletteType::Background2, PaletteType::Background3,
		PaletteType::Sprite0, PaletteType::Sprite1, PaletteType::Sprite2, PaletteType::Sprite3};

	for (int i = 0; i < types.size(); ++i)
	{
		int x_offset = i * (outline_width + palette_table_spacing) + palette_table_padding;
		bool is_selected = types[i] == selected_palette;
		this->renderPalette(palettes.at(types[i]), x_offset, is_selected);
	}
}

void PaletteRenderer::setRenderer(IRenderer* renderer)
{
	this->unloadRenderer();
	this->_renderer = renderer;
	this->loadRenderer();
}

void PaletteRenderer::loadRenderer()
{
	this->_texture = this->_renderer->createTexture(this->_width, this->_height);
}

void PaletteRenderer::unloadRenderer()
{
	if (this->_texture)
		this->_renderer->destroyTexture(this->_texture);
	this->_texture = nullptr;

	this->_renderer = nullptr;
}

void PaletteRenderer::renderPalette(const Palette& palette, const int x_offset,
	const bool is_selected)
{
	SDL_Color outline_color = is_selected ? selected_outline_color : unselected_outline_color;
	SDL_Color color1 = {palette.index0.r, palette.index0.g, palette.index0.b, 0xff};
	SDL_Color color2 = {palette.index1.r, palette.index1.g, palette.index1.b, 0xff};
	SDL_Color color3 = {palette.index2.r, palette.index2.g, palette.index2.b, 0xff};
	SDL_Color color4 = {palette.index3.r, palette.index3.g, palette.index3.b, 0xff};

	Rect dest = {
		x_offset,
		0,
		outline_width,
		outline_height,
	};
	this->_renderer->drawRectangle(dest, outline_color, false);

	dest = {
		x_offset + 1,
		1,
		palette_index_width,
		palette_index_height,
	};
	this->_renderer->drawRectangle(dest, color1, true);

	dest.x += palette_index_width;
	this->_renderer->drawRectangle(dest, color2, true);

	dest.x += palette_index_width;
	this->_renderer->drawRectangle(dest, color3, true);

	dest.x += palette_index_width;
	this->_renderer->drawRectangle(dest, color4, true);
}
