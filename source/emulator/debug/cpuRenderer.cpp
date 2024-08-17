#include "cpuRenderer.hpp"

#include <cstdio>
#include <format>

CpuRenderer::CpuRenderer(SDL_Renderer* renderer): _renderer(renderer)
{
	auto result = TTF_Init();
	if (result < 0)
	{
		printf("Couldn't initialize TTF. SDL_Error: %s\n", SDL_GetError());
		// return false;
	}

	this->_font = TTF_OpenFont(this->_font_name.c_str(), this->_font_size);
	if (this->_font == nullptr)
	{
		printf("Couldn't load font. SDL_Error: %s\n", SDL_GetError());
	}
	TTF_SetFontStyle(this->_font, TTF_STYLE_NORMAL);
	// TTF_SetFontOutline(this->_font, outline);
	// TTF_SetFontKerning(this->_font, 1);
	// TTF_SetFontHinting(this->_font, TTF_HINTING_NONE);

	this->createMainTexture();
	this->renderStaticText();
}

CpuRenderer::~CpuRenderer()
{
	if (this->_texture)
		SDL_DestroyTexture(this->_texture);
	this->_texture = nullptr;

	TTF_CloseFont(this->_font);
	this->_font = nullptr;

	TTF_Quit();

	this->_renderer = nullptr;
}

void CpuRenderer::produceFrame(const CpuStackFrame& cycle_data)
{
	this->renderDynamicText(cycle_data);
}

SDL_Texture* CpuRenderer::renderText(const std::string& text, const TextRenderOptions& options)
{
	auto temp_surface = TTF_RenderText_Shaded(options.font, text.c_str(), options.text_color,
		options.background_color);

	auto texture = SDL_CreateTextureFromSurface(this->_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);
	return texture;
}

void CpuRenderer::createMainTexture()
{
	this->_texture = SDL_CreateTexture(this->_renderer, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, this->_width, this->_height);

	SDL_SetRenderTarget(this->_renderer, this->_texture);
	{
		SDL_Rect main_rect = this->measureTexture(this->_texture);
		this->drawRectangle(main_rect, {0x00, 0x00, 0x00, 0xff});
	}
	SDL_SetRenderTarget(this->_renderer, nullptr);
}

void CpuRenderer::renderStaticText()
{
	auto textures = this->generateStaticText();

	SDL_SetRenderTarget(this->_renderer, this->_texture);
	{
		SDL_Rect main_rect = this->measureTexture(this->_texture);
		SDL_Rect source_rect = this->measureTexture(textures.label_texture_1);
		SDL_Rect dest_rect = {
			0,
			this->_row_height,
			source_rect.w,
			source_rect.h,
		};
		this->horizontallyCenterRects(main_rect, dest_rect);
		SDL_RenderCopy(this->_renderer, textures.label_texture_1, &source_rect, &dest_rect);

		source_rect = this->measureTexture(textures.label_texture_2);
		dest_rect = {
			0,
			(this->_row_height * 3) + this->_vertical_padding,
			source_rect.w,
			source_rect.h,
		};
		this->horizontallyCenterRects(main_rect, dest_rect);
		SDL_RenderCopy(this->_renderer, textures.label_texture_2, &source_rect, &dest_rect);
	}
	SDL_SetRenderTarget(this->_renderer, nullptr);

	this->destroyTextures(textures);
}

StaticTextures CpuRenderer::generateStaticText()
{
	TextRenderOptions text_render_options {this->_font};
	std::string row_1_labels = " A     X     Y     S     PC  ";
	auto label_texture_1 = this->renderText(row_1_labels, text_render_options);

	std::string row_2_labels = " ADDR   DATA    N V   B D I Z C";
	auto label_texture_2 = this->renderText(row_2_labels, text_render_options);

	return {label_texture_1, label_texture_2};
}

void CpuRenderer::destroyTextures(StaticTextures& textures)
{
	if (textures.label_texture_1)
		SDL_DestroyTexture(textures.label_texture_1);
	textures.label_texture_1 = nullptr;

	if (textures.label_texture_2)
		SDL_DestroyTexture(textures.label_texture_2);
	textures.label_texture_2 = nullptr;
}

