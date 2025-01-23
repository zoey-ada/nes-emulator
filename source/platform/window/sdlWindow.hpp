#pragma once

#include <map>

#include <SDL.h>

#include "../sdlRenderer.hpp"
#include "iWindow.hpp"
#include "sdlSubWindow.hpp"

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
	uint32_t getId() const override { return this->_id; }

	void setTitle(const std::string& title) override;
	void setSize(const uint32_t height, const uint32_t width) override;

	ISubWindow* openSubWindow(SubWindowCreateInfo sub_window_info) override;

private:
	int _height;
	int _width;
	uint32_t _id {0};

	SDL_Window* _window {nullptr};
	std::shared_ptr<SdlRenderer> _renderer {nullptr};

	std::map<WindowId, std::unique_ptr<SdlSubWindow>> _sub_windows;

	bool _run {true};
	Milliseconds _frametime {33};
	Milliseconds _last_draw {0};
	Milliseconds _prev_time {0};

	HandleKeyboardDelegate _handle_key_event;

	Milliseconds getTime() const;

	void handleEvent(const SDL_Event& e);
};
