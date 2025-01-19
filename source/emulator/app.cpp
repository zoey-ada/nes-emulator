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

	this->_nes_texture =
		this->_renderer->createTexture(this->_nes_base_width, this->_nes_base_height, true);
	this->_nes = std::make_unique<Nes>(this->_renderer);

	RenderFuncDelegate render_func = std::bind(&App::render, this, _1, _2);
	UpdateFuncDelegate update_func = std::bind(&App::update, this, _1, _2);
	HandleKeyboardDelegate handle_key_func = std::bind(&App::handleEvent, this, _1);

	this->_window->run(render_func, update_func, handle_key_func);
	this->_window->close();
}

void App::update(Milliseconds now, Milliseconds delta_ms)
{
	if (!this->_is_paused)
		this->_nes->cycle(this->_ppu_cycles_per_update);
}

void App::render(Milliseconds now, Milliseconds delta_ms)
{
	this->_renderer->preRender();
	this->renderFrame(this->_nes->getFrame());
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
			this->_nes->loadFile(path);
		}
		else if (key == SDL_Scancode::SDL_SCANCODE_RIGHT)
		{
			this->step();
		}
		else if (key == SDL_Scancode::SDL_SCANCODE_DOWN)
		{
			if ((mod & KMOD_CTRL) > 0)
				this->bound();
			else
				this->leap();
		}
		else if (key == SDL_Scancode::SDL_SCANCODE_SPACE)
		{
			if (this->_nes->isGameLoaded())
				this->_is_paused = !this->_is_paused;
		}
		else if (key == SDL_Scancode::SDL_SCANCODE_P)
		{
			this->_nes->nextPalette();
		}
		else if (key == SDL_Scancode::SDL_SCANCODE_O)
		{
			this->_nes->prevPalette();
		}
		else if (key == SDL_Scancode::SDL_SCANCODE_PERIOD)
		{
			this->_nes->nextFrame();
		}
		else if (key == SDL_Scancode::SDL_SCANCODE_D)
		{
			this->_nes->dumpMemory();
		}
	}
}

void App::renderFrame(NesFrame frame)
{
	// main nes
	Rect dest_rect {
		0,
		0,
		this->_nes_width,
		this->_nes_height,
	};
	this->_renderer->updateTexture(this->_nes_texture, frame.data(), sizeof(frame));
	this->_renderer->drawTexture(this->_nes_texture, dest_rect);

	// left pattern table
	dest_rect = {
		this->_nes_width + 1,
		this->_height - this->_pattern_table_height,
		this->_pattern_table_width,
		this->_pattern_table_height,
	};
	this->_renderer->drawTexture((Texture)this->_nes->getLeftPtTexture(), dest_rect);

	// right pattern table
	dest_rect = {
		this->_nes_width + 2 + this->_pattern_table_width,
		this->_height - this->_pattern_table_height,
		this->_pattern_table_width,
		this->_pattern_table_height,
	};
	this->_renderer->drawTexture((Texture)this->_nes->getRightPtTexture(), dest_rect);

	// cpu debug
	auto size = this->_renderer->measureTexture((Texture)this->_nes->getCpuDebugTexture());
	dest_rect = {
		this->_nes_width + 1,
		0,
		size.width,
		size.height,
	};
	this->_renderer->drawTexture((Texture)this->_nes->getCpuDebugTexture(), dest_rect);
}

void App::step()
{
	if (this->_nes->isGameLoaded() && this->_is_paused)
		this->_nes->step();
}

void App::leap()
{
	if (this->_nes->isGameLoaded() && this->_is_paused)
		this->_nes->leap();
}

void App::bound()
{
	if (this->_nes->isGameLoaded() && this->_is_paused)
		this->_nes->bound();
}
