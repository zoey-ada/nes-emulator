#pragma once

#include <cstdint>
#include <memory>

#include <SDL.h>
#include <platform/iWindow.hpp>

#include "patternTable.hpp"

class App
{
public:
	App() = default;
	virtual ~App();
	void run();

private:
	std::unique_ptr<IWindow> _window {nullptr};
	std::shared_ptr<IRenderer> _renderer {nullptr};

	std::unique_ptr<PatternTable> _left {nullptr};
	std::unique_ptr<PatternTable> _right {nullptr};

	int _height {512};
	int _width {1028};

	void update(Milliseconds now, Milliseconds delta_ms);
	void render(Milliseconds now, Milliseconds delta_ms);
	void handleEvent(const SDL_Event& e);
};