void CpuRenderer::renderDynamicText(const CpuStackFrame& cycle_data)
{
	auto textures = this->generateDynamicText(cycle_data);

	SDL_SetRenderTarget(this->_renderer, this->_texture);
	{
		SDL_Rect main_rect {0, 0, this->_width, this->_height};
		SDL_Rect source_rect = this->measureTexture(textures.data_texture_1);
		SDL_Rect dest_rect = {
			0,
			0,
			source_rect.w,
			source_rect.h,
		};
		this->horizontallyCenterRects(main_rect, dest_rect);
		SDL_RenderCopy(this->_renderer, textures.data_texture_1, &source_rect, &dest_rect);

		source_rect = this->measureTexture(textures.data_texture_2);
		dest_rect = {
			0,
			(this->_row_height * 2) + this->_vertical_padding,
			source_rect.w,
			source_rect.h,
		};
		this->horizontallyCenterRects(main_rect, dest_rect);
		SDL_RenderCopy(this->_renderer, textures.data_texture_2, &source_rect, &dest_rect);

		source_rect = this->measureTexture(textures.instruction_texture);
		dest_rect = {
			10,
			(this->_row_height * 4) + (this->_vertical_padding * 2),
			source_rect.w,
			source_rect.h,
		};
		SDL_RenderCopy(this->_renderer, textures.instruction_texture, &source_rect, &dest_rect);
	}
	SDL_SetRenderTarget(this->_renderer, nullptr);

	this->destroyTextures(textures);
}

DynamicTextures CpuRenderer::generateDynamicText(const CpuStackFrame& cycle_data)
{
	TextRenderOptions text_render_options {this->_font};
	std::string row_1_data = std::format("{:#04x}  {:#04x}  {:#04x}  {:#04x}  {:#06x}",
		cycle_data.a, cycle_data.x, cycle_data.y, cycle_data.s, cycle_data.program_counter);
	auto data_texture_1 = this->renderText(row_1_data, text_render_options);

	auto status = this->parseStatusFlags(cycle_data.p);
	std::string row_2_data = std::format("{:#06x}  {:#04x}    {:b} {:b} x {:b} {:b} {:b} {:b} {:b}",
		cycle_data.address_bus, cycle_data.data_bus, status.n, status.v, status.b, status.d,
		status.i, status.z, status.c);
	auto data_texture_2 = this->renderText(row_2_data, text_render_options);

	auto instruction_texture = this->renderText(cycle_data.instruction, text_render_options);

	return {data_texture_1, data_texture_2, instruction_texture};
}

void CpuRenderer::destroyTextures(DynamicTextures& textures)
{
	if (textures.data_texture_1)
		SDL_DestroyTexture(textures.data_texture_1);
	textures.data_texture_1 = nullptr;

	if (textures.data_texture_2)
		SDL_DestroyTexture(textures.data_texture_2);
	textures.data_texture_2 = nullptr;

	if (textures.instruction_texture)
		SDL_DestroyTexture(textures.instruction_texture);
	textures.instruction_texture = nullptr;
}

void CpuRenderer::horizontallyCenterRects(const SDL_Rect& big_rect, SDL_Rect& small_rect)
{
	if (big_rect.w == small_rect.w)
	{
		small_rect.x = 0;
		return;
	}

	int h_diff = big_rect.w - small_rect.w;
	if (h_diff < 0)
		return;

	small_rect.x = h_diff / 2;
}

SDL_Rect CpuRenderer::measureTexture(SDL_Texture* texture)
{
	int h, w;
	SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
	return {0, 0, w, h};
}

void CpuRenderer::drawRectangle(const SDL_Rect& pos, const SDL_Color& color, bool fill)
{
	uint8_t r, g, b, a;
	SDL_GetRenderDrawColor(this->_renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(this->_renderer, color.r, color.g, color.b, color.a);

	if (fill)
		SDL_RenderFillRect(this->_renderer, &pos);
	else
		SDL_RenderDrawRect(this->_renderer, &pos);

	SDL_SetRenderDrawColor(this->_renderer, r, g, b, a);
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
