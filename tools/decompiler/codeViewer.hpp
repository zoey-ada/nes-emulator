#pragma once

#include <cstdint>
#include <deque>
#include <map>
#include <memory>
#include <string>

#include <cartridge/cartridge.hpp>
#include <cartridge/cartridgeLoader.hpp>
#include <platform/iRenderer.hpp>

#include "decompiler.hpp"
#include "memoryWrapper.hpp"

class CodeViewer
{
public:
	CodeViewer(std::shared_ptr<IRenderer> renderer);
	virtual ~CodeViewer();

	inline Texture getTexture() const { return this->_texture; }

	void loadFile(const std::string& filepath);

	void scrollUp();
	void scrollDown();

private:
	CartridgeLoader _cart_loader;
	std::unique_ptr<Cartridge> _cart;

	std::shared_ptr<IRenderer> _renderer {nullptr};
	std::deque<Texture> _line_textures;
	Texture _texture {nullptr};

	SDL_Color _background_color {0x00, 0x00, 0x00, 0xff};
	SDL_Color _text_color {0xff, 0xff, 0xff, 0xff};
	std::string _font_name = "c:/repos/nes-emulator/source/emulator/FiraCode-Regular.ttf";
	int _font_size {24};
	Font _font {nullptr};

	int _width {400};
	int _line_height {30};
	int _vertical_padding {0};
	int _num_lines {20};
	int _height {(this->_line_height * this->_num_lines) +
		(this->_vertical_padding * (this->_num_lines - 1))};

	int _top_line {0};

	Decompiler _decompiler {nullptr};
	std::unique_ptr<MemoryWrapper> _program_rom {nullptr};
	std::map<uint16_t, std::string> _code;

	void updateMainTexture();
	Texture renderLine(uint64_t line_number);

	void cleanup_line_textures();
};
