#pragma once

#include <array>
#include <cstdint>
#include <string>

#include <SDL_ttf.h>
#include <cpu/debugCpu.hpp>

// const uint16_t cpu_width_in_pixels = 257;
// const uint16_t cpu_height_in_pixels = 50;
// const uint64_t cpu_total_pixels = cpu_width_in_pixels * cpu_height_in_pixels;

// using CpuFrame = std::array<Pixel, cpu_total_pixels>;

struct DynamicTextures
{
	SDL_Texture* data_texture_1 {nullptr};
	SDL_Texture* data_texture_2 {nullptr};
	SDL_Texture* instruction_texture {nullptr};
};

struct StaticTextures
{
	SDL_Texture* label_texture_1 {nullptr};
	SDL_Texture* label_texture_2 {nullptr};
};

struct TextRenderOptions
{
	TTF_Font* font {nullptr};
	SDL_Color text_color {0xff, 0xff, 0xff, 0xff};
	SDL_Color background_color {0x00, 0x00, 0x00, 0xff};
};

struct StatusFlags
{
	bool n;
	bool v;
	bool b;
	bool d;
	bool i;
	bool z;
	bool c;
};

class CpuRenderer
{
public:
	CpuRenderer(SDL_Renderer* renderer);
	virtual ~CpuRenderer();

	void produceFrame(const CpuStackFrame& cycle_data);
	SDL_Texture* getTexture() { return this->_texture; }
	// CpuFrame getFrame() { return this->_frame; }

private:
	// CpuFrame _frame;
	SDL_Renderer* _renderer {nullptr};
	SDL_Color _background_color {0x00, 0x00, 0x00, 0xff};
	SDL_Color _text_color {0xff, 0xff, 0xff, 0xff};
	std::string _font_name = "c:/repos/nes-emulator/source/emulator/FiraCode-Regular.ttf";
	TTF_Font* _font {nullptr};
	int _font_size {24};

	int _width {513};
	int _row_height {30};
	int _vertical_padding {20};
	int _height {(this->_row_height * 5) + (this->_vertical_padding * 2)};
	SDL_Texture* _texture {nullptr};

	void createMainTexture();
	SDL_Texture* renderText(const std::string& text, const TextRenderOptions& options);

	void renderStaticText();
	StaticTextures generateStaticText();
	void destroyTextures(StaticTextures& textures);

	void renderDynamicText(const CpuStackFrame& cycle_data);
	DynamicTextures generateDynamicText(const CpuStackFrame& cycle_data);
	void destroyTextures(DynamicTextures& textures);

	void horizontallyCenterRects(const SDL_Rect& big_rect, SDL_Rect& small_rect);
	SDL_Rect measureTexture(SDL_Texture* texture);
	void drawRectangle(const SDL_Rect& pos, const SDL_Color& color, bool fill = true);
	StatusFlags parseStatusFlags(uint8_t state);
};
