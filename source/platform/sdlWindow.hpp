#pragma once

#include <memory>

#include <SDL.h>

#include "iWindow.hpp"
#include "sdlRenderer.hpp"

class SdlWindow: public IWindow
{
public:
	SdlWindow(uint64_t width, uint64_t height);
	virtual ~SdlWindow() { close(); }

	bool open() override;
	void close() override;

	void run(RenderFuncDelegate render, UpdateFuncDelegate update,
		HandleKeyboardDelegate handle_key_event) override;

	std::string openFileDialog(std::vector<FileFilter> filters) const override;

	std::shared_ptr<IRenderer> getRenderer() override { return this->_renderer; }

private:
	int _height;
	int _width;

	SDL_Window* _window {nullptr};
	std::shared_ptr<SdlRenderer> _renderer {nullptr};

	bool _run {true};
	Milliseconds _frametime {33};
	Milliseconds _last_draw {0};
	Milliseconds _prev_time {0};

	HandleKeyboardDelegate _handle_key_event;

	Milliseconds getTime() const;

	void handleEvent(const SDL_Event& e);
};
