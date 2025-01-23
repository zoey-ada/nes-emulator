#pragma once

#include "../sdlRenderer.hpp"
#include "iSubWindow.hpp"

class SdlSubWindow: public ISubWindow
{
public:
	SdlSubWindow(uint64_t width, uint64_t height);
	virtual ~SdlSubWindow() { close(); }

	bool open(RenderFuncDelegate render_func) override;
	void close() override;

	void render(uint64_t now, uint64_t delta_ms) override;

	IRenderer* getRenderer() override { return this->_renderer.get(); }
	WindowId getId() const override { return this->_id; }

	void setTitle(const std::string& title) override;
	void setSize(const uint32_t height, const uint32_t width) override;

	void handleEvent(const SDL_Event& e);

	bool isRunning() const { return this->_running; }

private:
	int _height;
	int _width;
	uint32_t _id {0};

	SDL_Window* _window {nullptr};
	std::unique_ptr<SdlRenderer> _renderer {nullptr};

	bool _running {true};
	// Milliseconds _frametime {33};
	// Milliseconds _last_draw {0};
	// Milliseconds _prev_time {0};

	RenderFuncDelegate _render_func;

	// Milliseconds getTime() const;
};
