#pragma once

#include <cstdint>
#include <memory>

#include <SDL.h>
#include <platform/iWindow.hpp>

#include "codeViewer.hpp"

class App
{
public:
	App() = default;
	virtual ~App();
	void run();

private:
	std::unique_ptr<IWindow> _window {nullptr};
	std::shared_ptr<IRenderer> _renderer {nullptr};
	std::unique_ptr<CodeViewer> _viewer {nullptr};

	int _height {600};
	int _width {400};

	void update(Milliseconds now, Milliseconds delta_ms);
	void render(Milliseconds now, Milliseconds delta_ms);
	void handleEvent(const SDL_Event& e);
};
