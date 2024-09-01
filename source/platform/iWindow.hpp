#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include <SDL.h>

using Milliseconds = uint64_t;
using RenderFuncDelegate = std::function<void(Milliseconds now, Milliseconds delta_ms)>;
using UpdateFuncDelegate = std::function<void(Milliseconds now, Milliseconds delta_ms)>;
// TODO: abstract this
using HandleKeyboardDelegate = std::function<void(const SDL_Event& e)>;

class IRenderer;

struct FileFilter
{
	const char* description;
	const char* extension;
};

class IWindow
{
public:
	virtual ~IWindow() = default;

	virtual bool open() = 0;
	virtual void close() = 0;

	virtual void run(RenderFuncDelegate render, UpdateFuncDelegate update,
		HandleKeyboardDelegate handle_key_event) = 0;

	virtual std::string openFileDialog(std::vector<FileFilter> filters) const = 0;

	virtual std::shared_ptr<IRenderer> getRenderer() = 0;
};
