#pragma once

class SDL_Window;
class SDL_Surface;

class Window
{
public:
	Window() = default;
	virtual ~Window() { close(); }

	bool open();
	void close();

	void run();

private:
	int _height {480};
	int _width {640};
	SDL_Window* _window {nullptr};
	SDL_Surface* _surface {nullptr};
};
