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

	void drawTexture(Texture texture, const Rect& destination) override;
	Texture createTexture(const uint64_t width, const uint64_t height) override;
	void destroyTexture(Texture texture) override;
	Rect measureTexture(Texture texture) override;

	Font createFont(const std::string& font_filepath, const unsigned int font_size) override;
	void destroyFont(Font font) override;

	Texture renderText(const std::string& text, TextRenderOptions options) override;

	void drawRectangle(const Rect& destination, const SDL_Color& color, bool fill) override;

	void horizontallyCenterRectangle(const Rect& total_area, Rect& rectangle) override;

private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;
};
