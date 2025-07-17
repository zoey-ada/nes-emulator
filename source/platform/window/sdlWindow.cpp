#include "sdlWindow.hpp"

#include <chrono>

#include <nfd.hpp>

namespace chrono = std::chrono;

bool SdlWindow::open(const WindowCreateInfo& create_info)
{
	this->parseWindowSettings(create_info);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	this->_window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		this->_width, this->_height, SDL_WINDOW_SHOWN);

	if (this->_window == nullptr)
	{
		printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	this->_id = SDL_GetWindowID(this->_window);
	if (this->_id == 0)
	{
		printf("Window ID could not be retrieved. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	this->_renderer = std::make_shared<SdlRenderer>(this->_window);
	if (!this->_renderer)
		return false;

	return this->_renderer->initialize();
}

void SdlWindow::close()
{
	for (auto& sub_window : this->_sub_windows)
		sub_window.second->close();

	if (this->_window != nullptr)
	{
		SDL_DestroyWindow(this->_window);
		this->_window = nullptr;

		SDL_Quit();
	}

	NFD_Quit();
}

void SdlWindow::run(HandleKeyboardDelegate handle_key_event)
{
	this->_handle_key_event = handle_key_event;

	while (this->_run)
	{
		SDL_Event e;
		if (SDL_PollEvent(&e) != 0)
		{
			this->handleEvent(e);
		}
		else
		{
			auto now = this->getTime();
			auto delta_ms = now - this->_prev_time;
			auto draw_delta_ms = now - this->_last_draw;

			this->_prev_time = now;
			this->_update_func(now, delta_ms);

			if (draw_delta_ms >= this->_frametime)
			{
				this->_last_draw = now;
				this->_render_func(now, draw_delta_ms);

				for (auto& sub_window : this->_sub_windows)
				{
					sub_window.second->render(now, delta_ms);
				}
			}
		}
	}

	drainMessageQueue();
}

std::string SdlWindow::openFileDialog(std::vector<FileFilter> filters) const
{
	nfdopendialogu8args_t args = {0};
	args.filterList = (nfdu8filteritem_t*)filters.data();
	args.filterCount = filters.size();

	nfdu8char_t* out_path;
	nfdresult_t result = NFD_OpenDialogU8_With(&out_path, &args);
	if (result == nfdresult_t::NFD_OKAY)
	{
		std::string filepath(out_path);
		NFD_FreePathU8(out_path);
		return filepath;
	}
	else if (result == nfdresult_t::NFD_ERROR)
	{
		printf("Error: %s\n", NFD_GetError());
	}
	return std::string();
}

void SdlWindow::setTitle(const std::string& title)
{
	SDL_SetWindowTitle(this->_window, title.c_str());
}

void SdlWindow::setSize(const uint32_t height, const uint32_t width)
{
	this->_height = height;
	this->_width = width;
	SDL_SetWindowSize(this->_window, this->_width, this->_height);
}

ISubWindow* SdlWindow::openSubWindow(SubWindowCreateInfo sub_window_info)
{
	auto sub_window = std::make_unique<SdlSubWindow>(sub_window_info.width, sub_window_info.height);
	if (sub_window->open(sub_window_info.render_func))
	{
		WindowId sub_win_id = sub_window->getId();
		this->_sub_windows[sub_win_id] = std::move(sub_window);
		return this->_sub_windows[sub_win_id].get();
	}

	return nullptr;
}

void SdlWindow::parseWindowSettings(const WindowCreateInfo& settings)
{
	this->_height = settings.height;
	this->_width = settings.width;
	this->_frametime = static_cast<Milliseconds>(1000 / settings.frame_rate);
	this->_update_func = settings.update_func;
	this->_render_func = settings.render_func;
}

Milliseconds SdlWindow::getTime() const
{
	auto now = chrono::system_clock::now();
	auto now_ms = chrono::time_point_cast<chrono::milliseconds>(now);

	auto value = now_ms.time_since_epoch();
	return static_cast<uint64_t>(value.count());
}

void SdlWindow::handleEvent(const SDL_Event& e)
{
	if (e.type == SDL_EventType::SDL_WINDOWEVENT)
	{
		if (e.window.windowID == this->_id)
		{
			if (e.window.event == SDL_WINDOWEVENT_CLOSE)
			{
				this->_run = false;
			}
		}
		else
		{
			WindowId sub_win_id = e.window.windowID;
			auto sub_window = this->_sub_windows.find(sub_win_id);
			if (sub_window == this->_sub_windows.end())
				return;

			(*sub_window).second->handleEvent(e);
			if (this->_sub_windows[sub_win_id]->isRunning() == false)
				this->_sub_windows.erase(sub_win_id);
		}
	}
	else if (e.type == SDL_EventType::SDL_KEYDOWN || e.type == SDL_EventType::SDL_KEYUP)
	{
		this->_handle_key_event(e);
	}
	// else if (e.type == SDL_EventType::SDL_QUIT)
	// {
	// 	this->_run = false;
	// }
}

void SdlWindow::drainMessageQueue()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
		;
}
