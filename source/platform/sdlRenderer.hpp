#pragma once

#include "iRenderer.hpp"

class SdlRenderer: public IRenderer
{
public:
	SdlRenderer(SDL_Window* window);
	virtual ~SdlRenderer();

	bool initialize() override;
	void deinitialize() override;

	void preRender() override;
	void postRender() override;

	std::unique_ptr<IAlternateRenderTarget> swapRenderTarget(Texture texture) override;

	void setBackgroundColor(const SDL_Color& color) override { this->_background_color = color; }

	void drawTexture(Texture texture, const Rect& destination) override;
	Texture createTexture(const uint64_t width, const uint64_t height,
		bool updatable = true) override;
	void destroyTexture(Texture texture) override;
	void updateTexture(Texture texture, const Pixel* data, const uint64_t data_size) override;
	Rect measureTexture(Texture texture) override;

	Font createFont(const std::string& font_filepath, const unsigned int font_size) override;
	void destroyFont(Font font) override;

	Texture renderText(const std::string& text, TextRenderOptions options) override;

	void drawRectangle(const Rect& destination, const SDL_Color& color, bool fill) override;

	void horizontallyCenterRectangle(const Rect& total_area, Rect& rectangle) override;

private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;

	SDL_Color _background_color {0xff, 0x00, 0xff, 0xff};

	SDL_Rect toSdlRect(const Rect& rect);
	Rect fromSdlRect(const SDL_Rect& rect);
	SDL_Rect _measureTexture(Texture texture) const;
};
