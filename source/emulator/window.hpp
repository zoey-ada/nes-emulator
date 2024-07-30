#pragma once

class SDL_Window;
class SDL_Surface;
class SDL_Renderer;

#include "nes.hpp"

class Window
{
public:
	Window() = default;
	virtual ~Window() { close(); }

	bool open();
	void close();

	void run();
	void renderFrame(Frame frame);
	void prepareScene();
	void presentScene();

private:
	int _height {262};
	int _width {341};
	SDL_Window* _window {nullptr};
	SDL_Renderer* _renderer {nullptr};
	SDL_Surface* _surface {nullptr};
	SDL_Surface* _frame_buffer {nullptr};

	Nes _nes;
};
