#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string>

#include <platform/iRenderer.hpp>

#include "cpuStackFrame.hpp"

struct DynamicTextures
{
	Texture data_texture_1 {nullptr};
	Texture data_texture_2 {nullptr};
	Texture instruction_texture {nullptr};
};

struct StaticTextures
{
	Texture label_texture_1 {nullptr};
	Texture label_texture_2 {nullptr};
};

struct StatusFlags
{
	bool n;
	bool v;
	bool b;
	bool d;
	bool i;
	bool z;
	bool c;
};

class CpuRenderer
{
public:
	CpuRenderer(IRenderer* renderer);
	virtual ~CpuRenderer();

	void produceFrame(const CpuStackFrame& cycle_data);
	Texture getTexture() const { return this->_texture; }

	void setRenderer(IRenderer* renderer);

private:
	IRenderer* _renderer {nullptr};
	Color _background_color {0x00, 0x00, 0x00};
	Color _text_color {0xff, 0xff, 0xff};
	std::string _font_name = "c:/repos/nes-emulator/source/emulator/FiraCode-Regular.ttf";
	Font _font {nullptr};
	int _font_size {24};

	uint64_t _width {513};
	uint64_t _row_height {30};
	uint64_t _vertical_padding {20};
	uint64_t _height {(this->_row_height * 5) + (this->_vertical_padding * 2)};
	Texture _texture {nullptr};

	void loadRenderer();
	void unloadRenderer();

	void createMainTexture();

	void renderStaticText();
	StaticTextures generateStaticText();
	void destroyTextures(StaticTextures& textures);

	void renderDynamicText(const CpuStackFrame& cycle_data);
	DynamicTextures generateDynamicText(const CpuStackFrame& cycle_data);
	void destroyTextures(DynamicTextures& textures);

	StatusFlags parseStatusFlags(uint8_t state);
};
