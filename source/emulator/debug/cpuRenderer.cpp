#include "cpuRenderer.hpp"

#include <cstdio>
#include <format>

CpuRenderer::CpuRenderer(IRenderer* renderer): _renderer(renderer)
{
	this->_font = this->_renderer->createFont(this->_font_name, this->_font_size);
	this->createMainTexture();
	this->renderStaticText();
}

CpuRenderer::~CpuRenderer()
{
	if (this->_texture)
		this->_renderer->destroyTexture(this->_texture);
	this->_texture = nullptr;

	if (this->_font)
		this->_renderer->destroyFont(this->_font);
	this->_font = nullptr;
}

void CpuRenderer::produceFrame(const CpuStackFrame& cycle_data)
{
	this->renderDynamicText(cycle_data);
}

void CpuRenderer::setRenderer(IRenderer* renderer)
{
	this->unloadRenderer();
	this->_renderer = renderer;
	this->loadRenderer();
}

void CpuRenderer::loadRenderer()
{
	this->_font = this->_renderer->createFont(this->_font_name, this->_font_size);
	this->createMainTexture();
	this->renderStaticText();
}

void CpuRenderer::unloadRenderer()
{
	if (this->_texture)
		this->_renderer->destroyTexture(this->_texture);
	this->_texture = nullptr;

	if (this->_font)
		this->_renderer->destroyFont(this->_font);
	this->_font = nullptr;

	this->_renderer = nullptr;
}

void CpuRenderer::createMainTexture()
{
	this->_texture = this->_renderer->createTexture(this->_width, this->_height, false);

	auto swapped_target = this->_renderer->swapRenderTarget(this->_texture);
	SDL_Color black = {0x00, 0x00, 0x00, 0xff};
	auto size = this->_renderer->measureTexture(this->_texture);
	this->_renderer->drawRectangle(size, black, true);
}

void CpuRenderer::renderStaticText()
{
	auto textures = this->generateStaticText();

	auto swapped_target = this->_renderer->swapRenderTarget(this->_texture);

	Rect main_rect = this->_renderer->measureTexture(this->_texture);
	Rect source_rect = this->_renderer->measureTexture(textures.label_texture_1);
	Rect dest_rect = {
		0,
		this->_row_height,
		source_rect.width,
		source_rect.height,
	};
	this->_renderer->horizontallyCenterRectangle(main_rect, dest_rect);
	this->_renderer->drawTexture(textures.label_texture_1, dest_rect);

	source_rect = this->_renderer->measureTexture(textures.label_texture_2);
	dest_rect = {
		0,
		(this->_row_height * 3) + this->_vertical_padding,
		source_rect.width,
		source_rect.height,
	};
	this->_renderer->horizontallyCenterRectangle(main_rect, dest_rect);
	this->_renderer->drawTexture(textures.label_texture_2, dest_rect);

	this->destroyTextures(textures);
}

StaticTextures CpuRenderer::generateStaticText()
{
	SDL_Color white {0xff, 0xff, 0xff, 0xff};
	SDL_Color black {0x00, 0x00, 0x00, 0xff};
	TextRenderOptions text_render_options {this->_font, white, black};

	std::string row_1_labels = " A     X     Y     S     PC  ";
	auto label_texture_1 = this->_renderer->renderText(row_1_labels, text_render_options);

	std::string row_2_labels = " ADDR   DATA    N V   B D I Z C";
	auto label_texture_2 = this->_renderer->renderText(row_2_labels, text_render_options);

	return {label_texture_1, label_texture_2};
}

void CpuRenderer::destroyTextures(StaticTextures& textures)
{
	if (textures.label_texture_1)
		this->_renderer->destroyTexture(textures.label_texture_1);
	textures.label_texture_1 = nullptr;

	if (textures.label_texture_2)
		this->_renderer->destroyTexture(textures.label_texture_2);
	textures.label_texture_2 = nullptr;
}

void CpuRenderer::renderDynamicText(const CpuStackFrame& cycle_data)
{
	auto textures = this->generateDynamicText(cycle_data);

	auto swapped_target = this->_renderer->swapRenderTarget(this->_texture);

	Rect main_rect {0, 0, this->_width, this->_height};
	Rect source_rect = this->_renderer->measureTexture(textures.data_texture_1);
	Rect dest_rect = {
		0,
		0,
		source_rect.width,
		source_rect.height,
	};
	this->_renderer->horizontallyCenterRectangle(main_rect, dest_rect);
	this->_renderer->drawTexture(textures.data_texture_1, dest_rect);

	source_rect = this->_renderer->measureTexture(textures.data_texture_2);
	dest_rect = {
		0,
		(this->_row_height * 2) + this->_vertical_padding,
		source_rect.width,
		source_rect.height,
	};
	this->_renderer->horizontallyCenterRectangle(main_rect, dest_rect);
	this->_renderer->drawTexture(textures.data_texture_2, dest_rect);

	source_rect = this->_renderer->measureTexture(textures.instruction_texture);
	dest_rect = {
		10,
		(this->_row_height * 4) + (this->_vertical_padding * 2),
		source_rect.width,
		source_rect.height,
	};
	this->_renderer->drawTexture(textures.instruction_texture, dest_rect);

	this->destroyTextures(textures);
}

DynamicTextures CpuRenderer::generateDynamicText(const CpuStackFrame& cycle_data)
{
	SDL_Color white {0xff, 0xff, 0xff, 0xff};
	SDL_Color black {0x00, 0x00, 0x00, 0xff};
	TextRenderOptions text_render_options {this->_font, white, black};

	std::string row_1_data = std::format("{:#04x}  {:#04x}  {:#04x}  {:#04x}  {:#06x}",
		cycle_data.a, cycle_data.x, cycle_data.y, cycle_data.s, cycle_data.program_counter);
	auto data_texture_1 = this->_renderer->renderText(row_1_data, text_render_options);

	auto status = this->parseStatusFlags(cycle_data.p);
	std::string row_2_data = std::format("{:#06x}  {:#04x}    {:b} {:b} x {:b} {:b} {:b} {:b} {:b}",
		cycle_data.address_bus, cycle_data.data_bus, status.n, status.v, status.b, status.d,
		status.i, status.z, status.c);
	auto data_texture_2 = this->_renderer->renderText(row_2_data, text_render_options);

	auto instruction_texture =
		this->_renderer->renderText(cycle_data.instruction, text_render_options);

	return {data_texture_1, data_texture_2, instruction_texture};
}

void CpuRenderer::destroyTextures(DynamicTextures& textures)
{
	if (textures.data_texture_1)
		this->_renderer->destroyTexture(textures.data_texture_1);
	textures.data_texture_1 = nullptr;

	if (textures.data_texture_2)
		this->_renderer->destroyTexture(textures.data_texture_2);
	textures.data_texture_2 = nullptr;

	if (textures.instruction_texture)
		this->_renderer->destroyTexture(textures.instruction_texture);
	textures.instruction_texture = nullptr;
}

StatusFlags CpuRenderer::parseStatusFlags(uint8_t state)
{
	return {
		(state & 0b10000000) > 0,
		(state & 0b01000000) > 0,
		(state & 0b00010000) > 0,
		(state & 0b00001000) > 0,
		(state & 0b00000100) > 0,
		(state & 0b00000010) > 0,
		(state & 0b00000001) > 0,
	};
}
