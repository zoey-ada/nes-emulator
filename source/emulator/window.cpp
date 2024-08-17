#include "window.hpp"

#include <chrono>
#include <cstdio>

#include <SDL.h>
#include <nfd.h>

namespace chrono = std::chrono;

bool Window::open()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	this->_window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, this->_width, this->_height, SDL_WINDOW_SHOWN);

	if (this->_window == nullptr)
	{
		printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED);
	if (this->_renderer == nullptr)
	{
		printf("Renderer could not be created. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	this->_nes_texture = SDL_CreateTexture(this->_renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING, this->_nes_base_width, this->_nes_base_height);
	if (this->_nes_texture == nullptr)
	{
		printf("NES texture could not be created. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	this->_left_pattern_table_texture =
		SDL_CreateTexture(this->_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
			this->_pattern_table_base_width, this->_pattern_table_base_height);
	if (this->_left_pattern_table_texture == nullptr)
	{
		printf("left pattern table texture could not be created. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	this->_right_pattern_table_texture =
		SDL_CreateTexture(this->_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
			this->_pattern_table_base_width, this->_pattern_table_base_height);
	if (this->_right_pattern_table_texture == nullptr)
	{
		printf("right pattern table texture could not be created. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	this->_nes = std::make_unique<Nes>(this->_renderer);

	return true;
}

void Window::close()
{
	if (this->_window != nullptr)
	{
		SDL_DestroyWindow(this->_window);
		this->_window = nullptr;

		SDL_Quit();
	}
}

void Window::run()
{
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
			if (delta_ms == 0)
				continue;

			this->_prev_time = now;
			this->update(now, delta_ms);
			this->render(now, delta_ms);
		}
	}
}

void Window::renderFrame(NesFrame frame)
{
	// main nes
	void* pixels = nullptr;
	int pitch = 0;
	SDL_LockTexture(this->_nes_texture, nullptr, &pixels, &pitch);
	memcpy(pixels, frame.data(), sizeof(frame));
	pixels = nullptr;
	SDL_UnlockTexture(this->_nes_texture);

	SDL_Rect source_rect {0, 0, this->_nes_base_width, this->_nes_base_height};
	SDL_Rect dest_rect {0, 0, this->_nes_width, this->_nes_height};
	SDL_RenderCopy(this->_renderer, this->_nes_texture, &source_rect, &dest_rect);

	// left pattern table
	SDL_LockTexture(this->_left_pattern_table_texture, nullptr, &pixels, &pitch);
	auto left_pt_frame = this->_nes->getLeftPatternTableFrame();
	memcpy(pixels, left_pt_frame.data(), sizeof(left_pt_frame));
	pixels = nullptr;
	SDL_UnlockTexture(this->_left_pattern_table_texture);

	source_rect = {0, 0, this->_pattern_table_base_width, this->_pattern_table_base_height};
	dest_rect = {this->_nes_width + 1, this->_height - this->_pattern_table_height,
		this->_pattern_table_width, this->_pattern_table_height};
	SDL_RenderCopy(this->_renderer, this->_left_pattern_table_texture, &source_rect, &dest_rect);

	// right pattern table
	SDL_LockTexture(this->_right_pattern_table_texture, nullptr, &pixels, &pitch);
	auto right_pt_frame = this->_nes->getRightPatternTableFrame();
	memcpy(pixels, right_pt_frame.data(), sizeof(right_pt_frame));
	pixels = nullptr;
	SDL_UnlockTexture(this->_right_pattern_table_texture);

	source_rect = {0, 0, this->_pattern_table_base_width, this->_pattern_table_base_height};
	dest_rect = {this->_nes_width + 2 + this->_pattern_table_width,
		this->_height - this->_pattern_table_height, this->_pattern_table_width,
		this->_pattern_table_height};
	SDL_RenderCopy(this->_renderer, this->_right_pattern_table_texture, &source_rect, &dest_rect);

	// cpu debug
	auto cpu_texture = this->_nes->getCpuDebugTexture();
	int h, w;
	SDL_QueryTexture(cpu_texture, nullptr, nullptr, &w, &h);
	source_rect = {0, 0, w, h};
	dest_rect = {this->_nes_width + 1, 0, w, h};
	SDL_RenderCopy(this->_renderer, cpu_texture, &source_rect, &dest_rect);
	cpu_texture = nullptr;
}

void Window::openFileDialog()
{
	nfdu8filteritem_t filters[1] = {
		{"NES ROM", "nes"},
	};
	nfdopendialogu8args_t args = {0};
	args.filterList = filters;
	args.filterCount = 1;

	nfdu8char_t* out_path;
	nfdresult_t result = NFD_OpenDialogU8_With(&out_path, &args);
	if (result == nfdresult_t::NFD_OKAY)
	{
		std::string filepath(out_path);
		NFD_FreePathU8(out_path);
		this->_nes->loadFile(filepath);
	}
	else if (result == nfdresult_t::NFD_ERROR)
	{
		printf("Error: %s\n", NFD_GetError());
	}
}

void Window::step()
{
	if (this->_nes->isGameRunning() && this->_debugging)
		this->_nes->step();
}

void Window::leap()
{
	if (this->_nes->isGameRunning() && this->_debugging)
		this->_nes->leap();
}

void Window::bound()
{
	if (this->_nes->isGameRunning() && this->_debugging)
		this->_nes->bound();
}

uint64_t Window::getTime() const
{
	auto now = chrono::system_clock::now();
	auto now_ms = chrono::time_point_cast<chrono::milliseconds>(now);

	auto value = now_ms.time_since_epoch();
	return static_cast<uint64_t>(value.count());
}

void Window::update(Milliseconds now, Milliseconds delta_ms)
{
	if (!this->_debugging)
		this->_nes->cycle(this->_ppu_cycles_per_update);
}

void Window::render(Milliseconds now, Milliseconds delta_ms)
{
	if (now - this->_last_draw < this->_frametime)
		return;

	this->prepareScene();

	this->_nes->produceFrame();
	this->renderFrame(this->_nes->getFrame());

	this->presentScene();

	this->_last_draw = now;
}

void Window::prepareScene()
{
	SDL_SetRenderDrawColor(this->_renderer, 255, 0, 255, 255);
	SDL_RenderClear(this->_renderer);
}

void Window::presentScene()
{
	SDL_RenderPresent(this->_renderer);
}

void Window::handleEvent(const SDL_Event& e)
{
	if (e.type == SDL_EventType::SDL_KEYDOWN)
	{
		auto key = e.key.keysym.scancode;
		auto mod = e.key.keysym.mod;
		if (key == SDL_Scancode::SDL_SCANCODE_O && (mod & KMOD_CTRL) > 0)
		{
			this->openFileDialog();
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
			this->_debugging = !this->_debugging;
		}
	}
	if (e.type == SDL_EventType::SDL_QUIT)
	{
		this->_run = false;
	}
}
