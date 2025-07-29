#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <base/color.hpp>
#include <base/pixel.hpp>

#include "iAlternateRenderTarget.hpp"

// class _Color;
class _Font;
class _Texture;

// typedef _Color* Color;
typedef _Font* Font;
typedef _Texture* Texture;

struct Rect
{
	uint64_t x;
	uint64_t y;
	uint64_t width;
	uint64_t height;
};

struct TextRenderOptions
{
	Font font;
	Color text_color;
	Color background_color;
};

class IRenderer
{
public:
	virtual ~IRenderer() = default;

	virtual bool initialize() = 0;
	virtual void deinitialize() = 0;

	virtual void preRender() = 0;
	virtual void postRender() = 0;

	virtual std::unique_ptr<IAlternateRenderTarget> swapRenderTarget(Texture texture) = 0;

	virtual void setBackgroundColor(const Color& color) = 0;

	virtual void drawTexture(Texture texture, const Rect& destination) = 0;
	virtual Texture createTexture(const uint64_t width, const uint64_t height,
		bool updatable = true) = 0;
	virtual void destroyTexture(Texture texture) = 0;
	virtual void updateTexture(Texture texture, const Pixel* data, const uint64_t data_size) = 0;
	virtual Rect measureTexture(Texture texture) = 0;

	virtual Font createFont(const std::string& font_filepath, const unsigned int font_size) = 0;
	virtual void destroyFont(Font font) = 0;

	virtual Texture renderText(const std::string& text, TextRenderOptions options) = 0;

	virtual void drawRectangle(const Rect& destination, const Color& color, bool fill) = 0;

	virtual void horizontallyCenterRectangle(const Rect& total_area, Rect& rectangle) = 0;
};
