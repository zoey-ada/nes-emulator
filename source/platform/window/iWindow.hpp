#pragma once

#include <memory>
#include <string>
#include <vector>

#include <SDL.h>

#include "../platformBase.hpp"
#include "iSubWindow.hpp"

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
	virtual WindowId getId() const = 0;

	virtual void setTitle(const std::string& title) = 0;
	virtual void setSize(const uint32_t height, const uint32_t width) = 0;

	virtual ISubWindow* openSubWindow(SubWindowCreateInfo sub_window_info) = 0;
};
