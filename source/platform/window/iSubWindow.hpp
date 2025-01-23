#pragma once

#include <memory>
#include <string>

#include "../platformBase.hpp"

class IRenderer;

struct SubWindowCreateInfo
{
	uint64_t width;
	uint64_t height;
	RenderFuncDelegate render_func;
};

class ISubWindow
{
public:
	virtual ~ISubWindow() = default;

	virtual bool open(RenderFuncDelegate render_func) = 0;
	virtual void close() = 0;

	virtual void render(uint64_t now, uint64_t delta_ms) = 0;

	virtual IRenderer* getRenderer() = 0;
	virtual WindowId getId() const = 0;

	virtual void setTitle(const std::string& title) = 0;
	virtual void setSize(const uint32_t height, const uint32_t width) = 0;
};
