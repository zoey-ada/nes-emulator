#include "window.hpp"

#include <cstdio>

#include <SDL.h>
#include <SDL_surface.h>

bool Window::open()
{
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
			if (e.type == SDL_EventType::SDL_QUIT)
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
	// auto screen_format = SDL_GetPixelFormatName(_surface->format->format);
	// auto buffer_format = SDL_GetPixelFormatName(_left_table_frame_buffer->format->format);
	SDL_BlitScaled(this->_left_table_frame_buffer, &source_rect, this->_surface, &dest_rect);

	dest_rect.x = this->_pattern_table_width * this->_scale_factor;
	SDL_BlitScaled(this->_right_table_frame_buffer, &source_rect, this->_surface, &dest_rect);
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
