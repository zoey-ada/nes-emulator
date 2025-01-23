#include "sdlSubWindow.hpp"

// #include <chrono>

SdlSubWindow::SdlSubWindow(uint64_t width, uint64_t height): _width(width), _height(height)
{}

bool SdlSubWindow::open(RenderFuncDelegate render_func)
{
	this->_render_func = render_func;

	this->_window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		this->_width, this->_height, SDL_WINDOW_SHOWN);

	if (this->_window == nullptr)
	{
		printf("Sub window could not be created. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	this->_id = SDL_GetWindowID(this->_window);
	if (this->_id == 0)
	{
		printf("Sub window ID could not be retrieved. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	this->_renderer = std::make_unique<SdlRenderer>(this->_window);
	if (!this->_renderer)
		return false;

	return this->_renderer->initialize();
}

void SdlSubWindow::close()
{
	this->_renderer.release();

	if (this->_window != nullptr)
	{
		SDL_DestroyWindow(this->_window);
		this->_window = nullptr;
	}
}

void SdlSubWindow::render(uint64_t now, uint64_t delta_ms)
{
	// auto now = this->getTime();
	// auto delta_ms = now - this->_prev_time;
	// if (delta_ms == 0)
	// 	return;

	// this->_prev_time = now;
	this->_render_func(now, delta_ms);
}

void SdlSubWindow::setTitle(const std::string& title)
{
	SDL_SetWindowTitle(this->_window, title.c_str());
}

void SdlSubWindow::setSize(const uint32_t height, const uint32_t width)
{
	this->_height = height;
	this->_width = width;
	SDL_SetWindowSize(this->_window, this->_width, this->_height);
}

void SdlSubWindow::handleEvent(const SDL_Event& e)
{
	if (e.type == SDL_EventType::SDL_WINDOWEVENT && e.window.windowID == this->_id)
	{
		if (e.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			this->close();
			this->_running = false;
		}
	}
}

// Milliseconds SdlSubWindow::getTime() const
// {
// 	auto now = chrono::system_clock::now();
// 	auto now_ms = chrono::time_point_cast<chrono::milliseconds>(now);

// 	auto value = now_ms.time_since_epoch();
// 	return static_cast<uint64_t>(value.count());
// }
