#include "window.hpp"

#include <cstdio>

#include <SDL.h>
#include <SDL_surface.h>
#include <nfd.h>

bool Window::open()
{
	if (NFD_Init() != nfdresult_t::NFD_OKAY)
	{
		printf("NFD could not initialize. NFD Error: %s\n", NFD_GetError());
		return false;
	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	this->_window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, this->_width * this->_scale_factor,
		this->_height * this->_scale_factor, SDL_WINDOW_SHOWN);

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

	this->_surface = SDL_GetWindowSurface(this->_window);
	if (this->_surface == nullptr)
	{
		printf("Surface could not be gotten. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	this->_left_table_frame_buffer = SDL_CreateRGBSurface(0, this->_pattern_table_width,
		this->_pattern_table_height, 32, 0, 0, 0, 0);
	if (this->_left_table_frame_buffer == nullptr)
	{
		printf("Frame buffer could not be gotten. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	this->_right_table_frame_buffer = SDL_CreateRGBSurface(0, this->_pattern_table_width,
		this->_pattern_table_height, 32, 0, 0, 0, 0);
	if (this->_right_table_frame_buffer == nullptr)
	{
		printf("Frame buffer could not be gotten. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

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

	NFD_Quit();
}

void Window::run()
{
	bool quit = false;

	SDL_Event e;

	while (!quit)
	{
		this->prepareScene();

		this->_left.produceFrame();
		this->_right.produceFrame();
		this->renderFrame(this->_left.getFrame(), this->_right.getFrame());

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_EventType::SDL_KEYDOWN)
			{
				auto key = e.key.keysym.scancode;
				auto mod = e.key.keysym.mod;
				if (key == SDL_Scancode::SDL_SCANCODE_O && (mod & KMOD_CTRL) > 0)
				{
					this->openFileDialog();
				}
			}
			else if (e.type == SDL_EventType::SDL_QUIT)
			{
				quit = true;
			}
		}

		presentScene();
	}
}

void Window::renderFrame(Frame left_frame, Frame right_frame)
{
	SDL_LockSurface(this->_left_table_frame_buffer);
	memcpy(this->_left_table_frame_buffer->pixels, left_frame.data(), sizeof(left_frame));
	SDL_UnlockSurface(this->_left_table_frame_buffer);

	SDL_LockSurface(this->_right_table_frame_buffer);
	memcpy(this->_right_table_frame_buffer->pixels, right_frame.data(), sizeof(right_frame));
	SDL_UnlockSurface(this->_right_table_frame_buffer);

	const SDL_Rect source_rect {0, 0, this->_pattern_table_width, this->_pattern_table_height};
	SDL_Rect dest_rect {0, 0, this->_pattern_table_width * this->_scale_factor,
		this->_pattern_table_height * this->_scale_factor};
	SDL_BlitScaled(this->_left_table_frame_buffer, &source_rect, this->_surface, &dest_rect);

	dest_rect.x = this->_pattern_table_width * this->_scale_factor;
	SDL_BlitScaled(this->_right_table_frame_buffer, &source_rect, this->_surface, &dest_rect);
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
		this->_left.loadFile(filepath);
		this->_right.loadFile(filepath);
	}
	else if (result == nfdresult_t::NFD_ERROR)
	{
		printf("Error: %s\n", NFD_GetError());
	}
}

void Window::prepareScene()
{
	SDL_SetRenderDrawColor(this->_renderer, 255, 0, 255, 255);
	SDL_RenderClear(this->_renderer);
}

void Window::presentScene()
{
	SDL_UpdateWindowSurface(this->_window);
	// SDL_RenderPresent(this->_renderer);
}
