#pragma once

#include <map>

#include <SDL.h>

#include "../sdlRenderer.hpp"
#include "iWindow.hpp"
#include "sdlSubWindow.hpp"

class SdlWindow: public IWindow
{
public:
	SdlWindow() = default;
	virtual ~SdlWindow() { close(); }

	bool open(const WindowCreateInfo& create_info) override;
	void close() override;

	void run(HandleKeyboardDelegate handle_key_event) override;

	std::string openFileDialog(std::vector<FileFilter> filters) const override;

	std::shared_ptr<IRenderer> getRenderer() override { return this->_renderer; }
	uint32_t getId() const override { return this->_id; }

	void setTitle(const std::string& title) override;
	void setSize(const uint32_t height, const uint32_t width) override;

	ISubWindow* openSubWindow(SubWindowCreateInfo sub_window_info) override;

private:
	int _height {0};
	int _width {0};
	uint32_t _id {0};

	SDL_Window* _window {nullptr};
	std::shared_ptr<SdlRenderer> _renderer {nullptr};

	UpdateFuncDelegate _update_func;
	RenderFuncDelegate _render_func;

	std::map<WindowId, std::unique_ptr<SdlSubWindow>> _sub_windows;

	bool _run {true};
	Milliseconds _frametime {33};
	Milliseconds _last_draw {0};
	Milliseconds _prev_time {0};

	HandleKeyboardDelegate _handle_key_event;

	void parseWindowSettings(const WindowCreateInfo& create_info);
	Milliseconds getTime() const;

	void handleEvent(const SDL_Event& e);
	void drainMessageQueue();
};
