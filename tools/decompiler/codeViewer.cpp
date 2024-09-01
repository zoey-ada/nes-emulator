#include "codeViewer.hpp"

#include <SDL.h>

CodeViewer::CodeViewer(std::shared_ptr<IRenderer> renderer): _renderer(renderer)
{
	this->_font = this->_renderer->createFont(this->_font_name, this->_font_size);
	this->_texture = this->_renderer->createTexture(this->_width, this->_height);
}

CodeViewer::~CodeViewer()
{
	if (this->_font && this->_renderer)
		this->_renderer->destroyFont(this->_font);
	this->_font = nullptr;

	if (this->_texture)
		this->_renderer->destroyTexture(this->_texture);
	this->_texture = nullptr;

	this->cleanup_line_textures();
}

void CodeViewer::loadFile(const std::string& filepath)
{
	this->_cart = this->_cart_loader.load_cartridge(filepath);
	this->_program_rom = std::make_unique<MemoryWrapper>(this->_cart->getProgramRom());
	this->_decompiler.loadProgramMemory(this->_program_rom.get());
	this->_code = this->_decompiler.decompileAllInstructions(this->_program_rom->size() - 6);
	// this->_code.push_back(this->_program_rom[this->_program_rom->size() - 5]);
	// this->_code.push_back(this->_program_rom[this->_program_rom->size() - 4]);
	// this->_code.push_back(this->_program_rom[this->_program_rom->size() - 3]);
	// this->_code.push_back(this->_program_rom[this->_program_rom->size() - 2]);
	// this->_code.push_back(this->_program_rom[this->_program_rom->size() - 1]);

	this->cleanup_line_textures();
	this->_top_line = 0;

	for (int i = 0; i < this->_num_lines; ++i)
		this->_line_textures.push_back(this->renderLine(i));

	this->updateMainTexture();
}

void CodeViewer::scrollUp()
{
	if (this->_top_line == 0)
		return;

	auto texture = this->_line_textures.back();
	this->_line_textures.pop_back();
	this->_renderer->destroyTexture(texture);
	texture = nullptr;

	this->_top_line--;
	this->_line_textures.push_front(this->renderLine(this->_top_line));
	this->updateMainTexture();
}

void CodeViewer::scrollDown()
{
	int bottom_line = this->_top_line + this->_num_lines - 1;
	if (!this->_code.contains(bottom_line + 1))
		return;

	auto texture = this->_line_textures.front();
	this->_line_textures.pop_front();
	this->_renderer->destroyTexture(texture);
	texture = nullptr;

	this->_top_line++;
	this->_line_textures.push_back(this->renderLine(bottom_line));
	this->updateMainTexture();
}

void CodeViewer::updateMainTexture()
{
	auto swapped_render_target = this->_renderer->swapRenderTarget(this->_texture);
	auto rect = this->_renderer->measureTexture(this->_texture);
	this->_renderer->drawRectangle(rect, {0x00, 0x00, 0x00, 0xff}, true);

	for (int i = 0; i < this->_num_lines; ++i)
	{
		auto dest = this->_renderer->measureTexture(this->_line_textures[i]);
		dest.y = (this->_line_height + this->_vertical_padding) * i;
		this->_renderer->drawTexture(this->_line_textures[i], dest);
	}
}

Texture CodeViewer::renderLine(uint64_t line_number)
{
	TextRenderOptions options;
	options.font = this->_font;
	options.background_color = {0x00, 0x00, 0x00, 0xff};
	options.text_color = {0xff, 0xff, 0xff, 0xff};
	return this->_renderer->renderText(this->_code[line_number], options);
}

void CodeViewer::cleanup_line_textures()
{
	while (!this->_line_textures.empty())
	{
		auto texture = this->_line_textures.front();
		this->_line_textures.pop_front();
		this->_renderer->destroyTexture(texture);
		texture = nullptr;
	}
}
