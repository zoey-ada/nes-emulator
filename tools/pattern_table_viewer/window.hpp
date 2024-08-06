#pragma once

class SDL_Window;
class SDL_Surface;
class SDL_Renderer;

#include "patternTable.hpp"

class Window
{
public:
	Window() = default;
	virtual ~Window() { close(); }

	bool open();
	void close();

	void run();
	void renderFrame(Frame left_frame, Frame right_frame);
	void prepareScene();
	void presentScene();

	void openFileDialog();

private:
	int _height {128};
	int _width {256};
	int _scale_factor {4};

	SDL_Window* _window {nullptr};
	SDL_Renderer* _renderer {nullptr};
	SDL_Surface* _surface {nullptr};
	SDL_Surface* _left_table_frame_buffer {nullptr};
	SDL_Surface* _right_table_frame_buffer {nullptr};

	int _pattern_table_height {128};
	int _pattern_table_width {128};
	PatternTable _left = PatternTable(false);
	PatternTable _right = PatternTable(true);
};
