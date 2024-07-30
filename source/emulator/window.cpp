#include "window.hpp"

#include <cstdio>

#include <SDL.h>

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

	this->_surface = SDL_GetWindowSurface(this->_window);
	if (this->_surface == nullptr)
	{
		printf("Surface could not be gotten. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	this->_frame_buffer = SDL_CreateRGBSurface(0, 341, 262, 32, 0, 0, 0, 0);
	if (this->_frame_buffer == nullptr)
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

		this->_nes.produceFrame();
		this->renderFrame(this->_nes.getFrame());

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

void Window::renderFrame(Frame frame)
{
	SDL_LockSurface(this->_surface);

	memcpy(this->_frame_buffer->pixels, frame.data(), sizeof(frame));
	// for (int i = 0; i < total_pixel_parts; ++i)
	// 	((uint8_t*)this->_surface->pixels)[i] = frame[i];

	SDL_UnlockSurface(this->_surface);

	const SDL_Rect source_rect {0, 0, 341, 262};
	SDL_Rect dest_rect {0, 0, 341, 262};
	auto screen_format = SDL_GetPixelFormatName(_surface->format->format);
	auto buffer_format = SDL_GetPixelFormatName(_frame_buffer->format->format);
	SDL_BlitSurface(this->_frame_buffer, &source_rect, this->_surface, &dest_rect);
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
