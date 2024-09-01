#include "app.hpp"

#include <functional>
#include <vector>

#include <platform/iRenderer.hpp>
#include <platform/sdlWindow.hpp>

using namespace std::placeholders;

App::~App()
{
	this->_renderer = nullptr;
}

void App::run()
{
	this->_window = std::make_unique<SdlWindow>(this->_width, this->_height);

	this->_window->open();
	this->_renderer = this->_window->getRenderer();
	this->_viewer = std::make_unique<CodeViewer>(this->_renderer);

	RenderFuncDelegate render_func = std::bind(&App::render, this, _1, _2);
	UpdateFuncDelegate update_func = std::bind(&App::update, this, _1, _2);
	HandleKeyboardDelegate handle_key_func = std::bind(&App::handleEvent, this, _1);

	this->_window->run(render_func, update_func, handle_key_func);
	this->_window->close();
}

void App::update(Milliseconds now, Milliseconds delta_ms)
{}

void App::render(Milliseconds now, Milliseconds delta_ms)
{
	this->_renderer->preRender();
	Rect dest {0, 0, _width, _height};
	this->_renderer->drawTexture(this->_viewer->getTexture(), dest);
	this->_renderer->postRender();
}

void App::handleEvent(const SDL_Event& e)
{
	if (e.type == SDL_EventType::SDL_KEYDOWN)
	{
		auto key = e.key.keysym.scancode;
		auto mod = e.key.keysym.mod;
		if (key == SDL_Scancode::SDL_SCANCODE_O && (mod & KMOD_CTRL) > 0)
		{
			std::vector<FileFilter> filters;
			filters.push_back({"NES ROM", "nes"});
			auto path = this->_window->openFileDialog(filters);
			this->_viewer->loadFile(path);
		}
		else if (key == SDL_Scancode::SDL_SCANCODE_UP)
		{
			this->_viewer->scrollUp();
		}
		else if (key == SDL_Scancode::SDL_SCANCODE_DOWN)
		{
			this->_viewer->scrollDown();
		}
	}
}
