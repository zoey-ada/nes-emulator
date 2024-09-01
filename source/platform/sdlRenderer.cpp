#include "sdlRenderer.hpp"

#include <SDL_ttf.h>

#include "SdlAlternateRenderTarget.hpp"

SdlRenderer::SdlRenderer(SDL_Window* window): _window(window)
{}

SdlRenderer::~SdlRenderer()
{
	this->deinitialize();
}

bool SdlRenderer::initialize()
{
	this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED);
	if (this->_renderer == nullptr)
	{
		printf("Renderer could not be created. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	if (TTF_Init())
	{
		printf("Couldn't initialize SDL TTF. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

void SdlRenderer::deinitialize()
{
	TTF_Quit();

	if (this->_renderer)
		SDL_DestroyRenderer(this->_renderer);
	this->_renderer = nullptr;
}

void SdlRenderer::preRender()
{
	SDL_SetRenderDrawColor(this->_renderer, 255, 0, 255, 255);
	SDL_RenderClear(this->_renderer);
}

void SdlRenderer::postRender()
{
	SDL_RenderPresent(this->_renderer);
}

std::unique_ptr<IAlternateRenderTarget> SdlRenderer::swapRenderTarget(Texture texture)
{
	return std::make_unique<SdlAlternateRenderTarget>(this->_renderer, (SDL_Texture*)texture);
}

void SdlRenderer::drawTexture(Texture texture, const Rect& destination)
{
	auto dimensions = this->measureTexture(texture);
	SDL_RenderCopy(this->_renderer, (SDL_Texture*)texture, (SDL_Rect*)&dimensions,
		(SDL_Rect*)&destination);
}

Texture SdlRenderer::createTexture(const uint64_t width, const uint64_t height, bool updatable)
{
	if (updatable)
		return (Texture)SDL_CreateTexture(this->_renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING, width, height);
	else
		return (Texture)SDL_CreateTexture(this->_renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_TARGET, width, height);
}

void SdlRenderer::destroyTexture(Texture texture)
{
	if (texture)
		SDL_DestroyTexture((SDL_Texture*)texture);
}

void SdlRenderer::updateTexture(Texture texture, const Pixel* data, const uint64_t data_size)
{
	void* pixels = nullptr;
	int pitch = 0;
	SDL_LockTexture((SDL_Texture*)texture, nullptr, &pixels, &pitch);
	memcpy(pixels, data, data_size);
	pixels = nullptr;
	SDL_UnlockTexture((SDL_Texture*)texture);
}

Rect SdlRenderer::measureTexture(Texture texture)
{
	int h, w;
	SDL_QueryTexture((SDL_Texture*)texture, nullptr, nullptr, &w, &h);
	return {0, 0, w, h};
}

Font SdlRenderer::createFont(const std::string& font_filepath, const unsigned int font_size)
{
	auto font = TTF_OpenFont(font_filepath.c_str(), font_size);
	if (font == nullptr)
	{
		printf("Couldn't load font. SDL_Error: %s\n", SDL_GetError());
		return nullptr;
	}
	TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
	// TTF_SetFontOutline(font, outline);
	// TTF_SetFontKerning(font, 1);
	// TTF_SetFontHinting(font, TTF_HINTING_NONE);
	return (Font)font;
}

void SdlRenderer::destroyFont(Font font)
{
	TTF_CloseFont((TTF_Font*)font);
}

Texture SdlRenderer::renderText(const std::string& text, TextRenderOptions options)
{
	auto font = (TTF_Font*)options.font;

	auto temp_surface =
		TTF_RenderText_Shaded(font, text.c_str(), options.text_color, options.background_color);

	auto texture = SDL_CreateTextureFromSurface(this->_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);
	return (Texture)texture;
}

void SdlRenderer::drawRectangle(const Rect& destination, const SDL_Color& color, bool fill)
{
	uint8_t r, g, b, a;
	SDL_GetRenderDrawColor(this->_renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(this->_renderer, color.r, color.g, color.b, color.a);

	if (fill)
		SDL_RenderFillRect(this->_renderer, (SDL_Rect*)&destination);
	else
		SDL_RenderDrawRect(this->_renderer, (SDL_Rect*)&destination);

	SDL_SetRenderDrawColor(this->_renderer, r, g, b, a);
}

void SdlRenderer::horizontallyCenterRectangle(const Rect& total_area, Rect& rectangle)
{
	if (total_area.width == rectangle.width)
	{
		rectangle.x = 0;
		return;
	}

	int h_diff = total_area.width - rectangle.width;
	if (h_diff < 0)
		return;

	rectangle.x = h_diff / 2;
}
